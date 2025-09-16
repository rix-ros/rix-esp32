#include "rix/core/mediator.hpp"
#include "rix/core/node.hpp"
#include "wifi.h"
#include <memory>

extern "C" void app_main() {
  // Init Wi-Fi (host access point)
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  wifi_init_config_t *wifi_cfg = wifi_start();

  wifi_config_t *wifi_ap_cfg =
      access_point_init("esp_ap_test", "password", "192.168.4.2", 11, 0, 3);

  // Init rixhub
  auto mediator = std::make_shared<rix::core::Mediator>(  
      rix::ipc::Endpoint("0.0.0.0", 48104));

  if (!mediator->ok()) {
    rix::util::Log::error << "Failed to create rixhub." << std::endl;
    return;
  }

  // Spin
  mediator->spin();
  return;
}