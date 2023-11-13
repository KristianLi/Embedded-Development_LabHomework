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
uint8_t tab[]="2023.11.13+name"; //定义日期和姓名缩写
const uint8_t BUFFER_SIZE=sizeof(tab)/sizeof(tab[0]);
uint8_t i2c_buffer_write[BUFFER_SIZE];
uint8_t i2c_buffer_read[BUFFER_SIZE];
char machinenum[]="001";//在这填设备号
char tab1[]="2023.11.13+name";//在这填自定字符串
char ReadE2P[]="xxxxxx+xxx+xxx";//EEPROM读取缓存区
char Res[]="x";//实验结果
char Res1[]="1";
char Res0[]="0";
/* 系统初始化 */
void systemInit(void)
{
    systick_config();   // 系统时钟配置
    gd_XII_systeminit(); // 实验箱外设初始化

    OLED_Gpio_Init(); // OLED管脚复用初始化配置
    OLED_Init(); // OLED初始化

    i2c_gpio_config();// i2c管脚配置
    i2c_config(); // i2c配置
    i2c_eeprom_init(); // eeprom初始化

    OLED_Clear(); //清屏
}
//对写入EEPROM对数据进行核对
uint8_t i2c_24c02_test(void)
{
    uint16_t i;
    for(i=0;tab[i]!='\0';i++)
    {
        i2c_buffer_write[i]=tab[i];
    }
    OLED_ShowString(0,0,tab);//将tab字符串显示到OLED
    eeprom_buffer_write(i2c_buffer_write,EEP_FIRST_PAGE,BUFFER_SIZE);
    eeprom_buffer_read(i2c_buffer_read,EEP_FIRST_PAGE,BUFFER_SIZE);
    OLED_ShowString(0,4,i2c_buffer_read);
    for(i=0;i<BUFFER_SIZE;i++)
    {
        ReadE2P[i]=i2c_buffer_read[i];
    }
    for(i=0;i<BUFFER_SIZE;i++)//比较read缓存和write缓存
    {
        if(i2c_buffer_read[i]!=i2c_buffer_write[i])
        {
            return I2C_FAIL;
        }
    }
    return I2C_OK;
}
/* 基础实验	*/
void test1(void)
{
    OLED_Clear();
    while(1)
    {
        if(I2C_OK==i2c_24c02_test())
        {
            gd_eval_led_on(LED1);
            OLED_ShowCHinese(0,2,1);//显示对
            Uart0Printf("对 \r\n");
            Res[0]=Res1[0]; 
        }
        else
        {
            gd_eval_led_on(LED2);
            OLED_ShowCHinese(0,2,0);
            Uart0Printf("错 \r\n");
            Res[0]=Res0[0];
        }
        json_student_imformation03(machinenum,tab1,ReadE2P,Res);
        while(1)
        {

        }
    }
}

/* 进阶实验	*/
void test2(void)
{

}
