/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "nvs_flash.h"   
#include "esp_timer.h"
/*************************功能相关****************************/
#include "MyfileSystem.h"
#include "chaosi_nvs.h"
#include "chaosi_wifi_ap.h"
#include "chaosi_system_data.h"
#include "chaosi_wifi_sta.h"
#include "chaosi_wifi_ap_sta.h"
#include "chaosi_http_server.h"
/*************************lvgl相关****************************/
// #include "../user_dev/user_dev_inc.h"
// #include "User_mid_LCD.h"
// #include "lvgl.h"
// #include "../components/lvgl/lv_devices/lv_port_disp.h"
#include "lv_disp.h"
/*************************GUI相关****************************/
#include "chaosi_encoder.h"
#include "chaosi_http_request.h"
#include "chaosi_timer.h"
#include "bg_screen.h"
#include "home_page.h"
#include "menu_template.h"


#define GPIO_LED            2

static void background_task(void *arg)
{
   while(1)
   {
      vTaskDelay(1000 / portTICK_PERIOD_MS);

      if(get_has_http_requst() == true && get_wifi_sta_status() == WIFI_STA_MODE_CONNECT_SUCCESS)
      {
         send_http_request_all();
      }


      set_has_http_requst(false);   //在最后要清除请求标志位
   }

}

void app_main(void)
{
   /*文件spiffs系统测试*/
   // MyfileSystem_Init();
   // MyfileSystem_Test();

   /*nvs区域读写测试*/
   //nvs_wifi_init();
   nvs_wifi_init();
   nvs_write_wifi_info("yunlong", "chaosi_password");
   nvs_read_wifi_info();
   //nvs_read_wifi_info();
   //printf_wifi_info();
   // nvs_wifi_info_clean();
   // nvs_read_wifi_info();
   
   /*wifi的ap_sta模式测试*/
   //wifi_start();
   //wifi_ap_sta_start();

   /*http server测试*/
   //http_server_start();

   wifi_net_init();
   //wifi_ap_sta_init();

   
   
   system_data_init();
   //set_wifi_sta_isopen(true);


   lvgl_init();

   bg_screen_create();
   home_page();

   vTaskDelay(1000 / portTICK_PERIOD_MS);
   http_request_init();
  

   xTaskCreatePinnedToCore(background_task, "background_task", 4096*2, NULL, 3, NULL, APP_CPU_NUM);

   
   my_timer_init();
   
}
