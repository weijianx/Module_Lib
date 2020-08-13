/**
* @file         spi.c
* @brief        ģ��SPI��Ӧ��
* @details      ģ��SPI������ż���ص��½��ط�������
* @author       Τ����
* @date         2020-08-03
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/08/03  <td>1.0.0    <td>Τ����    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#include "spi.h"
#include "tim.h"

/**
* @brief		΢����ʱ����
* @para			None
* @retval		None
*/
//void delay_us(uint32_t tim)
//{
//	tim = tim*6;
//	while(tim--);	
//}

/**
* @brief		ʱ��ǰ�ز�����ʱ�Ӻ������
* @para			data,���͵�����
* @retval		None
*/
void spi_ReadWrite_data(uint8_t data)
{
	// CPOL = 0 CPHA = 0 SCLK�͵�ƽ��Ч ��ż���ز���
	uint8_t i=0;

	//SCLK=0;

	NSS_L;					//Ƭѡ���ͷ�������
	for(i=0; i<8; i++)
	{
		//���ݷ��ʹӸ�λ����λ
		if(data & (1<<(7-i)))
		{
			// MOSI=1;
			MOSI_H;
		}
		else
		{
			//MOSI=0;
			MOSI_L;
		}
		
		//ʱ������ı仯
		SCK_L;
		delay_tim_5us();		
	
		SCK_H;
		delay_tim_5us();

	}	
	//�������ݵ����λ�������ݲ��ν����ĸߵ�
	if(data & (1<<(7-0)))
	{
		// MOSI=1;
		MOSI_H;
	}
	else
	{
		//MOSI=0;
		MOSI_L;
	}
	
	SCK_L;			//����ʱ��ͼʱ������

	NSS_H;			//���ݷ������Ƭѡ����
	return;

}

/**
* @brief		��������
* @para			None
* @retval		rx_data,���յ�������
*/
uint8_t spi_Read_Data(void)
{
	uint8_t i;
	uint8_t rx_data = 0;
	
	NSS_L;
	for(i=0; i<8; i++)
	{
		if(MISO)
		{
			rx_data|=1<<(7-i);
		}
		
	}
	NSS_H;

	return rx_data;

}




