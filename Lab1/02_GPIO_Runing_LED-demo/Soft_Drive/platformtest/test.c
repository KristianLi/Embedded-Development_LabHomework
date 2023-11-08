#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "systick.h"
#include "test.h"
#include "user.h"

#define TEST1_1  0
#define TEST1_2  0
#define TEST1_3  1
#define TEST1_4  0
#define TEST2_1  0
#define TEST2_2  0

void system_init(void) {
	systick_config();
	gd_XII_systeminit();									//��ʼ��8��LED��
	dipinit();	//��ʼ��8�����뿪��
	gd_eval_key_init(USER1_KEY, KEY_MODE_GPIO);		//K1��ʼ����IOģʽ
	gd_eval_key_init(USER2_KEY, KEY_MODE_GPIO);		//K2��ʼ�����ж�ģʽ
	gd_eval_key_init(USER3_KEY, KEY_MODE_GPIO);		//K3��ʼ�����ж�ģʽ
	gd_eval_key_init(USER4_KEY, KEY_MODE_GPIO);		//K4��ʼ����IOģʽ
}

void test1(void) {
#if TEST1_1 > 0 
		//����ʵ��1��ͨ������GPIO����ʵ�����ϵ�LED��1��
		gd_eval_led_on(LED1);

#endif
	
#if TEST1_2 > 0
	  //����ʵ��2��ʹ�ò��뿪�ص���ʵ�����ϵ�LED�ƣ�
		if(gpio_input_bit_get(DIP1_GPIO_PORT, DIP1_PIN)== SET){
			gd_eval_led_on(LED1);
		}
		if(gpio_input_bit_get(DIP1_GPIO_PORT,DIP1_PIN) == RESET) {
			gd_eval_led_off(LED1) ;
		}
#endif
	
#if TEST1_3 > 0 
		//����ʵ��3��ʹ����ѯ��ʽ���ð������ص���LED�ƣ�
		if(RESET==gd_eval_key_state_get (USER1_KEY)){
			delay_1ms(10);
			if(RESET ==gd_eval_key_state_get (USER1_KEY) ){
				gd_eval_led_toggle (LED1);
			}
		}
#endif
		
#if TEST1_4 > 0
	  //����ʵ��4��ʹ���жϷ�ʽ���ð������ص���LED�ƣ����ð����жϣ�����2����LED3�𣬰���3����LED3��
		
#endif
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

void lightLEDImmediately(void) {
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

void extinguishLEDImmediately(void) {
	gd_eval_led_off(LED1);
	gd_eval_led_off(LED2);
	gd_eval_led_off(LED3);
	gd_eval_led_off(LED4);
	gd_eval_led_off(LED5);
	gd_eval_led_off(LED6);
	gd_eval_led_off(LED7);
	gd_eval_led_off(LED8);
}

void flashLED(void){
	gd_eval_led_on(LED2);
	delay_1ms(50);
	gd_eval_led_on(LED3);
	delay_1ms(50);
	gd_eval_led_on(LED4);
	delay_1ms(50);
	gd_eval_led_on(LED5);
	delay_1ms(50);
	gd_eval_led_on(LED6);
	delay_1ms(50);
	gd_eval_led_on(LED7);
	delay_1ms(50);
	gd_eval_led_on(LED8);
	delay_1ms(50);
}


void test2(void) {
	
#if TEST2_1 > 0 
	//����ʵ��1��ʹ�ò��뿪�ؿ���LED�Ƶ�����
	//�����뿪��1��Ϊ1ʱ�������뿪��2Ҳ��Ϊ1��LED��˳������
	//�����뿪��2��Ϊ0�����뿪��3��Ϊ1���˳��Ϩ��
	//�����뿪��2��3ͬʱ��Ϊ1������
	//�����뿪��1��Ϊ0�������۲��뿪��2��3Ϊ��ֵLED�ƾ�������
	if (gpio_input_bit_get(DIP1_GPIO_PORT, DIP1_PIN) == RESET) {
		extinguishLEDImmediately();
	}
	else{
		if (gpio_input_bit_get(DIP2_GPIO_PORT, DIP2_PIN) == SET && gpio_input_bit_get(DIP3_GPIO_PORT, DIP3_PIN) == RESET) {
			lightLED();
		}
		else if (gpio_input_bit_get(DIP2_GPIO_PORT, DIP2_PIN) == RESET && gpio_input_bit_get(DIP3_GPIO_PORT, DIP3_PIN) == SET) {
			extinguishLED();
		}
		else if (gpio_input_bit_get(DIP2_GPIO_PORT, DIP2_PIN) == SET && gpio_input_bit_get(DIP3_GPIO_PORT, DIP3_PIN) == SET) {
			lightLEDImmediately();
		}
		else{
			extinguishLEDImmediately();
		}
	}
#endif
		
#if TEST2_2 > 0

//	����ʵ��2��ʹ�ð������ؿ���LED�Ƶ�����
//	ʹ�ð������ؿ���LED�Ƶ����𣺽���һLED����Ϊָʾ�ơ�
//	���°���1������LED������
//	�����°���4����ָʾ�������еƼ����˸��
//	�ٴγ�������4������ʱ����Ϊ����ʱ����
//	����°���1������ָʾ�����ڵ����е�Ϩ�𣬴�ʱ���°���4��LED��Ҳ������
//	�����̽�LED1��Ϊָʾ�ƣ�	
	uint8_t lightKey1First = 0;
	uint8_t lightKey1Second = 0;
	uint8_t lightKey4First = 0;
	uint8_t lightKey4Second = 0;
	// gd_eval_key_state_get
	
	
#endif
}

void test3(void) {

}