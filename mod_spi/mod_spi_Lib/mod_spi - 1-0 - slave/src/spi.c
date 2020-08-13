/**
* @file         spi.c
* @brief        ģ��SPI��Ӧ��
* @details      ģ��SPI�ӻ���ż���ص������ؽ�������
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


/**
* @brief		΢����ʱ����
* @para			None
* @retval		None
*/
void delay_us(uint32_t tim)
{
	tim = tim*10;
	while(tim--);	
}

/**
* @brief		ʱ��ǰ�ز�����ʱ�Ӻ������
* @para			data,���������
* @retval		None

*/
void spi_ReadWrite_data(uint8_t data)
{
	// CPOL = 1 CPHA = 0 SCLK�͵�ƽ��Ч ��ż���ز���
	uint8_t i=0;

	GPIO_PinState CS_pin;
	//SCLK=0;

	CS_pin = NSS;
	if(CS_pin == GPIO_PIN_RESET)
	{
		for(i=0; i<8; i++)
		{
			if(data & (1<<(7-i)))
			{
				// MOSI=1;
				MISO_H;
			}
			else
			{
				//MOSI=0;
				MISO_L;
			}
			
			//�½��ز���
			//SCLK=1;
			while(SCK);
			while(!SCK);		


		}	
		
	}

	while(SCK);
	
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
	
	if(!NSS)
	{
		for(i=0; i<8; i++)
		{
			if(MOSI)
			{
				rx_data|=1<<(7-i);
			}
			
			while(SCK);
			while(!(SCK));
				
		}
	}
	return rx_data;

}




