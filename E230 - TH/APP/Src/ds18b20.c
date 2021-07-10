#include "ds18b20.h"



/* ʹ��RT-Thread����ϵͳ,USING_RT_THREAD_OS��main.h�ж��� */    
#ifdef USING_RT_THREAD_OS
void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t delta;
    rt_uint32_t current_delay;

    /* �����ʱ������tick�� */
    us = us * (SysTick->LOAD/(2000000/RT_TICK_PER_SECOND));

    /* ��õ�ǰʱ�� */
    delta = SysTick->VAL;

    /* ѭ����õ�ǰʱ�䣬ֱ���ﵽָ����ʱ����˳�ѭ�� */
    do
    {
			if ( delta > SysTick->VAL )
				current_delay = delta - SysTick->VAL;
			else
        /* ��ʱ��Խ��һ��OS tick�ı߽�ʱ�Ĵ��� */
				current_delay = SysTick->LOAD + delta - SysTick->VAL;
    } while( current_delay < us );
}

#else
void delay_us(uint32_t tim)
{
	tim = tim*10;
	while(tim--);	
}


/**
 * ����         : delay_750us(void)
 * ��������     : 
 * ����         : 
 * ����         : 750΢����ʱ
 * �������     : ��
 * �������     : ��
 * ���ؽ��     : ��
 * ע���˵��   : ��
 * �޸�����     : �� 
 */
void delay_750us(void)
{
	uint32_t tim=16000;
	while(tim--);	
}

/**
 * ����         : delay_15us(void)
 * ��������     : 
 * ����         : 
 * ����         : 15΢����ʱ
 * �������     : ��
 * �������     : ��
 * ���ؽ��     : ��
 * ע���˵��   : ��
 * �޸�����     : �� 
 */
void delay_15us(void)
{
	uint32_t tim=310;
	while(tim--);	
}

/**
 * ����         : delay_12us(void)
 * ��������     : 
 * ����         : 
 * ����         : 12΢����ʱ
 * �������     : ��
 * �������     : ��
 * ���ؽ��     : ��
 * ע���˵��   : ��
 * �޸�����     : �� 
 */
void delay_12us(void)
{
	uint32_t tim=248;
	while(tim--);	
}


/**
 * ����         : delay_50us(void)
 * ��������     : 
 * ����         : 
 * ����         : 50΢����ʱ
 * �������     : ��
 * �������     : ��
 * ���ؽ��     : ��
 * ע���˵��   : ��
 * �޸�����     : �� 
 */
void delay_50us(void)
{
	uint32_t tim=1048;
	while(tim--);	
}

/**
 * ����         : delay_60us(void)
 * ��������     : 
 * ����         : 
 * ����         : 60΢����ʱ
 * �������     : ��
 * �������     : ��
 * ���ؽ��     : ��
 * ע���˵��   : ��
 * �޸�����     : �� 
 */
void delay_60us(void)
{
	uint32_t tim=1260;
	while(tim--);	
}
#endif


/**
* @brief        ds18b20�����������
* @retval       None
*/
void DS18B20_Output(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
		/* configure PA2(ADC channel2) as analog input */
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6);
	
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	
//	GPIO_InitTypeDef GPIO_Initure;
//    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
//	
//    GPIO_Initure.Pin=GPIO_PIN_1;           	//PA0
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          //����
//    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);     //��ʼ��
 
//	DS18B20_Rst();
//	return DS18B20_Check();
}

/**
* @brief        ds18b20������������
* @retval       None
*/
void DS18B20_Input(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
		/* configure PA2(ADC channel2) as analog input */
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT , GPIO_PUPD_NONE, GPIO_PIN_6);
	
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	
//	GPIO_InitTypeDef GPIO_Initure;
//    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
//	
//    GPIO_Initure.Pin=GPIO_PIN_1;           	//PA0
//    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          //����
//    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);     //��ʼ��
 

}

/**
* @brief       ��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ��� 
* @retval      None
*/
void DS18B20_Init(void)
{
	DS18B20_Output();
	
	DS18B20_Rst();
	DS18B20_Check();
}

//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_Output();   //����Ϊ���
	DS18B20_DQ_OUT_L;  	//����DQ
#ifdef USING_RT_THREAD_OS
	rt_hw_us_delay(750);      //����750us
#else
	delay_750us();
#endif
	
	DS18B20_DQ_OUT_H;  	//DQ=1 
#ifdef USING_RT_THREAD_OS
	rt_hw_us_delay(15);       //15US
#else
	delay_15us();
#endif
}

//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_Input();    //����Ϊ���� 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
#ifdef USING_RT_THREAD_OS
		rt_hw_us_delay(1);
#else
		delay_us(1);
#endif
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
#ifdef USING_RT_THREAD_OS
		rt_hw_us_delay(1);
#else
		delay_us(1);
#endif
	};
	if(retry>=240)return 1;	    
	return 0;
}

//��DS18B20��ȡһ��λ
//����ֵ��1/0
uint8_t DS18B20_Read_Bit(void) 
{
	uint8_t data;
	DS18B20_Output();   //����Ϊ���
	DS18B20_DQ_OUT_L; 
#ifdef USING_RT_THREAD_OS
	rt_hw_us_delay(2);
#else
	delay_us(2);
#endif
	DS18B20_DQ_OUT_H; 
	DS18B20_Input();    //����Ϊ����
#ifdef USING_RT_THREAD_OS
	rt_hw_us_delay(12);
#else
	delay_12us();
#endif
	if(DS18B20_DQ_IN)data=1;
	else data=0;	 
#ifdef USING_RT_THREAD_OS
	rt_hw_us_delay(50);
#else
	delay_50us();
#endif         
	return data;
}

//��DS18B20��ȡһ���ֽ�
//����ֵ������������
uint8_t DS18B20_Read_Byte(void)   
{        
	uint8_t i,j,dat;
	dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
	return dat;
}

//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(uint8_t dat)     
 {             
    uint8_t j;
    uint8_t testb;
    DS18B20_Output();     //����Ϊ���
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if(testb)       // д1
        {
            DS18B20_DQ_OUT_L;
#ifdef USING_RT_THREAD_OS
			rt_hw_us_delay(2);
#else
			delay_us(2);
#endif                         
            DS18B20_DQ_OUT_H;
#ifdef USING_RT_THREAD_OS
			rt_hw_us_delay(60);
#else
			delay_60us();
#endif            
        }
        else            //д0
        {
            DS18B20_DQ_OUT_L;
#ifdef USING_RT_THREAD_OS
			rt_hw_us_delay(60);
#else
			delay_60us();
#endif                
            DS18B20_DQ_OUT_H;
#ifdef USING_RT_THREAD_OS
			rt_hw_us_delay(2);
#else
			delay_us(2);
#endif                            
        }
    }
}
 
/**
* @brief		��ʼ�¶�ת��
* @retval		None
*/
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}

/**
* @brief		��ds18b20�õ��¶�ֵ
* @retval		�����ͣ��¶�ֵ ��-55~125�� 
*/
float DS18B20_Get_Temp(void)
{
//    uint8_t temp;
    uint8_t TL,TH;
    short tem;
	float f_tem;
    DS18B20_Start ();           //��ʼת��
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);   // skip rom
    DS18B20_Write_Byte(0xbe);   // convert	    
    TL=DS18B20_Read_Byte();     // LSB   
    TH=DS18B20_Read_Byte();     // MSB   
//    if(TH>7)
//    {
//        TH=~TH;
//        TL=~TL; 
//        temp=0;//�¶�Ϊ��  
//    }else temp=1;//�¶�Ϊ��	  	  
//    tem=TH; //��ø߰�λ
//    tem<<=8;    
//    tem+=TL;//��õװ�λ
//    tem=(double)tem*0.625;//ת��     
//	if(temp)return tem; //�����¶�ֵ
//	else return -tem;    

	tem = TH<<8;
	tem = tem | TL;
	if(tem < 0 )
		f_tem = (~tem+1) * 0.0625;
	else
		f_tem = tem * 0.0625;
	
	return f_tem;
}


