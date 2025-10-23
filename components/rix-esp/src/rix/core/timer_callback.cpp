#include "rix/core/timer_callback.hpp"

namespace rix {

TimerCallback::TimerCallback(const Duration& duration, Callback callback) : Spinner(duration), duration_(duration), callback_(callback) {
  event_.current_real = Time::now();
  event_.current_expected = event_.last_expected = event_.last_real = Time(0.0);
  event_.last_duration = Duration(0.0);

  xTaskCreate(&Spinner::spin_task, "TimerTask", 4096, this, 2, &task_handle_);
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
  
  // Call callback outside of mutex to avoid deadlock
  if (callback_) {
    callback_(temp_event);
  }
}

void TimerCallback::timer_task(void *pvParameters) {
  TimerCallback *self = static_cast<TimerCallback *>(pvParameters);
  uint32_t delayMs = self->duration_.to_milliseconds();
  uint32_t delayTicks = delayMs / portTICK_PERIOD_MS;
  if (delayTicks == 0) {
    delayTicks = 1; // Ensure at least 1 tick delay
  }  
  while (self->ok()) {
    vTaskDelay(delayTicks);
    self->on_spin();
  }
  vTaskDelete(NULL); // Delete itself when done
}

void TimerCallback::set_callback(Callback callback) { callback_ = callback; }

TimerCallback::Callback TimerCallback::get_callback() const { return callback_; }

} // namespace rix