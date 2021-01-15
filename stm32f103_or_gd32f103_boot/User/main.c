/**@file        main.c
* @brief        ϵͳ����APP����
* @details      ������STM32F10x��GD32F10xϵ�е�Ƭ��
* @author       Τ����
* @date         2021-01-10
* @version      V1.0.0
* @copyright    2021-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2021/01/10  <td>1.0.0    <td>Τ����    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

#include "main.h"	
#include "stm32f10x_gpio.h"
#include "BSP.h"
#include "Flash_app.h"
#include "common.h"


uint8_t OneFrameOk;                                      //���ݽ�����ɱ�־
uint8_t SlaveAddr;                                       //�ӻ���ַ
uint8_t UpgradeInfo;                                     //������Ϣ
uint8_t ProgErase;                                       //���������־
uint8_t UpgradeWaitTime;                                 //�����ȴ�ʱ��

pFunction Jump_To_Application;                           //��ת��Ӧ�ó���
uint32_t JumpAddress;                                    //��ת��ַ


uint8_t SendLen;                                                                //���ڷ��ͳ���
uint16_t PacketCnt;                                                             //�����
uint16_t PacketNum;                                                             //�ܰ���

uint32_t Flashadrdst;                                                           //FLASH��ַ
uint32_t FileCheckSum;                                                          //�����ļ�У���
uint32_t FileRunCheckSum;                                                       //�����ļ�ʵʱУ���

uint8_t SendBuf[50];                                                            //���ڷ�����ʱ����
extern uint8_t u8SendNum;
//extern uint8_t UART1_RXBuff[USART1_MAX_DATALEN];//�������ݻ���
//extern uint8_t UART1_TXBUFF[USART1_MAX_DATALEN];//�������ݻ���

FlashPara_Typedef	 FlashPara = {0};
void SystemClock_Config(void);


/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
	uint32_t delay;
	
    delay = count;

    while(delay --){
    }
}


/**@brief       ��λ������APP������
* @para			None
* @return       None
*/
void HandleFrameInformation(void)
{
    uint8_t fmerror;                                                              //֡����
    uint8_t tmpaddr;                                                              //�ݴ��ַ
    uint16_t rcvlen;                                                              //�������ݳ���
    uint16_t prt;                                                                 //����
    uint16_t msgid;                                                               //��ϢID
    uint16_t datalen;                                                             //���ݳ���
    uint16_t packetnum;                                                           //�ܰ���
    uint16_t packetcnt;                                                           //�����
    uint32_t tpcksum;                                                             //��У���
    uint16_t *ramdata;                                                            //����ָ��

    fmerror = MODBUS_ASCII_RecvData(UART1_RXBuff, &rcvlen);                       //�������ݴ����򲻷���

    if((0x41 != UART1_RXBuff[1])||(0x00 == UART1_RXBuff[0]))                      //����������򲻷���
    {
        return ;
    }

    tmpaddr = UART1_RXBuff[0];                                                               //��ȡ�ӻ���ַ
    msgid = UART1_RXBuff[2]*256 + UART1_RXBuff[3];                                           //��ȡ��ϢID
    datalen = UART1_RXBuff[4]*256 + UART1_RXBuff[5];                                         //��ȡ���ݳ���
    packetnum = UART1_RXBuff[6]*256 + UART1_RXBuff[7];                                       //��ȡ�ܰ���
    packetcnt = UART1_RXBuff[8]*256 + UART1_RXBuff[9];                                       //��ȡ�����

    if((rcvlen - 0x06) != datalen)                                                          //���ݳ��Ȳ���ȷ�򲻷���
    {
        return ;
    }

    if((SlaveAddr != UART1_RXBuff[0])&&(0xFF != UART1_RXBuff[0]))                          //�ӻ���ַ��������Χ������
    {
        return ;
    }

    if((0 != UpgradeWaitTime)&&(0x0004 != msgid)&&(0x0001 != msgid))                         //����������Ϣ�йز�����
    {
        return ;
    }

    for(SendLen = 0; SendLen < 4; SendLen++)                                                 //�洢��������Ϣ
    {
        SendBuf[SendLen] = UART1_RXBuff[SendLen];
    }

    SendBuf[SendLen++] = 0x00;
    SendBuf[SendLen++] = 0x01;
    if(3 == fmerror)
    {
        switch(msgid)
        {
            case 0x0001:                                                        //��ʼ����
                UpgradeWaitTime = 0;
			    DIS_INT;
			    Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));						// ��Flash��ǰ��������
                FlashPara.Common.UpgradeFlag=0xFF;
		        FLASH_If_Erase(FLASH_BASE_ADDR);										// Flashд����֮ǰ �����Ȳ���
		        Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);       // ����ǰ����д��Flash��ǰ������	
		        EN_INT;																	// ���ж�ʧ�ܵ�ʹ�ܱ���Ӳ�����ʼ��д��ɹ�
                SendBuf[SendLen++] = 0x00;
                if((0 != datalen)&&(2 != datalen))
                {
                    SendBuf[SendLen - 1] = 0x01;
                    break;
                }
                prt = 0;
                PacketNum = 0;
                PacketCnt = 0;
                while(2048 > packetnum)//&&(packetnum))
                {
                    packetnum--;

//                    if(2046 != packetnum)
//                    {
//                        Eeprom_WriteByte(prt++, 0xFF);
//                    }
//                    else
//                    {
//                        if((0x0C == ProgErase) || (0xC0 == ProgErase))
//                        {
//                            Eeprom_WriteByte(2046, ProgErase);
//                        }
//                    }
					  if(2046 != packetnum)
					  {
//						    prt++;
							DIS_INT;
							Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));		// ��Flash��ǰ��������
							FlashPara.Common.UpgradeFlag=0xFF;                                                              //��ȡ������Ϣ
	                        SlaveAddr=FlashPara.Common.Addr=0xFF;
							FLASH_If_Erase(FLASH_BASE_ADDR);										// Flashд����֮ǰ �����Ȳ���
							Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);      // ����ǰ����д��Flash��ǰ������	
							EN_INT;
					  }
					  else
					  {
                         if((0x0C == ProgErase) || (0xC0 == ProgErase))
                         {
							DIS_INT;
							Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));		// ��Flash��ǰ��������
							FlashPara.Common.ProgEraseFlag=ProgErase;
							FLASH_If_Erase(FLASH_BASE_ADDR);										// Flashд����֮ǰ �����Ȳ���
							Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);       // ����ǰ����д��Flash��ǰ������	
							EN_INT;	
                         } 
					  }
                }
                break;

            case 0x0002:                                                        //���Դ����
                if(0 != datalen)
                {
                    SendBuf[SendLen++] = 0x01;
                    break;
                }
                ProgErase = 0x0C;
//                Eeprom_WriteByte(2046, ProgErase);
				DIS_INT;
				Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));		// ��Flash��ǰ��������
				FlashPara.Common.ProgEraseFlag=ProgErase;
				FLASH_If_Erase(FLASH_BASE_ADDR);										// Flashд����֮ǰ �����Ȳ���
				Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);       // ����ǰ����д��Flash��ǰ������	
				EN_INT;	
                DIS_INT;				
//                HAL_FLASH_Unlock();
                FLASH_If_Erase(APPLICATION_ADDRESS);  
                EN_INT;				
                Flashadrdst = APPLICATION_ADDRESS;
                SendBuf[SendLen++] = 0x00;
                break;

            case 0x0003:                                                        //���������ļ�
                if((0 == PacketNum) && (1 < packetnum) && (0 == packetcnt))
                {
                    FileCheckSum = 0;
                    FileRunCheckSum = 0;
                    PacketNum = packetnum;
                    PacketCnt = packetcnt;
                    SendBuf[SendLen++] = 0x00;

                    for(prt = 0; prt < 4; prt++)
                    {
                        FileCheckSum <<= 8;
                        FileCheckSum += UART1_RXBuff[10 + prt];
                    }
                }
                else if((PacketNum == packetnum) && (1 < packetnum) && (PacketCnt == (packetcnt - 1)) && (PacketNum > packetcnt))
                {
                    tpcksum = 0;
                    datalen = datalen - 4;

                    for(prt = 0; prt < datalen; prt++)
                    {
                    tpcksum += UART1_RXBuff[10 + prt];
                    }

                    Decoding(&UART1_RXBuff[10], datalen);

                    for(prt = 0; prt < datalen; prt++)
                    {
                        UART1_RXBuff[prt] = UART1_RXBuff[10 + prt];
                    }
                    ramdata = (uint16_t*)UART1_RXBuff;

                    if(Flash_Write_HalfWord(Flashadrdst, ramdata, datalen/2)  == 0)
                    {
						Flashadrdst+=datalen;
                        PacketCnt++;
                        SendBuf[SendLen++] = 0x00;
                        FileRunCheckSum += tpcksum;
                    }
                    else
                    {
                        SendBuf[SendLen++] = 0x01;
                    }
                }
                else if((PacketNum == packetnum)&&(1 < packetnum)&&(PacketCnt == packetcnt)&&(PacketNum > packetcnt))
                {
                    SendBuf[SendLen++] = 0x00;
                }
                else
                {
                    SendBuf[SendLen++] = 0x01;
                }
                break;

            case 0x0004:                                                                                  //ִ��Ӧ�ó���
                if(((FileRunCheckSum == FileCheckSum)&&((PacketCnt + 1) == PacketNum)||(0 == PacketNum))&&(0 == datalen))
                {
                    if(0 != PacketNum)
                    {
                        UpgradeWaitTime = 10;
                        SendBuf[SendLen++] = 0x00;
                    }
                    else if((0x0C == ProgErase)||(0xFF == ProgErase))
                    {
                        SendBuf[SendLen++] = 0x01;
                    }
                    else
                    {
                        UpgradeWaitTime = 10;
                        SendBuf[SendLen++] = 0x00;
                    }
                }
                else
                {
                    SendBuf[SendLen++] = 0x02;
                }
                break;

                default:
                    SendBuf[SendLen++] = 0x01;
                    break;
        }
    }
    else
    {
        SendBuf[SendLen++] = 0x02;                                                                     //У�����
    }
    if((0x00 != tmpaddr) && (SlaveAddr == tmpaddr) || (0xFF == tmpaddr))
    {
        if(0x0004 == msgid)
        {
            TimingDelay_SetTime(50);//�ӳ�Ӧ��
            while(!TimingDelay_TimeOut());
        }
        MODBUS_ASCII_SendData(SendBuf, SendLen);

		
    }
    if(0x0004 == msgid)
    {
        TimingDelay_SetTime(100);

        while(!TimingDelay_TimeOut());
    }
}


void JumpToUserApplication(void)
{
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
    
    /* Jump to user application */
    JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
    Jump_To_Application = (pFunction) JumpAddress;

    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
	

    /* Jump to application */
    Jump_To_Application();
}



/**@brief       ϵͳ��ʼ��
* @para			None
* @return       None
*/
int main(void)
{
	u8 ch[] = "ddgdssgsjjt\r\n";
		uint8_t u[] = {0x12, 0x13, 0x14, 0x15, 0x16, 0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,0x31,
				   0x13, 0x14, 0x15, 0x16, 0x17, 0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x35,
				   0x13, 0x14, 0x15, 0x16, 0x17, 0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x35,
				   0x13, 0x14, 0x15, 0x16, 0x17, 0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x35,
				};
	
	Target_Init();	//ϵͳ����

//	rs485_Init();
	UART1_Init();	//���ڳ�ʼ��
	
	DIS_INT;
	Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));						// ��Flash��ǰ��������
	EN_INT;
	UpgradeInfo=FlashPara.Common.UpgradeFlag;                                                              //��ȡ������Ϣ
	SlaveAddr=FlashPara.Common.Addr;
    ProgErase=FlashPara.Common.ProgEraseFlag;
    if((0x0C == UpgradeInfo) || (0xFF == UpgradeInfo) || (0x0C == ProgErase))   //������������������޵ȴ�
    {
        UpgradeWaitTime = 0;
    }
    else                                                                        //����ȴ�2��
    {
        UpgradeWaitTime = 2;
    }

    TimingDelay_SetTime(1000);

    while(1)
    {
        if(TimingDelay_TimeOut() && UpgradeWaitTime && (10 > UpgradeWaitTime))  //�ж��Ƿ����޵ȴ�
        {
            if(1 == UpgradeWaitTime)
            {
                break;
            }
            UpgradeWaitTime--;
            TimingDelay_SetTime(1000);
        }
        if(10 == UpgradeWaitTime)                                                   //�����������
        {
            break;
        }
        if(0x0C == OneFrameOk)                                                      //�������������                                                      
        {
            OneFrameOk = 0;
            HandleFrameInformation();                                                 //������յ�֡����
			
        }
    }
    if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
    {
        //uprintf("APP error!\r\n");
        
		DIS_INT;
		Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));		// ��Flash��ǰ��������
        FlashPara.Common.UpgradeFlag=0x00;
		Flash_erase_apage(FLASH_BASE_ADDR);										// Flashд����֮ǰ �����Ȳ���
		Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);       // ����ǰ����д��Flash��ǰ������	
		EN_INT;																	// ���ж�ʧ�ܵ�ʹ�ܱ���Ӳ�����ʼ��д��ɹ�
        
		DIS_INT;
		Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));		// ��Flash��ǰ��������
        FlashPara.Common.ProgEraseFlag=0xC0;
		Flash_erase_apage(FLASH_BASE_ADDR);										// Flashд����֮ǰ �����Ȳ���
		Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);       // ����ǰ����д��Flash��ǰ������	
		EN_INT;																	// ���ж�ʧ�ܵ�ʹ�ܱ���Ӳ�����ʼ��д��ɹ�
        JumpToUserApplication();                                                    //ִ��Ӧ�ó���
    }
	
	    
	while(1)
    {
//		FLASH_If_Erase(APPLICATION_ADDRESS);
//		Flash_Write_HalfWord(APPLICATION_ADDRESS, (uint16_t*)&u, sizeof(u)>>1);
//		Flash_Read_MultiBytes(APPLICATION_ADDRESS, (uint8_t *)&FlashPara, sizeof(FlashPara));
//		TX_ON; 
////		delay_1ms(100);
//		UART1_SendData(ch, sizeof(ch));
		
//        //uprintf("No APP!\r\n");
    }
	
}

/*������ʼ��*/
void Target_Init(void)
{
	RCC_ClocksTypeDef Rcc_clocks;						//ϵͳʱ�ӱ���
	RCC_GetClocksFreq(&Rcc_clocks); 				//��ȡʱ������״̬
	SysTick_Config(Rcc_clocks.HCLK_Frequency/TICKS_PER_SEC);	//����ϵͳʱ��,��ʹ���ж�						
}
///*ϵͳʱ���жϷ���*/
///*ʱ����TICKS_PER_SEC��������*/
//void SysTick_Handler(void)
//{
//	static uint32_t index=0;
//	index++;
//}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
