#include "gd32f4xx.h"
#include "i2c.h"
#include <stdio.h>

/*    EEPROM接线定义
			PB10   ---    IIC1_SCL	
			PB11   ---    IIC1_SDA   
*/

/*!
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_gpio_config(void) {
    /* enable GPIOB clock */

    /* enable I2C1 clock */


    /* connect PB10 to I2C1_SCL */

    /* connect PB11 to I2C1_SDA */

	
		/* configure I2C1 GPIO */

//gpio_mode_set(uint32_t gpio_periph,uint32_t mode,uint32_t pull_up_down,uint32_t pin);
//gpio_output_options_set(uint32_t gpio_periph,uint8_t otype,uint32_t speed,uint32_t pin)
}

/*!
    \brief      configure the I2C0 interfaces
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_config(void) {
    /* enable I2C clock */

    /* configure I2C clock */

    /* configure I2C address */

    /* enable I2C1 */

    /* enable acknowledge */

}





