#include "user.h"

//sim800c�õ��Ĵ���2


//stm32��gd32�Ĵ�������������stm32��usart1��usart2��usart3
//gd32��usart0��usart1��usart2
//��������Ϊ����2��������ʱʹ�õ���usart1


//����2���ͻ����� 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
 	  
//����2���ջ����� 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u8 USART2_RX_REC_ATCOMMAD;

//��ʼ��IO ����2
//bound:������	  
void USART2_Init(u32 bound)
{
	dma_single_data_parameter_struct dma_init_struct;
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART1);
	rcu_periph_clock_enable(RCU_DMA0);
	
	usart_deinit(USART1);//��λ����2

/* connect port to USARTx_Tx */
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_2);

	/* connect port to USARTx_Rx */
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_3);
	
//USART2_TX   PA.2
	//gpio_mode_set(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2); //��ʼ��PA�˿�
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP ,GPIO_PIN_2);					
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);
//USART2_RX	  PA.3
	//gpio_mode_set(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_3); //��ʼ��PA�˿�
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP ,GPIO_PIN_3);					
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
	


//���ݸ�ʽ����	
	  usart_deinit(USART1);
    usart_baudrate_set(USART1, 115200U);
   // usart_word_length_set(USART1, USART_WL_8BIT);//�ֳ�Ϊ8λ���ݸ�ʽ
   // usart_stop_bit_set(USART1, USART_STB_1BIT);//һ��ֹͣλ
   // usart_parity_config(USART1, USART_PM_NONE);//����żУ��λ
  //  usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    //usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);//��Ӳ������������
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);//ʹ��USART2����
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);//ʹ��USART2����
    usart_enable(USART1);//ʹ��USART2����
//����NVIC����
	nvic_irq_enable(USART1_IRQn,0,0);		
	 // usart_interrupt_enable(USART1, USART_INTEN_IDLEIE); //���������ж�
	usart_interrupt_enable(USART1, USART_INTEN_RBNEIE); //�����ж�
	  
//		
//	
//	//DMA���á������ڽ���RX
	  
    rcu_periph_clock_enable(RCU_DMA0);
    dma_deinit(DMA0, DMA_CH5);//ͨ��ѡ��
    dma_init_struct.direction =DMA_PERIPH_TO_MEMORY ;//�洢��������ģʽ
    dma_init_struct.memory0_addr = (uint32_t)USART2_RX_BUF;//DMA �洢������ַ
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;//�洢������ģʽ
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;//�洢�����ݳ���:8λ
    dma_init_struct.number = USART2_MAX_RECV_LEN;//���ݴ����� 
    dma_init_struct.periph_addr =USART1_DATA_ADDRESS;//DMA����ADC����ַ
		//dma_init_struct.periph_addr =USART_DATA(USART1);//DMA����ADC����ַ
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;//���������ģʽ
    //dma_init_struct.periph_width = DMA_PERIPH_WIDTH_8BIT;//�������ݳ���:8λ
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;//�е����ȼ�
    dma_single_data_mode_init(DMA0, DMA_CH5, dma_init_struct);//��ʼ��DMA Stream
    dma_circulation_disable(DMA0, DMA_CH5);//ѭ��ģʽ�����������Զ��ص���ͷ
		dma_channel_subperipheral_select(DMA0, DMA_CH5, DMA_SUBPERI4);
    //dma_memory_to_memory_disable(DMA0, DMA_CH5);//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
		dma_transfer_number_config(DMA0,DMA_CH5,USART2_MAX_RECV_LEN);
		dma_channel_enable(DMA0, DMA_CH5);//��ʽ����DMA����
//		
//		
////		//DMA���á������ڷ���TX
//    dma_deinit(DMA0, DMA_CH6);//ͨ��ѡ��
//    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;//���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
//    dma_init_struct.memory0_addr = (uint32_t)USART2_TX_BUF;//DMA �洢������ַ
//    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;//�ڴ��ַ�Ĵ�������
//    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;//�洢�����ݳ���:8λ
//    dma_init_struct.number = 0;//DMAͨ����DMA����Ĵ�С
//		dma_init_struct.periph_addr =USART1_DATA_ADDRESS;//DMA����ADC����ַ
//    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;//���������ģʽ
//    //dma_init_struct.periph_width = DMA_PERIPH_WIDTH_8BIT;//�������ݳ���:8λ
//    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;//�е����ȼ�
//    dma_single_data_mode_init(DMA0, DMA_CH6, dma_init_struct);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART2_Tx_DMA_Channel����ʶ�ļĴ���
//    dma_circulation_disable(DMA0, DMA_CH6);// ʹ����ͨģʽ ��ֻ����һ��
//		dma_channel_subperipheral_select(DMA0, DMA_CH6, DMA_SUBPERI4);
//    //dma_memory_to_memory_disable(DMA0, DMA_CH6);//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
//		//dma_channel_enable(DMA0, DMA_CH6);//��ʽ����DMA����
		
		usart_dma_transmit_config(USART1, USART_DENT_ENABLE); //ʹ�ܴ���2��DMA����
		usart_dma_receive_config(USART1, USART_DENR_ENABLE); //ʹ�ܴ���2 DMA����
}
  //////////////////////////////////////DMA�������ò���//////////////////////////////////	   		    
   

//DMAָ�븴λ������DMA����
	void UART_DMA_Enable(uint32_t dma_periph, dma_channel_enum channelx,uint32_t len)
	{
		dma_channel_disable(dma_periph,channelx);//�ر� ָʾ��ͨ�� 
		//dma_memory_address_config(dma_periph,channelx,DMA_MEMORY_0,len);
		dma_transfer_number_config(dma_periph,channelx,len);//����ͨ�����ж�������Ҫ����
		dma_channel_enable(dma_periph,channelx);//����DMA����
	}
		
		
	//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
//   while(RESET == dma_flag_get(DMA0, DMA_CH6, DMA_INTF_FTFIF)){};
//	 while(RESET == dma_flag_get(DMA0, DMA_CH5, DMA_INTF_FTFIF)){};
	while(DMA_CH6CNT(DMA0)!=0){};	//�ȴ�ͨ��7������� 
  UART_DMA_Enable(DMA0,DMA_CH6,strlen((const char*)USART2_TX_BUF)); 	//ͨ��dma���ͳ�ȥ
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
//	if(usart_interrupt_flag_get(USART1, USART_INT_IDLEIE) != RESET)  //��ʱ�ж�
//	{

//		dma_channel_disable(DMA0,DMA_CH5);
//		dma_flag_clear(DMA0,DMA_CH5,DMA_INTF_FTFIF);
//		usart_data_receive(USART1);
//    
//		Uart0Printf("%s\r\n",USART2_RX_BUF); //��ʽ��������Ϣ��ʾ,����ʱ��ش�
//			
//		
//		if(strstr((char*)USART2_RX_BUF,"CLOSED")!=NULL) 	 NET_flag = 0;	//�����ѶϿ�
//		else if(strstr((char*)USART2_RX_BUF,"CONNECT")!=NULL)  NET_flag = 1;	//����������
//		

//		usart_interrupt_flag_clear(USART1, USART_INT_IDLEIE);//����жϱ�־
//		//usart_flag_clear(USART1,USART_FLAG_TC);//����жϱ�־
//		//		memset(USART2_RX_BUF,0,USART2_MAX_RECV_LEN); 			//��������
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
	if(usart_interrupt_flag_get(USART1,USART_INT_RBNEIE) != RESET)  //��ʱ�ж�
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

usart_interrupt_flag_clear(USART1, USART_INT_RBNEIE);//����жϱ�־
	} 												
} 
	
	

