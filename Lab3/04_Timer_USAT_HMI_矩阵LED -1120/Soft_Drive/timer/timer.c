#include "gd32f4xx.h"
#include "timer.h"
#include "gd32f4xx_XII-IOT.h"
#include "user.h"


u8 led_check=0;

void TIM3_Init(uint16_t arr,uint16_t psc) {
		timer_parameter_struct timer_initpara;
	
		rcu_periph_clock_enable(RCU_TIMER3);
		timer_deinit(TIMER3);																		//复位定时器
	
		nvic_irq_enable(TIMER3_IRQn,3,3);												//配置NVIC分组
	
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

void TIM4_Init(uint16_t arr,uint16_t psc) {
		timer_parameter_struct timer_initpara;
	
		rcu_periph_clock_enable(RCU_TIMER4);
		timer_deinit(TIMER4);																		//复位定时器
	
		nvic_irq_enable(TIMER4_IRQn,3,3);												//配置NVIC分组

    timer_initpara.prescaler         = psc;									//设置用来作为TIMx时钟频率除数的预分频值
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;	//对齐
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;		//TIM向上计数模式
    timer_initpara.period            = arr;									//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		//设置时钟分割:TDTS = Tck_tim
    timer_init(TIMER4,&timer_initpara);

	  timer_interrupt_enable(TIMER4,TIMER_INT_UP);						//使能指定的TIM3中断,允许更新中断
		timer_enable(TIMER4);																		//使能TIM3
}






void TIMER3_IRQHandler(void)   
{
		if (SET==timer_interrupt_flag_get(TIMER3,TIMER_INT_UP)) //检查指定的TIM中断发生与否:TIM 中断源 
		{


			
		}
}












