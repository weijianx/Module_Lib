/*****************************************************************************
* @file    Project/Template/stm32f10x_sys.c 
* @author  ktfe
* @date    14/03/2012
* @brief   system init program body
*****************************************************************************/

#include "stm32f10x_sys.h"

//ϵͳоƬ�����߼���λ
void Soft_Reset(void)
{   
	SCB->AIRCR = 0X05FA0000|(u32)0x04;	  
} 
//���ж�
void Enable_IRQ(void)
{
	__ASM volatile ("cpsie i");
}
//���ж�
void Disable_IRQ(void)
{
	__ASM volatile ("cpsid i");
}
//JTAGģʽ����,��������JTAG��ģʽ
//mode:jtag,swdģʽ����;
//0:ȫʹ��;  1:ʹ��SWD;  2:ȫ�ر�;		  
void JTAG_Set(u8 mode)
{
	u32 temp;
	temp = mode;
	temp <<= 25;
	RCC->APB2ENR |= 0x00000001;     //��������ʱ��	   
	AFIO->MAPR &= 0XF8FFFFFF;       //���MAPR��[26:24]
	AFIO->MAPR |= temp;             //����jtagģʽ
	RCC->APB2ENR &= 0xfffffffe;     //�رո���ʱ��
}

