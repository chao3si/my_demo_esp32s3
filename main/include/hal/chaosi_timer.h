#ifndef __CHOASI_TIMER_H
#define __CHOASI_TIMER_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/timer.h"

#include "esp_log.h"

#include "chaosi_system_data.h"
#include "time_page.h"

#define TIMEGROUP_DIVIDER           16
#define ALARM_VALUE                 (TIMER_BASE_CLK / TIMEGROUP_DIVIDER / 1000 * 10)    //报警间隔始终是10ms

typedef struct {
    int timer_group;
    int timer_idx;
    int alarm_value;
    bool auto_reload;
} timer_info_t;

/*定时器设备*/
typedef struct {
    timer_info_t info;
    uint32_t timer_count_value;
    uint32_t timer_secont;
    uint32_t timer_minute;
    uint32_t timer_hour;
} timer_event_t;

void my_timer_init(void);

#endif
