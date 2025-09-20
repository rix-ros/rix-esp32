#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
namespace rix::util {
class lock_guard {
public:
    explicit lock_guard(SemaphoreHandle_t mutex) : mutex_(mutex) {
        if (mutex_) {
            xSemaphoreTake(mutex_, portMAX_DELAY);
        }
    }

    ~lock_guard() {
        if (mutex_) {
            xSemaphoreGive(mutex_);
        }
    }

    // Delete copy constructor and assignment operator
    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;

private:
    SemaphoreHandle_t mutex_;
};
} // namespace rix::util