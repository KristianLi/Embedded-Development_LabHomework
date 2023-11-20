#include "gd32f4xx.h"
#include "timer.h"
#include "gd32f4xx_XII-IOT.h"
#include "user.h"


u8 led_check=0;

void TIM3_Init(uint16_t arr,uint16_t psc) {
		timer_parameter_struct timer_initpara;
	
		rcu_periph_clock_enable(RCU_TIMER3);
		timer_deinit(TIMER3);																		//��λ��ʱ��
	
		nvic_irq_enable(TIMER3_IRQn,3,3);												//����NVIC����
	
		//��ʱ��TIM3��ʼ��
    timer_initpara.prescaler         = psc;									//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;	//����
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;		//TIM���ϼ���ģʽ
    timer_initpara.period            = arr;									//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		//����ʱ�ӷָ�:TDTS = Tck_tim
    timer_init(TIMER3,&timer_initpara);
	
	  timer_interrupt_enable(TIMER3,TIMER_INT_UP);						//ʹ��ָ����TIM3�ж�,��������ж�
	
		timer_enable(TIMER3);																		//ʹ��TIM3
}

void TIM4_Init(uint16_t arr,uint16_t psc) {
		timer_parameter_struct timer_initpara;
	
		rcu_periph_clock_enable(RCU_TIMER4);
		timer_deinit(TIMER4);																		//��λ��ʱ��
	
		nvic_irq_enable(TIMER4_IRQn,3,3);												//����NVIC����

    timer_initpara.prescaler         = psc;									//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;	//����
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;		//TIM���ϼ���ģʽ
    timer_initpara.period            = arr;									//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		//����ʱ�ӷָ�:TDTS = Tck_tim
    timer_init(TIMER4,&timer_initpara);

	  timer_interrupt_enable(TIMER4,TIMER_INT_UP);						//ʹ��ָ����TIM3�ж�,��������ж�
		timer_enable(TIMER4);																		//ʹ��TIM3
}






void TIMER3_IRQHandler(void)   
{
		if (SET==timer_interrupt_flag_get(TIMER3,TIMER_INT_UP)) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{


			
		}
}












