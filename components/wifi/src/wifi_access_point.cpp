#include "wifi/wifi_access_point.hpp"

WifiAccessPoint::WifiAccessPoint(const std::string &ssid,
                                 const std::string &password,
                                 const std::string &ip, uint8_t channel,
                                 uint8_t hidden, uint8_t max_connections)
    : access_point_hidden_(false), access_point_hosting_(false),
      ap_netif_(nullptr) {
  // memset(&cfg_, 0, sizeof(wifi_config_t));
  if (access_point_hosting_)
    return;

  esp_wifi_set_mode(WIFI_MODE_AP);
  ap_netif_ = esp_netif_create_default_wifi_ap();
  if (!ap_netif_)
    return;

  cfg_.ap.channel = channel;
  cfg_.ap.authmode = WIFI_AUTH_WPA2_PSK;
  cfg_.ap.max_connection = max_connections;
  cfg_.ap.beacon_interval = 100;
  strcpy((char *)cfg_.ap.ssid, ssid.c_str());
  cfg_.ap.ssid_len = ssid.size();
  strcpy((char *)cfg_.ap.password, password.c_str());
  cfg_.ap.ssid_hidden = hidden;
  cfg_.ap.pmf_cfg.required = 0;
  access_point_hidden_ = hidden;

  esp_wifi_set_config(WIFI_IF_AP, &cfg_);
  esp_wifi_start();
  start_event_handler();
  set_static_ip(ip, "255.0.0.0", "192.168.4.1");
  access_point_hosting_ = true;
  return;
}

WifiAccessPoint::~WifiAccessPoint() {
  if (!access_point_hosting_)
    return;
  esp_wifi_stop();
  stop_event_handler();
  access_point_hosting_ = false;
  ap_netif_ = nullptr;
}

bool WifiAccessPoint::ok() const { return access_point_hosting_; }

void WifiAccessPoint::hide_ssid() {
  if (!access_point_hosting_ || access_point_hidden_)
    return;
  esp_wifi_stop();
  cfg_.ap.ssid_hidden = 1;
  esp_wifi_set_config(WIFI_IF_AP, &cfg_);
  esp_wifi_start();
  access_point_hidden_ = true;
}

void WifiAccessPoint::show_ssid() {
  if (!access_point_hosting_ || !access_point_hidden_)
    return;
  esp_wifi_stop();
  cfg_.ap.ssid_hidden = 0;
  esp_wifi_set_config(WIFI_IF_AP, &cfg_);
  esp_wifi_start();
  access_point_hidden_ = false;
}

bool WifiAccessPoint::is_hidden() const { return access_point_hidden_; }

std::string WifiAccessPoint::get_ssid() const {
  return std::string(reinterpret_cast<const char *>(cfg_.ap.ssid));
}

std::string WifiAccessPoint::get_password() const {
  return std::string(reinterpret_cast<const char *>(cfg_.ap.password));
}

bool WifiAccessPoint::wait_for_new_connection(int32_t timeout_ms) {
  if (!access_point_hosting_)
    return false;

  int num_connected_stations_start = num_connected_stations_;
  TickType_t ticks_elapsed = 0;
  TickType_t ticks_to_wait =
      (timeout_ms < 0) ? portMAX_DELAY : (timeout_ms / portTICK_PERIOD_MS);
  while (ticks_elapsed < ticks_to_wait &&
         num_connected_stations_ <= num_connected_stations_start) {
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ticks_elapsed += 100 / portTICK_PERIOD_MS;
  }
  return num_connected_stations_ > num_connected_stations_start;
}

void WifiAccessPoint::get_connected_stations_ips(
    std::vector<std::string> &ips) const {
  if (!access_point_hosting_)
    return;
  ips = connected_stations_ips_;
}

void WifiAccessPoint::start_event_handler() {
  esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                      &WifiAccessPoint::ap_event_handler,
                                      (void *)this, nullptr);
  esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID,
                                      &WifiAccessPoint::ip_event_handler,
                                      (void *)this, nullptr);
}

void WifiAccessPoint::stop_event_handler() {
  esp_event_handler_instance_unregister(
      WIFI_EVENT, ESP_EVENT_ANY_ID, (void *)&WifiAccessPoint::ap_event_handler);
  esp_event_handler_instance_unregister(
      IP_EVENT, ESP_EVENT_ANY_ID, (void *)&WifiAccessPoint::ip_event_handler);
}

void WifiAccessPoint::ap_event_handler(void *arg, esp_event_base_t event_base,
                                       int32_t event_id, void *event_data) {
  WifiAccessPoint *self = static_cast<WifiAccessPoint *>(arg);
  if (event_id == WIFI_EVENT_AP_STACONNECTED) {
    wifi_event_ap_staconnected_t *event =
        (wifi_event_ap_staconnected_t *)event_data;
    ESP_LOGI("HOST", "station join, AID=%d", event->aid);
    self->num_connected_stations_++;
  } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
    wifi_event_ap_stadisconnected_t *event =
        (wifi_event_ap_stadisconnected_t *)event_data;
    ESP_LOGI("HOST", "station leave, AID=%d", event->aid);
    self->num_connected_stations_--;
  }
}

void WifiAccessPoint::ip_event_handler(void *arg, esp_event_base_t event_base,
                                       int32_t event_id, void *event_data) {
  WifiAccessPoint *self = static_cast<WifiAccessPoint *>(arg);
  if (event_id == IP_EVENT_AP_STAIPASSIGNED) {
    ip_event_ap_staipassigned_t *event =
        (ip_event_ap_staipassigned_t *)event_data;
    char *ip_str = ip4addr_ntoa((ip4_addr_t *)&event->ip);
    ESP_LOGI("HOST", "Assigned IP to station: %s", ip_str);
    // You can store ip_str in a map with the MAC address if needed
    self->connected_stations_ips_.push_back(std::string(ip_str));
  }
}

bool WifiAccessPoint::set_dns_server(esp_netif_t *netif, uint32_t addr,
                                     esp_netif_dns_type_t type) {
  if (addr && (addr != IPADDR_NONE)) {
    esp_netif_dns_info_t dns;
    dns.ip.u_addr.ip4.addr = addr;
    dns.ip.type = IPADDR_TYPE_V4;
    return esp_netif_set_dns_info(netif, type, &dns) == ESP_OK;
  }
  return true;
}

void WifiAccessPoint::set_static_ip(const std::string &static_ip,
                                    const std::string &static_netmask,
                                    const std::string &static_gw) {
  if (esp_netif_dhcpc_stop(ap_netif_) != ESP_OK) {
    ESP_LOGE("HOST", "Failed to stop dhcp client");
    return;
  }
  esp_netif_ip_info_t ip;
  memset(&ip, 0, sizeof(esp_netif_ip_info_t));
  ip.ip.addr = ipaddr_addr(static_ip.c_str());
  ip.netmask.addr = ipaddr_addr(static_netmask.c_str());
  ip.gw.addr = ipaddr_addr(static_gw.c_str());
  esp_netif_dhcps_stop(ap_netif_);
  esp_err_t err = esp_netif_set_ip_info(ap_netif_, &ip);
  if (err != ESP_OK) {
    ESP_LOGE("HOST", "Failed to set ip info. error: %s", esp_err_to_name(err));
    return;
  }
  esp_netif_dhcps_start(ap_netif_);
  ESP_LOGD("HOST", "Success to set static ip: %s, netmask: %s, gw: %s",
           static_ip.c_str(), static_netmask.c_str(), static_gw.c_str());
  set_dns_server(ap_netif_, ipaddr_addr(static_gw.c_str()), ESP_NETIF_DNS_MAIN);
  set_dns_server(ap_netif_, ipaddr_addr("0.0.0.0"), ESP_NETIF_DNS_BACKUP);
}