#pragma once

#include <memory>

namespace rix {

class Spinner {
public:
  Spinner(const Duration& period) : period_(period) {}
  Spinner(const Spinner& other) = default;
  Spinner& operator=(const Spinner& other) = default;
  Spinner(Spinner&& other) = default;
  Spinner& operator=(Spinner&& other) = default;
  virtual ~Spinner() = default;

  static void spin_task(void* pvParameters) {
    Spinner* self = static_cast<Spinner*>(pvParameters);
    while (self->ok()) {
      self->spin_once();
      vTaskDelay(self->period_.to_milliseconds() / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL); // Delete itself when done
  }

  void spin() {
    while (ok()) {
      spin_once();
    }
  }

  /**
   * @brief Returns true if loop should continue, false if loop should stop.
   *
   */
  void spin_once() {
    on_spin();
    // if (signal_received_) {
    //  shutdown();
    //  return;
    //}
  }

  /**
   * @brief Returns true if shutdown has not been called and the constructor
   * created the object without error.
   *
   */
  bool ok() const noexcept { return !shutdown_flag_; }

  /**
   * @brief Shuts down the object. ok() will return false after this call.
   *
   */
  void shutdown() noexcept { shutdown_flag_ = true; }

  // static void set_shutdown_signal(std::shared_ptr<GenericSignal> signal) { shutdown_signal_ = signal; }
  // static std::shared_ptr<GenericSignal> get_shutdown_signal() { return shutdown_signal_; }

private:
  bool shutdown_flag_{false};
  Duration period_;

  virtual void on_spin() = 0;
};

} // namespace rix