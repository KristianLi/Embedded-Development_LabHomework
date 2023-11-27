#ifndef _TEST_H
#define _TEST_H

#ifdef cplusplus
 extern "C" {
#endif
#include "gd32f4xx.h"
	 
void system_init(void);
void test1(void);	 
//void motor_drive(uint8_t speed);
void motor_fanz(uint8_t speed);
void motor_zhez(uint8_t speed);
void motor_stop(void);
	 
#ifdef cplusplus
}
#endif

#endif 


