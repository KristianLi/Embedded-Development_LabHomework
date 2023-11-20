#ifndef _TEST_H
#define _TEST_H

#ifdef cplusplus
 extern "C" {
#endif
	 
#include "gd32f4xx.h"
	 
void system_init(void);
void test1(void);	 
void test2(void);
void test3(void);
void lightLED(void);
void lightLEDImmediately(void);
void extinguishLED(void);
void extinguishLEDImmediately(void);
void flashLED(void);
uint8_t getDipNumber(void);	 
	 
#ifdef cplusplus
}
 
#endif

#endif 


