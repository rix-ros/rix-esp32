#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include "rix/core/common.hpp"
#include "rix/core/spinner.hpp"
#include "rix/sys_msgs/Operation.hpp"
#include "rix/sys_msgs/SrvRequest.hpp"
#include "rix/sys_msgs/SrvResponse.hpp"
#include "rix/sys_msgs/Status.hpp"
#include "rix/std_msgs/UInt32.hpp"
#include "rix/util/log.hpp"

namespace rix {

class Node; // Forward declaration

class ServiceClient : public Spinner {
  friend class Node;

public:
  ServiceClient(const ServiceClient&) = delete;
  ServiceClient& operator=(const ServiceClient&) = delete;
  ServiceClient(ServiceClient&&) = delete;
  ServiceClient& operator=(ServiceClient&&) = delete;

  ~ServiceClient();

  bool call(const Message& request, Message& response);

private:
  sys_msgs::SrvRequest request_;
  SocketFactory socket_factory_;
  Endpoint endpoint_;
  TaskHandle_t task_handle_{nullptr};
#ifdef RIX_MULTITHREADED
  std::thread spin_thread_{};
#endif

  using Spinner::spin;
  using Spinner::spin_once;
  void on_spin() override;

  ServiceClient(const sys_msgs::SrvRequest& request,const TaskConfig& config, SocketFactory factory, const Endpoint& rixhub_endpoint);
};

} // namespace rix