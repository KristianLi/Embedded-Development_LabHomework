#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "timer.h"
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

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void timer_config(void)
{
    /* -----------------------------------------------------------------------
    TIMER1 configuration: generate 3 PWM signals with 3 different duty cycles:
    TIMER1CLK = SystemCoreClock / 120 = 1MHz

    TIMER1 channel1 duty cycle = (4000/ 16000)* 100  = 25%
    TIMER1 channel2 duty cycle = (8000/ 16000)* 100  = 50%
    TIMER1 channel3 duty cycle = (12000/ 16000)* 100 = 75%
    ----------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER7);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER7);

    /* TIMER1 configuration */
    timer_initpara.prescaler         = 119;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 15999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER7,&timer_initpara);

    /* CH1,CH2 and CH3 configuration in PWM mode1 */
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;

		timer_channel_output_config(TIMER7,TIMER_CH_0,&timer_ocintpara);
    timer_channel_output_config(TIMER7,TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMER7,TIMER_CH_2,&timer_ocintpara);
    timer_channel_output_config(TIMER7,TIMER_CH_3,&timer_ocintpara);

    /* CH0 configuration in PWM mode1,duty cycle 25% */
    timer_channel_output_pulse_value_config(TIMER7,TIMER_CH_0,3999);
    timer_channel_output_mode_config(TIMER7,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER7,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);

    /* CH1 configuration in PWM mode1,duty cycle 25% */
    timer_channel_output_pulse_value_config(TIMER7,TIMER_CH_1,3999);
    timer_channel_output_mode_config(TIMER7,TIMER_CH_1,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER7,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);

    /* CH2 configuration in PWM mode1,duty cycle 50% */
    timer_channel_output_pulse_value_config(TIMER7,TIMER_CH_2,3999);
    timer_channel_output_mode_config(TIMER7,TIMER_CH_2,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER7,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);

    /* CH3 configuration in PWM mode1,duty cycle 75% */
    timer_channel_output_pulse_value_config(TIMER7,TIMER_CH_3,3999);
    timer_channel_output_mode_config(TIMER7,TIMER_CH_3,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER7,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER7);
    /* auto-reload preload enable */
    timer_enable(TIMER7);
}



//��ʱ��

u16 volatile tim3_count;
u8 led_check=0;
int test_flag = 0;//���ڼ�������Ƿ���ͬ������ͬ�͸�������������Ϣ

void TIM3_Init(u16 arr,u16 psc)
{
 timer_parameter_struct timer_initpara;
	
	rcu_periph_clock_enable(RCU_TIMER3);
	timer_deinit(TIMER3);//��λ��ʱ��
	
	//��ʱ��TIM3��ʼ��
	 /* TIMER3 configuration */
    timer_initpara.prescaler         = psc;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;//����
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;//TIM���ϼ���ģʽ
    timer_initpara.period            = arr;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;//����ʱ�ӷָ�:TDTS = Tck_tim
//    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER3,&timer_initpara);
//	  timer_interrupt_flag_clear(TIMER3,TIMER_INT_FLAG_UP); 
	  timer_interrupt_enable(TIMER3,TIMER_INT_UP);//ʹ��ָ����TIM3�ж�,��������ж�
	
	//����NVIC����
	nvic_irq_enable(TIMER3_IRQn,3,3);
	
	timer_enable(TIMER3);//ʹ��TIM3
	
}


void TIMER3_IRQHandler(void)   //TIM3�ж�
{
	if (SET==timer_interrupt_flag_get(TIMER3,TIMER_INT_UP)) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
			timer_interrupt_flag_clear(TIMER3,TIMER_INT_UP); //���TIMx���жϴ�����λ:TIM �ж�Դ 
			if(NET_flag == 1 && tim3_count%40 == 0 )//ÿ20s��������һ��
			{
				
			//Uart0Printf("hello\r\n");
//			test_check();					
				
			}
			tim3_count ++ ;
		  if(tim3_count>60000)
			tim3_count = 0;	
	}
}


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
void motor_stop(volatile uint8_t)
{
	gpio_bit_reset(GPIOC,GPIO_PIN_6);	
	gpio_bit_reset(GPIOC,GPIO_PIN_7);
	gpio_bit_reset(GPIOC,GPIO_PIN_8);
	gpio_bit_reset(GPIOC,GPIO_PIN_9);
}

















