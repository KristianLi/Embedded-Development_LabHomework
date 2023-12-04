#include "SHT30.h"
#include "gd32f4xx.h"                   // Device header
#include "E53_IA1.h"
#include "user.h"

// SHT30��λ����
void SHT30_reset(void) {
	uint8_t SHT3X_Resetcommand_Buffer[2] = {0x30, 0xA2}; // soft reset
	E53_IA1_I2C_Master_Transmit(SHT30_Addr << 1, SHT3X_Resetcommand_Buffer, 2);
	delay_ms(15);
}

// ��ʼ��SHT30�����ò�������
void Init_SHT30(void) {
	uint8_t SHT3X_Modecommand_Buffer[2] = {0x22, 0x36};
	E53_IA1_I2C_Master_Transmit(SHT30_Addr << 1, SHT3X_Modecommand_Buffer, 2);
}

/*
	SHT3x_CheckCrc
	���������ȷ��
data: ��ȡ��������
nbrOfBytes: ��Ҫ���������
checksum: ��ȡ����У�ӱ���ֵ
����ֵ У���� 0:�ɹ� 1:ʧ��
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
	�¶ȼ���
  u16sT: ��ȡ�����¶�ԭʼ����
	����ֵ �������¶�����
*/
float SHT3x_CalcTemperatureC(unsigned short u16sT) {
	float tempratureC = 0;
	
	u16sT &= ~0x0003;
	
	tempratureC = (175 * (float)u16sT / 65535 - 45);
	
	return tempratureC;
}

/*
	SHT3x_CalcRH
	ʪ�ȼ���
	u16sRH ��ȡ����ʪ��ԭʼ����
	����ֵ ������ʪ������
*/
float SHT3x_CalcRH(unsigned short u16sRH) {
	float humidityRH = 0;
	u16sRH &= ~0x0003;
	
	humidityRH = (100 * (float)u16sRH / 65535);
	
	return humidityRH;
}


