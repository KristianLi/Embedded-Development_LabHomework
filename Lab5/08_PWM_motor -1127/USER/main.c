#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "systick.h"
#include "test.h"
#include "user.h"






//char TCP_Server_IP[]  = "123.57.21.44";	//TCP������IP
//char TCP_Server_COM[] = "6668";			//TCP�������˿�

char TCP_Server_IP[]  = "47.99.144.16";	//о����������IP
char TCP_Server_COM[] = "8051";			//о�����������˿�

 

u8  res   = 1;//����4Gģ���Լ�
u8 NET_flag   = 2;	//0��ʾTCP�������ѶϿ���1��ʾ����


int main(void)
{  
	system_init();
	while(1){
	 test2();
		
	}		
}









