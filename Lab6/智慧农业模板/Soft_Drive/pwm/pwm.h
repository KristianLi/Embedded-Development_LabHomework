#ifndef _PWM_H
#define _PWM_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
	 
void motor_gpio_config(void);
void motor_stop(void);


#ifdef cplusplus
}
#endif

#endif 




