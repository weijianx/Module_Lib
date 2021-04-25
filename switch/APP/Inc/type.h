#ifndef _TYPE_H
#define _TYPE_H
#include "stm32l0xx.h"

typedef enum
{ 
  Bit_RESET = 0,
  Bit_SET = 1
}BitAction;



typedef struct 
{
    uint8_t SlaveAddr;                                                          //从机地址
    uint8_t Baudrate;		                                                 //波特率
    uint8_t Parity;             			                         //奇偶校验位
	uint8_t AutoUpLoadTime;

}UserTypeDef;



#endif

