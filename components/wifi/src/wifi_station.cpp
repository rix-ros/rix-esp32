#include "wifi/wifi_station.hpp"

WifiStation::WifiStation(int16_t max_attempts)
    : WifiBase(), conn_attempts_(0), max_conn_attempts_(max_attempts),
      in_station_mode_(false), station_connected_(false),
      station_connecting_(false), sta_event_group_(nullptr) {
  // memset(&cfg_, 0, sizeof(wifi_config_t));

  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
  if (!sta_netif)
    return;

  in_station_mode_ = true;
  sta_event_group_ = xEventGroupCreate();
  return;
}

WifiStation::~WifiStation() {
  if (!in_station_mode_)
    return;
  if (station_connected_)
    disconnect();
  esp_wifi_stop();
  in_station_mode_ = false;
  max_conn_attempts_ = -1;
  conn_attempts_ = 0;
  if (sta_event_group_) {
    vEventGroupDelete(sta_event_group_);
    sta_event_group_ = nullptr;
  }
}

bool WifiStation::ok() const { return in_station_mode_; }

void WifiStation::scan(std::vector<wifi_ap_record_t> &ap_records,
                       bool show_hidden, uint16_t max_aps) {
  if (!in_station_mode_ || station_connected_)
    return;

  std::memset(&cfg_, 0, sizeof(wifi_config_t));
  cfg_.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
  cfg_.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
  cfg_.sta.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;
  esp_wifi_set_config(WIFI_IF_STA, &cfg_);

  if (esp_wifi_start() != ESP_OK)
    return;

  ap_records.resize(max_aps);

  wifi_scan_config_t scan_cfg = {};
  scan_cfg.show_hidden = show_hidden;
  scan_cfg.scan_type = WIFI_SCAN_TYPE_ACTIVE;
  scan_cfg.scan_time.passive = 100;

  esp_wifi_scan_start(&scan_cfg, true);
  esp_wifi_scan_stop();
  esp_wifi_scan_get_ap_records(&max_aps, ap_records.data());
  esp_wifi_stop();
  ap_records.resize(max_aps);
}

bool WifiStation::connect(const std::string &ssid,
                          const std::string &password) {
  if (!in_station_mode_ || station_connected_)
    return false;

  start_event_handler();

  std::memset(&cfg_, 0, sizeof(wifi_config_t));
  strncpy((char *)cfg_.sta.ssid, ssid.c_str(), sizeof(cfg_.sta.ssid) - 1);
  strncpy((char *)cfg_.sta.password, password.c_str(),
          sizeof(cfg_.sta.password) - 1);
  esp_wifi_set_config(WIFI_IF_STA, &cfg_);

  esp_wifi_start();
  station_connecting_ = true;
  return true;
}

bool WifiStation::connect_enterprise(const std::string &ssid,
                                     const std::string &username,
                                     const std::string &password,
                                    const char *ca_cert) {
  if (!in_station_mode_ || station_connected_)
    return false;

  start_event_handler();

  std::memset(&cfg_, 0, sizeof(wifi_config_t));
  strncpy((char *)cfg_.sta.ssid, ssid.c_str(), sizeof(cfg_.sta.ssid) - 1);
  esp_wifi_set_config(WIFI_IF_STA, &cfg_);

  esp_eap_client_set_identity((uint8_t *)username.c_str(), username.length());
  esp_eap_client_set_username((uint8_t *)username.c_str(), username.length());
  esp_eap_client_set_password((uint8_t *)password.c_str(), password.length());
  esp_eap_client_set_ca_cert((const unsigned char*)ca_cert, strlen(ca_cert) + 1);
  esp_eap_client_set_eap_methods(ESP_EAP_TYPE_PEAP);
  esp_wifi_sta_enterprise_enable();

  esp_wifi_start();
  station_connecting_ = true;
  return true;
}

std::string WifiStation::get_ip() const {
  if (!in_station_mode_ || !station_connected_)
    return "";

  esp_netif_ip_info_t ip_info;
  esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
  if (!sta_netif)
    return "";
  if (esp_netif_get_ip_info(sta_netif, &ip_info) != ESP_OK)
    return "";

  char ip_str[16];
  snprintf(ip_str, sizeof(ip_str), IPSTR, IP2STR(&ip_info.ip));
  return std::string(ip_str);
}

std::string WifiStation::get_mac_address() const {
  if (!in_station_mode_)
    return "";

  uint8_t mac[6];
  if (esp_wifi_get_mac(WIFI_IF_STA, mac) != ESP_OK)
    return "";

  char mac_str[18];
  snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return std::string(mac_str);
}
bool WifiStation::wait_for_connection(int32_t timeout_ms) {
  if (!in_station_mode_)
    return false;
  if (station_connected_)
    return true;

  TickType_t ticks_elapsed = 0;
  TickType_t ticks_to_wait =
      (timeout_ms < 0) ? portMAX_DELAY : (timeout_ms / portTICK_PERIOD_MS);
  while (ticks_elapsed < ticks_to_wait && !station_connected_) {
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ticks_elapsed += 100 / portTICK_PERIOD_MS;
  }
  return station_connected_;
}

void WifiStation::disconnect() {
  if (!in_station_mode_ || !station_connected_)
    return;
  stop_event_handler();
  esp_wifi_disconnect();
  esp_wifi_stop();
  station_connected_ = false;
}

bool WifiStation::is_connected() const { return station_connected_; }

void WifiStation::start_event_handler() {
  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;
  esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                      &WifiStation::sta_event_handler, this,
                                      &instance_any_id);
  esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID,
                                      &WifiStation::sta_event_handler, this,
                                      &instance_got_ip);
}

void WifiStation::stop_event_handler() {
  esp_event_handler_instance_unregister(
      WIFI_EVENT, ESP_EVENT_ANY_ID, (void *)&WifiStation::sta_event_handler);
  esp_event_handler_instance_unregister(
      IP_EVENT, ESP_EVENT_ANY_ID, (void *)&WifiStation::sta_event_handler);
}

void WifiStation::sta_event_handler(void *arg, esp_event_base_t event_base,
                                    int32_t event_id, void *event_data) {
  WifiStation *self = static_cast<WifiStation *>(arg);
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    ESP_LOGI("CLIENT", "WiFi started, connecting to AP...");
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    self->station_connected_ = false;
    self->station_connecting_ = true;
    if (self->max_conn_attempts_ > 0 &&
        self->conn_attempts_ >= self->max_conn_attempts_) {
      self->station_connecting_ = false;
      return;
    }
    ESP_LOGI("CLIENT", "retry to connect to the AP");
    esp_wifi_connect();
    self->conn_attempts_++;
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    self->station_connected_ = true;
    self->conn_attempts_ = 0;
    // Print the obtained IP address
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI("CLIENT", "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
  } else {
    ESP_LOGI("CLIENT", "Unhandled event: %s, ID: %d", event_base, event_id);
  }
}