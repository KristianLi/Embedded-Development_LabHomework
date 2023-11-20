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
DB 01111100B
DB 01000100B
DB 01111100B
DB 01000100B
DB 01000100B
DB 01111100B
DB 01000100B
DB 00000000B
*/


uint8_t row[]      = {0x7F,0x49,0x49,0x49,0x49,0x7F,0x0,0x0};				//中字
uint8_t core[]     = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

uint8_t row_clear[]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};		      //8*8LED清屏
uint8_t cal_clear[]= {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

uint8_t row1[]     = {0x7F,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
uint8_t core1[]    = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};				  //中字第一笔画

uint8_t row2[]     = {0x7F,0x1,0x1,0x1,0x1,0x1,0x0,0x0};
uint8_t core2[]    = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 				//中字第二笔画

uint8_t row3[]     = {0x7F,0x1,0x1,0x1,0x1,0x7F,0x0,0x0};
uint8_t core3[]    = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 				//中字第三笔画

uint8_t row4[]     = {0x7F,0x9,0x9,0x9,0x9,0x7F,0x0,0x0};
uint8_t core4[]    = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 				//中字第四笔画

uint8_t row5[]     = {0x7F,0x9,0x9,0x9,0x9,0x7F,0x0,0x0};
uint8_t core5[]    = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 				//中字第五笔画


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

    int i=0;
    if(tim3_count==0)
    {
        for(i=0;i<7;i++)
        {
            HC595_Send_Byte(row_clear[i]);
            HC595_Send_Byte(cal_clear[i]);
            HC595_CS();
        }
    }
    if(tim3_count==1)
    {
        for(i=0;i<7;i++)
        {
            HC595_Send_Byte(row1[i]);
            HC595_Send_Byte(core1[i]);
            HC595_CS();
        }
    }
    if(tim3_count==2)
    {
        for(i=0;i<7;i++)
        {
            HC595_Send_Byte(row2[i]);
            HC595_Send_Byte(core2[i]);
            HC595_CS();
        }
    }
    if(tim3_count==3)
    {
        for(i=0;i<7;i++)
        {
            HC595_Send_Byte(row3[i]);
            HC595_Send_Byte(core3[i]);
            HC595_CS();
        }
    }
    if(tim3_count==4)
    {
        for(i=0;i<7;i++)
        {
            HC595_Send_Byte(row4[i]);
            HC595_Send_Byte(core4[i]);
            HC595_CS();
        }
    }
    if(tim3_count==5)
    {
        for(i=0;i<7;i++)
        {
            HC595_Send_Byte(row[i]);
            HC595_Send_Byte(core[i]);
            HC595_CS();
        }
    }
    if(tim3_count==6)
    {
        for(i=0;i<7;i++)
        {
            HC595_Send_Byte(row[i]);
            HC595_Send_Byte(core[i]);
            HC595_CS();
        }
    }

}




void led_play(void)
{
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
    while (0x00 == uart5_rx_buffer[8])
    {
        gd_eval_led_off(LED2);
    }

}


//实验二 串口触摸屏控制LED
void test2(void){
    led_play();
}
void lightLED(void) {
    gd_eval_led_on(LED1);
    delay_1ms(100);
    gd_eval_led_on(LED2);
    delay_1ms(100);
    gd_eval_led_on(LED3);
    delay_1ms(100);
    gd_eval_led_on(LED4);
    delay_1ms(100);
    gd_eval_led_on(LED5);
    delay_1ms(100);
    gd_eval_led_on(LED6);
    delay_1ms(100);
    gd_eval_led_on(LED7);
    delay_1ms(100);
    gd_eval_led_on(LED8);
    delay_1ms(100);
}
void extinguishLED(void){
    gd_eval_led_off(LED1);
    delay_1ms(100);
    gd_eval_led_off(LED2);
    delay_1ms(100);
    gd_eval_led_off(LED3);
    delay_1ms(100);
    gd_eval_led_off(LED4);
    delay_1ms(100);
    gd_eval_led_off(LED5);
    delay_1ms(100);
    gd_eval_led_off(LED6);
    delay_1ms(100);
    gd_eval_led_off(LED7);
    delay_1ms(100);
    gd_eval_led_off(LED8);
    delay_1ms(100);
}
void flashLED(void){
    gd_eval_led_on(LED1);
    delay_1ms(50);
    gd_eval_led_off(LED1);
    delay_1ms(50);
    gd_eval_led_on(LED2);
    delay_1ms(50);
    gd_eval_led_off(LED2);
    delay_1ms(50);
    gd_eval_led_on(LED3);
    delay_1ms(50);
    gd_eval_led_off(LED3);
    delay_1ms(50);
    gd_eval_led_on(LED4);
    delay_1ms(50);
    gd_eval_led_off(LED4);
    delay_1ms(50);
    gd_eval_led_on(LED5);
    delay_1ms(50);
    gd_eval_led_off(LED5);
    delay_1ms(50);
    gd_eval_led_on(LED6);
    delay_1ms(50);
    gd_eval_led_off(LED6);
    delay_1ms(50);
    gd_eval_led_on(LED7);
    delay_1ms(50);
    gd_eval_led_off(LED7);
    delay_1ms(50);
    gd_eval_led_on(LED8);
    delay_1ms(50);
    gd_eval_led_off(LED8);
    delay_1ms(50);
}
void test3()
{
    while (uart5_rx_buffer[5] == 0x1F) {
        delay_1ms(100);
        while (uart5_rx_buffer[8] == 0x01) {
            lightLED();
            delay_1ms(100);
            lightLED();
            delay_1ms(100);
        }
        while (uart5_rx_buffer[8] == 0x00) {
            flashLED();
            delay_1ms(50);
        }
    }
}


