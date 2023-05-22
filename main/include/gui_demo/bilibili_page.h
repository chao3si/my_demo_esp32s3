#ifndef __BILIBILI_PAGE_H
#define __BILIBILI_PAGE_H

#include "lvgl.h"
#include "chaosi_wifi_ap_sta.h"
#include "esp_heap_caps.h"
#include "lv_port_disp.h"
#include "chaosi_encoder.h"
#include "lv_port_indev.h"
#include "chaosi_anime.h"
#include "bg_screen.h"

void bilibili_page();
lv_obj_t* get_bilibili_btn_obj();
void bilibili_page_anim_in(uint32_t delay, int flag);
void bilibili_page_anim_out(uint32_t delay, int flag);

#endif
