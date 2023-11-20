#include "gd32f4xx.h"
#include "74HC595_LED.h"
#include "systick.h"

/********  74HC595 GPIO ���� *************************/
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
 *74HC595 ����һ���ֽ� 
 *����74HC595��DS���ŷ���һ���ֽ�
*/
void HC595_Send_Byte(uint8_t byte)
{
	uint8_t i;
	uint8_t j;
	for (i = 0; i < 8; i ++)  //һ���ֽ�8λ������8�Σ�һ��һλ��ѭ��8�Σ��պ�����8λ
	{
	  /****  ����1�������ݴ���DS����    ****/
		if (byte & 0x80)      	//�ȴ����λ��ͨ���������жϵڰ��Ƿ�Ϊ1
				HC595_Data(1);    	//����ڰ�λ��1������ 595 DS���ӵ���������ߵ�ƽ
		else                    //��������͵�ƽ
				HC595_Data(0);
		
		/*** ����2��SHCPÿ����һ�������أ���ǰ��bit�ͱ�������λ�Ĵ��� ***/
		HC595_SHCP(0);   // SHCP����
		for(j=0;j<10;j++);
		HC595_SHCP(1);   // SHCP���ߣ� SHCP����������

		byte <<= 1;		// ����һλ������λ����λ�ƣ�ͨ��	if (byte & 0x80)�жϵ�λ�Ƿ�Ϊ1
	}
}

/**
 *74HC595������� ʹ�� 
**/
void HC595_CS(void) {
	
	uint8_t j;
	/**  ����3��STCP����һ�������أ���λ�Ĵ�������������洢�Ĵ���  **/
	HC595_STCP(0);   // ��STCP����
	for(j=0;j<10;j++);
	HC595_STCP(1);  // �ٽ�STCP���ߣ�STCP���ɲ���һ��������
}


/**
 *���Ͷ���ֽ�
 *���ڼ���ʱ���ݵķ���
 *����N��������Ҫ����N���ֽڿ���HC595
***/
void HC595_Send_Multi_Byte(uint8_t *data, uint16_t len) {
	uint8_t i;
	for (i = 0; i < len; i ++ ) { // len ���ֽ�
		HC595_Send_Byte(data[i]);
	}
	
	HC595_CS(); //�Ȱ������ֽڷ����꣬��ʹ�����
}

