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
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6);
	
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_6);
	
	gpio_bit_reset(GPIOA,GPIO_PIN_6);
//	GPIO_BOP(GPIOA) = (uint32_t)GPIO_PIN_6;
//	GPIO_BC(GPIOA) = (uint32_t)GPIO_PIN_6;
}

void delayms()
{
	uint16_t i=0xFF,j=0xFF;
	for(;i>0;i--)
		for(;j>0;j--);
}


/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t current_sta = 0;
int main(void)
{

	uint8_t ch[] = "dggsgawegf\r\n";
	
	nvic_irq_enable(USART1_IRQn, 2);
     
    /* initilize the com */
    com_gpio_init();
    com_usart_init();
    
    /* enable USART TBE interrupt */  
//    usart_interrupt_enable(USART1, USART_INT_TBE);
    
	usart_interrupt_enable(USART1, USART_INT_RBNE);	
	
	DRV_IWDG_Init();
	
	timer_config();
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
				gpio_bit_reset(GPIOA,GPIO_PIN_6);
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
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
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
    rcu_periph_clock_enable(RCU_USART1);

	flash_read_MultiBytes(Baudrate_ADDR, &(UserPara.Baudrate), 1);
    /* USART configure */
    usart_deinit(USART1);
    switch(UserPara.Baudrate)
	{
		case 01:
			usart_baudrate_set(USART1, 2400U);
			break;
		
		case 02:
			usart_baudrate_set(USART1, 4800U);
			break;
		
		case 03:
			usart_baudrate_set(USART1, 9600U);
			break;
		
		case 04:
			usart_baudrate_set(USART1, 19200U);
			break;
		
		case 05:
			usart_baudrate_set(USART1, 38400U);
			break;

		case 06:
			usart_baudrate_set(USART1, 57600U);
			break;
		
		case 07:
			usart_baudrate_set(USART1, 115200U);
			break;
		default:
			usart_baudrate_set(USART1, 9600U);
			break;
	
	}
//	usart_baudrate_set(USART1, 9600U);
	
	rs485_Init();
	
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);

    usart_enable(USART1);
}




/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (uint8_t) ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}
