#pragma once

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_eap_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "lwip/sys.h"
#include <cstdint>
#include <cstring>

class WifiBase {
public:
  WifiBase();
  virtual ~WifiBase();
  bool ok() const;

protected:
  wifi_init_config_t wifi_cfg_;
  bool wifi_started_;
};