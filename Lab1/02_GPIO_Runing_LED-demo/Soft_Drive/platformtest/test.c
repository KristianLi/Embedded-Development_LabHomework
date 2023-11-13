#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "systick.h"
#include "test.h"
#include "user.h"


#define TEST1_1  0
#define TEST1_2  0
#define TEST1_3  0
#define TEST1_4  0
#define TEST2_1  0
#define TEST2_2  1

uint8_t lightKey1First=0;
uint8_t lightKey1Second=0;
uint8_t lightKey4First=0;
uint8_t lightKey4Second=0;
uint8_t fl=0;
void extinguishLEDImmediately1(void);
void li(void);
void lightLEDdelay(uint32_t tt);
void system_init(void) {
    systick_config();
    gd_XII_systeminit();							//初始化8个LED灯
    dipinit();												//初始化8个拨码开关
    gd_eval_key_init(USER1_KEY, KEY_MODE_GPIO);			//K1初始化成IO模式
    gd_eval_key_init(USER2_KEY, KEY_MODE_EXTI);			//K2初始化成中断模式
    gd_eval_key_init(USER3_KEY, KEY_MODE_EXTI);			//K3初始化成中断模式
    gd_eval_key_init(USER4_KEY, KEY_MODE_GPIO);			//K4初始化成IO模式

}

void test1(void) {
#if TEST1_1 > 0
    //基础实验1：通过配置GPIO点亮实验箱上的LED灯1；
		gd_eval_led_on(LED1);

#endif

#if TEST1_2 > 0
    //基础实验2：使用拨码开关点亮实验箱上的LED灯；
	if(gpio_input_bit_get(DIP1_GPIO_PORT,DIP1_PIN)==SET){
		gd_eval_led_on(LED1);
	}

		if(gpio_input_bit_get(DIP1_GPIO_PORT,DIP1_PIN)==RESET){
		gd_eval_led_off(LED1);
	}
#endif

#if TEST1_3 > 0
    //基础实验3：使用轮询方式，用按键开关点亮LED灯；
		if(RESET == gd_eval_key_state_get(USER1_KEY)){
			delay_1ms(10);
			if(RESET == gd_eval_key_state_get(USER1_KEY)){
				gd_eval_led_on(LED1);
			}
		}
#endif

#if TEST1_4 > 0
    //基础实验4：使用中断方式，用按键开关点亮LED灯。
	  //配置按键中断，按键2控制LED3灭，按键3控制LED3亮

#endif
}
void test2(void) {

#if TEST2_1 > 0
//进阶实验1：使用拨码开关控制LED灯的亮灭
//当拨码开关1拨为1时，将拨码开关2也拨为1则LED灯顺序亮起；
//将拨码开关2拨为0、拨码开关3拨为1则灯顺序熄灭；
//若拨码开关2、3同时拨为1灯亮起；
//若拨码开关1拨为0，则无论拨码开关2、3为何值LED灯均不亮起。
    if(gpio_input_bit_get(DIP1_GPIO_PORT,DIP1_PIN)==RESET){
        extinguishLEDImmediately();
    }

    else{
        if(gpio_input_bit_get(DIP2_GPIO_PORT,DIP2_PIN)==SET&& gpio_input_bit_get (DIP3_GPIO_PORT,DIP3_PIN)==RESET) lightLED();
        else if(gpio_input_bit_get(DIP2_GPIO_PORT,DIP2_PIN)==RESET&&gpio_input_bit_get (DIP3_GPIO_PORT,DIP3_PIN)==SET) extinguishLED();
        else if(gpio_input_bit_get(DIP2_GPIO_PORT,DIP2_PIN)==SET&&gpio_input_bit_get (DIP3_GPIO_PORT,DIP3_PIN)==SET) lightLEDImmediately();
        else extinguishLEDImmediately();
    }


#endif

#if TEST2_2 > 0

    //	进阶实验2：使用按键开关控制LED灯的亮灭：
//	使用按键开关控制LED灯的亮灭：将任一LED灯作为指示灯。
//	按下按键1，所有LED灯亮起；
//	而后按下按键4，除指示灯外所有灯间隔闪烁；
//	再次长按按键4，灯亮时长即为按键时长；
//	最后按下按键1，包括指示灯在内的所有灯熄灭，此时按下按键4，LED灯也不亮起。
//	此例程将LED1作为指示灯，
		uint32_t key4_start_time = 0;
		if(lightKey1First==1){
			if(lightKey4First==0)lightLEDImmediately();
			else{
					if(lightKey4Second==0){
							extinguishLEDImmediately();
							gd_eval_led_on(LED1);
							li();
					}
					else{
						if(lightKey1Second==0&&fl==0){
								lightLEDImmediately();
								if(SET == gd_eval_key_state_get(USER3_KEY)){
									extinguishLEDImmediately1();
									fl=1;
								}
						}
						else if(lightKey1Second==1){
							lightKey1First=0;lightKey1Second=0;lightKey4First=0;lightKey4Second=0;
							fl=0;
							extinguishLEDImmediately();
						}
					}
			}
		}

#endif

}


void test3(void) {

}
#if TEST1_4 > 0

void EXTI3_IRQHandler(void){
		if(RESET != exti_interrupt_flag_get(USER2_KEY_EXTI_LINE)){
			gd_eval_led_off(LED3);
			exti_interrupt_flag_clear(USER2_KEY_EXTI_LINE);
		}
}

void EXTI4_IRQHandler(void){
		if(RESET != exti_interrupt_flag_get(USER3_KEY_EXTI_LINE)){
			gd_eval_led_on(LED3);
			exti_interrupt_flag_clear(USER3_KEY_EXTI_LINE);
		}
}
#endif

#if TEST2_2 > 0

void EXTI3_IRQHandler(void){
	if (RESET != exti_interrupt_flag_get(USER2_KEY_EXTI_LINE)){
				if(lightKey1First==0){
					lightKey1First=1;
				}
				else{
					lightKey1Second=1;
				}
				exti_interrupt_flag_clear(USER2_KEY_EXTI_LINE);
    }

}

void EXTI4_IRQHandler(void){
	if (RESET != exti_interrupt_flag_get(USER3_KEY_EXTI_LINE)){

				//key4_start_time = systick_get_value();
        if (!lightKey4First) {
          lightKey4First = 1;
        }
        else{
					lightKey4Second = 1;
				}
				exti_interrupt_flag_clear(USER3_KEY_EXTI_LINE);
    }

}
#endif

void li(void){
    gd_eval_led_on(LED1);
    gd_eval_led_on(LED2);
    delay_1ms(100);
    gd_eval_led_off(LED2);
    gd_eval_led_on(LED3);
    delay_1ms(100);
    gd_eval_led_off(LED3);
    gd_eval_led_on(LED4);
    delay_1ms(100);
    gd_eval_led_off(LED4);
    gd_eval_led_on(LED5);
    delay_1ms(100);
    gd_eval_led_off(LED5);
    gd_eval_led_on(LED6);
    delay_1ms(100);
    gd_eval_led_off(LED6);
    gd_eval_led_on(LED7);
    delay_1ms(100);
    gd_eval_led_off(LED7);
    gd_eval_led_on(LED8);
    delay_1ms(100);
    gd_eval_led_off(LED8);
}
void lightLED(void){
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
void lightLEDdelay(uint32_t tt){
    gd_eval_led_on(LED1);
    gd_eval_led_on(LED2);
    gd_eval_led_on(LED3);
    gd_eval_led_on(LED4);
    gd_eval_led_on(LED5);
    gd_eval_led_on(LED6);
    gd_eval_led_on(LED7);
    gd_eval_led_on(LED8);
    delay_lms(tt);
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED4);
    gd_eval_led_off(LED5);
    gd_eval_led_off(LED6);
    gd_eval_led_off(LED7);
    gd_eval_led_off(LED8);
}
void lightLEDImmediately(void){
    gd_eval_led_on(LED1);
    gd_eval_led_on(LED2);
    gd_eval_led_on(LED3);
    gd_eval_led_on(LED4);
    gd_eval_led_on(LED5);
    gd_eval_led_on(LED6);
    gd_eval_led_on(LED7);
    gd_eval_led_on(LED8);
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
void extinguishLEDImmediately(void){
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED4);
    gd_eval_led_off(LED5);
    gd_eval_led_off(LED6);
    gd_eval_led_off(LED7);
    gd_eval_led_off(LED8);
}
void extinguishLEDImmediately1(void){
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED4);
    gd_eval_led_off(LED5);
    gd_eval_led_off(LED6);
    gd_eval_led_off(LED7);
    gd_eval_led_off(LED8);
}