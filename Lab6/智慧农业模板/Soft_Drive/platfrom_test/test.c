#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "systick.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "E53_IA1.h"
#include "I2C.h"
#include "oled.h"
#include "74HC595_LED.h"
#include "pwm.h"

E53_IA1_Data_TypeDef E53_IA1_Data;

#define all_led_on gd_eval_led_on(LED1);\
			gd_eval_led_on(LED2);\
			gd_eval_led_on(LED3);\
			gd_eval_led_on(LED4);\
			gd_eval_led_on(LED5);\
			gd_eval_led_on(LED6);\
			gd_eval_led_on(LED7);\
			gd_eval_led_on(LED8);\

#define all_led_off gd_eval_led_off(LED1);\
			gd_eval_led_off(LED2);\
			gd_eval_led_off(LED3);\
			gd_eval_led_off(LED4);\
			gd_eval_led_off(LED5);\
			gd_eval_led_off(LED6);\
			gd_eval_led_off(LED7);\
			gd_eval_led_off(LED8);\

float humd_ub = 80.0;
float humd_lb = 20.0;
float temp_ub = 30.0;
float temp_lb = 15.0;
float lux_ub = 500;
float lux_lb = 30;
float eps = 0.1;
float humd_last, temp_last, lux_last;
int cnt;

uint8_t humd[16];
uint8_t temp[16];
uint8_t lux[16];
uint8_t blank[16] = "        ";

void BEEP(uint32_t t) {
	gpio_bit_set(GPIOD, GPIO_PIN_12);
	delay_1ms(t);
	gpio_bit_reset(GPIOD, GPIO_PIN_12);
	delay_1ms(t);
}

void BEEP_LONG_3() { // 3声长鸣
	uint8_t i;
	for(i = 0; i < 3; i++) BEEP(1000);
}


void BEEP_SHORT_3() { // 3声长鸣
	uint8_t i;
	for(i = 0; i < 3; i++) BEEP(100);
}

void PWM_ON() {
	uint8_t speed = 5;
	gpio_bit_reset(GPIOC, GPIO_PIN_6);
	gpio_bit_set(GPIOC, GPIO_PIN_7);
	gpio_bit_set(GPIOC, GPIO_PIN_8);
	gpio_bit_set(GPIOC, GPIO_PIN_9);
	delay_1ms(speed);
	gpio_bit_set(GPIOC, GPIO_PIN_6);
	gpio_bit_reset(GPIOC, GPIO_PIN_7);
	gpio_bit_set(GPIOC, GPIO_PIN_8);
	gpio_bit_set(GPIOC, GPIO_PIN_9);
	delay_1ms(speed);
	gpio_bit_set(GPIOC, GPIO_PIN_6);
	gpio_bit_set(GPIOC, GPIO_PIN_7);
	gpio_bit_reset(GPIOC, GPIO_PIN_8);
	gpio_bit_set(GPIOC, GPIO_PIN_9);
	delay_1ms(speed);
	gpio_bit_set(GPIOC, GPIO_PIN_6);
	gpio_bit_set(GPIOC, GPIO_PIN_7);
	gpio_bit_set(GPIOC, GPIO_PIN_8);
	gpio_bit_reset(GPIOC, GPIO_PIN_9);
	delay_1ms(speed);
}


//初始化 系统时钟、XLL\拨码开关、OLED、智慧农业模块等按需编写
void system_init(void) {
	systick_config(); // 初始化系统时钟
	gd_XII_systeminit(); // 初始化8个LED灯
	dipinit(); // 初始化拨码开关
	OLED_Gpio_Init();
	OLED_Init();
	i2c_gpio_config();
	i2c_config();
	usart0Init(EVAL_COM0 ,115200U);
	Init_E53_IA1();
	gd_gpio_init(); // 板载蜂鸣器、继电器初始化
	HC595_GPIO_Config();
	motor_gpio_config(); // 步进电机初始化
}

//程序编写
void test(void) {
	OLED_ShowString(0, 0, (uint8_t *)"Humd: ");
	OLED_ShowString(0, 2, (uint8_t *)"Temp: ");
	OLED_ShowString(0, 4, (uint8_t *)"Lux : ");
	while(1) {
		
		E53_IA1_Read_Data();
		
		Uart0Printf("Humidity:%3.lf, Temprature:%3.lf°C, Lux: %3.lf \r\n", 
		E53_IA1_Data.Humidity,
		E53_IA1_Data.Temperature,
		E53_IA1_Data.Lux);
		sprintf((char *)humd, "%.1lf%%", E53_IA1_Data.Humidity);
		sprintf((char *)temp, "%.1lf`C", E53_IA1_Data.Temperature);
		sprintf((char *)lux, "%.1lf", E53_IA1_Data.Lux);
		
		if(abs(humd_last - E53_IA1_Data.Humidity) > eps || abs(temp_last - E53_IA1_Data.Temperature) > eps 
			|| abs(lux_last - E53_IA1_Data.Lux) > eps) {
			OLED_ShowString(42, 0, blank);
			OLED_ShowString(42, 2, blank);
			OLED_ShowString(42, 4, blank);
			OLED_ShowString(42, 0, humd);
			OLED_ShowString(42, 2, temp);
			OLED_ShowString(42, 4, lux);
		}
		
		
		
		/*Humidity 操作*/
		if(E53_IA1_Data.Humidity > humd_ub) {
			E53_IA1_Motor_StatusSet(ON); // 湿度超过高阈值 打开电机
			OLED_ShowString(112, 0, (uint8_t*)"H");
		}
		else if(E53_IA1_Data.Lux > lux_ub) {
			E53_IA1_Motor_StatusSet(OFF); // 湿度低于低阈值 关闭电机
			OLED_ShowString(112, 0, (uint8_t*)"L");
		}
		else { // 处于两阈值之间
			E53_IA1_Motor_StatusSet(OFF);
			OLED_ShowString(112, 0, (uint8_t*)"Y"); // 表示正常
		}
		/*Humudity 操作结束*/
		
		/*Temprature 操作*/
		if(E53_IA1_Data.Temperature > temp_ub) {
			OLED_ShowString(112, 2, (uint8_t*)"H");
		}
		else if(E53_IA1_Data.Temperature < temp_lb) {
			OLED_ShowString(112, 2, (uint8_t*)"L");
		}
		else { // 处于两阈值之间
			OLED_ShowString(112, 2, (uint8_t*)"Y"); // 表示正常
		}
		/*Temprature 操作结束*/
		
		/*Lux 操作*/
		if(E53_IA1_Data.Lux < lux_lb) { // 如果光照小于最低阈值 将补光灯开启 并将所有LED灯打开
			E53_IA1_Light_StatusSet(ON);
			all_led_on;
			OLED_ShowString(112, 4, (uint8_t*)"L");
		}
		else if(E53_IA1_Data.Lux > lux_ub) {
			E53_IA1_Light_StatusSet(OFF);
			all_led_off;
			OLED_ShowString(112, 4, (uint8_t*)"H");
		}
		else { // 处于两阈值之间
			E53_IA1_Light_StatusSet(OFF);
			all_led_off;
			OLED_ShowString(112, 4, (uint8_t*)"Y"); // 表示正常
		}
		/*Lux 操作结束*/
		
		bool flag = true;
		if((E53_IA1_Data.Temperature > temp_ub
		|| E53_IA1_Data.Temperature < temp_lb
		|| E53_IA1_Data.Humidity > humd_ub
		|| E53_IA1_Data.Humidity < humd_lb) && cnt == 0) { // 只有第一次进入这里才执行
			flag = false; // 如果进过这里 flag置false;
			cnt++; 
			BEEP_LONG_3();  //long beep for 3 times
			PWM_ON();
			BEEP_SHORT_3(); //short beep for 3 times
		}
		
		while(E53_IA1_Data.Temperature > temp_ub
		|| E53_IA1_Data.Temperature < temp_lb
		|| E53_IA1_Data.Humidity > humd_ub
		|| E53_IA1_Data.Humidity < humd_lb) E53_IA1_Read_Data();
		
		if(flag == false && E53_IA1_Data.Temperature <= temp_ub
		&& E53_IA1_Data.Temperature >= temp_lb
		&& E53_IA1_Data.Humidity <= humd_ub
		&& E53_IA1_Data.Humidity >= humd_lb) { // 进过上面
			BEEP(2000);
			cnt = 0; // 清空
		}
		
		
		// 保存上一次的数值
		humd_last = E53_IA1_Data.Humidity;
		temp_last = E53_IA1_Data.Temperature;
		lux_last = E53_IA1_Data.Lux;
		delay_1ms(400);
	}
}
