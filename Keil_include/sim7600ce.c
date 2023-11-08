#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"

uint8_t SIM900_CSQ[3];	//信号质量
uint8_t CSQ_Flag = 0;	//信号质量请求标志位
uint8_t dtbuf[50];   	//打印缓存器	，存服务器IP和端口号的信息
uint32_t send_data_count = 0; //测试用

//sim800c使能脚SIM_KEY，默认上电就使能
void GSM_ON_GPIO_Config(void) {
		rcu_periph_clock_enable(RCU_GPIOA);
		//gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_4); //初始化PA端口
		//gpio_bit_set(GPIOA,GPIO_PIN_4);//SIM使能
		//nopdelay_ms(1500);
		//gpio_bit_reset(GPIOA,GPIO_PIN_4);
}

//sim900a发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//其他,期待应答结果的位置(str的位置)
uint8_t* sim900a_check_cmd(uint8_t *str) {
		char *strx = 0;
		strx = strstr((const char*)USART2_RX_BUF,(const char*)str);
		return (uint8_t*)strx;
}

//向sim900a发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
uint8_t sim900a_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime) {
		uint8_t res=0; 
		UART_DMA_Enable(DMA0, DMA_CH6, USART2_MAX_RECV_LEN);//重新恢复DMA指针
		if((uint32_t)cmd<=0XFF)
		{
			while(DMA_CH6CNT(DMA0)!=0);	//等待通道7传输完成   
			USART_DATA(USART1)=(u32)cmd;
		}else u2_printf("%s\r\n",cmd);//发送命令
		
		if(ack&&waittime)		//需要等待应答
		{
			while(--waittime)	//等待倒计时
			{
				nopdelay_ms(10);
				if(sim900a_check_cmd(ack))
					break;//得到有效数据 
			}
			if(waittime==0)res=1; 
		}
		
		if(CSQ_Flag == 1)//由于数据每次会清零，只能将信号质量请求放在这
		{
			//void *memcpy(void *dest, const void *src, size_t n);
			//从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中。
			memcpy(SIM900_CSQ,USART2_RX_BUF+15,2);
			CSQ_Flag = 0;
		}
		memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN); //数组清零
		return res;
} 
/*
AT+CSQ

+CSQ: 15,99

其中的15为信号强度值，99为误码率，信号在（10～31）之间均为有效值，如当地信号强的话一般不会小于20．
误码率直接影响信号的质量，CDMA网络信号强度99表示误码率最小；
GPRS网络信号强度0表示误码率最小，GPRS接收信号的质量，一般要求误码<0.2%（0对应<0.2%）。
*/

/*
AT+CREG 网络注册和漫游
返回：+CREG :<mode>,<stat>
<mode>
0 : 禁止网络注册主动提供结果代码（默认设置）
<stat>
0：未注册，终端当前并未在搜寻新的运营商
1：已注册本地网络
2：未注册，终端正在搜寻基站
4：未知代码
5：已注册，处于漫游状态
*/

u8 sim900a_work_test(void)
{
	if(sim900a_send_cmd((u8 *)"AT",(u8 *)"OK",100))
	{
		if(sim900a_send_cmd((u8 *)"AT",(u8 *)"OK",100))return SIM_COMMUNTION_ERR;	//是否能与SIM800C通信
	}		
	if(sim900a_send_cmd((u8 *)"AT+CPIN?",(u8 *)"READY",400))return SIM_CPIN_ERR;	//查看SIM卡的状态
	if(sim900a_send_cmd((u8 *)"AT+CREG?",(u8 *)"0,1",400))							//网络注册情况
	{
		if(strstr((const char*)USART2_RX_BUF,"0,5")==NULL)  //非漫游时
		{
			CSQ_Flag = 1;
			sim900a_send_cmd((u8 *)"AT+CSQ",(u8 *)"OK",200);	
			return SIM_CREG_FAIL;	//等待附着到网络
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
			printf("GSM模块自检成功\r\n");
			break;
		case SIM_COMMUNTION_ERR:
			printf("与GSM模块未通讯成功，请等待\r\n");
			break;
		case SIM_CPIN_ERR:
			printf("没检测到SIM卡\r\n");
			break;
		case SIM_CREG_FAIL:
			printf("注册网络中....\r\n\r\n");
			printf("当前信号值：%s\r\n",SIM900_CSQ);
			break;		
		default:
			break;
	}
	return res;
}
// 7600CE用
u8 SIM900A_CONNECT_SERVER(u8 *IP_ADD,u8 *COM)
{
	
	if(sim900a_send_cmd((u8 *)"AT+CIPMODE=1",(u8 *)"OK",500))	 return 1;	//打开透传模式
	if(sim900a_send_cmd((u8 *)"AT+NETOPEN",(u8 *)"+NETOPEN: 0",800))	 return 2;	//打开网络

	sprintf((char*)dtbuf,"AT+CIPOPEN=0,\"TCP\",\"%s\",%s",IP_ADD,COM);	
	if(sim900a_send_cmd((u8 *)dtbuf,(u8 *)"NETOPEN",500))	{};		//建立TCP连接	
	
	return 0;
}
//7600，透传模式
u8 SIM900A_CONNECT_SERVER_SEND_INFOR(u8 *IP_ADD,u8 *COM)
{
	u8 res;	
	res=SIM900A_CONNECT_SERVER(IP_ADD,COM);
	switch(res)
	{
		case 1:
			printf("SIM卡开启网络失败\r\n");
			break;
		case 2:
			printf("透传模式设置失败\r\n");
			break;
		default:
			break;
	}
	return res;
}
//用来发送心跳包
//未用到
u8 SIM900A_GPRS_SEND_DATA(u8 *temp_data)
{		

	if(sim900a_send_cmd((u8 *)"AT+CIPSEND",(u8 *)">",200)==0)	
	{
		sim900a_send_cmd(temp_data,0 ,0);
		nopdelay_ms(40);
		sim900a_send_cmd((u8 *)0x1A,0,800);			
	}
	else 
		sim900a_send_cmd((u8*)0X1B,0,0);	//ESC,取消发送
	return 0;
}	

//透传模式，专门用来发送数组，len为数组定义的长度（包括2个CRC位）
//带crc_modbus检验
//在程序中直接用sizeof(temp_data)得到的值与实际不符，原因是temp_data进入函数中就是指针了，sizeof(temp_data)只能得到指针变量的大小，即u8,1个字节。
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
	while(DMA_CH6CNT(DMA0)!=0);//等待通道7传输完成 
	UART_DMA_Enable(DMA0, DMA_CH6,len);	//通过dma发送出去	
	nopdelay_ms(100);
	
	send_data_count++;
	
	return 0;
}

//透传模式，专门用来发送数组，len为数组定义的长度
//不带crc_modbus检验
u8 SIM900A_GPRS_SEND_DATA_NO_CRC(u8 *temp_data, u8 len)
{	
	if(len<=1) return 2;
	if(temp_data[1] == 0xFE) {
		send_heart_beat_count++;
	}
	memcpy (USART2_TX_BUF, temp_data, len);
	
	while(DMA_CH6CNT(DMA0)!=0);//等待通道7传输完成 
	UART_DMA_Enable(DMA0, DMA_CH6,len);	//通过dma发送出去	
	nopdelay_ms(100);
	
	send_data_count++;
	
	return 0;
}	










