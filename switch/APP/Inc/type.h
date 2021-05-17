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
    uint8_t SlaveAddr;                                                          //�ӻ���ַ
    uint8_t Baudrate;		                                                 //������
    uint8_t Parity;             			                         //��żУ��λ
	uint8_t AutoUpLoadTime;

}UserTypeDef;



#endif

