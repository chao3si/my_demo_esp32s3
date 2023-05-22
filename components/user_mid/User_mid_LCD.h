#ifndef _USER_LCD_H_
#define _USER_LCD_H_

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "user_dev_inc.h"
#include "stdio.h"

#define ZJY_1_54    (1)
#define PY_2_4      (0)

#if ZJY_1_54
#define LCD_W 240
#define LCD_H 240
#elif PY_2_4
#define LCD_W 320
#define LCD_H 240
#endif

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 
#define BRRED 			     0XFC07 
#define GRAY  			     0X8430 

#define LCD_RES GPIO_NUM_4
#define LCD_DC  GPIO_NUM_5
#define LCD_BLK GPIO_NUM_6
#define BLK_ON()    gpio_set_level(LCD_BLK,1);
#define BLK_OFF()   gpio_set_level(LCD_BLK,0);

#define LCD_SCL_SET() gpio_set_level(LCD_SCL,1);
#define LCD_SCL_CLR() gpio_set_level(LCD_SCL,0);
#define LCD_SDA_SET() gpio_set_level(LCD_SDA,1);
#define LCD_SDA_CLR() gpio_set_level(LCD_SDA,0);
#define LCD_CS_SET() gpio_set_level(LCD_CS,1);
#define LCD_CS_CLR() gpio_set_level(LCD_CS,0);
#define LCD_RES_SET() gpio_set_level(LCD_RES,1);
#define LCD_RES_CLR() gpio_set_level(LCD_RES,0);

#define LCD_DC_SET() gpio_set_level(LCD_DC,1);
#define LCD_DC_CLR() gpio_set_level(LCD_DC,0);

void LCD_IO_deinit(void);
void LCD_IO_Init(void);
void LCD_Clear(uint16_t Color);
void LCD_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t *color);	

#endif
