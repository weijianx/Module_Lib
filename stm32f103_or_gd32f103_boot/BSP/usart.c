#include "usart.h"
#include "BSP.h"
#include "stdarg.h"
//#include "type.h"


u8 UART1_RXBuff[MAX_USART1_DATA_LEN];     //接收缓冲,最大USART_REC_LEN = 255个字节.
u8 UART1_RXBuffLen = 0;
//u8 g_cyMaxLen = 0;

u8 UART1_TXBUFF[MAX_USART1_DATA_LEN];


u8 u8SendIndex = 0;
u8 u8SendNum = 0;
//BitAction g_bRevOverTime = Bit_RESET;

BitAction UartRecvNewData = Bit_RESET;
BitAction UartRecvFrameOK = Bit_RESET;

/**@brief       rs485初始化
* @param		None
* @return       None
*/
void rs485_Init(void)
{
	//GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //GPIO时钟使能
	
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RX_ON;
}


/**@brief       串口初始化
* @param		None
* @return       None
*/
void UART1_Init(void)
{
 	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	USART_TypeDef * COM;
	
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 9600;
	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 


	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRH &= 0xfffff00f;
	GPIOA->CRH |= 0x000004b0;	//PA10、PA9
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	COM = USART1;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;


	USART_ClockStructInit(&USART_ClockInitStruct);
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(COM, &USART_ClockInitStruct);
	USART_Init(COM, &USART_InitStructure);
	
	rs485_Init();
	
	USART_ITConfig(COM, USART_IT_RXNE, ENABLE);			//接收中断
	USART_ITConfig(COM, USART_IT_TC, ENABLE);				//发送完成中断
	USART_Cmd(COM, ENABLE);													//使能外设
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
    u8 UartReadTemp;
   
    if (RESET != USART_GetITStatus(USART1, USART_IT_TC))//响应发送完成中断
    {
        USART_ClearITPendingBit(USART1, USART_IT_TC);

        if (u8SendIndex >= u8SendNum)//已发送完
        {
            u8SendNum = 0;
            USART_ITConfig(USART1, USART_IT_TC, DISABLE);  //清发送完成中断
			RX_ON;                                        //禁止发送  开启接收
        }
        else                    //未发送完
        {
            USART_SendData(USART1, UART1_TXBUFF[u8SendIndex]);
            u8SendIndex++;
        }
    }

    //===============UART Recv===============//
	RX_ON;
    if (RESET != USART_GetITStatus(USART1, USART_IT_RXNE))  //响应接收缓冲不为空中断
    {
        UartReadTemp = USART_ReceiveData(USART1);
      
		MODBUS_ASCII_HandlRevData(UartReadTemp);

    }

    if (RESET != USART_GetFlagStatus(USART1, USART_IT_ORE))//响应数据溢出中断，
    {
        USART_ClearITPendingBit(USART1, USART_IT_ORE);//清中断
    }
    
}



u32 UART1_SendData(u8* UART1_SendBuff, u32 Len)
{
    u32 i = 0;
	
 
    if ((0 == Len) || (((u8*)0) == UART1_SendBuff))
        return 0;

    if (u8SendNum != 0)
    {
        return 0;
    }

    if (Len > (sizeof(UART1_TXBUFF) / sizeof(UART1_TXBUFF[0])))
    {
        Len = (sizeof(UART1_TXBUFF) / sizeof(UART1_TXBUFF[0]));
    }

    for (i = 0; i < Len; i++)
    {
        UART1_TXBUFF[i] = UART1_SendBuff[i];
    }

    USART_SendData(USART1, UART1_TXBUFF[0]);

    u8SendIndex = 1;
    u8SendNum = Len;
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);            //开启发送完成中断

    return(Len);
}


s32 UART1_RecvData(u8* UART1_RecvBuff, u32 Len)
{
    u32 i = 0;

    if ((0 == Len) || (((u8*)NULL) == UART1_RecvBuff))
    {
        return 0;
    }

    if ((Bit_RESET == UartRecvFrameOK) || (0 == UART1_RXBuffLen))
    {
        return 0;
    }

    if (Len < UART1_RXBuffLen)
    {
        return -1;
    }

    Len = UART1_RXBuffLen;

    for (i = 0; i < Len; i++)
    {
        UART1_RecvBuff[i] = UART1_RXBuff[i];
    }

    UartRecvFrameOK = Bit_RESET;

    UART1_RXBuffLen = 0;
    //g_cyMaxLen = 0;

    return Len;
}



void uprintf(const char *fmt,...)		                                //用usart_printf打印串口的数据，用于调试
{
    u32 StringLen;
    va_list marker;
    char buff[64];
    va_start(marker, fmt);
    vsprintf(buff, fmt, marker);
    va_end(marker);
    StringLen = strlen(buff);
    UART1_SendData((uint8_t*)buff, StringLen);
    while(0 != u8SendNum);
}




