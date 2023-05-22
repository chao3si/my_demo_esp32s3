#ifndef __FLUSH_PAGE_H
#define __FLUSH_PAGE_H

#include "lvgl.h"
#include "chaosi_wifi_ap_sta.h"
#include "esp_heap_caps.h"
#include "lv_port_disp.h"
#include "chaosi_encoder.h"
#include "lv_port_indev.h"
#include "chaosi_anime.h"
#include "bg_screen.h"

#include "chaosi_system_data.h"
#include "menu_template.h"

#include "flush_page.h"

lv_obj_t* get_flush_btn_obj();
void flush_page_anim_in(uint32_t delay, int flag);
void flush_page_anim_out(uint32_t delay, int flag);
void flush_page();

#endif
