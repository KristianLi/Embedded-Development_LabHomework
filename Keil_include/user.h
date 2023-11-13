#ifndef __USER_H
#define __USER_H	

#include "systick.h"
#include "gd32f4xx.h"
#include <stdio.h> 
#include "math.h"
#include "gd32f4xx_dma.h"
#include "crc16.h"
#include "string.h"
#include "stdarg.h"
#include "cJSON.h"
#include "gd32f4xx_XII-IOT.h"




typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

/*------------------------------usart2---------------------------------*/
//#define USART2_MAX_RECV_LEN		1024				//??????????????
//#define USART2_MAX_SEND_LEN		1024				//?????????????
#define USART1_DATA_ADDRESS    ((uint32_t)0x40004404)
#define USART2_MAX_RECV_LEN		1024				//??????????????
#define USART2_MAX_SEND_LEN		2048				//?????????????
#define USART2_RX_EN 			1					//0,??????;1,????.
#define ARRAYNUM(arr_name)     (uint32_t)(sizeof(arr_name)/sizeof(*(arr_name)))
#define USART0_DATA_ADDRESS    ((uint32_t)0x40013804)
extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//???????,???USART2_MAX_RECV_LEN???
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//???????,???USART2_MAX_SEND_LEN???

extern u8 USART2_RX_REC_ATCOMMAD;

extern volatile uint8_t motor_speed;
extern volatile uint8_t mode_select;
extern float speed;

void USART2_Init(u32 bound);				//????2????? 
//void gd_eval_dma_com_init(u32 bound);

void UART_DMA_Enable(uint32_t dma_periph, dma_channel_enum channelx,uint32_t len);
void u2_printf(char* fmt, ...);
void u1_printf(char* fmt, ...);
/*------------------------------4G---------------------------------*/
extern u8 NET_flag;

extern char STU_Name[];
extern char STU_Number[];
extern char STU_Class[] ;

extern char machinenum[];
extern char exph[];

extern char tab1[];
extern char ReadE2P[] ;
extern char Res[]  ;

extern char t1[] ;
extern char t2[] ;
extern char charused[];

extern char dac1[];
extern char adc1[] ;
extern char dac2[];
extern char adc2[] ;

extern char sdmemory[];
extern char sdtxt[] ;

extern char temperature[] ;
extern char humidity[] ;
extern char luminance[];

extern char longitude[] ;
extern char latitude[] ;
extern char somke[];

extern char speed_str[];
extern char dir[];

extern char TCP_Server_IP[] ;	//TCP??????IP
extern char TCP_Server_COM[];			//TCP?????????
extern struct record fields;
void json_student_imformation01 (char *machinenum,char *student_name, char *student_number);//?????????
void json_student_imformation02 (char *machinenum,char *exph);//????????????????
void json_student_imformation03 (char *machinenum,char *tab, char *ReadE2P,char *RES);//?????????????????
void json_student_imformation04 (char *machinenum,char *t1, char *t2, char *charused);//?????????????????
void json_student_imformation05 (char *machinenum,char *dac1, char *adc1,char *dac2, char *adc2);//?????????????????
void json_student_imformation07 (char *machinenum,char *sdmemory, char *sdtxt);//?????????????????
void json_student_imformation06 (char *machinenum,char *student_name, char *student_number);//?????????????????
void json_student_imformation08 (char *machinenum,char *speed_str,char *dir);//????????????????
void json_student_imformation09 (char *machinenum,char *student_name, char *student_number);////????????????????
void json_student_imformation10 (char *machinenum,char *student_name, char *student_number);//????????????????
void json_student_imformation_light (char *machinenum,char *luminance);//???¡¤?????????????????
void json_student_imformation_agri (char *machinenum,char *temperature,char *humidity,char *luminance);//????????????????????
void json_student_imformation_track (char *machinenum,char *longitude,char *latitude);//???????????????????
void json_student_imformation_smoke (char *machinenum,char *smoke);//??????????????????

/*------------------------------timer---------------------------------*/
extern volatile u16 tim3_count;
void TIM3_Init(u16 arr,u16 psc);
void TIM2_Init(u16 arr,u16 psc);
void TIM1_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void test_check(void); //???????????

extern u8 led_check;



/*------------------------------sim7600ce---------------------------------*/
#define SIM_OK 0
#define SIM_COMMUNTION_ERR 0xff
#define SIM_CPIN_ERR 0xfe
#define SIM_CREG_FAIL 0xfd
#define SIM_MAKE_CALL_ERR 0Xfc
#define SIM_ATA_ERR       0xfb

#define SIM_CMGF_ERR 0xfa
#define SIM_CSCS_ERR 0xf9
#define SIM_CSCA_ERR 0xf8
#define SIM_CSMP_ERR 0Xf7
#define SIM_CMGS_ERR       0xf6
#define SIM_CMGS_SEND_FAIL       0xf5

#define SIM_CNMI_ERR 0xf4

#define SIM_KEY PAout(4) //SIM800C????

extern u8 dtbuf[50]; //?????????
extern u32 send_data_count;

extern void GSM_ON_GPIO_Config(void);
extern u8 SIM900_CSQ[3];
extern u8 GSM_Dect(void);
extern u8 SIM900A_CONNECT_SERVER_SEND_INFOR(u8 *IP_ADD,u8 *COM);
extern u8 SIM900A_GPRS_SEND_DATA(u8 *temp_data);
extern u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime);

extern u8 SIM900A_GPRS_SEND_DATA_CRC(u8 *temp_data, u8 len);
u8 SIM900A_GPRS_SEND_DATA_NO_CRC(u8 *temp_data, u8 len);

void sim7600ce_connect(void);

///////////////////message_json////////////////////////////


extern u8 SIM900A_GPRS_SEND_JSON_STRINGS(char *temp_data, u8 len);//??json????????????
extern u8 SIM900A_GPRS_SEND_JSON_DATA_CRC(u8 *temp_data, u8 len);//??json???????????
void u8_int(char l,u8 *table_u,int *table_i);
void json_test1(void);//json ???????1
void json_test2(void);//json ???????2
void json_test3(void);//json ???????3
void init_4g (void);


#endif

