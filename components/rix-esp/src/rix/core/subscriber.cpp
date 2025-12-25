#include "rix/core/subscriber.hpp"

namespace rix {

Subscriber::Subscriber(const sys_msgs::SubInfo &info,
                       const TaskConfig &config, SocketFactory socket_factory,
                       const Endpoint &rixhub_endpoint)
    : Spinner(config), info_(info), socket_factory_(socket_factory),
      callback_(nullptr), rixhub_endpoint_(rixhub_endpoint),
      registered_flag_(false), sub_notify_acceptor_(*this, config) {

  server_ = socket_factory_();
  server_->set_reuse_address(true);
  server_->bind(Endpoint(info_.endpoint.address, info_.endpoint.port));
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

  // Register subscriber with rixhub
  auto client = socket_factory_();
  if (!client->connect(rixhub_endpoint_)) {
    shutdown();
    return;
  }
  if (!client->send_message(OPCODE::SUB_REGISTER, info_)) {
    shutdown();
    return;
  }

  sys_msgs::Operation op;
  sys_msgs::Status status;
  if (!client->recv_message(op, status)) {
    shutdown();
    return;
  }
  if (status.error) {
    shutdown();
    return;
  }

  registered_flag_ = true;

  Log::debug << "Subscriber created on topic \"" << info_.topic_info.name
             << "\"." << std::endl;
  snprintf(task_name_, sizeof(task_name_), "%" PRIu64, info_.id);
  xTaskCreate(&Spinner::spin_task, task_name_, config.STACK_SIZE, this,
              config.PRIORITY, &task_handle_);
  // xTaskCreate(&Spinner::spin_task, "Subscriber", config.STACK_SIZE, this,
  //             config.PRIORITY, &task_handle_);
  sub_notify_acceptor_.start();
}

Subscriber::~Subscriber() {

  // if (task_handle_) {
  //   vTaskDelete(task_handle_);
  // }
  shutdown();
  sub_notify_acceptor_.shutdown();
  vTaskDelay(pdMS_TO_TICKS(1500)); 
  if (registered_flag_) {
    auto client = socket_factory_();
    if (!client) {
      return;
    }
   // client->set_blocking(false);
    if (client->connect(rixhub_endpoint_)) {
      client->send_message(OPCODE::SUB_DEREGISTER, info_);
    }
  }
  Log::debug << "Subscriber on topic \"" << info_.topic_info.name
             << "\" destroyed." << std::endl;
}

size_t Subscriber::get_publisher_count() const {
  rix::util::LockGuard guard(callback_mutex_);
  return clients_.size();
}

/**< TODO: Implement the spin_once method */
void Subscriber::on_spin() {

  {
    rix::util::LockGuard guard(callback_mutex_);
    if (clients_.empty() || !callback_) {
      // Lock will be released when this scope exits
      // Delay will be called after the lock scope
    } else {
      std::vector<std::shared_ptr<GenericSocket>> sockets(clients_.begin(),
                                                          clients_.end());
      std::vector<std::shared_ptr<GenericSocket>> readable;

      if (GenericSocket::get_poller()) {

        std::vector<std::shared_ptr<GenericSocket>> exceptional;
        Duration timeout(1.0);
        GenericSocket::poll(sockets, timeout, PollFlag::READ, readable,
                            exceptional);

        // Remove any clients that have exceptions
        for (const auto &conn : exceptional) {
          clients_.erase(conn);
          Log::debug << "Removed exceptional publisher from topic \""
                     << info_.topic_info.name << "\"." << std::endl;
        }
        exceptional.clear();
      } else {
        // Fallback if poller is not available
        for (const auto &sock : sockets) {
          if (sock->is_readable()) {
            readable.push_back(sock);
          }
        }
      }

      auto it = readable.begin();
      while (it != readable.end()) {
        auto client = *it;

        // Read a message from the publisher
        sys_msgs::Operation op;
        if (!client->recv_message(op, *msg_instance_)) {
          clients_.erase(client);
          it++;
          Log::debug << "Removed exceptional publisher from topic \""
                     << info_.topic_info.name << "\"." << std::endl;
          continue;
        }

        if (op.opcode != OPCODE::PUB_MESSAGE) {
          clients_.erase(client);
          it++;
          Log::debug << "Removed exceptional publisher from topic \""
                     << info_.topic_info.name << "\"." << std::endl;
          continue;
        }

        Log::debugv << "Received message on topic \"" << info_.topic_info.name
                    << "\"." << std::endl;
        // Invoke the callback
        callback_(*msg_instance_);
        it++;
      }
      readable.clear();
    }
  }

  // Allow other tasks to run - prevent watchdog timeout
  vTaskDelay(pdMS_TO_TICKS(10)); // 100Hz frequency (10ms period)
}

Subscriber::SubNotifyAcceptor::SubNotifyAcceptor(Subscriber &parent,
                                                 const TaskConfig &config)
    : Spinner(config), parent(parent), config_(config) {
  // TODO: Add configurable priority and maybe stack size
  // Priority is used directly in start() method via config_.PRIORITY
  // Make the task name unique by adding 0xA to the subscriber ID
  printf("Spinning up SubNotifyAcceptor...\n");
  snprintf(task_name_, sizeof(task_name_), "%" PRIu64, parent.info_.id + 0xA);
}

void Subscriber::SubNotifyAcceptor::start() {
  xTaskCreate(&Spinner::spin_task, task_name_, config_.STACK_SIZE, this,
              config_.PRIORITY, &task_handle_);
  // xTaskCreate(&Spinner::spin_task, "SubNotifyAcceptor", config_.STACK_SIZE,
  //             this, config_.PRIORITY, &task_handle_);
}
void Subscriber::SubNotifyAcceptor::on_spin() {
  Duration timeout(1.0);
  // Check to see if rixhub has made a connection
  if (!parent.server_->wait_readable(timeout)) {
    return;
  }
    rix::Log::info << "Accepting connection from rixhub..." << std::endl;

  // Accept a connection from rixhub
  auto conn = parent.server_->accept();
  if (!conn) {
    return;
  }

  sys_msgs::Operation op;
  sys_msgs::SubNotify sub_notify;
  if (!conn->recv_message(op, sub_notify)) {
    rix::Log::error << "Failed to receive SubNotify from rixhub."
                  << std::endl;
    return;
  }
  if (op.opcode != OPCODE::SUB_NOTIFY) {
    rix::Log::error << "Received invalid opcode from rixhub." << std::endl;
    return;
  }
  rix::util::LockGuard guard(parent.callback_mutex_);
  {
    // Connect to the specified publishers (non-blocking)
    for (const auto &pub : sub_notify.publishers) {
      auto client = parent.socket_factory_();
      if (!client) {
        continue;
      }
      client->set_blocking(false);
      client->connect(Endpoint(pub.endpoint.address, pub.endpoint.port));
      parent.clients_.insert(client);
      rix::Log::debug << "Connected to publisher at \"" << pub.endpoint.address
                 << ":" << pub.endpoint.port << "\" on topic \""
                 << pub.topic_info.name << "\"." << std::endl;
    }
  }
  vTaskDelay(pdMS_TO_TICKS(10));
}

} // namespace rix