#include "chaosi_http_request.h"


static const char *TAG = "http_request";

xQueueHandle http_request_queue;

typedef struct {
    char loaction[20];
    char weather[20];
    char weather_code[3];
    char temperature[4];
} weather_t;

/*
{
    "sysTime2":"2023-05-12 11:44:30",
    "sysTime1":"20230512114430"
}
*/

void cjson_time_info(char *text)
{
    cJSON *root;
    char buf[20];
    strcpy(text, strchr(text, '{'));
    root = cJSON_Parse(text);

    cJSON *sysTime1 = cJSON_GetObjectItem(root, "sysTime1");
    sprintf(buf, "%s", sysTime1->valuestring);
    cJSON_Delete(root);

    uint8_t hour = (buf[8] - '0')*10 + buf[9] - '0';
    uint8_t minute = (buf[10] - '0')*10 + buf[11] - '0';
    uint8_t second = (buf[12] - '0')*10 + buf[13] - '0';

    update_request_time(hour, minute, second);  //  将请求的时间保存
}

/*
{
    "code":0,
    "message":"0",
    "ttl":1,
    "data":{
        "mid":2828592,
        "following":298,
        "whisper":0,
        "black":0,
        "follower":201801
    }
}
*/
void cjson_fans_info(char *text)
{
    cJSON *root;
    strcpy(text, strchr(text, '{'));
    root = cJSON_Parse(text);

    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *follower = cJSON_GetObjectItem(data, "follower");
    
    uint32_t fans = 0;
    fans = follower->valueint; 

    cJSON_Delete(root);

    ESP_LOGI("cjson_fans_info", "fans is %d", fans);
}

// {
//     "results":[
//         {
//             "location":{
//                 "id":"WWYMRT0VRMUG",
//                 "name":"Dalian",
//                 "country":"CN",
//                 "path":"Dalian,Dalian,Liaoning,China",
//                 "timezone":"Asia/Shanghai",
//                 "timezone_offset":"+08:00"
//             },
//             "now":{
//                 "text":"Sand",
//                 "code":"27",
//                 "temperature":"20"
//             },
//             "last_update":"2023-05-21T10:50:17+08:00"
//         }
//     ]
// }

void cjson_weather_info(char *text)
{
    cJSON *root;
    root = cJSON_Parse(strchr(text, '{'));

    cJSON *results = cJSON_GetObjectItem(root, "results");
    cJSON *results_root = cJSON_GetArrayItem(results, 0);

    cJSON *location = cJSON_GetObjectItem(results_root, "location");
    
}

esp_err_t http_fans_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buf;
    static int output_len;
    int Content_Length = 0;

    switch(evt->event_id)
    {
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            Content_Length = esp_http_client_get_content_length(evt->client);
            if(!esp_http_client_is_chunked_response(evt->client))   //我这里没有写分块编码的部分，这部分api是普通编码
            {
                if(output_buf == NULL)
                {
                    output_buf = (char *)malloc(Content_Length);  //正文内容
                    output_len = 0;
                    if (output_buf == NULL) {
                        ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                        return ESP_FAIL;
                    }
                }
                memcpy(output_buf + output_len, evt->data, evt->data_len);
                output_len += evt->data_len;  //移动指针准备接受下一个块
            }
            printf("%.*s\n", output_len, output_buf);
            if(output_len == Content_Length)
            {
                cjson_fans_info(output_buf);
            }
            break;
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADERS_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_FINISH:
            if(output_buf != NULL)
            {
                free(output_buf);
                output_buf = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            if (output_buf != NULL) {
                free(output_buf);
                output_buf = NULL;
            }
            output_len = 0;
            break;
    }
    return ESP_OK;
}

esp_err_t http_time_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buf;
    static int output_len;
    int Content_Length = 0;

    switch(evt->event_id)
    {
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            Content_Length = esp_http_client_get_content_length(evt->client);
            if(!esp_http_client_is_chunked_response(evt->client))   //我这里没有写分块编码的部分，这部分api是普通编码
            {
                if(output_buf == NULL)
                {
                    output_buf = (char *)malloc(Content_Length);  //正文内容
                    output_len = 0;
                    if (output_buf == NULL) {
                        ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                        return ESP_FAIL;
                    }
                }
                memcpy(output_buf + output_len, evt->data, evt->data_len);
                output_len += evt->data_len;  //移动指针准备接受下一个块
            }
            printf("%.*s\n", output_len, output_buf);
            if(output_len == Content_Length)
            {
                cjson_time_info(output_buf);
            }
            break;
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADERS_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_FINISH:
            if(output_buf != NULL)
            {
                free(output_buf);
                output_buf = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            if (output_buf != NULL) {
                free(output_buf);
                output_buf = NULL;
            }
            output_len = 0;
            break;
    }
    return ESP_OK;
}

esp_err_t http_weather_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buf;
    static int output_len;
    int Content_Length = 0;

    switch(evt->event_id)
    {
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            Content_Length = esp_http_client_get_content_length(evt->client);
            if(!esp_http_client_is_chunked_response(evt->client))   //我这里没有写分块编码的部分，这部分api是普通编码
            {
                if(output_buf == NULL)
                {
                    output_buf = (char *)malloc(Content_Length);  //正文内容
                    output_len = 0;
                    if (output_buf == NULL) {
                        ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                        return ESP_FAIL;
                    }
                }
                memcpy(output_buf + output_len, evt->data, evt->data_len);
                output_len += evt->data_len;  //移动指针准备接受下一个块
            }
            printf("%.*s\n", output_len, output_buf);
            if(output_len == Content_Length)
            {
                cjson_weather_info(output_buf);
            }
            break;
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADERS_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_FINISH:
            if(output_buf != NULL)
            {
                free(output_buf);
                output_buf = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            if (output_buf != NULL) {
                free(output_buf);
                output_buf = NULL;
            }
            output_len = 0;
            break;
    }
    return ESP_OK;
}


void http_get_time()
{
    char url[100];
    strcpy(url, "http://quan.suning.com/getSysTime.do");

    esp_http_client_config_t time_req = {       //请求结构体
        .method = HTTP_METHOD_GET,
        .url = url,
        .event_handler = http_time_event_handler,
        .skip_cert_common_name_check = true,
    };
    
    esp_http_client_handle_t time_client = esp_http_client_init(&time_req); //初始化配置
    esp_err_t err = esp_http_client_perform(time_client);                   //执行请求

    if(err == ESP_OK)
	{
		ESP_LOGI(TAG, "Status = %d, content_length = %d",
				esp_http_client_get_status_code(time_client),       //状态码
				esp_http_client_get_content_length(time_client));   //数据长度
	}
	esp_http_client_cleanup(time_client);   //断开并释放资源
}

void http_get_fans()
{
    char url[100];
    strcpy(url, "http://api.bilibili.com/x/relation/stat?vmid=2828592&jsonp=jsonp");

    esp_http_client_config_t fans_req = {       //请求结构体
        .method = HTTP_METHOD_GET,
        .url = url,
        .event_handler = http_fans_event_handler,
        .skip_cert_common_name_check = true,
    };
    
    esp_http_client_handle_t fans_client = esp_http_client_init(&fans_req); //初始化配置
    esp_err_t err = esp_http_client_perform(fans_client);                   //执行请求

    if(err == ESP_OK)
	{
		ESP_LOGI(TAG, "Status = %d, content_length = %d",
				esp_http_client_get_status_code(fans_client),       //状态码
				esp_http_client_get_content_length(fans_client));   //数据长度
	}
	esp_http_client_cleanup(fans_client);   //断开并释放资源
}

void http_get_weather()
{
    char url[200];
    strcpy(url, "http://api.seniverse.com/v3/weather/now.json?key=SmazqPcltzTft-X3v&location=guangzhou&language=zh-Hans&unit=c");

    esp_http_client_config_t weather_req = {       //请求结构体
        .method = HTTP_METHOD_GET,
        .url = url,
        .event_handler = http_weather_event_handler,
        .skip_cert_common_name_check = true,
    };
    
    esp_http_client_handle_t weather_client = esp_http_client_init(&weather_req); //初始化配置
    esp_err_t err = esp_http_client_perform(weather_client);                   //执行请求

    if(err == ESP_OK)
	{
		ESP_LOGI(TAG, "Status = %d, content_length = %d",
				esp_http_client_get_status_code(weather_client),       //状态码
				esp_http_client_get_content_length(weather_client));   //数据长度
	}
	esp_http_client_cleanup(weather_client);   //断开并释放资源
}

static void http_request_task()
{
    while(1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        HTTP_REQUEST_TYPE evt;
        xQueueReceive(http_request_queue, &evt, portMAX_DELAY);

        switch (evt)
        {
            case HTTP_FAN_REQUEST:
                http_get_fans();
                break;
            case HTTP_TIME_REQUEST:
                http_get_time();
                break;
            
            default:
                break;
        }
    }

}

void send_http_request_all()
{
    send_http_request(HTTP_FAN_REQUEST);
    send_http_request(HTTP_TIME_REQUEST);
}

void send_http_request(HTTP_REQUEST_TYPE type)
{
    HTTP_REQUEST_TYPE evt = type;
    xQueueSend(http_request_queue, &evt, portMAX_DELAY);
}

void http_request_init()
{ 
    http_request_queue = xQueueCreate(10, sizeof(HTTP_REQUEST_TYPE));
    xTaskCreatePinnedToCore(http_request_task, "http_request_task", 4096*2, NULL, 1, NULL, APP_CPU_NUM);
}
