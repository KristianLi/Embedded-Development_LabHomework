#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "systick.h"
#include "gd32f4xx.h"

/*
//		PB3   	SPI0_SCLK
//		PB4			D/C
//    PB5			SPI0_MOSI
//		PD4			CS
//		PD5			RES
*/

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   

/**
 * @brief	OLED控制接口初始化
 * @param   void
 * @return  void
 */
void OLED_Gpio_Init(void) {
    spi_parameter_struct spi_init_struct;

    rcu_periph_clock_enable(RCU_GPIOB);
		rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_SPI0);
		
		/*SPIO GPIO config*/
		gpio_af_set(GPIOB, GPIO_AF_5, GPIO_PIN_3 | GPIO_PIN_5);
		gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3 | GPIO_PIN_5);
		gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_5);
		
	  gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_4 | GPIO_PIN_13 | GPIO_PIN_14);
		gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_4 | GPIO_PIN_5);
		gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_13 | GPIO_PIN_14);
		gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5);
	
    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8 ;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;;
    spi_init(SPI0, &spi_init_struct);

    /* set crc polynomial */
   // spi_crc_polynomial_set(SPI0,8);
    /* enable SPI0 */
    spi_enable(SPI0);
 
    OLED_RST(1);
		delay_1ms(100);
    OLED_RST(0);
    delay_1ms(100);
    OLED_RST(1);
		delay_1ms(100);
}

/**
 * @brief	OLED底层SPI发送字节
 * @param   data	数据的起始地址
 * @return  void
 */
void OLED_SPI_Send(uint8_t data) {
		uint8_t retry=0;
		spi_i2s_data_transmit(SPI0,data);  
		while(spi_i2s_flag_get(SPI0,SPI_FLAG_TBE)==RESET)	{ } ;
		for(retry=0;retry<10;retry++) ; 
}

/**
 * @brief	写命令到OLED
 * @param   cmd		需要发送的命令
 * @return  void
 */
void OLED_Write_Cmd(uint8_t cmd) {
	  delay_1ms(1);
		OLED_CS(0);
    OLED_DC(0);
    OLED_SPI_Send(cmd);
		//OLED_CS(1);
}

/**
 * @brief	写数据到OLED
 * @param   data		需要发送的数据
 * @return  void
 */
void OLED_Write_Data(uint8_t data) {
		OLED_CS(0);
    OLED_DC(1);
    OLED_SPI_Send(data);
//		OLED_CS(1);
}

/**
 * @brief	写半个字的数据到LCD
 * @param   da		需要发送的数据
 * @return  void
 */
void OLED_WriteByte( uint16_t da) {
		OLED_DC(1);
		OLED_Write_Data(da>>8);
		OLED_Write_Data(da&0Xff);
}

//设置点的坐标
void OLED_Set_Pos(unsigned char x, unsigned char y)  { 
		OLED_Write_Cmd(0xb0+y);
		OLED_Write_Cmd(((x&0xf0)>>4)|0x10);
		OLED_Write_Cmd((x&0x0f)|0x01); 
}   	  
//开启OLED显示    
void OLED_Display_On(void) {
		OLED_Write_Cmd(0X8D);  //SET DCDC命令
		OLED_Write_Cmd(0X14);  //DCDC ON
		OLED_Write_Cmd(0XAF);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void) {
		OLED_Write_Cmd(0X8D);  //SET DCDC命令
		OLED_Write_Cmd(0X10);  //DCDC OFF
		OLED_Write_Cmd(0XAE);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void) {  
		uint8_t i,n;		    
		for(i=0;i<8;i++) {  
				OLED_Write_Cmd (0xb0+i);    //设置页地址（0~7）
				OLED_Write_Cmd (0x02);      //设置显示位置—列低地址
				OLED_Write_Cmd (0x10);      //设置显示位置—列高地址   
				for(n=0;n<128;n++)OLED_Write_Data(0); 
		} //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:

//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr) {      	
	  unsigned char c=0,i=0;	
			OLED_Display_On();
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){
		    x = 0;
		    y = y + 2;
		}
		if(SIZE == 16) {
				OLED_Set_Pos(x,y);	
				for(i=0;i<8;i++)
				OLED_Write_Data(F8X16[c*16+i]);
				OLED_Set_Pos(x,y+1);
				for(i=0;i<8;i++)
				OLED_Write_Data(F8X16[c*16+i+8]);
		}
	  else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_Write_Data(F6x8[c][i]);
	  }
}

//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n) {
		uint32_t result = 1;	 
		while(n--) result *= m;    
		return result;
}				  

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2) {         	
		uint8_t t,temp;
		uint8_t enshow=0;		
		OLED_Display_On();	
		for(t=0;t<len;t++) {
				temp=(num/oled_pow(10,len-t-1))%10;
				if(enshow==0&&t<(len-1)) {
						if(temp==0) {
								OLED_ShowChar(x+(size2/2)*t,y,' ');
								continue;
						}else enshow=1; 
				}
				OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); 
		}
} 

//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr) {
		unsigned char j=0;
			OLED_Display_On();
		while (chr[j]!='\0') {		
			  OLED_ShowChar(x,y,chr[j]);
				x+=8;
			  if(x>120){x=0;y+=2;}
				j++;
		}
}

//显示汉字
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no) {      			    
		uint8_t t,adder=0;

		OLED_Display_On();
		OLED_Set_Pos(x,y);	
		for(t=0;t<16;t++) {
		    OLED_Write_Data(Hzk[2*no][t]);
				adder+=1;
		}	
		OLED_Set_Pos(x,y+1);	
		for(t=0;t<16;t++) {	
				OLED_Write_Data(Hzk[2*no+1][t]);
				adder+=1;
			
		}					
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]) { 	
		unsigned int j=0;
		unsigned char x,y;
		OLED_Display_On();
		if(y1%8==0) y=y1/8;      
		else y=y1/8+1;
		for(y=y0;y<y1;y++) {
				OLED_Set_Pos(x0,y);
				for(x=x0;x<x1;x++) {      
						OLED_Write_Data(BMP[j++]);	    	
				}
		}
} 

/**
 * @brief	OLED初始化
 * @param   void
 * @return  void
 */
void OLED_Init( void ) {
		OLED_CS(0);
    OLED_RST(0);
    delay_1ms(30);
    OLED_RST(1);       	 //从上电到下面开始初始化要有足够的时间，即等待RC复位完毕   
    OLED_Write_Cmd(0xae);//--turn off oled panel
    OLED_Write_Cmd(0x00);//---set low column address
    OLED_Write_Cmd(0x10);//---set high column address
    OLED_Write_Cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_Write_Cmd(0x81);//--set contrast control register
    OLED_Write_Cmd(0xcf); // Set SEG Output Current Brightness
    OLED_Write_Cmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_Write_Cmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_Write_Cmd(0xa6);//--set normal display
    OLED_Write_Cmd(0xa8);//--set multiplex ratio(1 to 64)
    OLED_Write_Cmd(0x3f);//--1/64 duty
    OLED_Write_Cmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_Write_Cmd(0x00);//-not offset
    OLED_Write_Cmd(0xd5);//--set display clock divide ratio/oscillator frequency
    OLED_Write_Cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_Write_Cmd(0xd9);//--set pre-charge period
    OLED_Write_Cmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_Write_Cmd(0xda);//--set com pins hardware configuration
    OLED_Write_Cmd(0x12);
    OLED_Write_Cmd(0xdb);//--set vcomh
    OLED_Write_Cmd(0x40);//Set VCOM Deselect Level
    OLED_Write_Cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_Write_Cmd(0x02);//
    OLED_Write_Cmd(0x8d);//--set Charge Pump enable/disable
    OLED_Write_Cmd(0x14);//--set(0x10) disable
    OLED_Write_Cmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    OLED_Write_Cmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
    OLED_Write_Cmd(0xaf);//--turn on oled panel
    
    OLED_Clear();  //初始清屏
    OLED_Set_Pos(0,0);
    OLED_CS(1);		
}


