#include "lv_disp.h"

static SemaphoreHandle_t lvgl_mutex = NULL;

static esp_timer_handle_t lvgl_timer_handle = NULL;

static IRAM_ATTR void lv_timer_cb(void *arg)
{
   lv_tick_inc(1);
}

static esp_timer_create_args_t lvgl_timer = {
   .callback = &lv_timer_cb,
   .arg = NULL,
   .name = "lvgl_timer",
   .dispatch_method = ESP_TIMER_TASK,
};

void _lv_timer_create(void)
{
   esp_err_t err = esp_timer_create(&lvgl_timer, &lvgl_timer_handle);
   err = esp_timer_start_periodic(lvgl_timer_handle, 1000);
   if(err != ESP_FAIL)
   {
      ESP_LOGI("_lv_timer_create", "success");
   }
}

void GUI_task()
{
   while(1)
   {
      vTaskDelay(10 / portTICK_PERIOD_MS);
      if(pdTRUE == xSemaphoreTake(lvgl_mutex, portMAX_DELAY))
      {
         lv_task_handler();  
         xSemaphoreGive(lvgl_mutex);
      }
   }
}

void lvgl_init()
{
   lvgl_mutex = xSemaphoreCreateMutex();
   lv_init();
   lv_port_disp_init();
   lv_port_indev_init();
   _lv_timer_create();
   
   input_task_create();

   xTaskCreatePinnedToCore(GUI_task, "GUI_task", 4096*2, NULL, 4, NULL, APP_CPU_NUM);
}
