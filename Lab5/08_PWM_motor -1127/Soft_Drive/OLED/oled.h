
#ifndef __OLED_H
#define __OLED_H			  	 

#include "stdlib.h"	    	
#include "gd32f4xx.h"
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    	


//-----------------OLED端口定义----------------  

/*
	OLED_CS:	  PD4
	OLED_RES:		PD5
	OLED_DC:		PB4
*/
#define	OLED_CS(n)		(n?gpio_bit_set(GPIOD, GPIO_PIN_4):gpio_bit_reset(GPIOD, GPIO_PIN_4))
#define	OLED_RST(n)		(n?gpio_bit_set(GPIOD, GPIO_PIN_5):gpio_bit_reset(GPIOD, GPIO_PIN_5))
#define	OLED_DC(n)		(n?gpio_bit_set(GPIOB, GPIO_PIN_4):gpio_bit_reset(GPIOB, GPIO_PIN_4))



//OLED控制用函数

void OLED_Gpio_Init(void);
void OLED_SPI_Send(uint8_t data);
void OLED_Write_Cmd(uint8_t cmd);
void OLED_Write_Data(uint8_t data);
void OLED_WriteByte( uint16_t da);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
uint32_t oled_pow(uint8_t m,uint8_t n);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2);
void OLED_ShowString(uint8_t x,uint8_t y,char *chr);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_Init(void);

#endif  
	 



