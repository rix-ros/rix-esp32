#pragma once

#include <memory>
#include <mutex>
#include <set>

#include "rix/core/common.hpp"
#include "rix/core/spinner.hpp"
#include "rix/sys_msgs/Operation.hpp"
#include "rix/sys_msgs/PubInfo.hpp"
#include "rix/sys_msgs/Status.hpp"
#include "rix/sys_msgs/SubInfo.hpp"
#include "rix/std_msgs/UInt32.hpp"
#include "rix/util/log.hpp"

namespace rix {

class Node; // Forward declaration

class Publisher : public Spinner {
  friend class Node;

public:
  Publisher(const Publisher &) = delete;
  Publisher &operator=(const Publisher &) = delete;
  Publisher(Publisher &&) = delete;
  Publisher &operator=(Publisher &&) = delete;
  ~Publisher();

  void publish(const Message &msg);
  size_t get_subscriber_count() const;

private:
  sys_msgs::PubInfo info_;
  SocketFactory socket_factory_;
  std::shared_ptr<GenericSocket> server_;
  std::set<std::shared_ptr<GenericSocket>> connections_;
  //mutable std::mutex connections_mutex_;
  SemaphoreHandle_t connections_mutex_ = xSemaphoreCreateMutex();
  Endpoint rixhub_endpoint_;
  std::atomic<bool> registered_flag_;
  std::atomic<bool> shutdown_flag_;
  TaskHandle_t task_handle_{nullptr};
  char task_name_[32];
  // Message buffer for publishing
  // Using an unreasonable size for stress-testing purposes
  uint8_t messageBuffer[65536];
#ifdef RIX_MULTITHREADED
  std::thread spin_thread_{};
#endif

  Publisher(const sys_msgs::PubInfo &info, const TaskConfig& config, SocketFactory factory, Endpoint rixhub_endpoint);

  using Spinner::spin;
  using Spinner::spin_once;
  void on_spin() override;
};

} // namespace rix