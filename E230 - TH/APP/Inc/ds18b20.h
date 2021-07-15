#ifndef __DS18B20_H
#define __DS18B20_H

//#include "rtthread.h"
#include "gd32e230.h"


typedef struct {
	float	Ds18b20_temp;
		
}ds18b20_TypeDef;


//IO��������
//#define DS18B20_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
//#define DS18B20_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}
#define DS18B20_DQ_OUT_H  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET) //���ݶ˿�	PA0
#define DS18B20_DQ_OUT_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)  //���ݶ˿�	PA0
//IO��������											   
//#define	DS18B20_DQ_OUT PAout(0) //���ݶ˿�	PA0
//#define	DS18B20_DQ_IN  PAin(0)  //���ݶ˿�	PA0
#define	DS18B20_DQ_IN  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)  //���ݶ˿�	PA0

void DS18B20_Output(void);
void DS18B20_Input(void);
void DS18B20_Init(void);			//��ʼ��DS18B20
float DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(uint8_t dat);//д��һ���ֽ�
uint8_t DS18B20_Read_Byte(void);		//����һ���ֽ�
uint8_t DS18B20_Read_Bit(void);		//����һ��λ
uint8_t DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20 


#endif


