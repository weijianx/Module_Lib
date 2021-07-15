/*************************************************************************************
                               This file is the GD32F103 
				GD seiral mpu USART via DMA process communicate head file
*************************************************************************************/
#ifndef __USART_H
#define	__USART_H

//Includes
#include "Includes.h"

//Define macrco
#define	NUM_COM	2

//Define Communicate UART parameter
#define MODBUS_RTU      1
#define MODBUS_ASCII    2

#define	RS485CTL_SRC_CLK	RCU_GPIOA
#define	RS485CTL_PORT		GPIOA
#define	RS485CTL_PIN		GPIO_PIN_2
//#define RS485_RCV_ENABLE()	GPIO_BC( RS485CTL_PORT ) = RS485CTL_PIN;
//#define RS485_TXD_ENABLE()	GPIO_BOP( RS485CTL_PORT ) = RS485CTL_PIN;

#define RS485_RCV_ENABLE() 	gpio_bit_reset(GPIOA,GPIO_PIN_2)
#define RS485_TXD_ENABLE()	gpio_bit_set(GPIOA,GPIO_PIN_2)



//USART0 ADDRESS
#define	USART0_ADDR_BASE	0x40013800
#define	USART0_ADDR_DATA	0x40013828
#define	READ_USART0_DATA()	(*(volatile uint32_t *)(uint32_t)(0x40013828))


//USART1 ADDRESS
#define	USART1_ADDR_BASE	0x40014400
#define	USART1_ADDR_DATA	0x40004428

typedef struct 
{
	u8	SlaveAddr;
	u8	BaudRate;
    u8	Parity;
    u8	ModbusType;  //1-RTU 2-ASCII
}S_COMM_PARA,*PS_COMM_PARA;

typedef struct{
    u8* prcv;
    u16 rcvsize;
    u8* psnd;
    u16 sndsize;
}S_COMM_BUFPARA, *PS_COMM_BUFPARA;

typedef void (*ModbusParseFunc)(u8 *psrc, u32 len, u8 type );
typedef struct{
    u8*             pbuf;
    u32             bufsize;
    ModbusParseFunc pparse;
}S_PARSE_TAG, *PS_PARSE_TAG;

//Functions declare
bool USART_ConfigPaserParameters( PS_PARSE_TAG );
bool USART_CommunicateInitial( u8 com_sn, PS_COMM_BUFPARA, PS_COMM_PARA );
bool USART_CommunicateParaConfig( u8, PS_COMM_PARA );
void USART_SendDataViaUSART0DmaChannel( u16 datlen );
bool USART_SendDataViaUSART0DmaChnIsBusy( void );

//Timer2 init
void USART_Timer2_Configuation( u8 dlyid );

//--------------------------------modbus part-----------------------------------------------------
void GD32E23X_Modbus_UpgradeResponseProcess( u8* psrc, u16 len );

//Interrupt function
void USART0_IRQHandler( void );
void TIMER2_IRQHandler( void );

#endif


