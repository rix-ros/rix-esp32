#pragma once

#include "wifi_base.hpp"
#include <string>
#include <vector>

class WifiAccessPoint : public WifiBase {
public:
  WifiAccessPoint(const std::string &ssid, const std::string &password,
                  const std::string &ip, uint8_t channel, uint8_t hidden,
                  uint8_t max_connections);
  ~WifiAccessPoint();

  bool ok() const;

  void hide_ssid();
  void show_ssid();
  bool is_hidden() const;
  std::string get_ssid() const;
  std::string get_password() const;

  bool wait_for_new_connection(int32_t timeout_ms);

  void get_connected_stations_ips(std::vector<std::string> &ips) const;

private:
  wifi_config_t cfg_;
  bool access_point_hidden_;
  bool access_point_hosting_;
  esp_netif_t *ap_netif_;
  int num_connected_stations_;
  std::vector<std::string> connected_stations_ips_;

  void start_event_handler();
  void stop_event_handler();
  static void ap_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data);

  static void ip_event_handler(void *arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data);

  bool set_dns_server(esp_netif_t *netif, uint32_t addr,
                      esp_netif_dns_type_t type);

  void set_static_ip(const std::string &static_ip,
                     const std::string &static_netmask,
                     const std::string &static_gw);
};