#ifndef __CHAOSI_NVS_H
#define __CHAOSI_NVS_H


typedef enum{
    NVS_WIFI_INFO_ERROR = 0,
    NVS_WIFI_INFO_NULL,
    NVS_WIFI_INFO_SAVE,
}NVS_WIFI_STATUS;

void nvs_wifi_init();
NVS_WIFI_STATUS nvs_read_wifi_info();
void nvs_write_wifi_info(char *ssid, char *psw);
void nvs_wifi_info_clean();
void nvs_wifi_erase();


#endif
