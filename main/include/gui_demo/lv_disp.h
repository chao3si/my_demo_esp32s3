#ifndef __LV_DISP_H
#define __LV_DISP_H


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_err.h"

#include "../../../components/user_dev/user_dev_inc.h"
#include "../../../components/user_mid/User_mid_LCD.h"
#include "../../../components/lvgl/lvgl.h"
#include "../../../components/lvgl/lv_devices/lv_port_disp.h"
#include "../../../components/lvgl/lv_devices/lv_port_indev.h"

void lvgl_init();

#endif
 