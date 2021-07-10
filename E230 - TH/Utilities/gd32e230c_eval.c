/*!
    \file  gd32e230c_eval.c
    \brief firmware functions to manage leds, keys, COM ports
    
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

#include "gd32e230c_eval.h"
#include "type.h"
extern 	uint8_t bou,bou1;
extern UserTypeDef UserPara;

/* private variables */
static const uint32_t GPIO_PORT[LEDn]       = {LED1_GPIO_PORT,
                                               LED2_GPIO_PORT,
                                               LED3_GPIO_PORT,
                                               LED4_GPIO_PORT};

static const uint32_t GPIO_PIN[LEDn]        = {LED1_PIN,
                                               LED2_PIN,
                                               LED3_PIN,
                                               LED4_PIN};

static const rcu_periph_enum COM_CLK[COMn]  = {EVAL_COM_CLK};

static const uint32_t COM_TX_PIN[COMn]      = {EVAL_COM_TX_PIN};

static const uint32_t COM_RX_PIN[COMn]      = {EVAL_COM_RX_PIN};

static const rcu_periph_enum GPIO_CLK[LEDn] = {LED1_GPIO_CLK,
                                               LED2_GPIO_CLK,
                                               LED3_GPIO_CLK,
                                               LED4_GPIO_CLK};

static const uint32_t KEY_PORT[KEYn]        = {WAKEUP_KEY_GPIO_PORT, 
                                               TAMPER_KEY_GPIO_PORT
                                               };

static const uint32_t KEY_PIN[KEYn]         = {WAKEUP_KEY_PIN, 
                                               TAMPER_KEY_PIN
                                               };

static const rcu_periph_enum KEY_CLK[KEYn]  = {WAKEUP_KEY_GPIO_CLK, 
                                               TAMPER_KEY_GPIO_CLK
                                               };

static const exti_line_enum KEY_EXTI_LINE[KEYn] = {WAKEUP_KEY_EXTI_LINE,
                                                   TAMPER_KEY_EXTI_LINE
                                                   };

static const uint8_t KEY_PORT_SOURCE[KEYn]      = {WAKEUP_KEY_EXTI_PORT_SOURCE,
                                                   TAMPER_KEY_EXTI_PORT_SOURCE
                                                   };

static const uint8_t KEY_PIN_SOURCE[KEYn]       = {WAKEUP_KEY_EXTI_PIN_SOURCE,
                                                   TAMPER_KEY_EXTI_PIN_SOURCE
                                                   };

static const uint8_t KEY_IRQn[KEYn]             = {WAKEUP_KEY_EXTI_IRQn, 
                                                   TAMPER_KEY_EXTI_IRQn
                                                   };

/* eval board low layer private functions */
/*!
    \brief      configure led GPIO
    \param[in]  lednum: specify the led to be configured
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_led_init(led_typedef_enum lednum)
{
    /* enable the led clock */
    rcu_periph_clock_enable(GPIO_CLK[lednum]);
    /* configure led GPIO port */ 
    gpio_mode_set(GPIO_PORT[lednum], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN[lednum]);
    gpio_output_options_set(GPIO_PORT[lednum], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN[lednum]);

    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn on selected led
    \param[in]  lednum: specify the led to be turned on
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_led_on(led_typedef_enum lednum)
{
    GPIO_BOP(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn off selected led
    \param[in]  lednum: specify the led to be turned off
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_led_off(led_typedef_enum lednum)
{
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      toggle selected led
    \param[in]  lednum: specify the led to be toggled
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_led_toggle(led_typedef_enum lednum)
{
    GPIO_TG(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      configure key
    \param[in]  keynum: specify the key to be configured
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
    \param[in]  keymode: specify button mode
      \arg        KEY_MODE_GPIO: key will be used as simple IO
      \arg        KEY_MODE_EXTI: key will be connected to EXTI line with interrupt
    \param[out] none
    \retval     none
*/
void gd_eval_key_init(key_typedef_enum keynum, keymode_typedef_enum keymode)
{
    /* enable the key clock */
    rcu_periph_clock_enable(KEY_CLK[keynum]);
    rcu_periph_clock_enable(RCU_CFGCMP);

    /* configure button pin as input */
    gpio_mode_set(KEY_PORT[keynum], GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_PIN[keynum]);

    if (keymode == KEY_MODE_EXTI) {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(KEY_IRQn[keynum], 2U);

        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(KEY_PORT_SOURCE[keynum], KEY_PIN_SOURCE[keynum]);

        /* configure key EXTI line */
        exti_init(KEY_EXTI_LINE[keynum], EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(KEY_EXTI_LINE[keynum]);
    }
}

/*!
    \brief      return the selected key state
    \param[in]  keynum: specify the key to be checked
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
    \param[out] none
    \retval     the key's GPIO pin value
*/
uint8_t gd_eval_key_state_get(key_typedef_enum keynum)
{
    return gpio_input_bit_get(KEY_PORT[keynum], KEY_PIN[keynum]);
}

/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        EVAL_COM: COM on the board
    \param[out] none
    \retval     none
*/
void gd_eval_com_init(uint32_t com)
{
    uint32_t COM_ID;
    
    COM_ID = 0U;

    /* enable COM GPIO clock */
    rcu_periph_clock_enable(EVAL_COM_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);

    /* connect port to USARTx_Tx */
    gpio_af_set(EVAL_COM_GPIO_PORT, EVAL_COM_AF, COM_TX_PIN[COM_ID]);

    /* connect port to USARTx_Rx */
    gpio_af_set(EVAL_COM_GPIO_PORT, EVAL_COM_AF, COM_RX_PIN[COM_ID]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(EVAL_COM_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_TX_PIN[COM_ID]);
    gpio_output_options_set(EVAL_COM_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, COM_TX_PIN[COM_ID]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(EVAL_COM_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_RX_PIN[COM_ID]);
    gpio_output_options_set(EVAL_COM_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, COM_RX_PIN[COM_ID]);

    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, 115200U);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);

    usart_enable(com);
}



uint8_t u8SendNum = 0;                  //当前发送数据个数
uint8_t u8SendIndex = 0;                //发送数据缓冲前缀
uint8_t UARTx_RXBuffLen = 0;            //接收数据缓存长度


uint8_t UARTx_RXBuff[USART_MAX_DATALEN];//接收数据缓存
uint8_t UARTx_TXBUFF[USART_MAX_DATALEN];//发送数据缓存

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/

char ch;
void USART1_IRQHandler(void)
{
//	flash_read_MultiBytes(BAUDRATE, &bou, 1);
//		if(bou1 != bou)
//			 com_usart_init();
			//nvic_system_reset();
//	uint16_t i;
    if(RESET != usart_interrupt_flag_get(EVAL_COM1, USART_INT_FLAG_RBNE)){
        /* receive data */
		usart_interrupt_flag_clear(EVAL_COM1, USART_INT_FLAG_RBNE);
//		memset((void*)&ch, 0, 1) ;
//        UARTx_RXBuff[UARTx_RXBuffLen++] = usart_data_receive(EVAL_COM);
		ch = usart_data_receive(EVAL_COM1);

		MODBUS_ASCII_HandlRevData(ch);
	
        if(UARTx_RXBuffLen >= (sizeof(UARTx_RXBuff) / sizeof(UARTx_RXBuff[0]))){
            usart_interrupt_disable(EVAL_COM1, USART_INT_RBNE);
        }

    }

    if(RESET != usart_interrupt_flag_get(EVAL_COM1, USART_INT_FLAG_TBE)){
        /* transmit data */
		usart_interrupt_flag_clear(EVAL_COM1, USART_INT_FLAG_TBE);
        /* transmit data */
        usart_data_transmit(EVAL_COM1, UARTx_TXBUFF[u8SendIndex++]);
		
        if(u8SendNum <= u8SendIndex){
			gpio_bit_reset(GPIOA,GPIO_PIN_6);
            usart_interrupt_disable(EVAL_COM1, USART_INT_TBE);
//			memset(UARTx_TXBUFF, 0, USART_MAX_DATALEN) ;
			u8SendNum = 0;
        }
    }
}


//******************************************************************************
// 名称               : UART1_SendData()
// 创建日期           : 2017-11-27
// 作者               : 梅梦醒
// 功能               : UART1发送数据函数
// 输入参数           : uint8_t *UART1_SendBuff 发送数据缓存
//                      uint32_t Len            发送数据长度        
// 输出参数           : 无
// 返回结果           : 无
// 注意和说明         : 
// 修改内容           :
//******************************************************************************
uint32_t UARTx_SendData(uint8_t* UARTx_SendBuff, uint32_t Len)
{
    uint32_t i = 0;

    if ((0 == Len) || (((uint8_t*)0) == UARTx_SendBuff))
        return 0;

    if (u8SendNum != 0)
    {
        return 0;
    }

    if (Len > (sizeof(UARTx_TXBUFF) / sizeof(UARTx_TXBUFF[0])))
    {
        Len = (sizeof(UARTx_TXBUFF) / sizeof(UARTx_TXBUFF[0]));
    }

    for (i = 0; i < Len; i++)
    {
        UARTx_TXBUFF[i] = UARTx_SendBuff[i];
    }

//    TX_ON;
	gpio_bit_set(GPIOA,GPIO_PIN_6);

    usart_data_transmit(USART1, UARTx_TXBUFF[0]);

    u8SendIndex = 1;
    u8SendNum = Len;
    usart_interrupt_enable(USART1, USART_INT_TBE);

    return(Len);
}


