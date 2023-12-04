#include "SHT30.h"
#include "gd32f4xx.h"                   // Device header
#include "E53_IA1.h"
#include "user.h"

// SHT30复位函数
void SHT30_reset(void) {
	uint8_t SHT3X_Resetcommand_Buffer[2] = {0x30, 0xA2}; // soft reset
	E53_IA1_I2C_Master_Transmit(SHT30_Addr << 1, SHT3X_Resetcommand_Buffer, 2);
	delay_ms(15);
}

// 初始化SHT30，设置测量周期
void Init_SHT30(void) {
	uint8_t SHT3X_Modecommand_Buffer[2] = {0x22, 0x36};
	E53_IA1_I2C_Master_Transmit(SHT30_Addr << 1, SHT3X_Modecommand_Buffer, 2);
}

/*
	SHT3x_CheckCrc
	检查数据正确性
data: 读取到的数据
nbrOfBytes: 需要检验的数量
checksum: 读取到的校队比验值
返回值 校验结果 0:成功 1:失败
*/
uint8_t SHT3x_CheckCrc(char data[], char nbrOfBytes, char checksum) {
	char crc = 0xff;
	char bit = 0;
	char byteCtr;
	
	// calculate 8-bit checksum with given polynomial
	for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++) {
		crc ^= (data[byteCtr]);
		for(bit = 8; bit; bit--) {
			if(crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
			else crc = (crc << 1);
		}
	}
	
	if(crc != checksum) return 1;
	else return 0;
}

/*
	SHT3x_CalcTempratureC
	温度计算
  u16sT: 读取到的温度原始数据
	返回值 计算后的温度数据
*/
float SHT3x_CalcTemperatureC(unsigned short u16sT) {
	float tempratureC = 0;
	
	u16sT &= ~0x0003;
	
	tempratureC = (175 * (float)u16sT / 65535 - 45);
	
	return tempratureC;
}

/*
	SHT3x_CalcRH
	湿度计算
	u16sRH 读取到的湿度原始数据
	返回值 计算后的湿度数据
*/
float SHT3x_CalcRH(unsigned short u16sRH) {
	float humidityRH = 0;
	u16sRH &= ~0x0003;
	
	humidityRH = (100 * (float)u16sRH / 65535);
	
	return humidityRH;
}


