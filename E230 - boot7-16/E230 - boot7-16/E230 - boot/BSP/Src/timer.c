#include "timer.h"
#include "type.h"



/*!
    \brief      TIMER configuration function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config(void)
{
   timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
	timer_ic_parameter_struct timer_icinitpara;

    rcu_periph_clock_enable(RCU_TIMER0);
	rcu_periph_clock_enable(RCU_GPIOA);
//	rcu_periph_clock_enable(RCU_AF);
	
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_8 | GPIO_PIN_9);
	gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_8 | GPIO_PIN_9);
//	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_9);
	
	timer_deinit(TIMER0);

    /* TIMER configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 65535;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0,&timer_initpara);
	

	timer_channel_input_struct_para_init(&timer_icinitpara);    //将输入捕获结构体参数变为初始值
	timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_RISING;    //通道输入极性
	timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI; //通道输入模式选择
	timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;           //通道输入预分频器
	timer_icinitpara.icfilter    = 0x00;                         //通道输入捕获滤波
	timer_input_capture_config(TIMER0, TIMER_CH_0, &timer_icinitpara);
	

	timer_channel_input_struct_para_init(&timer_icinitpara);    //将输入捕获结构体参数变为初始值
	timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_FALLING;    //通道输入极性
	timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI; //通道输入模式选择
	timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;           //通道输入预分频器
	timer_icinitpara.icfilter    = 0x00;                         //通道输入捕获滤波
	timer_input_capture_config(TIMER0, TIMER_CH_1, &timer_icinitpara);
	
	
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
//    timer_primary_output_config(TIMER0, ENABLE);
	timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_CH0);
	timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_CH1);
	
	timer_interrupt_enable(TIMER0, TIMER_INT_FLAG_CH0);
	timer_interrupt_enable(TIMER0, TIMER_INT_FLAG_CH1);
	
	nvic_irq_enable(TIMER0_Channel_IRQn, 2);
	

	timer_enable(TIMER0);
	
}


void TIMER2_Init(void)
{
	timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER2);

    timer_deinit(TIMER2);

    /* TIMER configuration */
    timer_initpara.prescaler         = 7199;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 499;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

	nvic_irq_enable(TIMER2_IRQn, 2);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
//    timer_primary_output_config(TIMER0, ENABLE);
	
	timer_interrupt_disable(TIMER2, TIMER_INT_UP);
	timer_disable(TIMER2);

	
}

uint16_t tim_cnt = 0;   //总时间累加
//uint16_t tim_1min_cnt = 0;   //1min时间累加

//uint8_t Time_5s_flag = 0;
//uint8_t Time_1s_flag = 0;


void TIMER2_IRQHandler(void)
{
//	uint16_t read_pulse = 0;  //当前脉冲数
//	
//	uint8_t ch[] = "awegf\r\n";
	if(timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP))
	{
		timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
		tim_cnt++;

//		UARTx_SendData(ch, sizeof(ch));

	}


}

void TIMDelay_N50ms(uint16_t times)
{
	timer_interrupt_enable(TIMER2, TIMER_INT_UP);
	timer_enable(TIMER2);

	while(1)
	{
		if(times == tim_cnt)
		{	
			tim_cnt = 0;
		
			break;
		}
	
	}
	
	timer_interrupt_disable(TIMER2, TIMER_INT_UP);
	timer_disable(TIMER2);
}





