#include "chaosi_system_data.h"

//const static char *TAG = "system_data";

DEV_INFO DesktopScreen;
void system_data_init()
{
    memset(&DesktopScreen, 0, sizeof(DEV_INFO));

    DesktopScreen.hour = 12;
    DesktopScreen.minute = 34;
    DesktopScreen.second = 50;
    DesktopScreen.first_time_request = 1;
}

//设置wifi信息
void set_system_wifi_info(char *ssid, char *psw)
{
    uint8_t ssid_len = strlen(ssid);
    uint8_t psw_len = strlen(psw);
    DesktopScreen.wifi_ssid_len = ssid_len;
    DesktopScreen.wifi_psw_len = psw_len;
    memcpy(DesktopScreen.wifi_ssid, ssid, ssid_len);
    memcpy(DesktopScreen.wifi_psw, psw, psw_len);
    DesktopScreen.wifi_ssid[ssid_len] = '\0';
    DesktopScreen.wifi_psw[psw_len] = '\0';
    DesktopScreen.has_wifi_info =  1;
}
//打印wifi信息
void printf_wifi_info()
{
    printf("ssid :");
    for(int i = 0; i < DesktopScreen.wifi_ssid_len; i++)
    {
        printf("%c", DesktopScreen.wifi_ssid[i]);
    }
    printf("\r\npsw :");
    for(int i = 0; i < DesktopScreen.wifi_psw_len; i++)
    {
        printf("%c", DesktopScreen.wifi_psw[i]);
    }
    printf("\r\n");
}

void set_wifi_ap_isopen(bool val)
{
    DesktopScreen.wifi_ap_isopen = val;
}

bool get_wifi_ap_isopen()
{
    return DesktopScreen.wifi_ap_isopen;
}

void set_wifi_sta_status(WIFI_STA_STATUS_E status)
{
    DesktopScreen.wifi_sta_status = status;
}

WIFI_STA_STATUS_E get_wifi_sta_status()
{
    return DesktopScreen.wifi_sta_status;
}

void set_wifi_ap_status(WIFI_AP_STATUS_E status)
{
    DesktopScreen.wifi_ap_status = status;
}

WIFI_AP_STATUS_E get_wifi_ap_status()
{
    return DesktopScreen.wifi_ap_status;
}

void set_has_http_requst(bool val)
{
    DesktopScreen.first_time_request = val;
}

bool get_has_http_requst()
{
    return DesktopScreen.first_time_request;
}

void set_wifi_sta_isopen(bool val)
{
    DesktopScreen.wifi_sta_isopen = val;
}

bool get_wifi_sta_isopen()
{
    return DesktopScreen.wifi_sta_isopen;
}

void update_request_time(uint8_t hour, uint8_t minute, uint8_t second)
{
    DesktopScreen.hour = hour;
    DesktopScreen.minute = minute;
    DesktopScreen.second = second;
}

void update_system_time()
{
    if(DesktopScreen.second >= 59)
    {
        DesktopScreen.second = 0;
        if(DesktopScreen.minute >= 59)
        {
            DesktopScreen.minute = 0;
            if(DesktopScreen.hour >= 23)
            {
                DesktopScreen.hour = 0;
            }
            else
            {
                DesktopScreen.hour++;
            }
        }
        else
        {
            DesktopScreen.minute++;
        }
    }
    else
    {
        DesktopScreen.second++;
    }
}

//返回当前桌面显示设备信息
DEV_INFO get_system_data()
{
    return DesktopScreen;
}