#include "gd32f4xx.h"
#include "i2c.h"
#include <stdio.h>

/*    EEPROM���߶���
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
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C1);
    /* connect PB10 to I2C1_SCL */
    gpio_af_set(GPIOB,GPIO_AF_4,GPIO_PIN_10);
    /* connect PB11 to I2C1_SDA */
    gpio_af_set(GPIOB,GPIO_AF_4,GPIO_PIN_11);
	/* configure I2C1 GPIO */
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_10);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ,GPIO_PIN_10);
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_11);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ,GPIO_PIN_11);
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
    rcu_periph_clock_enable(RCU_I2C1);
    /* configure I2C clock */
    i2c_clock_config(I2C1,I2C1_SPEED,I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(I2C1,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,I2C1_SLAVE_ADDRESS7);
    /* enable I2C1 */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1,I2C_ACK_ENABLE);
}





