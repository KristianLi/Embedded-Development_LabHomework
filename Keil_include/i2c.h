#ifndef I2C_H
#define I2C_H

#include "gd32f4xx.h"

#define I2C1_SPEED              100000
#define I2C1_SLAVE_ADDRESS7     0xA0
#define I2C_PAGE_SIZE           8

/* configure the GPIO ports */
void i2c_gpio_config(void);
/* configure the I2C1 interfaces */
void i2c_config(void);

#endif  /* I2C_H */


