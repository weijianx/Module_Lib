/*****************************************************************************
* @file    Project/includes.h 
* @author  ktfe
* @version V1.0.0
* @date    24/01/2014
* @brief   include
*****************************************************************************/

#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "stm32f10x_sys.h"
#include "stm32f10x_gpio.h"


typedef  void (*pFunction)(void);

/*Set the number of ticks in one second*/
#define TICKS_PER_SEC 1000


typedef struct
{
	uint8_t UpgradeFlag;														// ϵͳ������־ �洢�����ַ Addr_0x08003000 
	uint8_t Addr;																    // �ӻ�ID 		�洢�����ַ Addr_0x08003001
	uint8_t Baud;																    // ������		�洢�����ַ Addr_0x08003002
	uint8_t Parity;																  // ��żУ�� 	�洢�����ַ Addr_0x08003003
	uint8_t OutPutMod;															// ���ģʽ 	�洢�����ַ Addr_0x08003004
	uint8_t ProgEraseFlag;
	uint8_t Reserved[3];														// ���� �������� ������Ӹ������ ��16�ֽڶ���
}CommonPara_Typedef;															// ͨ�ò����ṹ�� ͨ�ò�����ͻ��ɲ��������ṹ�岿�ֲ����ص� ���洢��ʽ��ͬ
																				// Ŀ����Ϊ�˼���BootLoad��Modbus�ȶ���
typedef struct
{
	uint8_t Flag;								  // Flashд��־
//  uint16_t TempH;                             	  // Addr_0x02 ���ݸ�16λ 
//  uint16_t TempL;                             	  // Addr_0x03 ���ݵ�16λ
	uint8_t Addr;                             		  // Addr_0x30 �豸��ַ
	uint8_t Baud;                             		  // Addr_0x31 ������
	uint8_t Parity;                            	  // Addr_0x32 ��żУ��
	uint8_t Compensation;                      	  // Addr_0x34 ����ʹ��
	uint8_t UploadFreq;                      		  // Addr_0x3F �Զ��ϴ�ʱ��
	uint8_t UpgradeFlag;								// Addr_0x40 ϵͳ������־
	uint8_t Reset;                             	  // Addr_0x51 �ָ���������
	uint8_t OutPutMod;                         	  // Addr_0x60 ���ģʽ
//	uint8_t Reserved[5];													  // ���� �������� ������Ӹ������ ��16�ֶ���
}CustomerPara_Typedef;													  // �ͻ��ɲ��������ṹ��



typedef struct
{
	CommonPara_Typedef		Common;												// ͨ�ò����ṹ�� ͨ�ò�����ͻ��ɲ��������ṹ�岿�ֲ����ص� ���洢��ʽ��ͬ Ϊ����BootLoad��Modbus�淶 
	CustomerPara_Typedef	Customer;											// �ͻ��ɲ��������ṹ�� 

}FlashPara_Typedef;																// Flash��������ṹ�� �ṹ���е�Data������������


/*������ʼ��*/
void Target_Init(void);


#endif
