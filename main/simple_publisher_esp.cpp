#include "nvs_flash.h"
#include "rix/core/node.hpp"
#include "rix/std_msgs/Header.hpp"
#include "wifi/wifi_access_point.hpp"
#include "wifi/wifi_station.hpp"
#include <memory>

std::shared_ptr<rix::Publisher> headerPub = nullptr;
const char *ca_cert_pem = R"(
  -----BEGIN CERTIFICATE-----
MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB
iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl
cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV
BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw
MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV
BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU
aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy
dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK
AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B
3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY
tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/
Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2
VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT
79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6
c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT
Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l
c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee
UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE
Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd
BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G
A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF
Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO
VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3
ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs
8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR
iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze
Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ
XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/
qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB
VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB
L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG
jjxDah2nGN59PRbxYvnKkKj9
-----END CERTIFICATE-----
  )";


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
    rix::Log::info << "SSID: " << ap.ssid
                   << ", RSSI: " << static_cast<int>(ap.rssi)
                   << ", Channel: " << static_cast<int>(ap.primary)
                   << std::endl;
  }
  rix::Log::info << "Connecting to Wi-Fi..." << std::endl;
  //sta.connect_enterprise("eduroam", "umid@umich.edu", "password",
  //                       ca_cert_pem);
  sta.connect("Sputnik", "calibird");
  sta.wait_for_connection(50000);
  if (!sta.is_connected()) {
    rix::Log::error << "Failed to connect to Wi-Fi." << std::endl;
    return;
  }

  std::string ip = sta.get_ip();
  if (ip.empty()) {
    rix::Log::error << "Failed to get IP address." << std::endl;
    return;
  }
  std::string mac_addr = sta.get_mac_address();
  if (mac_addr.empty()) {
    rix::Log::error << "Failed to get MAC address." << std::endl;
    return;
  }
  rix::Log::info << "Connected! IP: " << ip << ", MAC: " << mac_addr
                 << std::endl;

  rix::TaskConfig node_config;
  node_config.STACK_SIZE = 4096;
  node_config.PRIORITY = 5;
  node_config.MAX_TIMEOUT = rix::Duration(2.0);
  std::shared_ptr<rix::Node> node = std::make_shared<rix::Node>(
      "ESP_Node", node_config, rix::Endpoint(rix::RIXHUB_IP, rix::RIXHUB_PORT));
  if (!node->ok()) {
    rix::Log::error << "Failed to create node." << std::endl;
    return;
  }

  rix::TaskConfig pub_config;
  pub_config.STACK_SIZE = 4096;
  pub_config.PRIORITY = 5;
  pub_config.MAX_TIMEOUT = rix::Duration(2.0);
  headerPub = node->create_publisher<rix::std_msgs::Header>(
      "/chatter", pub_config,
      rix::Endpoint(rix::RIXHUB_IP, 8000));
  if (!headerPub || !headerPub->ok()) {
    rix::Log::error << "Failed to create publisher." << std::endl;
    return;
  }

  rix::TaskConfig timer_config;
  timer_config.STACK_SIZE = 4096;
  timer_config.PRIORITY = 5;
  timer_config.MAX_TIMEOUT = rix::Duration(1.0);
  auto timer = node->create_timer(
      timer_config, [](const rix::TimerCallback::Event &event) {
        static uint32_t seq = 0;
        rix::std_msgs::Header header_msg;
        header_msg.seq = seq++;
        header_msg.stamp = rix::Time::now().to_msg();
        header_msg.frame_id = "esp32_frame";
        if (headerPub && headerPub->ok()) {
          headerPub->publish(header_msg);
          rix::Log::info << "Published header message with seq: " << header_msg.seq
                         << std::endl;
        }
      });
  if (!timer || !timer->ok()) {
    rix::Log::error << "Failed to create timer." << std::endl;
    return;
  }
  // We should never get here
  for (;;) {
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Main thread just waits
  };
  return;
}