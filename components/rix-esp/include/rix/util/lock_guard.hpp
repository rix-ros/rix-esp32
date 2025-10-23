#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace rix {
namespace util {
class LockGuard {
public:
    explicit LockGuard(SemaphoreHandle_t mutex) : mutex_(mutex) {
        if (mutex_) {
            xSemaphoreTake(mutex_, portMAX_DELAY);
        }
    }

    ~LockGuard() {
        if (mutex_) {
            xSemaphoreGive(mutex_);
        }
    }

    // Delete copy constructor and assignment operator
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;

private:
    SemaphoreHandle_t mutex_;
};
} // namespace rix::util
} // namespace rix