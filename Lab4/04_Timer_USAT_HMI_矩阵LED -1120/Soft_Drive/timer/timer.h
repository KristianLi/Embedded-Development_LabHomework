#ifndef _TIMER_H
#define _TIMER_H

#ifdef cplusplus
 extern "C" {
#endif
	 
#include "gd32f4xx.h"	 
extern volatile uint16_t  tim3_count;	 
void TIM3_Init(uint16_t arr, uint16_t psc);	 
void TIM4_Init(uint16_t arr,uint16_t psc);	 
void TIMER4_IRQHandler(void);

	 
#ifdef cplusplus
}
#endif

#endif	 



