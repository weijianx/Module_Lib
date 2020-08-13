/**
* @file         spi.h
* @brief        ģ��SPI��ͷ�ļ�
* @details      ģ��SPI�������ŵĺ궨�塢spi.c�ļ�����������
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

//SPI����ʱ�����ź궨��
#define 	SCK_L		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET)
#define 	SCK_H		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET)

//SPI�����������ź궨��
#define 	MISO		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)

//SPI����������ź궨��
#define 	MOSI_L		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define 	MOSI_H		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

//SPI����Ƭѡ���ź궨��
#define 	NSS_L		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define 	NSS_H		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)

/**
* @brief		΢����ʱ����
* @para			None
* @retval		None
*/
void delay_us(uint32_t nus);

/**
* @brief		ʱ��ǰ�ز�����ʱ�Ӻ������
* @para			data,���������
* @retval		None

*/
void spi_ReadWrite_data(uint8_t data);

/**
* @brief		��������
* @para			None
* @retval		rx_data,���յ�������

*/
uint8_t spi_Read_Data(void);

#endif

