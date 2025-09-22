#include "rix/core/timer.hpp"

namespace rix::core {

Timer::Timer(const rix::util::Duration &duration, Callback callback) : duration_(duration), callback_(callback) {
  event_.current_real = rix::util::Time::now();
  event_.current_expected = event_.last_expected = event_.last_real = rix::util::Time(0.0);
  event_.last_duration = rix::util::Duration(0.0);
  // Initialize timer task at priority 1 (lowest) with stack size of 2048 bytes
  // TODO: Make priority and stack size configurable
  xTaskCreate(&Timer::timer_task, "TimerTask", 2048, this, 1, &task_handle_);
  
}

Timer::~Timer() {
  if (task_handle_) vTaskDelete(task_handle_);
}

bool Timer::ok() const { return !shutdown_flag_; }

void Timer::shutdown() { 
  if (task_handle_) vTaskDelete(task_handle_);
  shutdown_flag_ = true;
}

void Timer::spin_once() {
  // event_.current_real = rix::util::Time::now();
  // if (event_.current_real - event_.last_real > duration_) {
  //   rix::util::lock_guard guard(callback_mutex_);
  //   event_.last_duration = event_.current_real - event_.last_real;
  //   if (event_.current_expected == 0.0) {
  //     event_.current_expected = event_.current_real;
  //   } else {
  //     event_.current_expected += duration_;
  //   }
  //   callback_(event_);
  //   event_.last_real = event_.current_real;
  //   event_.last_expected = event_.current_expected;
  // }
  rix::util::LockGuard guard(callback_mutex_);
  event_.current_real = rix::util::Time::now();
  event_.last_duration = event_.current_real - event_.last_real;
  event_.current_expected = event_.current_real;
  callback_(event_);
  event_.last_real = event_.current_real;
  event_.last_expected = event_.current_expected;
}

void Timer::timer_task(void *pvParameters) {
    Timer *self = static_cast<Timer*>(pvParameters);
    while (self->ok()) {
        vTaskDelay(self->duration_.to_milliseconds() / portTICK_PERIOD_MS);
        self->spin_once();
    }
    vTaskDelete(NULL); // Delete itself when done
}

void Timer::set_callback(Callback callback) { callback_ = callback; }

Timer::Callback Timer::get_callback() const { return callback_; }

} // namespace rix::core