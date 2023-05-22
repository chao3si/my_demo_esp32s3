#ifndef _CHAOSI_HTTP_REQUEST_H_
#define _CHAOSI_HTTP_REQUEST_H_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_http_client.h"
#include "esp_tls.h"

#include "cJSON.h"

#include "chaosi_nvs.h"
#include "chaosi_system_data.h"
#include "MyfileSystem.h"

typedef enum {
    HTTP_FAN_REQUEST = 0,
    HTTP_TIME_REQUEST
}HTTP_REQUEST_TYPE;

void send_http_request(HTTP_REQUEST_TYPE type);
void http_request_init();
void send_http_request_all();

#endif