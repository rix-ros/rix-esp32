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
  if (pub->ok()) {
    printf("Timer callback: Publishing message #%d...\n", i);
    printf("Subscriber count: %zu\n", pub->get_subscriber_count());
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

  WifiStation sta(10);

  // Do a scan
  std::vector<wifi_ap_record_t> ap_records;
  sta.scan(ap_records, true, 64);

  for (const auto &ap : ap_records) {
    rix::util::Log::info << "SSID: " << ap.ssid
                         << ", RSSI: " << static_cast<int>(ap.rssi)
                         << ", Channel: " << static_cast<int>(ap.primary)
                         << std::endl;
  }
  rix::util::Log::info << "Connecting to Wi-Fi..." << std::endl;
  // sta.connect_enterprise("eduroam", "email@umich.edu", "password");
  sta.connect("Robolink", "i<3robots!");
  sta.wait_for_connection(50000);
  if (!sta.is_connected()) {
    rix::util::Log::error << "Failed to connect to Wi-Fi." << std::endl;
    return;
  }

  std::string ip = sta.get_ip();
  if (ip.empty()) {
    rix::util::Log::error << "Failed to get IP address." << std::endl;
    return;
  }
  rix::util::Log::info << "Connected! IP: " << ip << std::endl;
  std::shared_ptr<rix::core::Node> node = std::make_shared<rix::core::Node>(
      "ESP_Node", rix::ipc::Endpoint("192.168.0.124", 8000));
  if (!node->ok()) {
    rix::util::Log::error << "Failed to create node." << std::endl;
    return;
  }
  pub = node->create_publisher<rix::msg::standard::Header>(
      "/chatter", rix::ipc::Endpoint(ip, 8000));

  if (!pub || !pub->ok()) {
    rix::util::Log::error << "Failed to create publisher." << std::endl;
    return;
  }
  printf("Publisher created successfully on %s:8000\n", ip.c_str());

  auto timer = node->create_timer(rix::util::Duration(1.0), timer_callback);
  if (!timer || !timer->ok()) {
    rix::util::Log::error << "Failed to create timer." << std::endl;
    return;
  }
  printf("Timer created successfully with 1.0s interval\n");
  while (node->ok()) {
    node->spin_once();                    // Check node health
    vTaskDelay(100 / portTICK_PERIOD_MS); // Check health every 100ms
  }
  for (;;) {
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Main thread just waits
  };
  return;
}