#include "user.h"

//sim800c用到的串口2


//stm32和gd32的串口命名有区别，stm32是usart1，usart2，usart3
//gd32是usart0，usart1，usart2
//以下声明为串口2，但配置时使用的是usart1


//串口2发送缓存区 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//发送缓冲,最大USART2_MAX_SEND_LEN字节
 	  
//串口2接收缓存区 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8 USART2_RX_REC_ATCOMMAD;

//初始化IO 串口2
//bound:波特率	  
void USART2_Init(u32 bound)
{
	dma_single_data_parameter_struct dma_init_struct;
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART1);
	rcu_periph_clock_enable(RCU_DMA0);
	
	usart_deinit(USART1);//复位串口2

/* connect port to USARTx_Tx */
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_2);

	/* connect port to USARTx_Rx */
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_3);
	
//USART2_TX   PA.2
	//gpio_mode_set(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2); //初始化PA端口
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP ,GPIO_PIN_2);					
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);
//USART2_RX	  PA.3
	//gpio_mode_set(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_3); //初始化PA端口
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP ,GPIO_PIN_3);					
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
	


//数据格式配置	
	  usart_deinit(USART1);
    usart_baudrate_set(USART1, 115200U);
   // usart_word_length_set(USART1, USART_WL_8BIT);//字长为8位数据格式
   // usart_stop_bit_set(USART1, USART_STB_1BIT);//一个停止位
   // usart_parity_config(USART1, USART_PM_NONE);//无奇偶校验位
  //  usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    //usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);//无硬件数据流控制
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);//使能USART2接收
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);//使能USART2发送
    usart_enable(USART1);//使能USART2串口
//配置NVIC分组
	nvic_irq_enable(USART1_IRQn,0,0);		
	 // usart_interrupt_enable(USART1, USART_INTEN_IDLEIE); //开启空闲中断
	usart_interrupt_enable(USART1, USART_INTEN_RBNEIE); //开启中断
	  
//		
//	
//	//DMA配置——串口接收RX
	  
    rcu_periph_clock_enable(RCU_DMA0);
    dma_deinit(DMA0, DMA_CH5);//通道选择
    dma_init_struct.direction =DMA_PERIPH_TO_MEMORY ;//存储器到外设模式
    dma_init_struct.memory0_addr = (uint32_t)USART2_RX_BUF;//DMA 存储器基地址
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;//存储器增量模式
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;//存储器数据长度:8位
    dma_init_struct.number = USART2_MAX_RECV_LEN;//数据传输量 
    dma_init_struct.periph_addr =USART1_DATA_ADDRESS;//DMA外设ADC基地址
		//dma_init_struct.periph_addr =USART_DATA(USART1);//DMA外设ADC基地址
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;//外设非增量模式
    //dma_init_struct.periph_width = DMA_PERIPH_WIDTH_8BIT;//外设数据长度:8位
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;//中等优先级
    dma_single_data_mode_init(DMA0, DMA_CH5, dma_init_struct);//初始化DMA Stream
    dma_circulation_disable(DMA0, DMA_CH5);//循环模式，超过数组自动回到开头
		dma_channel_subperipheral_select(DMA0, DMA_CH5, DMA_SUBPERI4);
    //dma_memory_to_memory_disable(DMA0, DMA_CH5);//DMA通道x没有设置为内存到内存传输
		dma_transfer_number_config(DMA0,DMA_CH5,USART2_MAX_RECV_LEN);
		dma_channel_enable(DMA0, DMA_CH5);//正式驱动DMA传输
//		
//		
////		//DMA配置——串口发送TX
//    dma_deinit(DMA0, DMA_CH6);//通道选择
//    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;//数据传输方向，从内存读取发送到外设
//    dma_init_struct.memory0_addr = (uint32_t)USART2_TX_BUF;//DMA 存储器基地址
//    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;//内存地址寄存器递增
//    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;//存储器数据长度:8位
//    dma_init_struct.number = 0;//DMA通道的DMA缓存的大小
//		dma_init_struct.periph_addr =USART1_DATA_ADDRESS;//DMA外设ADC基地址
//    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;//外设非增量模式
//    //dma_init_struct.periph_width = DMA_PERIPH_WIDTH_8BIT;//外设数据长度:8位
//    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;//中等优先级
//    dma_single_data_mode_init(DMA0, DMA_CH6, dma_init_struct);//根据DMA_InitStruct中指定的参数初始化DMA的通道USART2_Tx_DMA_Channel所标识的寄存器
//    dma_circulation_disable(DMA0, DMA_CH6);// 使用普通模式 ，只发送一次
//		dma_channel_subperipheral_select(DMA0, DMA_CH6, DMA_SUBPERI4);
//    //dma_memory_to_memory_disable(DMA0, DMA_CH6);//DMA通道x没有设置为内存到内存传输
//		//dma_channel_enable(DMA0, DMA_CH6);//正式驱动DMA传输
		
		usart_dma_transmit_config(USART1, USART_DENT_ENABLE); //使能串口2的DMA发送
		usart_dma_receive_config(USART1, USART_DENR_ENABLE); //使能串口2 DMA接收
}
  //////////////////////////////////////DMA发送配置部分//////////////////////////////////	   		    
   

//DMA指针复位，开启DMA传输
	void UART_DMA_Enable(uint32_t dma_periph, dma_channel_enum channelx,uint32_t len)
	{
		dma_channel_disable(dma_periph,channelx);//关闭 指示的通道 
		//dma_memory_address_config(dma_periph,channelx,DMA_MEMORY_0,len);
		dma_transfer_number_config(dma_periph,channelx,len);//配置通道还有多少数据要缓存
		dma_channel_enable(dma_periph,channelx);//开启DMA传输
	}
		
		
	//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
//   while(RESET == dma_flag_get(DMA0, DMA_CH6, DMA_INTF_FTFIF)){};
//	 while(RESET == dma_flag_get(DMA0, DMA_CH5, DMA_INTF_FTFIF)){};
	while(DMA_CH6CNT(DMA0)!=0){};	//等待通道7传输完成 
  UART_DMA_Enable(DMA0,DMA_CH6,strlen((const char*)USART2_TX_BUF)); 	//通过dma发送出去
	//gd_eval_led_on(LED3);
}


void u1_printf(char* fmt,...)  
{  
	uint32_t i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		
	for(j=0;j<i;j++)						
	{
		while (RESET == usart_flag_get(USART1, USART_FLAG_TBE))
		;	
		usart_data_transmit(USART1, (uint8_t)USART2_TX_BUF[j]);
	}	
}



//void USART1_IRQHandler(void)
//{
//	//u8 rxlen;
//	//u8 Phone_num = 0;
//	//u8 IMEI_num  = 0;
//	//u8 i = 0;
//	//u8 t = 0;
//	if(usart_interrupt_flag_get(USART1, USART_INT_IDLEIE) != RESET)  //闲时中断
//	{

//		dma_channel_disable(DMA0,DMA_CH5);
//		dma_flag_clear(DMA0,DMA_CH5,DMA_INTF_FTFIF);
//		usart_data_receive(USART1);
//    
//		Uart0Printf("%s\r\n",USART2_RX_BUF); //各式各样的信息显示,调试时务必打开
//			
//		
//		if(strstr((char*)USART2_RX_BUF,"CLOSED")!=NULL) 	 NET_flag = 0;	//网络已断开
//		else if(strstr((char*)USART2_RX_BUF,"CONNECT")!=NULL)  NET_flag = 1;	//网络已连接
//		

//		usart_interrupt_flag_clear(USART1, USART_INT_IDLEIE);//清除中断标志
//		//usart_flag_clear(USART1,USART_FLAG_TC);//清除中断标志
//		//		memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN); 			//数组清零
//		//gd_eval_led_off(LED4);
//		UART_DMA_Enable(DMA0,DMA_CH5,USART2_MAX_RECV_LEN);
//	} 												
//}  
	
	void USART1_IRQHandler(void)
{
		unsigned char t=0;
//	for(t=0;t<12;t++)
//			{				
//		USART2_RX_BUF[t]=(u8)usart_data_receive(USART1);
//			}
//  	for(t=0;t<11;t++)
//			{				
//		    Uart0Printf("%c",USART2_RX_BUF[t]);
//			}  
//		Uart0Printf("%c\r\n",USART2_RX_BUF[11]);
	if(usart_interrupt_flag_get(USART1,USART_INT_RBNEIE) != RESET)  //闲时中断
	{

	for(t=0;t<13;t++)
			{				
		USART2_RX_BUF[t]=(u8)usart_data_receive(USART1);
			}
  	for(t=0;t<13;t++)
			{				
		    Uart0Printf("%c",USART2_RX_BUF[t]);
			}  
		Uart0Printf("%c\r\n",USART2_RX_BUF[11]);

usart_interrupt_flag_clear(USART1, USART_INT_RBNEIE);//清除中断标志
	} 												
} 
	
	

