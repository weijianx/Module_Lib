/*!
    \file  main.c
    \brief USART HyperTerminal Interrupt
    
    \version 2018-06-19, V1.0.0, firmware for GD32E230
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32e230.h"
#include "gd32e230c_eval.h"
#include "systick.h"
#include <stdio.h>

#include "timer.h"
#include "type.h"

#include "SHT3x.h"
#include "TH_IIC.h"

#include "para.h"

uint8_t receive = 1;
uint32_t receive1=0;
uint8_t bou,bou1;
uint16_t tt1=0;
uint32_t time1=0;
extern BitAction UartRecvFrameOK;

uint8_t transmitter_buffer[] = "\n\rUSART interrupt test\n\r";

uint8_t time_tick = 10;  //默认滤波方式  为平稳滤波

extern SHT3_Typedef T_H;

void DRV_IWDG_Init(void)
{
	rcu_osci_on(RCU_IRC40K);
    while(SUCCESS != rcu_osci_stab_wait(RCU_IRC40K));
	
	/*	FWDG clock is independent lsi 40KHz, DIV128 IS 312.5Hz, that is 3.2 ms per clock
	reload value is 1000, delay time 2500 * 3.2ms = 8 s	*/
	fwdgt_config( 2500, FWDGT_PSC_DIV256 );
	fwdgt_enable();
	
//	//	 /* enable WWDGT clock */
//    rcu_periph_clock_enable(RCU_WWDGT);
//    
//    /*
//     *  set WWDGT clock = (PCLK1 (72MHz)/4096)/8 = 2197Hz (~455 us)
//     *  set counter value to 127
//     *  set window value to 80
//     *  refresh window is: ~455 * (127-80)= 21.3ms < refresh window < ~455 * (127-63) =29.1ms.
//     */
//    wwdgt_config(127,80,WWDGT_CFG_PSC_DIV8);
//    wwdgt_enable();
}



void rs485_Init(void)
{
	/* enable the GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
	/* configure PA2(ADC channel2) as analog input */
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_2);
	
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
	
	gpio_bit_reset(GPIOA,GPIO_PIN_2);
//	GPIO_BOP(GPIOA) = (uint32_t)GPIO_PIN_6;
//	GPIO_BC(GPIOA) = (uint32_t)GPIO_PIN_6;
}

void delayms()
{
	uint16_t i=0xFF,j=0xFF;
	for(;i>0;i--)
		for(;j>0;j--);
}

uint8_t tx_buffer[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
                       0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 
                       0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 
                       0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
                       0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
                       0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
                       0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
                       0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
                       0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
                       0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
                       0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
                       0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
                       0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
                       0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
                       0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
                       0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF };
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t current_sta = 0;
int main(void)
{
	nvic_vector_table_set(FLASH_BASE,0x4000);
	
	uint8_t ch[] = "dggsgawegf\r\n";
	
	nvic_irq_enable(USART0_IRQn, 2);
     
    /* initilize the com */
    com_gpio_init();
    com_usart_init();
    
    /* enable USART TBE interrupt */  
//    usart_interrupt_enable(USART1, USART_INT_TBE);
    
	usart_interrupt_enable(USART0, USART_INT_RBNE);	
	
	DRV_IWDG_Init();
	
//	timer_config();
	TIMER2_Init();

	TIMER15_Init();
		
	ReadPara(); 
	
	TH_IIC_Init(); //iic???
    SHT3_Init();   //sht3?????
	

//	if(time1 >100000)
//	{
//		time1=0;

//	}	
    while (1)
	{
//		Flash_Write_MultiBytes(0x00, tx_buffer, sizeof(tx_buffer));
		fwdgt_counter_reload();

		if(Time_1s_flag)
		{
			Time_1s_flag = 0;
			ReadTH(); 
		}
//			ReadTH();

		if(receive == 0)
		{
			receive1++;
			if(receive1 == 1000)
			{
				gpio_bit_reset(GPIOA,GPIO_PIN_2);
				receive = 1;
				receive1=0;
			}
		}
//		T_H.dev_num = current_sta;

		if(UartRecvFrameOK == Bit_SET)
		{
			MBASC_Function();
		}
		
		else if(T_H.Upload_persist[T_H.dev_num] != MBASC_AUTO_UPLOAD_NONE)
		{
				timer_interrupt_enable(TIMER15, TIMER_INT_UP);
				timer_enable(TIMER15);

			if(AutoUpLoad_counter == T_H.Upload_persist[T_H.dev_num])
			{

				MBASC_AutoUpLoadFrame();
					
				timer_interrupt_disable(TIMER15, TIMER_INT_UP);
				timer_disable(TIMER15);
				AutoUpLoad_counter = 0;
				
//				current_sta = T_H.dev_num;

			}
    	}

	}
	
}



/*!
    \brief      initilize the com GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void com_gpio_init(void)
{
    /* enable COM GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
}

/*!
    \brief      initilize the USART configuration of the com
    \param[in]  none
    \param[out] none
    \retval     none
*/
extern UserTypeDef UserPara;
void com_usart_init(void)
{
//	rs485_Init();
	
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

	flash_read_MultiBytes(Baudrate_ADDR, &(UserPara.Baudrate), 1);
    /* USART configure */
    usart_deinit(USART0);
    switch(UserPara.Baudrate)
	{
		case 01:
			usart_baudrate_set(USART0, 2400U);
			break;
		
		case 02:
			usart_baudrate_set(USART0, 4800U);
			break;
		
		case 03:
			usart_baudrate_set(USART0, 9600U);
			break;
		
		case 04:
			usart_baudrate_set(USART0, 19200U);
			break;
		
		case 05:
			usart_baudrate_set(USART0, 38400U);
			break;

		case 06:
			usart_baudrate_set(USART0, 57600U);
			break;
		
		case 07:
			usart_baudrate_set(USART0, 115200U);
			break;
		default:
			usart_baudrate_set(USART0, 9600U);
			break;
	
	}
//	usart_baudrate_set(USART1, 9600U);
	
	rs485_Init();
	
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

    usart_enable(USART0);
}




/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}
