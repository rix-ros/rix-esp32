#include "rix/core/timer_callback.hpp"

namespace rix {

TimerCallback::TimerCallback(const TaskConfig& config, Callback callback) : Spinner(config), duration_(config.MAX_TIMEOUT), callback_(callback) {
  event_.current_real = Time::now();
  event_.current_expected = event_.last_expected = event_.last_real = Time(0.0);
  event_.last_duration = Duration(0.0);

  Time currentTime = Time::now();
  snprintf(task_name_, sizeof(task_name_), "%" PRIu64, currentTime.to_nanoseconds());
  xTaskCreate(&Spinner::spin_task, task_name_, config.STACK_SIZE, this, config.PRIORITY, &task_handle_);
// #ifdef RIX_MULTITHREADED
//   spin_thread_ = std::thread([this]() { this->spin(); });
// #endif
}

TimerCallback::~TimerCallback() {
  if (task_handle_)
    vTaskDelete(task_handle_);
#ifdef RIX_MULTITHREADED
  shutdown();
  if (spin_thread_.joinable()) {
    spin_thread_.join();
  }
#endif
}

void TimerCallback::on_spin() {

  uint32_t delayMs = duration_.to_milliseconds();
  uint32_t delayTicks = delayMs / portTICK_PERIOD_MS;

  if (!callback_mutex_ || shutdown_flag_) {
    return;
  }
  
  // Capture event data first
  Event temp_event;
  {
    rix::util::LockGuard guard(callback_mutex_);
    event_.current_real = rix::Time::now();
    event_.last_duration = event_.current_real - event_.last_real;
    event_.current_expected = event_.current_real;
    temp_event = event_;
    event_.last_real = event_.current_real;
    event_.last_expected = event_.current_expected;
  }
  
  if (delayTicks == 0) {
    delayTicks = 1; // Ensure at least 1 tick delay
  }  
  // Call callback outside of mutex to avoid deadlock
  if (callback_) {
    callback_(temp_event);
  }

  vTaskDelay(delayTicks);
}

void TimerCallback::set_callback(Callback callback) { callback_ = callback; }

TimerCallback::Callback TimerCallback::get_callback() const { return callback_; }

} // namespace rix