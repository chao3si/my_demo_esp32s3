#ifndef _USER_DEV_SPI_H_
#define _USER_DEV_SPI_H_
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#define LCD_HOST    SPI2_HOST
#define DMA_CHAN    2

#define LCD_MISO -1
#define LCD_MOSI 11
#define LCD_CLK  12
#define LCD_CS   10


void lcd_cmd(const uint8_t cmd);
void VSPI_data16(const uint16_t dat);
void vspi_init(void);
IRAM_ATTR void VSPI_data_x(uint16_t *dat,uint32_t len);
IRAM_ATTR void VSPI_data_queue(uint16_t *dat,uint32_t len,uint32_t user_fg);

#endif
