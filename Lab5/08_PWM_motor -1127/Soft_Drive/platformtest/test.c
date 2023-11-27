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
	systick_config();	//时钟
    gd_XII_systeminit();	//XLL
    motor_gpio_config();	//电机GPIO
    OLED_Init();	//OLED
    //按键初始化？
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


//基础 电机转动
void test1(void)
{
    if(gpio_input_bit_get(DIP4_GPIO_PORT, DIP4_PIN) == RESET)
    {
        char str[4];
        speed=motor_speed;
        sprintf(str, "%.2f", speed);

        OLED_ShowString(0, 0, "speed:");
        OLED_ShowString(50, 0, str);
        sprintf(speed_str, "%.2f", speed);
        if(mode_select == 0)
            OLED_ShowString(0,2,"REVERSE");
        else OLED_ShowString(0,2,"POSITIVE");
    }
    else if(gpio_input_bit_get(DIP4_GPIO_PORT,DIP4_PIN)==SET)
    {
        if(forward==mode_select)
            motor_fanz(motor_speed);
        else if (mode_select==reverse)
            motor_zhez(motor_speed);
        else if(mode_select==stop)
            motor_stop(motor_speed);
    }
}

//进阶 按键控制电机转速、转向