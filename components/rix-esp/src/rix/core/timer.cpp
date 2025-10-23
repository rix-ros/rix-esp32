// #include "rix/core/timer.hpp"

// namespace rix::core {

// Timer::Timer(const rix::util::Duration &duration, Callback callback)
//     : duration_(duration), callback_(callback), shutdown_flag_(false) {
//   event_.current_real = rix::util::Time::now();
//   event_.current_expected = event_.last_expected = event_.last_real =
//       rix::util::Time(0.0);
//   event_.last_duration = rix::util::Duration(0.0);
//   // Initialize timer task at priority 2 with stack size of 4096 bytes
//   // TODO: Make priority and stack size configurable
//   xTaskCreate(&Timer::timer_task, "TimerTask", 4096, this, 2, &task_handle_);
// }

// Timer::~Timer() {
//   if (task_handle_)
//     vTaskDelete(task_handle_);
// }

// bool Timer::ok() const { return !shutdown_flag_; }

// void Timer::shutdown() {
//   if (task_handle_)
//     vTaskDelete(task_handle_);
//   shutdown_flag_ = true;
// }

// void Timer::spin_once() {
//   if (!callback_mutex_ || shutdown_flag_) {
//     return;
//   }
  
//   // Capture event data first
//   Event temp_event;
//   {
//     rix::util::LockGuard guard(callback_mutex_);
//     event_.current_real = rix::util::Time::now();
//     event_.last_duration = event_.current_real - event_.last_real;
//     event_.current_expected = event_.current_real;
//     temp_event = event_;
//     event_.last_real = event_.current_real;
//     event_.last_expected = event_.current_expected;
//   }
  
//   // Call callback outside of mutex to avoid deadlock
//   if (callback_) {
//     callback_(temp_event);
//   }
// }

// void Timer::timer_task(void *pvParameters) {
//   Timer *self = static_cast<Timer *>(pvParameters);
//   uint32_t delayMs = self->duration_.to_milliseconds();
//   uint32_t delayTicks = delayMs / portTICK_PERIOD_MS;
//   if (delayTicks == 0) {
//     delayTicks = 1; // Ensure at least 1 tick delay
//   }  
//   while (self->ok()) {
//     vTaskDelay(delayTicks);
//     self->spin_once();
//   }
//   vTaskDelete(NULL); // Delete itself when done
// }

// void Timer::set_callback(Callback callback) { callback_ = callback; }

// Timer::Callback Timer::get_callback() const { return callback_; }

// } // namespace rix::core