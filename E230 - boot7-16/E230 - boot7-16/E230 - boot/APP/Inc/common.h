#ifndef _COMMON_H
#define _COMMON_H

#include "gd32e230.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Constants used by Serial Command Line Mode */
#define CMD_STRING_SIZE       128
#define USART_ASCII_MAX_DATALEN     600
#define USART_NOASC_MAX_DATALEN     300
#define USART_NOMAL_MAX_DATALEN     100
#define USART_ASCII_MIN_DATALEN     80

/* Exported macro ------------------------------------------------------------*/
/* Common routines */
    
unsigned long BECharArrayToU32(unsigned char *pData);
void ClearMemory(unsigned char *pbuf, unsigned long num, unsigned char ucData);
void Decoding(unsigned char *pSource, unsigned long Length);

void TimingDelay_Decrement(void);
void TimingDelay_SetTime(unsigned long Delay);
unsigned long TimingDelay_TimeOut(void);
#endif


