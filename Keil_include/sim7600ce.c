#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"

uint8_t SIM900_CSQ[3];	//�ź�����
uint8_t CSQ_Flag = 0;	//�ź����������־λ
uint8_t dtbuf[50];   	//��ӡ������	���������IP�Ͷ˿ںŵ���Ϣ
uint32_t send_data_count = 0; //������

//sim800cʹ�ܽ�SIM_KEY��Ĭ���ϵ��ʹ��
void GSM_ON_GPIO_Config(void) {
		rcu_periph_clock_enable(RCU_GPIOA);
		//gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_4); //��ʼ��PA�˿�
		//gpio_bit_set(GPIOA,GPIO_PIN_4);//SIMʹ��
		//nopdelay_ms(1500);
		//gpio_bit_reset(GPIOA,GPIO_PIN_4);
}

//sim900a���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//����,�ڴ�Ӧ������λ��(str��λ��)
uint8_t* sim900a_check_cmd(uint8_t *str) {
		char *strx = 0;
		strx = strstr((const char*)USART2_RX_BUF,(const char*)str);
		return (uint8_t*)strx;
}

//��sim900a��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
uint8_t sim900a_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime) {
		uint8_t res=0; 
		UART_DMA_Enable(DMA0, DMA_CH6, USART2_MAX_RECV_LEN);//���»ָ�DMAָ��
		if((uint32_t)cmd<=0XFF)
		{
			while(DMA_CH6CNT(DMA0)!=0);	//�ȴ�ͨ��7�������   
			USART_DATA(USART1)=(u32)cmd;
		}else u2_printf("%s\r\n",cmd);//��������
		
		if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
		{
			while(--waittime)	//�ȴ�����ʱ
			{
				nopdelay_ms(10);
				if(sim900a_check_cmd(ack))
					break;//�õ���Ч���� 
			}
			if(waittime==0)res=1; 
		}
		
		if(CSQ_Flag == 1)//��������ÿ�λ����㣬ֻ�ܽ��ź��������������
		{
			//void *memcpy(void *dest, const void *src, size_t n);
			//��Դsrc��ָ���ڴ��ַ����ʼλ�ÿ�ʼ����n���ֽڵ�Ŀ��dest��ָ���ڴ��ַ����ʼλ���С�
			memcpy(SIM900_CSQ,USART2_RX_BUF+15,2);
			CSQ_Flag = 0;
		}
		memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN); //��������
		return res;
} 
/*
AT+CSQ

+CSQ: 15,99

���е�15Ϊ�ź�ǿ��ֵ��99Ϊ�����ʣ��ź��ڣ�10��31��֮���Ϊ��Чֵ���統���ź�ǿ�Ļ�һ�㲻��С��20��
������ֱ��Ӱ���źŵ�������CDMA�����ź�ǿ��99��ʾ��������С��
GPRS�����ź�ǿ��0��ʾ��������С��GPRS�����źŵ�������һ��Ҫ������<0.2%��0��Ӧ<0.2%����
*/

/*
AT+CREG ����ע�������
���أ�+CREG :<mode>,<stat>
<mode>
0 : ��ֹ����ע�������ṩ������루Ĭ�����ã�
<stat>
0��δע�ᣬ�ն˵�ǰ��δ����Ѱ�µ���Ӫ��
1����ע�᱾������
2��δע�ᣬ�ն�������Ѱ��վ
4��δ֪����
5����ע�ᣬ��������״̬
*/

u8 sim900a_work_test(void)
{
	if(sim900a_send_cmd((u8 *)"AT",(u8 *)"OK",100))
	{
		if(sim900a_send_cmd((u8 *)"AT",(u8 *)"OK",100))return SIM_COMMUNTION_ERR;	//�Ƿ�����SIM800Cͨ��
	}		
	if(sim900a_send_cmd((u8 *)"AT+CPIN?",(u8 *)"READY",400))return SIM_CPIN_ERR;	//�鿴SIM����״̬
	if(sim900a_send_cmd((u8 *)"AT+CREG?",(u8 *)"0,1",400))							//����ע�����
	{
		if(strstr((const char*)USART2_RX_BUF,"0,5")==NULL)  //������ʱ
		{
			CSQ_Flag = 1;
			sim900a_send_cmd((u8 *)"AT+CSQ",(u8 *)"OK",200);	
			return SIM_CREG_FAIL;	//�ȴ����ŵ�����
		}
	}	
	return SIM_OK;
}


u8 GSM_Dect(void)
{
	u8 res;
	res=sim900a_work_test();	
	switch(res)
	{
		case SIM_OK:
			printf("GSMģ���Լ�ɹ�\r\n");
			break;
		case SIM_COMMUNTION_ERR:
			printf("��GSMģ��δͨѶ�ɹ�����ȴ�\r\n");
			break;
		case SIM_CPIN_ERR:
			printf("û��⵽SIM��\r\n");
			break;
		case SIM_CREG_FAIL:
			printf("ע��������....\r\n\r\n");
			printf("��ǰ�ź�ֵ��%s\r\n",SIM900_CSQ);
			break;		
		default:
			break;
	}
	return res;
}
// 7600CE��
u8 SIM900A_CONNECT_SERVER(u8 *IP_ADD,u8 *COM)
{
	
	if(sim900a_send_cmd((u8 *)"AT+CIPMODE=1",(u8 *)"OK",500))	 return 1;	//��͸��ģʽ
	if(sim900a_send_cmd((u8 *)"AT+NETOPEN",(u8 *)"+NETOPEN: 0",800))	 return 2;	//������

	sprintf((char*)dtbuf,"AT+CIPOPEN=0,\"TCP\",\"%s\",%s",IP_ADD,COM);	
	if(sim900a_send_cmd((u8 *)dtbuf,(u8 *)"NETOPEN",500))	{};		//����TCP����	
	
	return 0;
}
//7600��͸��ģʽ
u8 SIM900A_CONNECT_SERVER_SEND_INFOR(u8 *IP_ADD,u8 *COM)
{
	u8 res;	
	res=SIM900A_CONNECT_SERVER(IP_ADD,COM);
	switch(res)
	{
		case 1:
			printf("SIM����������ʧ��\r\n");
			break;
		case 2:
			printf("͸��ģʽ����ʧ��\r\n");
			break;
		default:
			break;
	}
	return res;
}
//��������������
//δ�õ�
u8 SIM900A_GPRS_SEND_DATA(u8 *temp_data)
{		

	if(sim900a_send_cmd((u8 *)"AT+CIPSEND",(u8 *)">",200)==0)	
	{
		sim900a_send_cmd(temp_data,0 ,0);
		nopdelay_ms(40);
		sim900a_send_cmd((u8 *)0x1A,0,800);			
	}
	else 
		sim900a_send_cmd((u8*)0X1B,0,0);	//ESC,ȡ������
	return 0;
}	

//͸��ģʽ��ר�������������飬lenΪ���鶨��ĳ��ȣ�����2��CRCλ��
//��crc_modbus����
//�ڳ�����ֱ����sizeof(temp_data)�õ���ֵ��ʵ�ʲ�����ԭ����temp_data���뺯���о���ָ���ˣ�sizeof(temp_data)ֻ�ܵõ�ָ������Ĵ�С����u8,1���ֽڡ�
u8 SIM900A_GPRS_SEND_DATA_CRC(u8 *temp_data, u8 len)
{	
	u16 crc = 0;

	if(len<3) return 2;
	crc = crc_modbus(temp_data,(len-2));
	temp_data[len-1] = (u8)((crc&0xff00)>>8);
	temp_data[len-2] = (u8)(crc&0x00ff);
	
	if(temp_data[1] == 0x0F) {
		send_relay_op_count++;
	} else if (temp_data[1] == 0x01) {
		send_relay_status_count++;
	} else if (temp_data[1] == 0x04) {
		send_current_count++;
	} else if (temp_data[1] == 0x0D) {
		send_card_count++;
	} else if (temp_data[1] == 0x02) {
		send_start_connect_count++;
	} else if (temp_data[1] == 0x05) {
		send_mcu_out_count++;
	} else if (temp_data[1] == 0x06) {
		send_mcu_in_count++;
	}

	memcpy (USART2_TX_BUF, temp_data, len);
	while(DMA_CH6CNT(DMA0)!=0);//�ȴ�ͨ��7������� 
	UART_DMA_Enable(DMA0, DMA_CH6,len);	//ͨ��dma���ͳ�ȥ	
	nopdelay_ms(100);
	
	send_data_count++;
	
	return 0;
}

//͸��ģʽ��ר�������������飬lenΪ���鶨��ĳ���
//����crc_modbus����
u8 SIM900A_GPRS_SEND_DATA_NO_CRC(u8 *temp_data, u8 len)
{	
	if(len<=1) return 2;
	if(temp_data[1] == 0xFE) {
		send_heart_beat_count++;
	}
	memcpy (USART2_TX_BUF, temp_data, len);
	
	while(DMA_CH6CNT(DMA0)!=0);//�ȴ�ͨ��7������� 
	UART_DMA_Enable(DMA0, DMA_CH6,len);	//ͨ��dma���ͳ�ȥ	
	nopdelay_ms(100);
	
	send_data_count++;
	
	return 0;
}	










