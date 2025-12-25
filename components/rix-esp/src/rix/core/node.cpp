#include "rix/core/node.hpp"

namespace rix {

Node::Node(const std::string& name, const TaskConfig& config, const Endpoint& endpoint)
    : Spinner(config), rixhub_endpoint_(Endpoint(RIXHUB_IP, RIXHUB_PORT)), registered_flag_(false) {
  server_ = socket_factory_();
  if (!server_) {
    shutdown();
    return;
  }

  server_->set_reuse_address(true);
  server_->bind(Endpoint(endpoint.address, endpoint.port));
  server_->listen(MAX_CONN);

  // Ensure server was intitialized properly
  if (server_->is_exception()) {
    shutdown();
    return;
  }

  auto server_endpoint = server_->local_endpoint();
  // Update the endpoint in case the port was set to 0 (ephemeral)
  info_.endpoint.address = server_endpoint.address;
  info_.endpoint.port = server_endpoint.port;

  info_.id = id_factory_();
  info_.name = name;

  auto client = socket_factory_();
  if (!client) {
    printf("Failed to create client socket\n");
    shutdown();
    return;
  }
  if (!client->connect(rixhub_endpoint_)) {
    printf("Failed to connect to RIX hub\n");
    shutdown();
    return;
  }
  if (!client->send_message(OPCODE::NODE_REGISTER, info_)) {
    printf("Failed to send NODE_REGISTER message\n");
    shutdown();
    return;
  }

  sys_msgs::Operation op;
  sys_msgs::Status status;
  if (!client->recv_message(op, status)) {
    printf("Failed to receive NODE_REGISTER response\n");
    shutdown();
    return;
  }
  if (status.error) {
    printf("RIX hub returned error on NODE_REGISTER\n");
    shutdown();
    return;
  }

  registered_flag_ = true;

  // Create timer to handle pings at 2Hz
  TaskConfig ping_timer_config;
  ping_timer_config.STACK_SIZE = 2048;
  ping_timer_config.PRIORITY = 5;
  ping_timer_config.MAX_TIMEOUT = Duration(2);
  create_timer(ping_timer_config, [this](const TimerCallback::Event&) {
    // Check for ping
    // std::cout << "Checking for ping..." << std::endl;
    if (server_->is_readable()) {
      auto conn = server_->accept();
      if (conn) {
        sys_msgs::Operation op;
        conn->recv_message(op, op.size());
        if (op.opcode == OPCODE::PING) {
          sys_msgs::Status status;
          status.id = info_.id;
          status.error = 0;
          conn->send_message(OPCODE::STATUS_RESPONSE, status);
        }
      }
    }
  });

  xTaskCreate(&Spinner::spin_task, "NodeTask", config.STACK_SIZE, this, config.PRIORITY,
              &task_handle_);
}

Node::~Node() {

  // if( task_handle_) {
  //   vTaskDelete(task_handle_);
  // }
  shutdown();
  vTaskDelay(pdMS_TO_TICKS(1500));

  if (registered_flag_) {
    auto client = socket_factory_();
    if (!client) {
      return;
    }
    //client->set_blocking(false);  // ADD THIS LINE
    if (client->connect(rixhub_endpoint_)) {
      client->send_message(OPCODE::NODE_DEREGISTER, info_);
    }
  }
  while (!components_.empty()) {
    components_.pop_back(); // Preserve order of destruction
  }
}

void Node::shutdown() noexcept {
  // Shutdown all components first
  for (auto& component : components_) {
    if (component) {
      component->shutdown();
    }
  }
  
  // Then shutdown the node itself
  Spinner::shutdown();
}

void Node::on_spin() {
  // Spin all components, remove ones that are not 'ok'
  auto it = components_.begin();
  while (it != components_.end()) {
    auto component = *it;
    if (!component->ok()) {
      it = components_.erase(it);
      continue;
    }
    it++;
  }
  vTaskDelay(pdMS_TO_TICKS(10));
#ifdef RIX_MULTITHREADED
  // Sleep to prevent busy waiting
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
#endif
}

std::shared_ptr<Publisher> Node::create_publisher(const sys_msgs::TopicInfo& topic_info, const TaskConfig& config,
                                                  const Endpoint& rixhub_endpoint,
                                                  const Endpoint& endpoint) {
  sys_msgs::PubInfo pub_info;
  pub_info.id = id_factory_();
  pub_info.node_id = info_.id;
  pub_info.topic_info = topic_info;
  pub_info.endpoint.address = endpoint.address;
  pub_info.endpoint.port = endpoint.port;
  auto pub = std::shared_ptr<Publisher>(new Publisher(pub_info, config, socket_factory_, rixhub_endpoint_));
  components_.push_back(pub);
  return pub;
}

std::shared_ptr<Subscriber> Node::create_subscriber(const sys_msgs::TopicInfo& topic_info, const TaskConfig& config,
                                                    const Endpoint& rixhub_endpoint,
                                                    const Endpoint& endpoint) {
  sys_msgs::SubInfo sub_info;
  sub_info.id = id_factory_();
  sub_info.node_id = info_.id;
  sub_info.topic_info = topic_info;
  sub_info.endpoint.address = endpoint.address;
  sub_info.endpoint.port = endpoint.port;
  auto sub = std::shared_ptr<Subscriber>(new Subscriber(sub_info, config, socket_factory_, rixhub_endpoint_));
  components_.push_back(sub);
  return sub;
}

std::shared_ptr<Service>
Node::create_service(sys_msgs::SrvInfo& service_info,const TaskConfig& config, const Endpoint& rixhub_endpoint, const Endpoint& endpoint) {
  service_info.id = id_factory_();
  service_info.node_id = info_.id;
  service_info.endpoint.address = endpoint.address;
  service_info.endpoint.port = endpoint.port;
  auto srv = std::shared_ptr<Service>(new Service(service_info, config, socket_factory_, rixhub_endpoint_));
  components_.push_back(srv);
  return srv;
}

bool Node::get_system_info(sys_msgs::SystemInfo& info) {
  auto client = socket_factory_();
  if (!client->connect(rixhub_endpoint_))
    return false;

  std_msgs::UInt64 node_id;
  node_id.data = info_.id;
  if (!client->send_message(OPCODE::SYSTEM_GET_REQUEST, node_id)) {
    return false;
  }

  sys_msgs::Operation op;
  if (!client->recv_message(op, info)) {
    return false;
  }
  if (op.opcode != OPCODE::SYSTEM_GET_RESPONSE) {
    return false;
  }
  Log::debug << "Retrieved system info from RIXHub." << std::endl;
  return true;
}

std::shared_ptr<ServiceClient> Node::create_service_client(const sys_msgs::SrvRequest& service_request,
                                                           const TaskConfig& config,
                                                           const Endpoint& rixhub_endpoint,
                                                           const Endpoint& endpoint) {
  auto srv_cli = std::shared_ptr<ServiceClient>(new ServiceClient(service_request, config, socket_factory_, rixhub_endpoint));
  components_.push_back(srv_cli);
  return srv_cli;
}

} // namespace rix