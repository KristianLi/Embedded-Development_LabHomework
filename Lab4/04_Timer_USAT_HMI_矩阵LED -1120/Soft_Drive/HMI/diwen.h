#ifndef _DIWEN_H
#define _DIWEN_H

#include "gd32f4xx.h"
#include <stdio.h>
#include <string.h>

#define ENABLE_INT()	              __set_PRIMASK(0)    //使能全局中断	
#define DISABLE_INT()	              __set_PRIMASK(1)    //禁止全局中断

extern uint8_t byUart5RecState;
//#define Uart5Rec5A_Flag                       BIT_ADDR((u32)&byUart1RecState,0)//串口1数据接收到5A标志
//#define Uart5RecStart_Flag                    BIT_ADDR((u32)&byUart1RecState,1)//串口1数据接收完整包头标志
//#define Uart5RecEnd_Flag                      BIT_ADDR((u32)&byUart1RecState,2)//串口1接收完成标志
//#define Uart5RecData_Flag                     BIT_ADDR((u32)&byUart1RecState,3)//串口1有数据接收标志

/*  
 *  状态
*******************************************************************************/
#define STAUS_IDLE      0x00
#define STAUS_CMD1      0x01
#define STAUS_CMD2      0x02
#define STAUS_START     0x03
#define STAUS_O001      0x04
#define STAUS_O002      0x05
#define STAUS_O003      0x06
#define STAUS_OVER      0x07

/*  
 *  位操作
*******************************************************************************/
#define SHIFT_LEFT(x,n)		    ((x) << (n))        		
#define SHIFT_RIGHT(x,n)      ((x) >> (n))        		
#define BIT_SET(x,n) 		      (x |= (1<<n))     			
#define BIT_CLR(x,n) 		      (x &= (~(1<<n)))  
#define BIT_CHECK(x,n) 		    (((x) & (1<<(n))) != 0)	
#define BITs_SET(x,msk)		    (x |= (msk))          		
#define BITs_CLR(x,msk)		    (x &= (~msk))       		
#define BITs_CHECK(x,msk)	    (((x) & (msk)) == (msk))	

/*  
 *  通信接口  
*******************************************************************************/
#define TRANS_COM       	EVAL_COM5

/*  
 *  帧头  
*******************************************************************************/
#define FRAME_HRAD      0x5AA5 

/*  
 *  RGB888 转 RGB565
*******************************************************************************/
#define ASSEMBLE_RGB(R, G, B) ((((R)& 0xF8) << 8) | (((G) & 0xFC) << 3) | (((B) & 0xF8) >> 3))


/*  
 *  文本字体宽度
*******************************************************************************/
#define ENABLE_TEXT_RESIZE      0x80
#define DISABLE_TEXT_RESIZE     0x00

/*  
 *  文本字体背景色
*******************************************************************************/
#define ENABLE_TEXT_BCOLOR      0x40
#define DISABLE_TEXT_BCOLOR     0x00

/*  
 *  数字字体背景色
*******************************************************************************/
#define ENABLE_NUM_BCOLOR       0x80
#define DISABLE_NUM_BCOLOR      0x00

/*  
 *  数字字体符号
*******************************************************************************/
#define ENABLE_SIGNED_NUM       0x40
#define DISABLE_SIGNED_NUM      0x00

/*  
 *  数字字体是否显示无效0
*******************************************************************************/
#define ENABLE_NUM_ZERO         0x20
#define DISABLE_NUM_ZERO        0x00

/*  
 *  数字字体是否显示无效0为空格
*******************************************************************************/
#define ENABLE_NUM_ZERO_S       0x10
#define DISABLE_NUM_ZERO_S      0x00

/*  
 *  文本字体大小
*******************************************************************************/
#define FONT0612    0x00
#define FONT0816    0x01
#define FONT1020    0x02
#define FONT1224    0x03
#define FONT1428    0x04
#define FONT1632    0x05
#define FONT2040    0x06
#define FONT2448    0x07
#define FONT2856    0x08
#define FONT2364    0x09

/*  
 *  
*******************************************************************************/
#define NOTIFY_SHAKE                0x00    //握手回复

/*  
 *  文本结构体  
*******************************************************************************/
typedef struct{
    uint8_t mode;							  //	显示模式 
    
    uint16_t sx, sy;						//	字符串显示的左上角坐标 			
    uint16_t f_color, b_color;	//  字符显示颜色 && 字符背景显示颜色

    uint8_t num_i;							// 	显示的整数位数，0x01-0x14 
    uint8_t num_f;							//  显示的小数位数，0x00-0x14，Num_I+Num_F 之和不能超过20

    uint8_t  p_len;             //  待发送的数据字节数
    uint8_t *p_data;            //  指向数据缓存区的指针
}DWIN_TEXT_STR;

/*  
 *  发送宏操作  
*******************************************************************************/
#define SEND_DATA(P)    {usart_data_transmit(TRANS_COM, P);while(RESET == usart_flag_get(TRANS_COM, USART_FLAG_TBE));}//comSendChar(TRANS_COM, P)
#define TX_8(P1)        SEND_DATA((P1)&0xFF)                // send one byte
#define TX_16(P1)       TX_8((P1)>>8);TX_8(P1)              // send two byte
#define TX_32(P1)       TX_16((P1)>>16);TX_16((P1)&0xFFFF)  // send four byte
#define BEGIN_CMD()     TX_16(FRAME_HRAD)

/*  
 *  接收缓冲区长度定义  
*******************************************************************************/
#define MAX_DWIN_RX_LEN 249

/*  
 *  帧数据结构体  
*******************************************************************************/
typedef struct{
    uint8_t len;                   // 参数长度
    uint8_t cmd1;                  // 指令1
    uint8_t data[MAX_DWIN_RX_LEN]; // 指令参数 有效数据缓存区
}DWIN_DATA_STR;

/*  
 *  队列结构体  
*******************************************************************************/
typedef struct{
    DWIN_DATA_STR *Out;     //指向数据输出位置 	
    DWIN_DATA_STR *In;      //指向数据输入位置 		
    DWIN_DATA_STR *End;     //指向Buf的结束位置
    DWIN_DATA_STR *Buf;     //存储数据的空间	

    uint16_t NData;           //队列中数据个数			
		uint16_t MaxData;	        //队列中允许存储的数据个数
}DWIN_QUEUE_STR;

/*  
 *  队列结构体长度定义  
*******************************************************************************/
#define DWIN_RX_QUEUE_SIZE  32

/*  
 *  应答处理函数结构体  
*******************************************************************************/
typedef struct{
    void (*handle_shake)(void);                                       // 应答处理
}DWIN_HANDLE_STR;

void diwen_init(void);
void usart_dma_config(void);
void init_dwin(void);

#endif
