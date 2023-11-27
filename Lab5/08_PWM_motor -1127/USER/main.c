#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "systick.h"
#include "test.h"
#include "user.h"






//char TCP_Server_IP[]  = "123.57.21.44";	//TCP服务器IP
//char TCP_Server_COM[] = "6668";			//TCP服务器端口

char TCP_Server_IP[]  = "47.99.144.16";	//芯力量服务器IP
char TCP_Server_COM[] = "8051";			//芯力量服务器端口

 

u8  res   = 1;//用于4G模块自检
u8 NET_flag   = 2;	//0表示TCP服务器已断开，1表示连接


int main(void)
{  
	system_init();
	while(1){
	 test2();
		
	}		
}









