#ifndef _TEST_H
#define _TEST_H

#include "gd32f4xx.h"

#define EEP_FIRST_PAGE           0x00						//EEPROM起始地址
#define I2C_OK                 	 0
#define I2C_FAIL               	 1					
#define NR(x)  (sizeof(x) / sizeof(x[0]));				//计算数组的长度

void systemInit(void);
void test1(void);
void test2(void);
void test3(void);
uint8_t i2c_24c02_test(void);
ErrStatus trng_ready_check(void);
ErrStatus trng_configuration(void);
uint8_t trng_random_range_get(uint8_t min, uint8_t max);


#endif  


