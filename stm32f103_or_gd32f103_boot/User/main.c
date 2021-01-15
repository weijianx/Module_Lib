/**@file        main.c
* @brief        系统处理，APP升级
* @details      适用于STM32F10x、GD32F10x系列单片机
* @author       韦剑祥
* @date         2021-01-10
* @version      V1.0.0
* @copyright    2021-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2021/01/10  <td>1.0.0    <td>韦剑祥    <td>创建初始版本
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


uint8_t OneFrameOk;                                      //数据接收完成标志
uint8_t SlaveAddr;                                       //从机地址
uint8_t UpgradeInfo;                                     //升级信息
uint8_t ProgErase;                                       //程序擦除标志
uint8_t UpgradeWaitTime;                                 //升级等待时间

pFunction Jump_To_Application;                           //跳转到应用程序
uint32_t JumpAddress;                                    //跳转地址


uint8_t SendLen;                                                                //串口发送长度
uint16_t PacketCnt;                                                             //包序号
uint16_t PacketNum;                                                             //总包数

uint32_t Flashadrdst;                                                           //FLASH地址
uint32_t FileCheckSum;                                                          //升级文件校验和
uint32_t FileRunCheckSum;                                                       //升级文件实时校验和

uint8_t SendBuf[50];                                                            //串口发送临时缓存
extern uint8_t u8SendNum;
//extern uint8_t UART1_RXBuff[USART1_MAX_DATALEN];//接收数据缓存
//extern uint8_t UART1_TXBUFF[USART1_MAX_DATALEN];//发送数据缓存

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


/**@brief       上位机升级APP处理函数
* @para			None
* @return       None
*/
void HandleFrameInformation(void)
{
    uint8_t fmerror;                                                              //帧错误
    uint8_t tmpaddr;                                                              //暂存地址
    uint16_t rcvlen;                                                              //接收数据长度
    uint16_t prt;                                                                 //计数
    uint16_t msgid;                                                               //消息ID
    uint16_t datalen;                                                             //数据长度
    uint16_t packetnum;                                                           //总包数
    uint16_t packetcnt;                                                           //包序号
    uint32_t tpcksum;                                                             //包校验和
    uint16_t *ramdata;                                                            //数据指针

    fmerror = MODBUS_ASCII_RecvData(UART1_RXBuff, &rcvlen);                       //接收数据错误则不返回

    if((0x41 != UART1_RXBuff[1])||(0x00 == UART1_RXBuff[0]))                      //功能码错误则不返回
    {
        return ;
    }

    tmpaddr = UART1_RXBuff[0];                                                               //获取从机地址
    msgid = UART1_RXBuff[2]*256 + UART1_RXBuff[3];                                           //获取消息ID
    datalen = UART1_RXBuff[4]*256 + UART1_RXBuff[5];                                         //获取数据长度
    packetnum = UART1_RXBuff[6]*256 + UART1_RXBuff[7];                                       //获取总包数
    packetcnt = UART1_RXBuff[8]*256 + UART1_RXBuff[9];                                       //获取包序号

    if((rcvlen - 0x06) != datalen)                                                          //数据长度不正确则不返回
    {
        return ;
    }

    if((SlaveAddr != UART1_RXBuff[0])&&(0xFF != UART1_RXBuff[0]))                          //从机地址不在允许范围不处理
    {
        return ;
    }

    if((0 != UpgradeWaitTime)&&(0x0004 != msgid)&&(0x0001 != msgid))                         //不是升级信息有关不处理
    {
        return ;
    }

    for(SendLen = 0; SendLen < 4; SendLen++)                                                 //存储待发送信息
    {
        SendBuf[SendLen] = UART1_RXBuff[SendLen];
    }

    SendBuf[SendLen++] = 0x00;
    SendBuf[SendLen++] = 0x01;
    if(3 == fmerror)
    {
        switch(msgid)
        {
            case 0x0001:                                                        //开始升级
                UpgradeWaitTime = 0;
			    DIS_INT;
			    Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));						// 将Flash当前参数读出
                FlashPara.Common.UpgradeFlag=0xFF;
		        FLASH_If_Erase(FLASH_BASE_ADDR);										// Flash写数据之前 必须先擦除
		        Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);       // 将当前数据写入Flash当前参数区	
		        EN_INT;																	// 总中断失能到使能必须从擦除开始到写入成功
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
							Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));		// 将Flash当前参数读出
							FlashPara.Common.UpgradeFlag=0xFF;                                                              //读取升级信息
	                        SlaveAddr=FlashPara.Common.Addr=0xFF;
							FLASH_If_Erase(FLASH_BASE_ADDR);										// Flash写数据之前 必须先擦除
							Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);      // 将当前数据写入Flash当前参数区	
							EN_INT;
					  }
					  else
					  {
                         if((0x0C == ProgErase) || (0xC0 == ProgErase))
                         {
							DIS_INT;
							Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));		// 将Flash当前参数读出
							FlashPara.Common.ProgEraseFlag=ProgErase;
							FLASH_If_Erase(FLASH_BASE_ADDR);										// Flash写数据之前 必须先擦除
							Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);       // 将当前数据写入Flash当前参数区	
							EN_INT;	
                         } 
					  }
                }
                break;

            case 0x0002:                                                        //清除源程序
                if(0 != datalen)
                {
                    SendBuf[SendLen++] = 0x01;
                    break;
                }
                ProgErase = 0x0C;
//                Eeprom_WriteByte(2046, ProgErase);
				DIS_INT;
				Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));		// 将Flash当前参数读出
				FlashPara.Common.ProgEraseFlag=ProgErase;
				FLASH_If_Erase(FLASH_BASE_ADDR);										// Flash写数据之前 必须先擦除
				Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);       // 将当前数据写入Flash当前参数区	
				EN_INT;	
                DIS_INT;				
//                HAL_FLASH_Unlock();
                FLASH_If_Erase(APPLICATION_ADDRESS);  
                EN_INT;				
                Flashadrdst = APPLICATION_ADDRESS;
                SendBuf[SendLen++] = 0x00;
                break;

            case 0x0003:                                                        //传输升级文件
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

            case 0x0004:                                                                                  //执行应用程序
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
        SendBuf[SendLen++] = 0x02;                                                                     //校验错误
    }
    if((0x00 != tmpaddr) && (SlaveAddr == tmpaddr) || (0xFF == tmpaddr))
    {
        if(0x0004 == msgid)
        {
            TimingDelay_SetTime(50);//延迟应答
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



/**@brief       系统初始化
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
	
	Target_Init();	//系统配置

//	rs485_Init();
	UART1_Init();	//串口初始化
	
	DIS_INT;
	Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));						// 将Flash当前参数读出
	EN_INT;
	UpgradeInfo=FlashPara.Common.UpgradeFlag;                                                              //读取升级信息
	SlaveAddr=FlashPara.Common.Addr;
    ProgErase=FlashPara.Common.ProgEraseFlag;
    if((0x0C == UpgradeInfo) || (0xFF == UpgradeInfo) || (0x0C == ProgErase))   //如果有升级请求则无限等待
    {
        UpgradeWaitTime = 0;
    }
    else                                                                        //否则等待2秒
    {
        UpgradeWaitTime = 2;
    }

    TimingDelay_SetTime(1000);

    while(1)
    {
        if(TimingDelay_TimeOut() && UpgradeWaitTime && (10 > UpgradeWaitTime))  //判断是否无限等待
        {
            if(1 == UpgradeWaitTime)
            {
                break;
            }
            UpgradeWaitTime--;
            TimingDelay_SetTime(1000);
        }
        if(10 == UpgradeWaitTime)                                                   //升级完成跳出
        {
            break;
        }
        if(0x0C == OneFrameOk)                                                      //如果有数据则处理                                                      
        {
            OneFrameOk = 0;
            HandleFrameInformation();                                                 //处理接收的帧数据
			
        }
    }
    if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
    {
        //uprintf("APP error!\r\n");
        
		DIS_INT;
		Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));		// 将Flash当前参数读出
        FlashPara.Common.UpgradeFlag=0x00;
		Flash_erase_apage(FLASH_BASE_ADDR);										// Flash写数据之前 必须先擦除
		Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);       // 将当前数据写入Flash当前参数区	
		EN_INT;																	// 总中断失能到使能必须从擦除开始到写入成功
        
		DIS_INT;
		Flash_Read_MultiBytes(FLASH_BASE_ADDR, (uint8_t *)&FlashPara, sizeof(FlashPara));		// 将Flash当前参数读出
        FlashPara.Common.ProgEraseFlag=0xC0;
		Flash_erase_apage(FLASH_BASE_ADDR);										// Flash写数据之前 必须先擦除
		Flash_Write_HalfWord(FLASH_BASE_ADDR, (uint16_t *)&FlashPara, sizeof(FlashPara)>>1);       // 将当前数据写入Flash当前参数区	
		EN_INT;																	// 总中断失能到使能必须从擦除开始到写入成功
        JumpToUserApplication();                                                    //执行应用程序
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

/*启动初始化*/
void Target_Init(void)
{
	RCC_ClocksTypeDef Rcc_clocks;						//系统时钟变量
	RCC_GetClocksFreq(&Rcc_clocks); 				//获取时钟配置状态
	SysTick_Config(Rcc_clocks.HCLK_Frequency/TICKS_PER_SEC);	//配置系统时钟,并使能中断						
}
///*系统时钟中断服务*/
///*时间由TICKS_PER_SEC参数配置*/
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
