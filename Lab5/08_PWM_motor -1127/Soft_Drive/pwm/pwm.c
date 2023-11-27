#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "pwm.h"
#include "systick.h"
#include <stdio.h>
#include "user.h"

char speed_str[4];
//char *dir;
	 	 

void motor_gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
		//rcu_periph_clock_enable(RCU_AF);
    
//    /*Configure PC6 PC7 PC8 PC9(TIMER7 CH0 CH1 CH2 CH3) as alternate function*/
//    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
//    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);

//    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
//    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);

//    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
//    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8);
//	
//    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
//    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);	
//	
//		gpio_af_set(GPIOC, GPIO_AF_3, GPIO_PIN_6);
//    gpio_af_set(GPIOC, GPIO_AF_3, GPIO_PIN_7);
//    gpio_af_set(GPIOC, GPIO_AF_3, GPIO_PIN_8);
//		gpio_af_set(GPIOC, GPIO_AF_3, GPIO_PIN_9);

    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);

    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);

    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_8);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8);
	
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);	
}
uint16_t phase[]={0x0200,0x0100,0x0080,0x0040};
/**
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
  */

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */





void motor_fanz(uint8_t speed)
{
	gpio_bit_set(GPIOC,GPIO_PIN_6);
	gpio_bit_reset(GPIOC,GPIO_PIN_7);
	gpio_bit_reset(GPIOC,GPIO_PIN_8);
	gpio_bit_reset(GPIOC,GPIO_PIN_9);
	delay_1ms(speed);
	gpio_bit_reset(GPIOC,GPIO_PIN_6);
	gpio_bit_set(GPIOC,GPIO_PIN_7);
	gpio_bit_reset(GPIOC,GPIO_PIN_8);
	gpio_bit_reset(GPIOC,GPIO_PIN_9);
	delay_1ms(speed);
	gpio_bit_reset(GPIOC,GPIO_PIN_6);
	gpio_bit_reset(GPIOC,GPIO_PIN_7);
	gpio_bit_set(GPIOC,GPIO_PIN_8);
	gpio_bit_reset(GPIOC,GPIO_PIN_9);
	delay_1ms(speed);
	gpio_bit_reset(GPIOC,GPIO_PIN_6);
	gpio_bit_reset(GPIOC,GPIO_PIN_7);
	gpio_bit_reset(GPIOC,GPIO_PIN_8);
	gpio_bit_set(GPIOC,GPIO_PIN_9);
	delay_1ms(speed);
}

void motor_zhez(uint8_t speed)
{
	gpio_bit_reset(GPIOC,GPIO_PIN_6);
	gpio_bit_reset(GPIOC,GPIO_PIN_7);
	gpio_bit_reset(GPIOC,GPIO_PIN_8);
	gpio_bit_set(GPIOC,GPIO_PIN_9);
	delay_1ms(speed);
	gpio_bit_reset(GPIOC,GPIO_PIN_6);
	gpio_bit_reset(GPIOC,GPIO_PIN_7);
	gpio_bit_set(GPIOC,GPIO_PIN_8);
	gpio_bit_reset(GPIOC,GPIO_PIN_9);
	delay_1ms(speed);
	gpio_bit_reset(GPIOC,GPIO_PIN_6);
	gpio_bit_set(GPIOC,GPIO_PIN_7);
	gpio_bit_reset(GPIOC,GPIO_PIN_8);
	gpio_bit_reset(GPIOC,GPIO_PIN_9);
	delay_1ms(speed);
	gpio_bit_set(GPIOC,GPIO_PIN_6);
	gpio_bit_reset(GPIOC,GPIO_PIN_7);
	gpio_bit_reset(GPIOC,GPIO_PIN_8);
	gpio_bit_reset(GPIOC,GPIO_PIN_9);
	delay_1ms(speed);
}
void motor_stop(void)
{
	gpio_bit_reset(GPIOC,GPIO_PIN_6);	
	gpio_bit_reset(GPIOC,GPIO_PIN_7);
	gpio_bit_reset(GPIOC,GPIO_PIN_8);
	gpio_bit_reset(GPIOC,GPIO_PIN_9);
}







