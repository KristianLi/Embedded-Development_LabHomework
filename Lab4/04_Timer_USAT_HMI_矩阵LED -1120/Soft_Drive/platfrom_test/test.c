#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "diwen.h"
#include "74HC595_LED.h"
#include "timer.h"
#include "systick.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include "user.h"
volatile uint16_t  tim3_count;
/*
00010000
11111110
10010010
10010010
11111110
10010010
00010000
00010000  //示例“中”字
*/
/*

uint8_t row[]      = {0x3e,0x12,0x12,0xff,0x12,0x12,0x3e,0x00};					//中字
uint8_t core[]     = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 

uint8_t row_clear[]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};		      //8*8LED清屏
uint8_t cal_clear[]= {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

uint8_t row1[]     = {0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t core1[]    = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};				  /*中字第一笔画

uint8_t row2[]     = {0x3e,0x02,0x02,0x02,0x02,0x02,0x3e,0x00};
uint8_t core2[]    = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 				/*中字第二笔画

uint8_t row3[]     = {0x3e,0x12,0x12,0x12,0x12,0x12,0x3e,0x00};
uint8_t core3[]    = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 				/*中字第三笔画

uint8_t row4[]     = {0x3e,0x12,0x12,0xff,0x12,0x12,0x3e,0x00};
uint8_t core4[]    = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 				/*中字第四笔画
*/

//系统初始化（systick、定时器、GD_XLL、HC595、DIWIN ）
void system_init(void) {
    systick_config();
    TIM3_Init(30000, 10000);
    gd_XII_systeminit();
    HC595_GPIO_Config();
    diwen_init();
}




//实验一 LED写汉字
void test1(void){
    while (0x1A == uart5_rx_buffer[5])
    {
        delay_1ms(100);
        while (0x01 == uart5_rx_buffer[8])
        {
            gd_eval_led_on(LED1);
        }
        while (0x00 == uart5_rx_buffer[5])
        {
            gd_eval_led_off(LED1);
        }
        
        
    }

    while (0x11 == uart5_rx_buffer[5])
    {
        delay_1ms(100);
    }
    while (0x01 == uart5_rx_buffer[8])
    {
        gd_eval_led_on(LED2);
    }
    while (0x00 == uart5_rx_buffer[5])
    {
        gd_eval_led_off(LED2);
    }
}






//实验二 串口触摸屏控制LED




