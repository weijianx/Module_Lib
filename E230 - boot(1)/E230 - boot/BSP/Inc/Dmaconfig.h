/*************************************************************************************
                               This file is the GD32F103 
				GD seiral mpu USART via DMA congfig communicate head file
*************************************************************************************/
#ifndef __DMACONFIG_H
#define __DMACONFIG_H
//Includes
#include "Includes.h"
#include "usart.h"

//Datatype define
typedef struct{
	u16	uTxdsize;
	u8*	uTxdbuf;
	u16	uRxdsize;
	u8*	uRxdbuf;
}_USART_DMA_PARA;


//Functions declare
void DMA_Rcv_TransDataParaConfig( PS_COMM_BUFPARA );


//----------------------------------------------------------------------------------------
/*
typedef struct{
	rcu_periph_enum	xDMA_Rcu_ID;
	IRQn_Type		
}_USART_DMA_PARA;
*/

//Functions declare
void DMA_Channel1_2_IRQHandler( void );

#endif
