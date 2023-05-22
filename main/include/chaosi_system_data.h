#ifndef __CHAOSI_SYSTEM_DATA__H
#define __CHAOSI_SYSTEM_DATA__H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define WIFI_SSID_MAXLEN            32
#define WIFI_PSW_MAXLEN             64

typedef enum {
    WIFI_STA_MODE_INIT = 0,
    WIFI_STA_MODE_CONNECT_SUCCESS,
    WIFI_STA_MODE_CONNECT_FAILED,
} WIFI_STA_STATUS_E;

typedef enum {
    WIFI_AP_MODE_INIT = 0,
    WIFI_SET_AP_MODE_SUCCESS,
    WIFI_SET_AP_MODE_FAILED,
} WIFI_AP_STATUS_E;

typedef struct DEV_INFO
{
    /*WIFI相关信息*/
    char wifi_ssid[WIFI_SSID_MAXLEN];
    uint8_t wifi_ssid_len;
    char wifi_psw[WIFI_PSW_MAXLEN];
    uint8_t wifi_psw_len;
    uint8_t has_wifi_info;

    /*网络状态相关*/
    WIFI_STA_STATUS_E wifi_sta_status;
    bool wifi_ap_status;
    WIFI_AP_STATUS_E wifi_sta_isopen;
    bool wifi_ap_isopen;

    /*request相关*/
    bool first_time_request;

    /*系统时间相关信息*/
    uint8_t second;
    uint8_t minute;
    uint8_t hour;

} DEV_INFO;


/*****************************系统相关**************************/
DEV_INFO get_system_data();
void system_data_init();
/**************************WIFI相关*****************************/
void set_system_wifi_info(char *ssid, char *psw);
void printf_wifi_info();
/**************************WIFI状态相关*****************************/
void set_wifi_sta_isopen(bool val);
bool get_wifi_sta_isopen();
void set_wifi_ap_isopen(bool val);
bool get_wifi_ap_isopen();
void set_wifi_sta_status(WIFI_STA_STATUS_E status);
WIFI_STA_STATUS_E get_wifi_sta_status();
void set_wifi_ap_status(WIFI_AP_STATUS_E status);
WIFI_AP_STATUS_E get_wifi_ap_status();
/**************************http请求状态相关*****************************/
void set_has_http_requst(bool val);
bool get_has_http_requst();
/**************************系统时间相关*****************************/
void update_system_time();
void update_request_time(uint8_t hour, uint8_t minute, uint8_t second);

#endif
