#include "nvs_flash.h"
#include "rix/core/mediator.hpp"
#include "rix/core/node.hpp"
#include "rix/msg/standard/Header.hpp"
#include "wifi/wifi_access_point.hpp"
#include "wifi/wifi_station.hpp"
#include <memory>

std::shared_ptr<rix::core::Publisher> pub = nullptr;

void timer_callback(const rix::core::Timer::Event &event) {
  static int i = 0;
  if (pub) {
    rix::msg::standard::Header header;
    header.frame_id = "Hello, world!";
    header.seq = i++;
    header.stamp = rix::util::Time::now().to_msg();
    pub->publish(header);
  }
}

extern "C" void app_main() {
  // Init Wi-Fi (host access point)
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // WifiAccessPoint ap("esp_ap_test", "password", "192.168.4.2", 11, 0, 3);
  // ap.wait_for_new_connection(60000 * 5);

  // std::vector<std::string> ips;
  // while (ips.empty()) {
  //   ap.get_connected_stations_ips(ips);
  //   vTaskDelay(500 / portTICK_PERIOD_MS);
  // }

  // WifiStation sta(5);

  // Do a scan
  // std::vector<wifi_ap_record_t> ap_records;
  // sta.scan(ap_records, true, 64);

  // for (const auto &ap : ap_records) {
  //   rix::util::Log::info << "SSID: " << ap.ssid
  //                        << ", RSSI: " << static_cast<int>(ap.rssi)
  //                        << ", Channel: " << static_cast<int>(ap.primary)
  //                        << std::endl;
  // }
  // return;

  // rix::util::Log::info << "Connecting to Wi-Fi..." << std::endl;
  // sta.connect_enterprise("eduroam", "email@umich.edu", "password");
  // sta.wait_for_connection(30000);
  // if (!sta.is_connected()) {
  //   rix::util::Log::error << "Failed to connect to Wi-Fi." << std::endl;
  //   return;
  // }

  // std::string ip = sta.get_ip();
  // if (ip.empty()) {
  //   rix::util::Log::error << "Failed to get IP address." << std::endl;
  //   return;
  // }
  // rix::util::Log::info << "Connected! IP: " << ip << std::endl;

  // // Init rixhub
  // auto mediator =
  //     std::make_shared<rix::core::Mediator>(rix::ipc::Endpoint("0.0.0.0",
  //     48104));

  // if (!mediator->ok()) {
  //   rix::util::Log::error << "Failed to create rixhub." << std::endl;
  //   return;
  // }

  // Spin
  // mediator->spin();

  // std::shared_ptr<rix::core::Node> node;
  // while (true) {
  //   node = std::make_shared<rix::core::Node>("test_node",
  //                                            rix::ipc::Endpoint(ips[0],
  //                                            48104));
  //   if (!node->ok()) {
  //     rix::util::Log::error << "Failed to create node." << std::endl;
  //     vTaskDelay(1000 / portTICK_PERIOD_MS);
  //     continue;
  //   }
  //   break;
  // }

  // pub = node->create_publisher<rix::msg::standard::Header>("/chatter",
  // rix::ipc::Endpoint("192.168.4.2", 8000)); auto timer =
  // node->create_timer(rix::util::Duration(1.0), timer_callback);

  // node->spin();

  std::vector<std::shared_ptr<rix::ipc::GenericSocket>> sockets;
  sockets.push_back(rix::ipc::create_socket());
  sockets.push_back(rix::ipc::create_socket());
  sockets.push_back(rix::ipc::create_socket());

  std::vector<std::shared_ptr<rix::ipc::GenericSocket>> readable_sockets;
  std::vector<std::shared_ptr<rix::ipc::GenericSocket>> exception_sockets;
  auto ready_sockets = rix::ipc::select(
      readable_sockets, exception_sockets, sockets.begin(), sockets.end(),
      rix::util::Duration(0.0), rix::ipc::SelectFlag::READ);

  return;
}