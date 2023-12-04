#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "pwm.h"
#include "systick.h"
#include <stdio.h>
#include "user.h"

//char speed_str[4];
//char *dir;
	 	 

uint16_t phase[]={0x0200,0x0100,0x0080,0x0040};
/**
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
  */

void TIM3_Init(uint16_t arr,uint16_t psc) {               //修改部分   增加定时器中断
		timer_parameter_struct timer_initpara;
	
		rcu_periph_clock_enable(RCU_TIMER3); 
		timer_deinit(TIMER3);																		//复位定时器
	
		//定时器TIM3初始化
    timer_initpara.prescaler         = psc;									//设置用来作为TIMx时钟频率除数的预分频值
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;	//对齐
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;		//TIM向上计数模式
    timer_initpara.period            = arr;									//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		//设置时钟分割:TDTS = Tck_tim
    timer_init(TIMER3,&timer_initpara);
	
	  timer_interrupt_enable(TIMER3,TIMER_INT_UP);						//使能指定的TIM3中断,允许更新中断
	
		timer_enable(TIMER3);																		//使能TIM3
}

void motor_gpio_config(void)
{
 	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	
	gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
	
	gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_8);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	
	gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
}

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */


void motor_stop(void)
{
	gpio_bit_set(GPIOC, GPIO_PIN_6);
	gpio_bit_set(GPIOC, GPIO_PIN_7);
	gpio_bit_set(GPIOC, GPIO_PIN_8);
	gpio_bit_set(GPIOC, GPIO_PIN_9);
}







