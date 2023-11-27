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
char dir[] = "xxxxxxxxxx"; // 电机旋转方向
char pos[] = "Positive";
char rev[] = "Reverse";
char sto[] = "stop";

typedef enum {
    forward = 0,
    reverse = 1,
    stop = 2
}motor_mode;
//初始化 时钟、xll、电机gpio、oled、按键
void system_init(void) {
    systick_config();    //时钟
    gd_XII_systeminit();    //XLL
    motor_gpio_config();    //电机GPIO
    OLED_Gpio_Init(); // OLED?????????
    OLED_Init();    //OLED
    dipinit();
    //按键初始化
    gd_eval_key_init(USER1_KEY, KEY_MODE_EXTI);            //K1初始化成中断模式
    gd_eval_key_init(USER2_KEY, KEY_MODE_EXTI);            //K2初始化成中断模式
    gd_eval_key_init(USER3_KEY, KEY_MODE_EXTI);            //K3初始化成中断模式
    gd_eval_key_init(USER4_KEY, KEY_MODE_EXTI);            //K4初始化成中断模式}
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


//基础 电机转动
void test1(void)
{
	OLED_ShowString(0, 0, "speed:");
		motor8();
}

//进阶 按键控制电机转速、转向
void test2() {
    uint16_t i;
    if(gpio_input_bit_get(DIP4_GPIO_PORT, DIP4_PIN) == RESET) { // 拨码开关关闭 设置电机的转速和方向
        char str[4];
        sprintf(str, "%d.00", motor_speed); // 将全局motor_speed转换为字符串
        OLED_ShowString(0, 0, "speed:");
        OLED_ShowString(64, 0, str);

        if(mode_select == forward) { // 如果是正转 OLED上显示Positive
            OLED_ShowString(0, 2, "Positive  ");
            for(i = 0; i < 15; i++) {
                dir[i] = pos[i];
            }
        } else if(mode_select == reverse){ // 反转 显示Reverse
            OLED_ShowString(0, 2, "Reverse   ");
            for(i = 0; i < 15; i++) {
                dir[i] = rev[i];
            }
        } else {
            OLED_ShowString(0, 2, "Stop      "); // 停止 显示Stop
            for(i = 0; i < 15; i++) {
                dir[i] = sto[i];
            }
        }
    } else if(gpio_input_bit_get(DIP4_GPIO_PORT, DIP4_PIN) == SET) { // 拨码开关开启
        if(mode_select == reverse) { // 选择不同的模式运行
            motor_zhez(13-motor_speed);
        } else if(mode_select == forward) {
            motor_fanz(13-motor_speed);
        } else if(mode_select == stop) {
            motor_stop();
        }
    }
}

// 按下按键1，改变转速
void EXTI2_IRQHandler(void) {
    if(exti_interrupt_flag_get(USER1_KEY_EXTI_LINE)) {
        if(motor_speed++ >= 10) {
            motor_speed = 3;
        }
        exti_interrupt_flag_clear(USER1_KEY_EXTI_LINE);  // 清空标志
    }
}
// 按下按键2，改变转动的方向
void EXTI3_IRQHandler(void) {
    if(exti_interrupt_flag_get(USER2_KEY_EXTI_LINE)) {
        if(mode_select++ >= 2) {
            mode_select = 0;
        }
        exti_interrupt_flag_clear(USER2_KEY_EXTI_LINE);  // 清空标志
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