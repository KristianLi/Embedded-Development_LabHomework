#include "gd32f4xx.h"
#include "gd32f4xx_XII-IOT.h"
#include "oled.h"
#include "at24cxx.h"
#include "i2c.h"
#include "bmp.h"
#include "test.h"
#include "systick.h"
#include <stdlib.h>
#include "user.h"
uint8_t tab[]="2023.11.13+name"; //�������ں�������д
const uint8_t BUFFER_SIZE=sizeof(tab)/sizeof(tab[0]);
uint8_t i2c_buffer_write[BUFFER_SIZE];
uint8_t i2c_buffer_read[BUFFER_SIZE];
char machinenum[]="001";//�������豸��
char tab1[]="2023.11.13+name";//�������Զ��ַ���
char ReadE2P[]="xxxxxx+xxx+xxx";//EEPROM��ȡ������
char Res[]="x";//ʵ����
char Res1[]="1";
char Res0[]="0";
/* ϵͳ��ʼ�� */
void systemInit(void)
{
    systick_config();   // ϵͳʱ������
    gd_XII_systeminit(); // ʵ���������ʼ��

    OLED_Gpio_Init(); // OLED�ܽŸ��ó�ʼ������
    OLED_Init(); // OLED��ʼ��

    i2c_gpio_config();// i2c�ܽ�����
    i2c_config(); // i2c����
    i2c_eeprom_init(); // eeprom��ʼ��

    OLED_Clear(); //����
}
//��д��EEPROM�����ݽ��к˶�
uint8_t i2c_24c02_test(void)
{
    uint16_t i;
    for(i=0;tab[i]!='\0';i++)
    {
        i2c_buffer_write[i]=tab[i];
    }
    OLED_ShowString(0,0,tab);//��tab�ַ�����ʾ��OLED
    eeprom_buffer_write(i2c_buffer_write,EEP_FIRST_PAGE,BUFFER_SIZE);
    eeprom_buffer_read(i2c_buffer_read,EEP_FIRST_PAGE,BUFFER_SIZE);
    OLED_ShowString(0,4,i2c_buffer_read);
    for(i=0;i<BUFFER_SIZE;i++)
    {
        ReadE2P[i]=i2c_buffer_read[i];
    }
    for(i=0;i<BUFFER_SIZE;i++)//�Ƚ�read�����write����
    {
        if(i2c_buffer_read[i]!=i2c_buffer_write[i])
        {
            return I2C_FAIL;
        }
    }
    return I2C_OK;
}
/* ����ʵ��	*/
void test1(void)
{
    OLED_Clear();
    while(1)
    {
        if(I2C_OK==i2c_24c02_test())
        {
            gd_eval_led_on(LED1);
            OLED_ShowCHinese(0,2,1);//��ʾ��
            Uart0Printf("�� \r\n");
            Res[0]=Res1[0]; 
        }
        else
        {
            gd_eval_led_on(LED2);
            OLED_ShowCHinese(0,2,0);
            Uart0Printf("�� \r\n");
            Res[0]=Res0[0];
        }
       // json_student_imformation03(machinenum,tab1,ReadE2P,Res);
        while(1)
        {

        }
    }
}

/* ����ʵ��	*/
void test2(void)
{
    //char nums[]={"00","01","10","11"};
    int index=0;
    while(1)
    {
        OLED_Clear();
        index=(index+1)%4;
        if(index==0)
        {
            OLED_DrawBMP(0,0,127,7,BMP1);
            delay_1ms(3000);
        }
        if(index==1)
        {
            OLED_DrawBMP(0,0,127,7,BMP2);
            delay_1ms(3000);
        }
        if(index==2)
        {
            OLED_DrawBMP(0,0,127,7,img1);
            delay_1ms(3000);
        }
        if(index==3)
        {
            OLED_DrawBMP(0,0,127,7,img2);
            delay_1ms(3000);
        }
        
    }
}
