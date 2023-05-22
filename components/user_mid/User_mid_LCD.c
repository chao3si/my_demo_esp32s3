#include "User_mid_LCD.h"
#include "esp_log.h"

#define DELAY(X) vTaskDelay(((X)/ portTICK_PERIOD_MS))

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_DC_SET();//写数据
	lcd_cmd(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_DC_SET();//写数据
	lcd_cmd(dat>>8);
	lcd_cmd(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_CLR();//写命令
	lcd_cmd(dat);
}


/*x1=0 y1=0 x2=4 y2=4*/

/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_WR_REG(0x2a); //列地址设置
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x2b); //行地址设置
#if ZJY_1_54
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);
#elif PY_2_4
	LCD_WR_DATA(y1 + 0);
	LCD_WR_DATA(y2 + 0);
#endif
	LCD_WR_REG(0x2c); //储存器写
}

/******************************************************************************
      函数说明：LCD清屏函数
      入口数据：无
      返回值：  无
******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	LCD_Address_Set(0, 0, LCD_W - 1, LCD_H - 1);
	LCD_DC_SET();
	uint16_t data[LCD_W * 2];
	for(uint32_t i = 0; i < LCD_W * 2; i++)
   	{
      data[i] = Color;
	  //ESP_LOGI("LCD_Clear","data is %x", data[i]);
   	}
	for(uint32_t i = 0; i < LCD_H / 2; i++)
	{
		VSPI_data_x(data, LCD_W * 2 * 16);
	// for(int i =0;i<LCD_W * 2;i++)
	// {
	// 	lcd_cmd(data[i]>>8);
	// 	lcd_cmd(data[i]);
	// }
	}
	

}


/*16bit rgb 565*/
IRAM_ATTR void LCD_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t *color)	
{
	//uint32_t block = LCD_W * 2 * 10;  //一次刷新10行的块 bit
	uint32_t color_block = LCD_W * 1 * 10;	//16位数组
	uint32_t total_size = (x2 - x1 + 1) * (y2 - y1 + 1); // 单位是16bit
	uint32_t block_size = total_size / color_block;
	uint32_t excess_size = total_size % color_block;	//剩下的不成块的数据 单位是16bit
	LCD_Address_Set(x1, y1, x2, y2);
	LCD_DC_SET();
	
	for(int i = 0; i < block_size; i++)
	{
		if((excess_size == 0) && ((i + 1) == block_size))
		{
			VSPI_data_queue(&color[i * color_block], color_block * 16, 4);
		}
		else
		{
			VSPI_data_queue(&color[i * color_block], color_block * 16, 3);
		}	
	}
	if(excess_size != 0)
	{
		VSPI_data_queue(&color[block_size * color_block], excess_size * 16, 4);
	}

	// uint32_t color_block = LCD_W * 1 * 10;	//16位数组
	// uint32_t total_size = (x2 - x1 + 1) * (y2 - y1 + 1); // 单位是16bit
	// uint32_t block_size = total_size / color_block;
	// uint32_t excess_size = total_size % color_block;	//剩下的不成块的数据 单位是16bit
	// LCD_Address_Set(x1, y1, x2, y2);
	// LCD_DC_SET();
	
	// for(int i = 0; i < block_size; i++)
	// {
	// 	if((excess_size == 0) && ((i + 1) == block_size))
	// 	{
	// 		VSPI_data_x(&color[i * color_block], color_block * 16);
	// 	}
	// 	else
	// 	{
	// 		VSPI_data_x(&color[i * color_block], color_block * 16);
	// 	}	
	// }
	// if(excess_size != 0)
	// {
	// 	VSPI_data_x(&color[block_size * color_block], excess_size * 16);
	// }



	// uint32_t size = (x2+1-x1)*(y2+1-y1)*2;
	// LCD_Address_Set(x1,y1,x2,y2);
	// LCD_DC_SET();//写数据
	// uint32_t send_cnt 	= size/480;//需要整行刷新多少行
	// uint32_t send_cnt2	= size%480;//整行刷不完还剩多少字节

	// if(size==0)
	// {
	// 	printf("size=%u\r\n",size);
	// 	return;
	// }
	// for(int i=0;i<send_cnt;i++){
	// 	VSPI_data_x(&color[i*240],240*16);
	// }
	// if(send_cnt2!=0){
	// 	VSPI_data_x(&color[send_cnt*240],send_cnt2*8);
	// }
}

void LCD_IO_deinit(void)
{
	BLK_OFF();
	DELAY(100);
	LCD_RES_CLR();
	DELAY(100);
	LCD_RES_SET();
	DELAY(100);
}

/*中景圆1.54屏*/
void ZJY_154_LCD_init(void)
{
	LCD_WR_REG(0x11); 
	DELAY(120);

	LCD_WR_REG(0x36);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0x3A); 
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33); 

	LCD_WR_REG(0xB7); 
	LCD_WR_DATA8(0x35);  

	LCD_WR_REG(0xBB);
	LCD_WR_DATA8(0x19);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x12);   

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x20);  

	LCD_WR_REG(0xC6); 
	LCD_WR_DATA8(0x0F);    

	LCD_WR_REG(0xD0); 
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x11);

	LCD_WR_REG(0x29);

}

void PY_2_4_LCD_init(void)
{
	LCD_WR_REG(0x36);
	LCD_WR_DATA8(0xa0);
	LCD_WR_REG(0x3A); 
	LCD_WR_DATA8(0x55);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33); 

	LCD_WR_REG(0xB7); 
	LCD_WR_DATA8(0x35);  

	LCD_WR_REG(0xBB);
	LCD_WR_DATA8(0x28);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x0b);   

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x20);  

	LCD_WR_REG(0xC6); 
	LCD_WR_DATA8(0x0F);    

	LCD_WR_REG(0xD0); 
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x0f);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x2a);
	LCD_WR_DATA8(0x36);
	LCD_WR_DATA8(0x55);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x3a);
	LCD_WR_DATA8(0x0b);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x20);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0a);
	LCD_WR_DATA8(0x0b);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x34);
	LCD_WR_DATA8(0x43);
	LCD_WR_DATA8(0x4a);
	LCD_WR_DATA8(0x2b);
	LCD_WR_DATA8(0x1b);
	LCD_WR_DATA8(0x1c);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x1f);

	//LCD_WR_REG(0x21); 

	LCD_WR_REG(0x11);

	LCD_WR_REG(0x29);
}

/*st7789*/
void LCD_IO_Init(void)
{
	gpio_pad_select_gpio(LCD_RES);
	gpio_pad_select_gpio(LCD_DC);
	gpio_pad_select_gpio(LCD_BLK);
	gpio_set_direction(LCD_RES, GPIO_MODE_OUTPUT); //设置io22为输出模式
	gpio_set_direction(LCD_DC, GPIO_MODE_OUTPUT);  //设置io21为输出模式
	gpio_set_direction(LCD_BLK, GPIO_MODE_OUTPUT); //设置io12为输出模式

	vspi_init();
	LCD_RES_CLR();
	DELAY(100);
	LCD_RES_SET();
	DELAY(100);
	BLK_ON();
	DELAY(100);

	LCD_WR_REG(0x11);	// Sleep Out
	DELAY(120);
#if ZJY_1_54
	ZJY_154_LCD_init();
#elif PY_2_4
	PY_2_4_LCD_init();
#endif
	LCD_Clear(WHITE);
	DELAY(100);
}
