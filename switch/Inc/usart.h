/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "type.h"

#define EN_485_PORT             GPIOA
#define EN_485_PIN              GPIO_PIN_4 
#define TX_ON                   EN_485_PORT->BSRR = EN_485_PIN
#define TX_OFF                  EN_485_PORT->BRR = EN_485_PIN

#define USART_MAX_DATALEN       128    
//#define USART_MIN_DATALEN       600

extern BitAction UartRecvFrameOK;

#define LPUSART1_PORT           GPIOA
#define LPUSART1_RX             GPIO_PIN_2
#define LPUSART1_TX             GPIO_PIN_3

void Uart_Config_Init(void);
void Enable_Lpuart1(void);
void Disable_Lpuart1(void);
int32_t UART1_RecvData(uint8_t *UART1_RecvBuff, uint32_t Len);
uint32_t UART1_SendData(uint8_t *UART1_SendBuff, uint32_t Len);

void MX_TIM2_Init(void);
void uprintf(const char *fmt,...);

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
