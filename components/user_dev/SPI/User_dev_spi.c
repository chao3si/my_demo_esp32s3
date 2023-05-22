#include "User_dev_spi.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include <stdio.h>


const char *TAG = "User_dev_spi";

static spi_device_handle_t spi;    //创建spi

extern void spi_disp_flush_ready(void);


IRAM_ATTR  void spi_ready(spi_transaction_t *trans)
{
    uint32_t spi_cnt = (uint32_t)trans->user;

    if (spi_cnt == 4)
    {
        spi_disp_flush_ready();
    }
}

static const spi_bus_config_t buscfg = {
    .miso_io_num = LCD_MISO,
    .mosi_io_num = LCD_MOSI,
    .sclk_io_num = LCD_CLK,
    .quadwp_io_num=-1, //不使用的一定要配置，不设置这个不知道为什么总是WDT复位，而且还会出现spi频率错误这种
    .quadhd_io_num=-1,
    .max_transfer_sz = 4094,
};

static const spi_device_interface_config_t devcfg = {
    .mode = 0,
    .clock_speed_hz = SPI_MASTER_FREQ_80M,
    .spics_io_num = LCD_CS,
    .queue_size = 80,
    .cs_ena_pretrans = 1,
    .post_cb = spi_ready,
    //.flags = SPI_DEVICE_HALFDUPLEX,
};

void vspi_init(void)
{
    esp_err_t err;
    err = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "spi_bus_initialize failed");
    }
    err = spi_bus_add_device(LCD_HOST, &devcfg, &spi);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "spi_bus_add_device failed");
    }
}

/*------------------------------------LCD传输8位指令----------------------------------------*/
void lcd_cmd(const uint8_t cmd)
{
    spi_transaction_t t = {
        .length = 8,
        .tx_buffer = &cmd,
        .user = (void *)0,
    };
    if(spi_device_polling_transmit(spi, &t) != ESP_OK)
    {
        ESP_LOGI(TAG, "spi sneding cmd failed");
    }
}
/*------------------------------------LCD传输16位指令---------------------------------------*/

void VSPI_data16(const uint16_t dat)
{
    spi_transaction_t t = {
    .length = 16,
    .tx_buffer = &dat,
    .user = (void *)0,
    };
    if(spi_device_polling_transmit(spi, &t) != ESP_OK)
    {
        ESP_LOGI(TAG, "spi sending data failed");
    }
}

IRAM_ATTR void VSPI_data_x(uint16_t *dat, uint32_t len)
{
    spi_transaction_t t = {
    .length = len,
    .tx_buffer = dat,
    .user = (void *)0,
    };
    if(spi_device_polling_transmit(spi, &t) != ESP_OK)
    {
        ESP_LOGI(TAG, "spi sending data_x failed");
    }
}

// 是的，spi_device_queue_trans 在传输完成之前会阻塞其他任务调用该函数。具体来说，该函数会将 spi_transaction_t 结构体加入到 SPI 总线队列中，然后会等待直到队列中的所有传输都已经完成，然后再返回。在此期间，其他任务如果调用了该函数，它们也会被阻塞，直到先前的传输完成并且队列中没有挂起的传输为止。这样可以确保 SPI 总线上的传输按照正确的顺序进行，并且不会出现冲突或错误。
IRAM_ATTR void VSPI_data_queue(uint16_t *dat, uint32_t len, uint32_t user_fg)
{
    static spi_transaction_t t[80];
    static uint32_t i = 0;

    memset(&t[i], 0, sizeof(spi_transaction_t));  //注意：不加这一句程序i跑一轮之后会崩溃,原因未知
    t[i].length = len;
    t[i].tx_buffer = dat;
    t[i].user = (void *)user_fg;
    if(spi_device_queue_trans(spi, &t[i], portMAX_DELAY) != ESP_OK)
    {
        ESP_LOGI(TAG, "spi_device_queue_trans failed %d", i);
    }
    i++;
    if(i == 80)
    {
        i = 0;
    }
}
