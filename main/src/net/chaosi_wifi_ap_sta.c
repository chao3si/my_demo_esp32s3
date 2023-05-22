#include "chaosi_wifi_ap_sta.h"

#define ESP_AP_WIFI_SSID            "chaosi"
#define ESP_AP_WIFI_PASS            "chaosi_password"

#define ESP_CHANNEL                 11
#define ESP_MAX_CONNECTION          3

#define ESP_MAXIMUM_RETRY           0
 
#define WIFI_CONNECTED_BIT          BIT0
#define WIFI_FAIL_BIT               BIT1

static const char *TAG = "wifi_ap_sta";
/*设置重连次数*/
static int s_retry_num = 0;
/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;
/*esp_netif_t  lwip协议栈初始化句柄，用于管理生命周期*/
esp_netif_t *sta = NULL;
esp_netif_t *ap = NULL;
/*注册回调函数句柄，用于生命周期管理*/
esp_event_handler_instance_t instance_any_id;
esp_event_handler_instance_t instance_got_ip;
/*wifi消息队列*/
QueueHandle_t wifi_event_queue;

//static SemaphoreHandle_t wifi_cb_mutex = NULL;

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    //AP相关回调函数
    if(event_id == WIFI_EVENT_AP_START)
    {
        set_wifi_ap_status(WIFI_SET_AP_MODE_SUCCESS);
    }
    else if (event_id == WIFI_EVENT_AP_STOP)
    {
        set_wifi_ap_status(WIFI_SET_AP_MODE_FAILED);
    }
    else if (event_id == WIFI_EVENT_AP_STACONNECTED) 
    {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } 
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) 
    {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }

    //STA相关回调函数
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_STOP)
    {
        ESP_LOGI(TAG, "WIFI_EVENT_STA_STOP");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        esp_wifi_connect();
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        if (s_retry_num < ESP_MAXIMUM_RETRY) 
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } 
        else 
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_ap_sta_init()
{
    /*初始化事件组循环*/
    s_wifi_event_group = xEventGroupCreate();
    /*初始化协议栈*/ 
    ESP_ERROR_CHECK(esp_netif_init());
    /*初始化wifi驱动*/
    sta = esp_netif_create_default_wifi_sta();
    ap = esp_netif_create_default_wifi_ap();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    /*注册回调函数*/
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA) );
                                                        
    /*wifi配置阶段*/
    wifi_config_t wifi_sta_config = {
        .sta = {
            //.ssid = EXAMPLE_ESP_WIFI_SSID,
            //.password = EXAMPLE_ESP_WIFI_PASS,
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,
	     .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };

    memcpy(wifi_sta_config.sta.ssid,get_system_data().wifi_ssid, 32);
    memcpy(wifi_sta_config.sta.password,get_system_data().wifi_psw,64);

    wifi_config_t wifi_ap_config = {
        .ap = {
            .ssid = ESP_AP_WIFI_SSID,
            .password = ESP_AP_WIFI_PASS,
            .ssid_len = strlen(ESP_AP_WIFI_SSID),
            .channel = ESP_CHANNEL,
            .max_connection = ESP_MAX_CONNECTION,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };


    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_sta_config) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_config) );
    //ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "wifi_init_ap_sta finished.");


    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",wifi_ap_config.ap.ssid, wifi_ap_config.ap.password, wifi_ap_config.ap.channel);
}

static void wifi_update_sta_data()
{
    wifi_config_t wifi_sta_config = {
        .sta = {
            //.ssid = EXAMPLE_ESP_WIFI_SSID,
            //.password = EXAMPLE_ESP_WIFI_PASS,
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,
	     .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };

    memcpy(wifi_sta_config.sta.ssid,get_system_data().wifi_ssid, 32);
    memcpy(wifi_sta_config.sta.password,get_system_data().wifi_psw,64);

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_sta_config) );
}

void wifi_send_event(WIFI_SET_EVENT_E evt)
{
    xQueueSend(wifi_event_queue, &evt, portMAX_DELAY);
}

static void wifi_net_task()
{
    /*初始化服务器*/
    http_server_start();

    EventBits_t bits;
    WIFI_SET_EVENT_E evt;
    while(1)
    {
        vTaskDelay(100); 
        xQueueReceive(wifi_event_queue, &evt, portMAX_DELAY);

        switch(evt)
        {
            case WIFI_STA_START:
                esp_wifi_start();
                break;
            case WIFI_STA_UPDATE:
                wifi_update_sta_data();
                esp_wifi_connect(); //应该可以做到断线重连
                break;
            case WIFI_STA_STOP:
                esp_wifi_stop();
                set_wifi_ap_status(WIFI_AP_MODE_INIT);
                ESP_LOGI("set_wifi_ap_status", "WIFI_AP_MODE_INIT");
                set_wifi_sta_status(WIFI_STA_MODE_INIT);
                ESP_LOGI("set_wifi_sta_status", "WIFI_STA_MODE_INIT");
                break;

            default:
                break;
        }

        bits = xEventGroupWaitBits(s_wifi_event_group,
                                    WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                    pdTRUE,
                                    pdFALSE,
                                    (10*1000 / portTICK_PERIOD_MS));

        if (bits & WIFI_CONNECTED_BIT) {
            set_wifi_sta_status(WIFI_STA_MODE_CONNECT_SUCCESS);
            ESP_LOGI(TAG, "WIFI_STA_MODE_CONNECT_SUCCESS");
        } else if (bits & WIFI_FAIL_BIT) {
            set_wifi_sta_status(WIFI_STA_MODE_CONNECT_FAILED);
            ESP_LOGI(TAG, "WIFI_STA_MODE_CONNECT_FAILED");
        } else {
            ESP_LOGE(TAG, "UNEXPECTED EVENT");
        }
    }
    
}

void wifi_net_init()
{
    wifi_event_queue = xQueueCreate(3, sizeof(WIFI_SET_EVENT_E));
    xTaskCreatePinnedToCore(wifi_net_task, "wifi_net_task", 4096, NULL, 1, NULL, APP_CPU_NUM);
    //wifi_cb_mutex = xSemaphoreCreateMutex();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_ap_sta_init();
}

void wifi_ap_sta_stop()
{
    ESP_LOGI(TAG, "Stopping the wifi...\r\n");
    /* The event will not be processed after unregister */
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler));
    /*清理事件组*/
    vEventGroupDelete(s_wifi_event_group);
    /*断开wifi*/
    ESP_ERROR_CHECK(esp_wifi_stop());
    /*终止wifi驱动程序*/
    ESP_ERROR_CHECK(esp_wifi_deinit());
    /*清理wifi驱动*/
    ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(ap));
    ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(sta));
    esp_netif_destroy_default_wifi(ap);
    esp_netif_destroy_default_wifi(sta);
    ESP_LOGI(TAG, "wifi has been stopped\r\n");
}