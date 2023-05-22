#ifndef __CHAOSI_WIFI_AP_STA_H
#define __CHAOSI_WIFI_AP_STA_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "chaosi_system_data.h"
#include "chaosi_http_server.h"
#include "chaosi_http_request.h"


typedef enum {
    WIFI_AP_START = 0,
    WIFI_STA_START,
    WIFI_STA_UPDATE,
    WIFI_AP_STOP,
    WIFI_STA_STOP,
} WIFI_SET_EVENT_E;

//void wifi_ap_sta_start();
void wifi_ap_sta_stop();
void wifi_send_event(WIFI_SET_EVENT_E evt);
void wifi_net_init();
void wifi_ap_sta_init();
void wifi_start();

void ds_wifi_ap_sta_start(void);
void ds_wifi_ap_sta_stop(void);
#endif

