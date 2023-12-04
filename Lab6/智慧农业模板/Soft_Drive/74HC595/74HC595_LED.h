#ifndef _74HC595_LED_H
#define	_74HC595_LED_H
 
#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h" 

/*
		PB1   12管脚		STCP/RCK，输出存储器锁存时钟线/存储寄存器时钟输入
		PB13	11管脚		SHCP/SCK, 数据输入时钟线/移位寄存器时钟输入
		PB15  14管脚		DS        数据线
		
		Q7S   9管脚							  串行数据输出
		OE		13管脚接地
		SCLR	10管脚接3.3V        主复位
		
*/

#define SHCP_GPIO_PORT      GPIOB			              
#define SHCP_GPIO_CLK 	    RCU_GPIOB	
#define SHCP_GPIO_PIN	    	GPIO_PIN_13			       
 
#define STCP_GPIO_PORT      GPIOB			              
#define STCP_GPIO_CLK 	    RCU_GPIOB			
#define STCP_GPIO_PIN	    	GPIO_PIN_1			        
 
#define DS_GPIO_PORT        GPIOB			              
#define DS_GPIO_CLK 	    	RCU_GPIOB		
#define DS_GPIO_PIN	    		GPIO_PIN_15	
 
#define HC595_SHCP(n)      	n?gpio_bit_set( SHCP_GPIO_PORT, SHCP_GPIO_PIN ):gpio_bit_reset(SHCP_GPIO_PORT, SHCP_GPIO_PIN)
#define HC595_STCP(n)     	n?gpio_bit_set( STCP_GPIO_PORT, STCP_GPIO_PIN ):gpio_bit_reset(STCP_GPIO_PORT, STCP_GPIO_PIN)
#define HC595_Data(n)       n?gpio_bit_set( DS_GPIO_PORT, DS_GPIO_PIN ):gpio_bit_reset(DS_GPIO_PORT, DS_GPIO_PIN)
 
void HC595_GPIO_Config(void);
void HC595_Send_Byte(uint8_t byte);
void HC595_CS(void);
void HC595_Send_Multi_Byte(uint8_t *data, uint16_t len);
void HC595_SelecTube(uint16_t wei,uint16_t duan);



#ifdef cplusplus
}
#endif
#endif


