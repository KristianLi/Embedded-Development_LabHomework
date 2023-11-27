#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "pwm.h"
#include "test.h"
#include "systick.h"
#include "oled.h"
#include "user.h"
#include "diwen.h"
volatile uint16_t tim3_count;
volatile uint8_t motor_speed = 3;
volatile uint8_t mode_select;
char dir[] = "xxxxxxxxxx"; // �����ת����
char pos[] = "Positive";
char rev[] = "Reverse";
char sto[] = "stop";

typedef enum {
    forward = 0,
    reverse = 1,
    stop = 2
}motor_mode;
//��ʼ�� ʱ�ӡ�xll�����gpio��oled������
void system_init(void) {
    systick_config();    //ʱ��
    gd_XII_systeminit();    //XLL
    motor_gpio_config();    //���GPIO
    OLED_Gpio_Init(); // OLED?????????
    OLED_Init();    //OLED
    dipinit();
    //������ʼ��
    gd_eval_key_init(USER1_KEY, KEY_MODE_EXTI);            //K1��ʼ�����ж�ģʽ
    gd_eval_key_init(USER2_KEY, KEY_MODE_EXTI);            //K2��ʼ�����ж�ģʽ
    gd_eval_key_init(USER3_KEY, KEY_MODE_EXTI);            //K3��ʼ�����ж�ģʽ
    gd_eval_key_init(USER4_KEY, KEY_MODE_EXTI);            //K4��ʼ�����ж�ģʽ}
}

void motor4Z(void)
{
    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(3);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_reset(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(3);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_reset(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(3);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_reset(GPIOC, GPIO_PIN_9);
    delay_1ms(3);
}

void motor4(void)
{
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_reset(GPIOC, GPIO_PIN_9);
    delay_1ms(3);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_reset(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(3);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_reset(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(3);
    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(3);
}

void motor8(void)
{
    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(2);
    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    gpio_bit_reset(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(2);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_reset(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(2);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_reset(GPIOC, GPIO_PIN_7);
    gpio_bit_reset(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(2);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_reset(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(2);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_reset(GPIOC, GPIO_PIN_8);
    gpio_bit_reset(GPIOC, GPIO_PIN_9);
    delay_1ms(2);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_reset(GPIOC, GPIO_PIN_9);
    delay_1ms(2);
    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_reset(GPIOC, GPIO_PIN_9);
    delay_1ms(2);
}


//���� ���ת��
void test1(void)
{
	OLED_ShowString(0, 0, "speed:");
		motor8();
}

//���� �������Ƶ��ת�١�ת��
void test2() {
    uint16_t i;
    if(gpio_input_bit_get(DIP4_GPIO_PORT, DIP4_PIN) == RESET) { // ���뿪�عر� ���õ����ת�ٺͷ���
        char str[4];
        sprintf(str, "%d.00", motor_speed); // ��ȫ��motor_speedת��Ϊ�ַ���
        OLED_ShowString(0, 0, "speed:");
        OLED_ShowString(64, 0, str);

        if(mode_select == forward) { // �������ת OLED����ʾPositive
            OLED_ShowString(0, 2, "Positive  ");
            for(i = 0; i < 15; i++) {
                dir[i] = pos[i];
            }
        } else if(mode_select == reverse){ // ��ת ��ʾReverse
            OLED_ShowString(0, 2, "Reverse   ");
            for(i = 0; i < 15; i++) {
                dir[i] = rev[i];
            }
        } else {
            OLED_ShowString(0, 2, "Stop      "); // ֹͣ ��ʾStop
            for(i = 0; i < 15; i++) {
                dir[i] = sto[i];
            }
        }
    } else if(gpio_input_bit_get(DIP4_GPIO_PORT, DIP4_PIN) == SET) { // ���뿪�ؿ���
        if(mode_select == reverse) { // ѡ��ͬ��ģʽ����
            motor_zhez(13-motor_speed);
        } else if(mode_select == forward) {
            motor_fanz(13-motor_speed);
        } else if(mode_select == stop) {
            motor_stop();
        }
    }
}

// ���°���1���ı�ת��
void EXTI2_IRQHandler(void) {
    if(exti_interrupt_flag_get(USER1_KEY_EXTI_LINE)) {
        if(motor_speed++ >= 10) {
            motor_speed = 3;
        }
        exti_interrupt_flag_clear(USER1_KEY_EXTI_LINE);  // ��ձ�־
    }
}
// ���°���2���ı�ת���ķ���
void EXTI3_IRQHandler(void) {
    if(exti_interrupt_flag_get(USER2_KEY_EXTI_LINE)) {
        if(mode_select++ >= 2) {
            mode_select = 0;
        }
        exti_interrupt_flag_clear(USER2_KEY_EXTI_LINE);  // ��ձ�־
    }
}
void test3()
{
    while (uart5_rx_buffer[5] == 0x2A) {
        delay_1ms(100);
        test2();
        while (uart5_rx_buffer[8] == 0x00) {
            if(motor_speed++ >= 10) {
                motor_speed = 3;
            }
        }
    }
    while (uart5_rx_buffer[5] == 0x2B) {
        delay_1ms(100);
        test2();
        while (uart5_rx_buffer[8] == 0x00) {
            if(mode_select++ >= 2) {
                mode_select = 0;
            }
        }
    }
}
// 102A 102B