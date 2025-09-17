#pragma once

#include "freertos/FreeRTOS.h"

namespace rix::core {

class Spinner {
public:
  Spinner() = default;
  Spinner(const Spinner &other) = default;
  Spinner &operator=(const Spinner &other) = default;
  virtual ~Spinner() = default;

  void spin() {
    while (ok()) {
      spin_once();
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
  }

  /**
   * @brief Returns true if loop should continue, false if loop should stop.
   *
   */
  virtual void spin_once() = 0;

  /**
   * @brief Returns true if shutdown has not been called and the constructor
   * created the object without error.
   *
   */
  virtual bool ok() const = 0;

  /**
   * @brief Shuts down the object. ok() will return false after this call.
   *
   */
  virtual void shutdown() = 0;
};

} // namespace rix::core