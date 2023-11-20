#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "diwen.h"
#include "systick.h"
#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"


#define ARRAYNUM(arr_name)   (uint32_t)(sizeof(arr_name)/sizeof(*(arr_name)))
#define BUFFER_SIZE     128

//#define urt5Rec5A_Flag                       BIT_ADDR((uint32_t)&byUart5RecState,0)//串口1数据接收到5A标志
//#define urt5RecStart_Flag                    BIT_ADDR((uint32_t)&byUart5RecState,1)//串口1数据接收完整包头标志
//#define urt5RecEnd_Flag                      BIT_ADDR((uint32_t)&byUart5RecState,2)//串口1接收完成标志
//#define urt5RecData_Flag                     BIT_ADDR((uint32_t)&byUart5RecState,3)//串口1有数据接收标志
#define USART_RECV_BUF_SIZE 32
#define USART_SEND_BUF_SIZE 32

static uint8_t USART_Rx_Buf[USART_RECV_BUF_SIZE];// ???? RX ??????
static uint16_t USART_Rx_Len = 0;                // ???? RX ?????????
static uint16_t USART_Rx_Count = 0;              // ???? RX ???????????????
static uint8_t USART_Tx_Buf[USART_SEND_BUF_SIZE];// ???? Tx ??????
static uint16_t USART_Tx_Len = 0;                // ???? Tx ?????????
static uint16_t USART_Tx_Count = 0;


uint8_t rx_buffer[BUFFER_SIZE];
uint8_t uart5_rx_buffer[20];
uint16_t rrx_counter = 0, get_count = 0,rx_counter = 0;
__IO uint16_t offset_rx = 0;

static uint8_t uart5_rcev_index = 0;
uint8_t usart5_txBuf[6];


uint8_t  byUart5RecState=0;
uint8_t Uart5SendDataBuffLength;
uint8_t  uart5tempreclength;
uint8_t  Uart5RecDataBuffLength=0;
uint8_t  Uart5SendBuff[100];
uint8_t  Uart5RecBuff[20];

//uint8_t uart5Rec5A_Flag   =0;                   
//uint8_t uart5RecStart_Flag =0;                    
//uint8_t uart5RecEnd_Flag   =0;                      
//uint8_t uart5RecData_Flag  =0;                       



//触摸屏初始化程序
void diwen_init(void) {
	nvic_irq_enable(USART5_IRQn, 0, 0);
	usart5Init(EVAL_COM5, 115200u);
	usart_interrupt_enable(EVAL_COM5, USART_INTEN_RBNEIE);
	init_dwin();
}


//串口5中断服务函数
//void USART5_IRQHandler(void) {
//	uint8_t ch=0;
//    if(RESET != usart_interrupt_flag_get(USART5, USART_INT_RBNEIE)){
//        /* read one byte from the receive data register */
//			  
//			ch=(uint8_t)usart_data_receive(USART5);
//        rx_buffer[rx_counter] = (uint8_t)usart_data_receive(USART5);
//			  rx_counter++;
//		
//		    DISABLE_INT();
//		    offset_rx++;
//		    ENABLE_INT();
//		
//        if(rx_counter >= BUFFER_SIZE){
//            rx_counter = 0;    
//        }
//    }
//}

void USART5_IRQHandler(void) {

		if (SET==usart_interrupt_flag_get(USART5, USART_INT_RBNEIE)) //检查指定的TIM中断发生与否:TIM 中断源 
		{
	unsigned char t=0;
	for(t=0;t<9;t++)
			{				
				uart5_rx_buffer[t] = (uint8_t)usart_data_receive(USART5);
			}
				  usart_flag_clear(USART5, USART_FLAG_TBE);
		}
	
}
	
	
	
//}







//void USART5_IRQHandler(void)
//{
//  uint8_t tempdata=0;
//   /***********************************串口1接收数据处理流程****************************************/	
//   if(RESET != usart_interrupt_flag_get(USART5, USART_INT_RBNEIE))   //串口1接收中断	
//   {
// 
//    usart_flag_clear(USART5, USART_FLAG_RBNE);	 
//	 // Uart1RecTimerOut=200;  
//	uart5RecData_Flag=1;//串口有数据接收标志置1 
//	  tempdata=usart_data_receive(USART5);
//	  if(uart5RecStart_Flag)//如果接收到完整包头标志
//	  {
//         Uart5RecBuff[Uart5RecDataBuffLength]=tempdata; 
//         Uart5RecDataBuffLength++;
//		 if(Uart5RecDataBuffLength>0)
//		 {
//		    uart5tempreclength=Uart5RecBuff[0];//获取数据接收长度
//			if(Uart5RecDataBuffLength>uart5tempreclength)//数据接收完毕
//			{
//			   uart5RecStart_Flag=0;
//			   Uart5RecDataBuffLength=0;
//			   uart5tempreclength=0;
////			   Uart1RecTimerOut=0;
//			   uart5RecEnd_Flag=1;
//			}
//         }	
//		 if(Uart5RecDataBuffLength>=20)//如果串口1接收数据数组溢出,则接收长度为0
//		 {
//			Uart5RecDataBuffLength=0;
//		 }	
//      }
//	  else if(uart5Rec5A_Flag)
//	  {
//	     if(tempdata==0xA5)//接收到0xA5标志
//		 {
//		    uart5Rec5A_Flag=0;
//	        uart5RecStart_Flag=1;
//	        Uart5RecDataBuffLength=0; 
//		 } 
//	  } 
//	  else if(tempdata==0x5A)
//	  {
//	     uart5Rec5A_Flag=1;//接收0x5A包头标志
//		 uart5RecStart_Flag=0;
//	     Uart5RecDataBuffLength=0;  
//	  }    
//   }	
//	  uart5RecData_Flag=0;
//   /***********************************串口1发送数据处理流程****************************************/		
//if(RESET != usart_interrupt_flag_get(USART5, USART_INT_RBNEIE))//串口1发送中断
//   {
//	  usart_flag_clear(USART5, USART_FLAG_TBE);		
//   }
//}


// 发送握手指令
void init_dwin(void){
    BEGIN_CMD();
    TX_8(0x00);
}

//背光亮度调节
void dwin_set_backlight(uint8_t level){
    BEGIN_CMD();
    TX_8(0x30);
    TX_8(level);
}

void dwin_disp_string(DWIN_TEXT_STR *pstr){
    uint8_t i = 0;
    BEGIN_CMD();
    TX_8(0x11);
    TX_8(pstr->mode);
    TX_16(pstr->f_color);
    TX_16(pstr->b_color);
    TX_16(pstr->sx);
    TX_16(pstr->sy);
    //while(pstr->p_len--) TX_8(*(pstr->p_data++)); 
    while(pstr->p_len--) TX_8(pstr->p_data[i++]);    
}

uint8_t dwin_frame_read(uint8_t indata, DWIN_DATA_STR *frameT) {
		static uint8_t recvOffset = 0;
    static uint8_t status = STAUS_IDLE;
	
		return 0;
}


//单色清屏
void dwin_clear_screen(uint16_t color){
    BEGIN_CMD();
    TX_8(0x01);
    TX_16(color);
}

//
void dwin_proc_shake(void){
    // 清屏
    dwin_clear_screen(ASSEMBLE_RGB(31,33,33));

}

//获取串口数据
uint8_t get_com_data(uint8_t *rdata) {
	if((get_count == rx_counter) && (offset_rx == 0)) return 0;
	
	*rdata = rx_buffer[get_count++];
	
	DISABLE_INT();
	offset_rx--;
	ENABLE_INT();
	
	if(get_count >= BUFFER_SIZE) {
		get_count = 0;    
	}
	
	return 1;
}

//触摸屏处理函数
void dwin_handle_porcess(void) {
		uint8_t value;
		static DWIN_DATA_STR rFrame;
		DWIN_DATA_STR *frame = &rFrame;
		
		if(!get_com_data(&value)) return;

		if(!dwin_frame_read((uint8_t)value, frame)) return;
		
		switch(frame->cmd1){
				case NOTIFY_SHAKE:      // 握手应答
						if((frame->len != 2) && ((frame->data[0] != 0x4F)&&(frame->data[1] != 0x4B)))
								return;
						dwin_proc_shake();     
				break;
		}
}


//#define USART5_DATA_ADDRESS    ((uint32_t)0x40011404)
//volatile ErrStatus transfer_status = ERROR;

//extern uint8_t usart5_rxBuf[50];
//extern uint8_t usart5_txBuf[50];
//extern uint8_t HMI_SEND_BUF[20];

//void usart_dma_config(void)
//{
//    dma_single_data_parameter_struct dma_init_struct;
//    /* enable DMA1 */
//    rcu_periph_clock_enable(RCU_DMA1);
//    /* deinitialize DMA channel7(USART0 tx) */
//    dma_deinit(DMA1, DMA_CH7);
//    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
//    dma_init_struct.memory0_addr = (uint32_t)tx_buffer;
//    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
//    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
//    dma_init_struct.number = ARRAYNUM(tx_buffer);
//    dma_init_struct.periph_addr = USART5_DATA_ADDRESS;
//    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
//    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
//    dma_single_data_mode_init(DMA1, DMA_CH7, dma_init_struct);
//    /* configure DMA mode */
//    dma_circulation_disable(DMA1, DMA_CH7);
//    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI5);
//    
//    dma_deinit(DMA1, DMA_CH2);
//    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
//    dma_init_struct.memory0_addr = (uint32_t)rx_buffer;
//    dma_single_data_mode_init(DMA1, DMA_CH2, dma_init_struct);
//    /* configure DMA mode */
//    dma_circulation_disable(DMA1, DMA_CH2);
//    dma_channel_subperipheral_select(DMA1, DMA_CH2, DMA_SUBPERI5);
//}

//void PLC_Data_Wr2HMI(uint16_t HMI_start_addr,uint16_t PLC_return_data_start_addr,uint8_t len)
//{
//	uint8_t i;
//	usart5_txBuf[0]=0x5a;
//	usart5_txBuf[1]=0xa5;
//	usart5_txBuf[2]=len*2+3;
//	usart5_txBuf[3]=0x82;
//	usart5_txBuf[4]=((uint8_t)(HMI_start_addr>>8))&0XFF;
//	usart5_txBuf[5]=(uint8_t)(HMI_start_addr&0XFF);
//	for(i=0;i<len;i++)
//	{
//		usart5_txBuf[i*2+6]=((HMI_SEND_BUF[i+PLC_return_data_start_addr] >>8)&0xff);
//		usart5_txBuf[i*2+7]=(HMI_SEND_BUF[i+PLC_return_data_start_addr] &0xff);
//	}
//	//HAL_UART_Transmit(&huart1, usart1_txBuf, len*2+6, 10);
//}

//void WRITE_HMI(uint16_t addr,uint8_t data1,uint8_t data2,uint8_t data3,uint8_t data4,uint8_t len)
//{
//		usart5_txBuf[0]=0x5a;
//		usart5_txBuf[1]=0xa5;
//		usart5_txBuf[2]=len*2+3;
//		usart5_txBuf[3]=0x82;
//		usart5_txBuf[4]=(uint8_t)((addr>>8)&0xff);
//		usart5_txBuf[5]=(uint8_t)(addr&0xff);
//		usart5_txBuf[6]=data1;
//		usart5_txBuf[7]=data2;
//		usart5_txBuf[8]=data3;
//		usart5_txBuf[9]=data4;
//		//HAL_UART_Transmit(&huart1, usart1_txBuf, len*2+6 , 20);
//}

void READ_HMI(uint16_t addr,uint8_t num)
{
		usart5_txBuf[0]=0x5a;
		usart5_txBuf[1]=0xa5;
		usart5_txBuf[2]=0x04;
		usart5_txBuf[3]=0x83;
		usart5_txBuf[4]=(uint8_t)((addr>>8)&0xff);
		usart5_txBuf[5]=(uint8_t)(addr&0xff);
//		usart5_txBuf[6]=num;

//		HAL_UART_Transmit(&huart1, usart1_txBuf, 7 , 20);
}







