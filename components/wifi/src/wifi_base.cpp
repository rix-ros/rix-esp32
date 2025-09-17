#include "wifi/wifi_base.hpp"

WifiBase::WifiBase() : wifi_started_(false) {
  memset(&wifi_cfg_, 0, sizeof(wifi_init_config_t));
  esp_err_t err = esp_netif_init();
  if (err != ESP_OK) {
    return;
  }
  err = esp_event_loop_create_default();
  if (err != ESP_OK) {
    return;
  }
  wifi_cfg_ = WIFI_INIT_CONFIG_DEFAULT();
  err = esp_wifi_init(&wifi_cfg_);
  if (err != ESP_OK) {
    return;
  }
  wifi_started_ = true;
}

bool WifiBase::ok() const { return wifi_started_; }

WifiBase::~WifiBase() {
  if (wifi_started_) {
    esp_wifi_stop();
    esp_wifi_deinit();
    wifi_started_ = false;
  }
}