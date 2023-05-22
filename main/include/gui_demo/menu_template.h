#ifndef __MENU_TEMPLATE_H
#define __MENU_TEMPLATE_H

#include "lvgl.h"
#include "chaosi_wifi_ap_sta.h"
#include "esp_heap_caps.h"
#include "lv_port_disp.h"
#include "chaosi_encoder.h"
#include "lv_port_indev.h"
#include "chaosi_anime.h"

#include "bg_screen.h"
#include "time_page.h"
#include "bilibili_page.h"
#include "wifi_page.h"
#include "home_page.h"
#include "flush_page.h"

#define TOTAL_PAGE          4

// #define HOME_PAGE           0
// #define TIME_PAGE           1
// #define WEATHER_PAGE        2
// #define BILIBILI_PAGE       3

typedef enum {
    HOME_PAGE = 0,
    TIME_PAGE,
    WIFI_PAGE,
    BILIBILI_PAGE,
    FLUSH_PAGE,
} PAGE_TYPE_E;

typedef struct {
    PAGE_TYPE_E page;
    bool status;
} PAGE_MANAGE_T;

extern PAGE_MANAGE_T ui;

void menu_page(void);
PAGE_MANAGE_T get_now_show_page();
void menu_page_remove(uint32_t delay);
void back_to_menu_page(int flag);

#endif