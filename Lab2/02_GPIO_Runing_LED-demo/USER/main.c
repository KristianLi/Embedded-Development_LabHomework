#include "test.h"
#include "user.h"
char machinenum[] = "001";//������д�豸��
char exph[] = "110B";//������д�Զ���ʾ����

char TCP_Server_IP[]  = "47.99.144.16";	//о����������IP
char TCP_Server_COM[] = "8051";			//о�����������˿�

 

u8  res   = 1;//����4Gģ���Լ�
u8 NET_flag   = 2;	//0��ʾTCP�������ѶϿ���1��ʾ����



int main(void) {  
	system_init();

	while(1){

//		test1();

		test2();
//		test3();

  }	
}
