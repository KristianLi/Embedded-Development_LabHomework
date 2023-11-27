#ifndef _TIMER_H
#define _TIMER_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
	 
void motor_gpio_config(void);
void timer_config(void);	 
void motor_drive(void);


#ifdef cplusplus
}
#endif

#endif 




