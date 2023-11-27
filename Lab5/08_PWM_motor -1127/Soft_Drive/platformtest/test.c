#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "pwm.h"
#include "test.h"
#include "systick.h"
#include "oled.h"
#include "user.h"


//初始化 时钟、xll、电机gpio、oled、按键
void system_init(void)
{
}

void motor_gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_8);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
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

void EXIT2_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(USER1_KEY_EXTI_LINE)) {
        if (motor_speed++ >=10)
            motor_speed = 3;
        exti_interrupt_flag_clear(USER1_KEY_EXTI_LINE);
    }
}

void EXIT3_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(USER2_KEY_EXTI_LINE)) {
        if (mode_select++ >= 2)
            mode_select = 0;
        exti_interrupt_flag_clear(USER2_KEY_EXTI_LINE);
    }
}


void motor_fanz1(uint8_t speed)
{
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

void motor_zhez1(uint8_t speed)
{
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_reset(GPIOC, GPIO_PIN_9);
    delay_1ms(speed);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_reset(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(speed);
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_reset(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(speed);
    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
    delay_1ms(speed);
}

void motor_stop(void)
{
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
}

//基础 电机转动
void test1(void)
{

}

//进阶 按键控制电机转速、转向