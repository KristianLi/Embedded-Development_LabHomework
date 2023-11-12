#include "test.h"
#include "user.h"
char machinenum[] = "001";//在这填写设备号
char exph[] = "110B";//在这填写自定演示符号

char TCP_Server_IP[]  = "47.99.144.16";	//芯力量服务器IP
char TCP_Server_COM[] = "8051";			//芯力量服务器端口

 

u8  res   = 1;//用于4G模块自检
u8 NET_flag   = 2;	//0表示TCP服务器已断开，1表示连接



int main(void) {  
	system_init();

	while(1){

//		test1();

		test2();
//		test3();

  }	
}
