/* Non-Volatile Storage (NVS) Read and Write a Value - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

#include "chaosi_nvs.h"
#include "chaosi_system_data.h"

const static char *TAG = "chaosi_nvs";
//初始化nvs
void nvs_wifi_init()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        //ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
}
//包括擦除
void nvs_wifi_erase()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
}

//清除注册进去的wifi信息
void nvs_wifi_info_clean()
{
    esp_err_t err;
    //err = nvs_flash_init();

    nvs_handle_t nvs_handle;
    err = nvs_open("wificonfig", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    uint8_t wifi_config = NVS_WIFI_INFO_NULL; 
    ESP_ERROR_CHECK(nvs_set_u8(nvs_handle, "wifi_flag", (uint8_t)wifi_config));
    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
}

//读出flash内部加密的的wifi信息
NVS_WIFI_STATUS nvs_read_wifi_info()
{
    esp_err_t err;
    //nvs_wifi_init();

    nvs_handle_t nvs_handle;
    err = nvs_open("wificonfig", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return NVS_WIFI_INFO_ERROR;
    }

    uint8_t wifi_config = NVS_WIFI_INFO_NULL; //查看是否存在wifi信息
    err = nvs_get_u8(nvs_handle, "wifi_flag", &wifi_config);
    if(err != ESP_OK)
    {
        ESP_LOGI(TAG, "The key is null");
        return NVS_WIFI_INFO_ERROR;
    }
    
    if(wifi_config == NVS_WIFI_INFO_SAVE)
    {
        ESP_LOGI(TAG, "There has wifi imformation!");
        char ssid[32];
        char psw[64];
        size_t ssid_len = sizeof(ssid);
        size_t psw_len = sizeof(psw);
        ESP_ERROR_CHECK(nvs_get_str(nvs_handle, "ssid", ssid, &ssid_len));
        ESP_ERROR_CHECK(nvs_get_str(nvs_handle, "psw", psw, &psw_len));
        set_system_wifi_info(ssid, psw);
        printf_wifi_info();
    }else{
        ESP_LOGI(TAG, "There has no wifi imformation!");
    }
    nvs_close(nvs_handle);
    return wifi_config;
}
//向flash中写入wifi信息
void nvs_write_wifi_info(char *ssid, char *psw)
{
    esp_err_t err;
    //nvs_wifi_init();

    //set_system_wifi_info(ssid, psw);
    nvs_handle_t nvs_handle;
    err = nvs_open("wificonfig", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }

    uint8_t wifi_flag = NVS_WIFI_INFO_SAVE; //写入wifi信息标志
    ESP_ERROR_CHECK(nvs_set_u8(nvs_handle, "wifi_flag", (uint8_t)wifi_flag));
    ESP_ERROR_CHECK(nvs_set_str(nvs_handle, "ssid", ssid));
    ESP_ERROR_CHECK(nvs_set_str(nvs_handle, "psw", psw));
    ESP_ERROR_CHECK(nvs_commit(nvs_handle));
    nvs_close(nvs_handle);

    ESP_LOGI(TAG, "NVS is successfully Writed");
}

