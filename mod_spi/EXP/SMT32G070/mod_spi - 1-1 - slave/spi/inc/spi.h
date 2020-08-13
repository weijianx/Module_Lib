/**
* @file         spi.h
* @brief        ģ��SPI��ͷ�ļ�
* @details      ģ��SPI�ӻ����ŵĺ궨�塢spi.c�ļ�����������
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

#ifndef __SPI_H
#define __SPI_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//SPI�ӻ�ʱ�����ź궨��
#define 	SCK			HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)

//SPI�ӻ�������ŵĺ궨��
#define 	MISO_L		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define 	MISO_H		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)

//SPI�ӻ��������ŵĺ궨��
#define 	MOSI		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)

//SPI�ӻ�Ƭѡ�ĺ궨��
#define 	NSS			HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)

/**
* @brief		΢����ʱ����
* @para			None
* @retval		None
*/
void delay_us(uint32_t nus);

/**
* @brief		ʱ��ǰ�ز�����ʱ�Ӻ������
* @para			data,���������
* @retval		dNone

*/
void spi_ReadWrite_data(uint8_t data);

/**
* @brief		��������
* @para			None
* @retval		rx_data,���յ�������
*/
uint8_t spi_Read_Data(void);

#endif

