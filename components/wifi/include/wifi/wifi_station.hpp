#pragma once

#include <string>
#include <vector>

#include "wifi_base.hpp"

class WifiStation : public WifiBase {
public:
  WifiStation(int16_t max_attempts);
  ~WifiStation();

  bool ok() const;

  void scan(std::vector<wifi_ap_record_t> &ap_records, bool show_hidden = false,
            uint16_t max_aps = 16);

  bool connect(const std::string &ssid, const std::string &password);
  bool connect_enterprise(const std::string &ssid, const std::string &username,
                          const std::string &password, const char *ca_cert);

  std::string get_ip() const;
  std::string get_mac_address() const;
  bool wait_for_connection(int32_t timeout_ms);
  void disconnect();

  bool is_connected() const;

private:
  wifi_config_t cfg_;
  int16_t conn_attempts_;
  int16_t max_conn_attempts_;
  bool in_station_mode_;
  bool station_connected_;
  bool station_connecting_;
  EventGroupHandle_t sta_event_group_;

  void start_event_handler();
  void stop_event_handler();
  static void sta_event_handler(void *arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data);
};