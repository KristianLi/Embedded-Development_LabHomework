#include "user.h"
#include "test.h"
//ѧ�����ʵ��1�󣬴�S1���أ������Զ���鴫����ȷ���������ȷ�����Զ�����ѧ����Ϣ����ȷ��ʶ������������



//char TCP_Server_IP[]  = "123.57.21.44";	//TCP������IP
//char TCP_Server_COM[] = "6668";			//TCP�������˿�

char TCP_Server_IP[]  = "47.99.144.16";	//о����������IP
char TCP_Server_COM[] = "8051";			//о�����������˿�



u8  res   = 1;//����4Gģ���Լ�
u8 NET_flag   = 2;	//0��ʾTCP�������ѶϿ���1��ʾ����

int main(void) {  
  system_init();
	
		while(1) {
			
   test();
		}
}
