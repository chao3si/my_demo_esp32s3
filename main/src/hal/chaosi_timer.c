#include "chaosi_timer.h"

timer_event_t evt;
static xQueueHandle timer_queue;

static bool IRAM_ATTR timer_group_isr_callback(void *args)
{
    timer_info_t *info = (timer_info_t *) args;

    /* Prepare basic event data that will be then sent back to task */
    
    evt.info.timer_group = info->timer_group;
    evt.info.timer_idx = info->timer_idx;
    evt.info.auto_reload = info->auto_reload;
    evt.info.alarm_value = info->alarm_value;
    evt.timer_count_value++;
    
    if(evt.timer_count_value >= 100)
    {
        evt.timer_count_value = 0;
        evt.timer_secont++;
        update_system_time();
        xQueueSendFromISR(timer_queue, &evt, NULL);
    }
    
    return pdTRUE;
}
/**
 * @brief Initialize selected timer of timer group
 *
 * @param group Timer Group number, index from 0
 * @param timer timer ID, index from 0
 * @param auto_reload whether auto-reload on alarm event
 * @param timer_interval_sec interval of alarm
 */
static void timer_group_init(int group, int timer, bool auto_reload)
{
    timer_config_t config = {
        .divider = TIMEGROUP_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .auto_reload = TIMER_ALARM_EN,
        .alarm_en = auto_reload,
    };// default clock source is APB

    timer_init(group, timer, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(group, timer, 0); //设置初始值

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(group, timer, ALARM_VALUE);
    /*使能定时器中断*/
    timer_enable_intr(group, timer);
    /*分配空间给传递到中断的定时器设备*/
    timer_info_t *timer_info = calloc(1, sizeof(timer_info_t));
    timer_info->timer_group = group;
    timer_info->timer_idx = timer;
    timer_info->alarm_value = ALARM_VALUE;

    /*添加中断处理函数*/
    timer_isr_callback_add(group, timer, timer_group_isr_callback, timer_info, 0);
    /*使能定时器*/
    timer_start(group, timer);
}

static void timer_task(void *arg)
{
    while(1)
    {
        timer_event_t evt;
        xQueueReceive(timer_queue, &evt, portMAX_DELAY);
        
        time_page_update();
    }
}

void my_timer_init(void)
{
    timer_group_init(TIMER_GROUP_0, TIMER_0, true);
    memset(&evt, 0, sizeof(timer_event_t));
    timer_queue = xQueueCreate(10, sizeof(timer_event_t));
    xTaskCreate(timer_task, "timer_task", 2048, NULL, 1, NULL);
}