#include "nvs_flash.h"
#include "rix/core/mediator.hpp"
#include "rix/core/node.hpp"
#include "rix/msg/standard/Header.hpp"
#include "rix/msg/standard/UInt64Array.hpp"
#include "wifi/wifi_access_point.hpp"
#include "wifi/wifi_station.hpp"
#include <memory>



std::shared_ptr<rix::core::Publisher> headerPub = nullptr;
std::shared_ptr<rix::core::Publisher> vecPub = nullptr;
std::vector<uint64_t> large_data;

void timer_callback(const rix::core::Timer::Event &event) {
  static int i = 0;
  if (headerPub->ok()) {
    printf("Timer callback: Publishing message #%d...\n", i);
    printf("Subscriber count: %zu\n", headerPub->get_subscriber_count());
    rix::msg::standard::Header header;
    header.frame_id = "Hello, world!";
    header.seq = i++;
    header.stamp = rix::util::Time::now().to_msg();
    headerPub->publish(header);
  }
}

void timer_callback_uint64(const rix::core::Timer::Event &event) {
  static int i = 0;
  if (vecPub->ok()) {
    printf("Timer callback: Publishing large vector #%d...\n", i++);
    printf("Subscriber count: %zu\n", vecPub->get_subscriber_count());
    rix::msg::standard::UInt64Array uint_msg;
    uint_msg.data = large_data;
    // Note: UInt64Array does not have a stamp field
    vecPub->publish(uint_msg);
  }
}

void fillUintVector(std::vector<uint64_t> &vec) {
  vec.resize(7500); // Resize to 7500 elements
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] = static_cast<uint64_t>(i % 256);
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
  headerPub = node->create_publisher<rix::msg::standard::Header>(
      "/chatter", rix::ipc::Endpoint(ip, 8000));

  // Publisher for large data (testing)
  vecPub = node->create_publisher<rix::msg::standard::UInt64Array>(
      "/large_data", rix::ipc::Endpoint(ip, 8001));
  if (!headerPub || !headerPub->ok() || !vecPub || !vecPub->ok()) {
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

  fillUintVector(large_data);
  auto timer_uint = node->create_timer(rix::util::Duration(0.5), timer_callback_uint64);
  if (!timer_uint || !timer_uint->ok()) {
    rix::util::Log::error << "Failed to create timer." << std::endl;
    return;
  }
  printf("Timer created successfully with 0.5s interval\n");

  while (node->ok()) {
    node->spin_once();                    // Check node health
    vTaskDelay(100 / portTICK_PERIOD_MS); // Check health every 100ms
  }
  // We should never get here
  for (;;) {
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Main thread just waits
  };
  return;
}