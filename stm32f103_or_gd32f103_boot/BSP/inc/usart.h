#ifndef _USART_H
#define _USART_H
#include "stdio.h"

#include "main.h"	

//#define MB_ASCII_MODE          0x01
#define MB_RTU_MODE            0x00
#define MAX_USART1_DATA_LEN    600


extern u8 UART1_RXBuff[MAX_USART1_DATA_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

extern BitAction UartRecvFrameOK ;


//#include "type.h"

#define TX_ON    GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define RX_ON    GPIO_ResetBits(GPIOA, GPIO_Pin_8)

void rs485_Init(void);
	
void UART1_Init(void);
u32 UART1_SendData( u8* UART1_SendBuff, u32 Len );
void UART1_IRQService( void ) ;
s32 UART1_RecvData( u8* UART1_RecvBuff, u32 Len );

void uprintf(const char *fmt,...);//调试用
#endif


