#ifndef _DIWEN_H
#define _DIWEN_H

#include "gd32f4xx.h"
#include <stdio.h>
#include <string.h>

#define ENABLE_INT()	              __set_PRIMASK(0)    //ʹ��ȫ���ж�	
#define DISABLE_INT()	              __set_PRIMASK(1)    //��ֹȫ���ж�

extern uint8_t byUart5RecState;
//#define Uart5Rec5A_Flag                       BIT_ADDR((u32)&byUart1RecState,0)//����1���ݽ��յ�5A��־
//#define Uart5RecStart_Flag                    BIT_ADDR((u32)&byUart1RecState,1)//����1���ݽ���������ͷ��־
//#define Uart5RecEnd_Flag                      BIT_ADDR((u32)&byUart1RecState,2)//����1������ɱ�־
//#define Uart5RecData_Flag                     BIT_ADDR((u32)&byUart1RecState,3)//����1�����ݽ��ձ�־

/*  
 *  ״̬
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
 *  λ����
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
 *  ͨ�Žӿ�  
*******************************************************************************/
#define TRANS_COM       	EVAL_COM5

/*  
 *  ֡ͷ  
*******************************************************************************/
#define FRAME_HRAD      0x5AA5 

/*  
 *  RGB888 ת RGB565
*******************************************************************************/
#define ASSEMBLE_RGB(R, G, B) ((((R)& 0xF8) << 8) | (((G) & 0xFC) << 3) | (((B) & 0xF8) >> 3))


/*  
 *  �ı�������
*******************************************************************************/
#define ENABLE_TEXT_RESIZE      0x80
#define DISABLE_TEXT_RESIZE     0x00

/*  
 *  �ı����屳��ɫ
*******************************************************************************/
#define ENABLE_TEXT_BCOLOR      0x40
#define DISABLE_TEXT_BCOLOR     0x00

/*  
 *  �������屳��ɫ
*******************************************************************************/
#define ENABLE_NUM_BCOLOR       0x80
#define DISABLE_NUM_BCOLOR      0x00

/*  
 *  �����������
*******************************************************************************/
#define ENABLE_SIGNED_NUM       0x40
#define DISABLE_SIGNED_NUM      0x00

/*  
 *  ���������Ƿ���ʾ��Ч0
*******************************************************************************/
#define ENABLE_NUM_ZERO         0x20
#define DISABLE_NUM_ZERO        0x00

/*  
 *  ���������Ƿ���ʾ��Ч0Ϊ�ո�
*******************************************************************************/
#define ENABLE_NUM_ZERO_S       0x10
#define DISABLE_NUM_ZERO_S      0x00

/*  
 *  �ı������С
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
#define NOTIFY_SHAKE                0x00    //���ֻظ�

/*  
 *  �ı��ṹ��  
*******************************************************************************/
typedef struct{
    uint8_t mode;							  //	��ʾģʽ 
    
    uint16_t sx, sy;						//	�ַ�����ʾ�����Ͻ����� 			
    uint16_t f_color, b_color;	//  �ַ���ʾ��ɫ && �ַ�������ʾ��ɫ

    uint8_t num_i;							// 	��ʾ������λ����0x01-0x14 
    uint8_t num_f;							//  ��ʾ��С��λ����0x00-0x14��Num_I+Num_F ֮�Ͳ��ܳ���20

    uint8_t  p_len;             //  �����͵������ֽ���
    uint8_t *p_data;            //  ָ�����ݻ�������ָ��
}DWIN_TEXT_STR;

/*  
 *  ���ͺ����  
*******************************************************************************/
#define SEND_DATA(P)    {usart_data_transmit(TRANS_COM, P);while(RESET == usart_flag_get(TRANS_COM, USART_FLAG_TBE));}//comSendChar(TRANS_COM, P)
#define TX_8(P1)        SEND_DATA((P1)&0xFF)                // send one byte
#define TX_16(P1)       TX_8((P1)>>8);TX_8(P1)              // send two byte
#define TX_32(P1)       TX_16((P1)>>16);TX_16((P1)&0xFFFF)  // send four byte
#define BEGIN_CMD()     TX_16(FRAME_HRAD)

/*  
 *  ���ջ��������ȶ���  
*******************************************************************************/
#define MAX_DWIN_RX_LEN 249

/*  
 *  ֡���ݽṹ��  
*******************************************************************************/
typedef struct{
    uint8_t len;                   // ��������
    uint8_t cmd1;                  // ָ��1
    uint8_t data[MAX_DWIN_RX_LEN]; // ָ����� ��Ч���ݻ�����
}DWIN_DATA_STR;

/*  
 *  ���нṹ��  
*******************************************************************************/
typedef struct{
    DWIN_DATA_STR *Out;     //ָ���������λ�� 	
    DWIN_DATA_STR *In;      //ָ����������λ�� 		
    DWIN_DATA_STR *End;     //ָ��Buf�Ľ���λ��
    DWIN_DATA_STR *Buf;     //�洢���ݵĿռ�	

    uint16_t NData;           //���������ݸ���			
		uint16_t MaxData;	        //����������洢�����ݸ���
}DWIN_QUEUE_STR;

/*  
 *  ���нṹ�峤�ȶ���  
*******************************************************************************/
#define DWIN_RX_QUEUE_SIZE  32

/*  
 *  Ӧ�������ṹ��  
*******************************************************************************/
typedef struct{
    void (*handle_shake)(void);                                       // Ӧ����
}DWIN_HANDLE_STR;

void diwen_init(void);
void usart_dma_config(void);
void init_dwin(void);

#endif
