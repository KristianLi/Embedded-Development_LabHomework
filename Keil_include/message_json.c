#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "user.h"


void u8_int(char l,u8 *table_u,int *table_i) //u8 数组转 int数组
{
    u8 t;// unsigned char
    char i;
    for(i=0;i<l;i++)
    {
        if(table_u[i]>127){
            t = ~table_u[i]+1;
            table_i[i]=-(int)t;
        }else
        {
            table_i[i]=table_u[i];
        }        
    }
}





u8 SIM900A_GPRS_SEND_JSON_DATA_CRC(u8 *temp_data, u8 len)//以json格式发送数组
{
	u16 crc = 0;
	//u8 t;// unsigned char
	int temp_data_i[(int)len];
	
	cJSON *root;
	char *out;
	int i;
	

	if(len<3) return 2;
	crc = crc_modbus(temp_data,(len-2));
	temp_data[len-1] = (u8)((crc&0xff00)>>8);
	temp_data[len-2] = (u8)(crc&0x00ff);//奇偶校验
	
	u8_int(len,temp_data,temp_data_i);
	
	root=cJSON_CreateArray();
	for (i=0;i<len;i++) cJSON_AddItemToArray(root,cJSON_CreateIntArray(temp_data_i,len));
	out=cJSON_Print(root);
	cJSON_Delete(root);
	Uart0Printf("%s\n",out);
	
	u1_printf("%s\r\n",out);
	free(out);
	
	return 0;
}


u8 SIM900A_GPRS_SEND_JSON_STRINGS(char *temp_data, u8 len)//以json格式发送字符串
{
	cJSON *root;
	char *out;
	const char *temp_data_s = temp_data;
	
	root=cJSON_CreateStringArray(&temp_data_s,len);
	out=cJSON_Print(root);
	cJSON_Delete(root);
	Uart0Printf("%s\n",out);
	
	u1_printf("%s\r\n",out);
	free(out);
	
	return 0;
}


void json_test1()//json 输出测试1
{
	struct record {const char *precision;double lat,lon;const char *address,*city,*state,*zip,*country; };
	struct record fields[2]={
		{"zip",37.7668,-1.223959e+2,"","SAN FRANCISCO","CA","94107","US"},
		{"zip",37.371991,-1.22026e+2,"","SUNNYVALE","CA","94085","US"}};
	
		cJSON *root,*fld;char *out;int i;
		
		root=cJSON_CreateArray();
	for (i=0;i<1;i++)
	{
		cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
		cJSON_AddStringToObject(fld, "precision", fields[i].precision);
		cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
		cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
		cJSON_AddStringToObject(fld, "Address", fields[i].address);
		cJSON_AddStringToObject(fld, "City", fields[i].city);
		cJSON_AddStringToObject(fld, "State", fields[i].state);
		cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
		cJSON_AddStringToObject(fld, "Country", fields[i].country);
	}
	
/*	cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root,1),"City",cJSON_CreateIntArray(ids,4)); */
	
	out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	u1_printf("%s\r\n",out);
	free(out);
	
	
}

void json_test2()//json 输出测试2
{
	cJSON *root;char *out;int i;
  int numbers[3][3]={{0,-1,0},{1,0,0},{0,0,1}};	
	root=cJSON_CreateArray();
	for (i=0;i<3;i++) cJSON_AddItemToArray(root,cJSON_CreateIntArray(numbers[i],3));
	
	out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\n",out); 
	u1_printf("%s\r\n",out);
	free(out);
}

void json_test3()//json 输出测试3
{
	cJSON *root,*img,*thm;char *out;
	int ids[4]={116,943,234,38793};
	root=cJSON_CreateObject();
	cJSON_AddItemToObject(root, "Image", img=cJSON_CreateObject());
	cJSON_AddNumberToObject(img,"Width",800);
	cJSON_AddNumberToObject(img,"Height",600);
	cJSON_AddStringToObject(img,"Title","View from 15th Floor");
	cJSON_AddItemToObject(img, "Thumbnail", thm=cJSON_CreateObject());
	cJSON_AddStringToObject(thm, "Url", "http:/*www.example.com/image/481989943");
	cJSON_AddNumberToObject(thm,"Height",125);
	cJSON_AddStringToObject(thm,"Width","100");
	cJSON_AddItemToObject(img,"IDs", cJSON_CreateIntArray(ids,4));
	
	out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\n",out); 
	u1_printf("%s\r\n",out);
	free(out);
}

void json_student_imformation01 (char *machinenum,char *student_name, char *student_number)//实验一学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", machinenum);
		cJSON_AddStringToObject(fld, "Name", student_name);
		cJSON_AddStringToObject(fld, "Number", student_number);
	
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation02 (char *machinenum,char *exph)//实验2学生信息上传封装
{

  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", machinenum);
		cJSON_AddStringToObject(fld, "Experiment_phenomenon", exph);
	  out=cJSON_Print(root);	Uart0Printf("5\r\n"); cJSON_Delete(root);	
	  Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation03 (char *machinenum,char *tab, char *ReadE2P,char *RES)//实验3学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", machinenum);
		cJSON_AddStringToObject(fld, "tab", tab);
		cJSON_AddStringToObject(fld, "ReadE2P", ReadE2P);
	  cJSON_AddStringToObject(fld, "result", RES);
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation04 (char *machinenum,char *t1, char *t2, char *charused)//实验四学生信息上传封装
{ 
  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", machinenum);
		cJSON_AddStringToObject(fld, "t1", t1);
		cJSON_AddStringToObject(fld, "t2", t2);
		cJSON_AddStringToObject(fld, "char", charused);
	  out=cJSON_Print(root);	 cJSON_Delete(root);	
	  Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation05 (char *machinenum,char *dac1, char *adc1,char *dac2, char *adc2)//实验五学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", machinenum);
		cJSON_AddStringToObject(fld, "dac1", dac1);
		cJSON_AddStringToObject(fld, "adc1", adc1);
			cJSON_AddStringToObject(fld, "dac2", dac2);
		cJSON_AddStringToObject(fld, "adc2", adc2);
	
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}
void json_student_imformation06 (char *machinenum,char *student_name, char *student_number)//实验六学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", machinenum);
		cJSON_AddStringToObject(fld, "Name", student_name);
		cJSON_AddStringToObject(fld, "Number", student_number);
	
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation07 (char *machinenum,char *sdmemory, char *sdtxt)//实验七学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", machinenum);
		cJSON_AddStringToObject(fld, "sdmemory", sdmemory);
		cJSON_AddStringToObject(fld, "sdtxt", sdtxt);
	
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation08 (char *machinenum,char *speed_str,char *dir)//实验八学生信息上传封装
{
 
	cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", machinenum);
	  cJSON_AddStringToObject(fld, "speed", speed_str);
	  cJSON_AddStringToObject(fld, "deraction", dir);
	
	
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation09 (char *student_name,char *student_card, char *student_class)//学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", student_name);
		cJSON_AddStringToObject(fld, "Name", student_card);
		cJSON_AddStringToObject(fld, "Number", student_class);
	
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation10 (char *machinenum,char *student_name, char *student_number)//实验十学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", machinenum);
		cJSON_AddStringToObject(fld, "Name", student_name);
		cJSON_AddStringToObject(fld, "Number", student_number);
	
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation_light (char *machinenum,char *luminance)//智慧路灯实验学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	cJSON_AddStringToObject(fld, "Machinenumbe", machinenum);
	cJSON_AddStringToObject(fld, "Luminance", luminance);

	
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation_agri (char *machinenum,char *temperature,char *humidity,char *luminance)//智慧农业实验学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	cJSON_AddStringToObject(fld, "Machinenumbe", machinenum);
	cJSON_AddStringToObject(fld, "Temperature", temperature);
	cJSON_AddStringToObject(fld, "Humidity", humidity);
	cJSON_AddStringToObject(fld, "Luminance", luminance);

	
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation_track (char *machinenum,char *longitude,char *latitude)//智慧物流学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	cJSON_AddStringToObject(fld, "Machinenumbe", machinenum);
	cJSON_AddStringToObject(fld, "Longitude Value",longitude);
	cJSON_AddStringToObject(fld, "Latitude Value",latitude);

	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}

void json_student_imformation_smoke (char *machinenum,char *smoke)//智慧烟感学生信息上传封装
{
	  cJSON *root,*fld;char *out;
		root=cJSON_CreateArray();
	
	  cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
	  cJSON_AddStringToObject(fld, "Machinenumber", machinenum);
		cJSON_AddStringToObject(fld, "Somke Value", smoke);

	
	  out=cJSON_Print(root);	cJSON_Delete(root);	Uart0Printf("%s\r\n",out); 
	  u1_printf("%s\r\n",out);
	  free(out);
}
