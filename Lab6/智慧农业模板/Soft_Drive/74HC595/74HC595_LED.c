#include "gd32f4xx.h"
#include "74HC595_LED.h"
#include "systick.h"

/********  74HC595 GPIO 配置 *************************/
void HC595_GPIO_Config(void)
{		
	rcu_periph_clock_enable(RCU_GPIOB);
	
	gpio_mode_set(SHCP_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SHCP_GPIO_PIN);
	gpio_output_options_set(SHCP_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SHCP_GPIO_PIN);
	
	gpio_mode_set(STCP_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, STCP_GPIO_PIN);
	gpio_output_options_set(STCP_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, STCP_GPIO_PIN);
	
	gpio_mode_set(DS_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DS_GPIO_PIN);
	gpio_output_options_set(DS_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, DS_GPIO_PIN);
	
}

/***
 *74HC595 发送一个字节 
 *即往74HC595的DS引脚发送一个字节
*/
void HC595_Send_Byte(uint8_t byte)
{
	uint8_t i;
	uint8_t j;
	for (i = 0; i < 8; i ++)  //一个字节8位，传输8次，一次一位，循环8次，刚好移完8位
	{
	  /****  步骤1：将数据传到DS引脚    ****/
		if (byte & 0x80)      	//先传输高位，通过与运算判断第八是否为1
				HC595_Data(1);    	//如果第八位是1，则与 595 DS连接的引脚输出高电平
		else                    //否则输出低电平
				HC595_Data(0);
		
		/*** 步骤2：SHCP每产生一个上升沿，当前的bit就被送入移位寄存器 ***/
		HC595_SHCP(0);   // SHCP拉低
		for(j=0;j<10;j++);
		HC595_SHCP(1);   // SHCP拉高， SHCP产生上升沿

		byte <<= 1;		// 左移一位，将低位往高位移，通过	if (byte & 0x80)判断低位是否为1
	}
}

/**
 *74HC595输出锁存 使能 
**/
void HC595_CS(void) {
	
	uint8_t j;
	/**  步骤3：STCP产生一个上升沿，移位寄存器的数据移入存储寄存器  **/
	HC595_STCP(0);   // 将STCP拉低
	for(j=0;j<10;j++);
	HC595_STCP(1);  // 再将STCP拉高，STCP即可产生一个上升沿
}


/**
 *发送多个字节
 *便于级联时数据的发送
 *级联N级，就需要发送N个字节控制HC595
***/
void HC595_Send_Multi_Byte(uint8_t *data, uint16_t len) {
	uint8_t i;
	for (i = 0; i < len; i ++ ) { // len 个字节
		HC595_Send_Byte(data[i]);
	}
	
	HC595_CS(); //先把所有字节发送完，再使能输出
}

