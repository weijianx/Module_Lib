
#include "modbus_asc.h"
#include "modbus_ascii.h"
#include "type.h"
#include "para.h"
#include "string.h"
#include "TH_IIC.h"
#include "SHT3x.h"


uint8_t StartFlag = 0;
uint8_t StartCountFlag = 0;
uint8_t SendLen;
uint8_t SendBuf[100];
extern uint8_t UARTx_RXBuff[USART_MAX_DATALEN];
extern UserTypeDef UserPara;
extern UserTypeDef status;
extern uint32_t time1;
extern uint8_t u8SendNum;
extern BitAction UartRecvFrameOK;
extern BitAction StartFillBufFlag;


extern uint8_t Cur_Param[USER_DEFAULT_LEN];
extern uint8_t const  User_Default_Param[USER_DEFAULT_LEN];


uint8_t const SensorSoftVersion[10] = {0x07, 'G', 'D', '1', '.', '0', '.', '0'};      //软件版本  20200413

extern SHT3_Typedef T_H;
//**************************************************************************************************
// 名称         	: MBASCII_GetSlaveAddr()
// 创建日期   	        : 2015-10-29
// 作者         	: wang
// 功能         	:获得设备地址
// 输入参数     	:接收的缓冲数据(uint8_t *uint8_tMsg)
// 输出参数     	: 设备地址
// 返回结果     	: 无
// 注意和说明   	:
// 修改内容     	:
//**************************************************************************************************

uint32_t MBASC_GetSlaveAddr(uint8_t *uint8_tMsg)
{
    return(uint8_tMsg[0]);
}


//******************************************************************************
// 名称         : MBASC_SendMsg()
// 创建日期     : 2018-06-08
// 作者         : MMX
// 功能         : 以ASCII格式发送消息
// 输入参数     : 帧缓存区(uint8_t *uint8_tMsg),帧长(uint8_t uint8_tMsgLen)
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无
//******************************************************************************
void MBASC_SendMsg(uint8_t *u8Msg, uint8_t u8MsgLen)
{
    if(MB_ADDRESS_BROADCAST != u8Msg[0])
    {
        Delay_Ms(50);
        MODBUS_ASCII_SendData1(u8Msg, u8MsgLen);
    }
}

//******************************************************************************
// 名称         : MBASC_SendMsg_NoLimit()
// 创建日期     : 2018-06-08
// 作者         : MMX
// 功能         : 无限制性的以ASCII格式发送消息
// 输入参数     : 帧缓存区(uint8_t *uint8_tMsg),帧长(uint8_t uint8_tMsgLen)
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无
//******************************************************************************
void MBASC_SendMsg_NoLimit(uint8_t *uint8_tMsg, uint8_t uint8_tMsgLen)
{
    Delay_Ms(50);
    MODBUS_ASCII_SendData(uint8_tMsg, uint8_tMsgLen);
}

//******************************************************************************
// 名称         : MBASC_SendErr()
// 创建日期     : 2016-06-08
// 作者         : MMX
// 功能         : 发送错误码的响应帧
// 输入参数     : 错误码(uint8_t ErrCode)
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无
//******************************************************************************
void MBASC_SendErr(uint8_t ErrCode)
{
    SendLen = 1;
    SendBuf[SendLen++] |= 0x80;
    SendBuf[SendLen++] = ErrCode;
    MBASC_SendMsg(SendBuf, SendLen);
}

//**************************************************************************************************
// 名称         : MBASC_Fun03()
// 创建日期     : 2016-09-05
// 作者         : 庄明群
// 功能         : 03功能码，读单个寄存器操作
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 
// 修改内容     :  1.将从机地址修改为0x45，对应的寄存器地址修改为0x45xx   2016.09.08
//                 2.修改从机地址后，对应的寄存地址可以变化，如：修改为0x46,则可以读取0x4630地址数据
//**************************************************************************************************
void MBASC_Fun03(void)
{
    uint8_t i;
         
    uint16_t Data_Buf;
    uint16_t Register_Num = 0;
    uint8_t ReadAdrH, ReadAdrL;
    
    ReadAdrH = UARTx_RXBuff[2];
    ReadAdrL = UARTx_RXBuff[3];

    Register_Num = ((uint16_t)UARTx_RXBuff[4] << 8) + UARTx_RXBuff[5];
     
    SendLen = 0;
    SendBuf[SendLen++] = MBASC_GetSlaveAddr(UARTx_RXBuff);
    SendBuf[SendLen++] = 0x03;	                        //???
    SendBuf[SendLen++] = Register_Num * 2;		                        //????
                                                                                //????????
    if ((!(((ReadAdrL >= MBASC_HOLDING_REG_REGION_BGEIN) && (ReadAdrL <= MBASC_HOLDING_REG_REGION_END)
        && (ReadAdrL + Register_Num <= (MBASC_HOLDING_REG_REGION_END + 1)))&& (0 != Register_Num)))
        || ((ReadAdrH != UserPara.SlaveAddr_Temp) && (ReadAdrH != UserPara.SlaveAddr_Humi) 
        && (ReadAdrH != MB_ADDRESS_BROADCAST)))
    {
        MBASC_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    for (uint8_t k = 0; k < Register_Num; ReadAdrL++, k++)
    {
        switch (ReadAdrL)
        {
          case 0x30:
            if(T_H.dev_num)
            {
                Data_Buf = UserPara.SlaveAddr_Humi;				        //????
            }
            else
            {
                Data_Buf = UserPara.SlaveAddr_Temp;				        //????
            }
            break;

          case 0x31:
            Data_Buf = UserPara.Baudrate;				        
            break;
            
          case 0x36:
            Data_Buf = T_H.Upload_persist[T_H.dev_num];     //??????
            break;
             
          case 0x40:
            Data_Buf =  T_H.UpThreshold[T_H.dev_num];//;	//???????	        
            break;

          case 0x41:                                                        
            Data_Buf = T_H.DoThreshold[T_H.dev_num]; //???????
            break;
            
          case 0x42:
            Data_Buf = T_H.Duration[T_H.dev_num]; //??????                                
            break; 
            
         
            
          default:
            Data_Buf = 0;
            break;
        }
        for (i = 2; i > 0; i--)
        {
            SendBuf[SendLen++] = (uint8_t)(Data_Buf >> ((i - 1) * 8));
        }
    }

      
    MBASC_SendMsg_NoLimit(SendBuf, SendLen);                                      
}


//**************************************************************************************************
// 名称         : MBASC_Fun04()
// 创建日期     : 2016-09-05
// 作者         : 庄明群
// 功能         : 04功能码，读双个寄存器操作
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 
// 修改内容     :  按照协议修改数据发送方式,发送浮点数据改为发送整形     2016.09.12
//**************************************************************************************************
void MBASC_Fun04(void)	
{
    uint8_t i;
    //uint32_t cnt = 0;
	uint8_t ReadAdr_L;
	
    uint16_t ReadAdr = 0;
    uint32_t Data_Buf;
    uint16_t Register_Num = 0 ;
    uint8_t SlaveAdd = UARTx_RXBuff[2];
    

	    ReadAdr_L = UARTx_RXBuff[3];
    
    Register_Num = (uint16_t)UARTx_RXBuff[4] * 256 + UARTx_RXBuff[5];

    SendLen = 0;
    
//    if(SlaveAdd >= MB_TEMP_DEV_MIN_ADDR && SlaveAdd <= MB_HUMI_DEV_MAX_ADDR)
//    {
        SendBuf[SendLen++] = MBASC_GetSlaveAddr(UARTx_RXBuff);
//    }
//    else
//    {
//        SendBuf[SendLen++] = MBASC_GetSlaveAddr_Humi(UART1_RXBuff) ? UserPara.SlaveAddr_Humi : 0x00;
//    }
    
    SendBuf[SendLen++] = MB_FUNC_READ_INPUT_REGISTER;
    SendBuf[SendLen++] = Register_Num * 2;		                        //????

    if(!((((ReadAdr_L <= MBASC_INPUT_REG_REGION1_END)
    && ((ReadAdr_L + Register_Num) <= (MBASC_INPUT_REG_REGION1_END + 2)))
    || ((ReadAdr_L >= MBASC_INPUT_REG_REGION2_BGEIN) && (ReadAdr_L <= MBASC_INPUT_REG_REGION2_END)
    && ((ReadAdr_L + Register_Num) <= (MBASC_INPUT_REG_REGION2_END + 2)))
    ||((ReadAdr_L >= MBASC_INPUT_REG_REGION3_BGEIN) && (ReadAdr_L <= MBASC_INPUT_REG_REGION3_END)
    && ((ReadAdr_L + Register_Num) <= (MBASC_INPUT_REG_REGION3_END + 2))))
    && ((0 != Register_Num) && (0 == (Register_Num & 0x01)) && (0 == (ReadAdr_L & 0x01)))))   
    {
        MBASC_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }
    
	
    for (uint8_t k = 0; k < Register_Num; ReadAdr_L += 2, k += 2)
    {
        switch (ReadAdr_L)
        {
          case 0x00:                                                         
            if(SlaveAdd >= 0x21 && SlaveAdd <= 0x25)
            {
                //Data_Buf = (uint32_t)T_H.Temp[SlaveAdd - 0x21];       
                Data_Buf = (uint32_t)T_H.Temp[0];   

            }
            else if(SlaveAdd >= 0x26 && SlaveAdd <= 0x29)
            {
                //Data_Buf = (uint32_t)T_H.Humi[SlaveAdd - 0x26];   
                Data_Buf = (uint32_t)T_H.Humi[0]; 
            }
            else
                Data_Buf = 0;
            
            break;

          case 0x02:                                                          
            if(SlaveAdd >= 0x21 && SlaveAdd <= 0x25)
               //Data_Buf = T_H.Duration[SlaveAdd - 0x21];
              Data_Buf = T_H.Duration[0];
            else if(SlaveAdd >= 0x26 && SlaveAdd <= 0x29)
                //Data_Buf = T_H.Duration[SlaveAdd + 0x01 - 0x26];
                Data_Buf = T_H.Duration[1];
            else
                Data_Buf = 0;	                                        
            break;

          case 0x04:                                                          
            if(SlaveAdd >= 0x21 && SlaveAdd <= 0x25)
             // Data_Buf = T_H.AlarmSta[SlaveAdd - 0x21]; 
              Data_Buf = T_H.AlarmSta[0];
                
            else if(SlaveAdd >= 0x26 && SlaveAdd <= 0x29)
                //Data_Buf = T_H.AlarmSta[SlaveAdd + 0x01 - 0x26];
                Data_Buf = T_H.AlarmSta[01];
            else
                Data_Buf = 0;		                                
            break;
            
          default:
            Data_Buf = 0;
            break;
        }
        for(i = 4; i > 0; i--)
        {
            SendBuf[SendLen++] = (uint8_t)(Data_Buf >> ((i - 1) * 8));
        }
    }
    

    MBASC_SendMsg_NoLimit(SendBuf, SendLen);
}




//**************************************************************************************************
// 名称         : MBASC_Fun10()
// 创建日期     : 2016-09-05
// 作者         : 庄明群
// 功能         : 10功能码，写多个寄存器操作，用于修改参数
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 
// 修改内容     : 1.先接收自动上传时间编码，再把编码转换成时间，解决发指令修改编码不重启后自动上传时
//                间不是想要的时间问题      2016.09.10
//                2.增加串口初始化（修改波特率后可以不重启单片机）
//**************************************************************************************************
void MBASC_Fun10()
{
    uint32_t index = 0;
    uint16_t Register_Num = 0;
    uint8_t ReadAdrH, ReadAdrL;
	
	uint16_t slave_addr;
	
	uint16_t upload_persist;
    
    ReadAdrH = UARTx_RXBuff[2];
    ReadAdrL = UARTx_RXBuff[3];
        
    Register_Num = ((uint16_t)UARTx_RXBuff[4] << 8) + UARTx_RXBuff[5]; 
    
    SendLen = 0;
//    SendBuf[SendLen++] = UARTx_RXBuff[0] ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = MBASC_GetSlaveAddr(UARTx_RXBuff);
	SendBuf[SendLen++] = 0x10;
    SendBuf[SendLen++] = Register_Num * 2;
                                                                                //????????
    if ((!((((ReadAdrL >= MBASC_MUL_REG_REGION_BGEIN) && (ReadAdrL <= MBASC_MUL_REG_REGION_END)
            && (ReadAdrL + Register_Num <= (MBASC_MUL_REG_REGION_END + 1)))
            || (ReadAdrL == 0x70))
            && ((0 != Register_Num)) && ((Register_Num * 2) == UARTx_RXBuff[6])))
            || (((ReadAdrH != UserPara.SlaveAddr_Temp) && (ReadAdrH != UserPara.SlaveAddr_Humi) ) && (ReadAdrH != MB_ADDRESS_BROADCAST)))
    {
        MBASC_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    for (uint8_t k = 0; k < Register_Num; ReadAdrL++, k++)
    {
        switch (ReadAdrL)
        {
            case 0x30:						                //????
//                UserPara.SlaveAddr = ((uint16_t)UARTx_RXBuff[7+index] << 8) + UARTx_RXBuff[8+index];
//                if(UserPara.SlaveAddr >= 33 && UserPara.SlaveAddr <= 37)
//                {
//                    Flash_Write_MultiBytes(SLAVE_ADDR_TEMP, &UserPara.SlaveAddr , 1);
//                }
				
				slave_addr = ((uint16_t)UARTx_RXBuff[7+index] << 8) + UARTx_RXBuff[8+index];                        
                if(slave_addr >= 0x21 && slave_addr <= 0x25)
                {
                    UserPara.SlaveAddr_Temp = slave_addr;
					Flash_Write_MultiBytes(SLAVE_ADDR_TEMP, &UserPara.SlaveAddr_Temp , 1);
                }
                else if(slave_addr >= 0x26 && slave_addr <= 0x29)
                {
                    UserPara.SlaveAddr_Humi = slave_addr;
					Flash_Write_MultiBytes(SLAVE_ADDR_HUMI, &UserPara.SlaveAddr_Humi , 1);
                }
                break;

            case 0x31:						
				UserPara.Baudrate = ((uint16_t)UARTx_RXBuff[7+index] << 8) + UARTx_RXBuff[8+index];

                Flash_Write_MultiBytes(Baudrate_ADDR, &UserPara.Baudrate , 1);
                	
                break;

            case 0x32:						             
                break;

            case 0x33:                                                             
                break;

            case 0x34:                                                              
                break;

            case 0x35:						                
                break;

            case 0x36:  
				upload_persist = ((uint16_t)UARTx_RXBuff[7+index] << 8) + UARTx_RXBuff[8+index];
//                if(upload_persist < MBASC_AUTO_UPLOAD_NONE || upload_persist > MBASC_AUTO_UPLOAD_30S)
//                {
//                    break;
//                }
                T_H.Upload_persist[T_H.dev_num] = upload_persist;
                Flash_Write_MultiBytes(TEM_UPLOAD_PERSIST + T_H.dev_num, &T_H.Upload_persist[T_H.dev_num], 1);
                break;

            case 0x37:                                                           
                break;

            case 0x38:
                break;

            case 0x39:						                
                break;

            case 0x3A:
                break; 

            case 0x3B:						                
                break;

            case 0x3C:						                
                break;

            case 0x3D:						             
                break;

            case 0x3E:						                
                break; 

            case 0x3F:						               
                break;       

            case 0x40:
                T_H.UpThreshold[0] = ((uint16_t)UARTx_RXBuff[7+index] << 8) + UARTx_RXBuff[8+index];
                Flash_Write_MultiBytes(TEM_UP_THR, (uint8_t*)&T_H.UpThreshold[0], 2);
                break; 

            case 0x41:
                T_H.DoThreshold[0] = ((uint16_t)UARTx_RXBuff[7+index] << 8) + UARTx_RXBuff[8+index];
                Flash_Write_MultiBytes(TEM_DO_THR, (uint8_t*)&T_H.DoThreshold[0], 2);
                break;

            case 0x42:
                T_H.Duration[0] = ((uint16_t)UARTx_RXBuff[7+index] << 8) + UARTx_RXBuff[8+index];
                Flash_Write_MultiBytes(TEM_DU_THR, (uint8_t*)&T_H.Duration[0], 2);
                break;                    

            case 0x43:						               
                break;  
                
            case 0x44:						               
                break;
                
            case 0x45:						               
                break;
                
            case 0x46:						               
                break;
                
            case 0x47:						               
                break;
                
            case 0x48:						               
                break;
                
            case 0x49:						               
                break;
                
            case 0x4A:						               
                break;
                
            case 0x4B:						               
                break;
                
            case 0x4C:						               
                break;
                
            case 0x4D:						               
                break;
                
            case 0x4E:						               
                break;
                
            case 0x4F:						               
                break;                

            case 0x70:		
				return ;
//                break; 
                
            default:
                break;
        }
        index += 2;
    }
    
    MBASC_SendMsg_NoLimit(UARTx_RXBuff, 6);
}





//**************************************************************************************************
// 名称         : MBASC_Fun2B()
// 创建日期     : 2020-04-17
// 作者         : LHL
// 功能         : 用于读取设备序列号信息
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 一起读5个对象时数据量很大，注意SendBuf的长度
// 修改内容     :
//**************************************************************************************************
void MBASC_Fun2B(void)
{
    uint16_t Object_Num = 0, ReadAdr = 0;
    uint8_t CompanyNameLen = 0;//ProductionCodeLen = 0, HardwareVersionLen = 0;
    uint8_t SoftwareVersionLen = 0;//DeviceIdLen = 0, CustomerCodeLen = 0;
    
//    if(UserPara.SlaveAddr != UARTx_RXBuff[2])                                   //????????0x45?,????????
//    {
//        ReadAdr = UARTx_RXBuff[2] * 256 + UARTx_RXBuff[3];
//    }
	if(UserPara.SlaveAddr_Humi != UARTx_RXBuff[2] && UserPara.SlaveAddr_Temp != UARTx_RXBuff[2])                                   //????????0x45?,????????
    {
        ReadAdr = UARTx_RXBuff[2] * 256 + UARTx_RXBuff[3];
    }
    else
    {
        ReadAdr = 0x51 * 256 + UARTx_RXBuff[3];
    }
    Object_Num = UARTx_RXBuff[4] * 256 + UARTx_RXBuff[5];                       //?????   
    
    SendLen = 0;
//    SendBuf[SendLen++] = (MBASC_GetSlaveAddr(UARTx_RXBuff)) ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = MBASC_GetSlaveAddr(UARTx_RXBuff);
	SendBuf[SendLen++] = 0x2B;
    SendBuf[SendLen++] = UARTx_RXBuff[4];
    SendBuf[SendLen++] = UARTx_RXBuff[5];                      
    
    if (!(((ReadAdr >= 0x51E0) && (ReadAdr <= 0x51E5) && (ReadAdr + Object_Num) <= (0x71E5 + 1))
        && (0 != Object_Num)))
    {
        MBASC_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }   
    
    for (uint8_t k = 0; k < Object_Num; ReadAdr++, k++)
    {
        switch (ReadAdr)
        {
        case 0x51E0:
            SendBuf[SendLen++] = 0xE0;
            //CompanyNameLen = I2C1_ReadByte(EEPROM_ADDRESS, COMPANY);            //?????????????
            if((0 == CompanyNameLen) || (0x32 < CompanyNameLen))                //?????
            {
                SendBuf[SendLen++] = 0x01;                                      //?????? 0x00                             
                SendBuf[SendLen++] = 0x00;
            }
            else
            {
                SendBuf[SendLen++] = CompanyNameLen;
            }
            break;
        
        case 0x51E1:
            SendBuf[SendLen++] = 0xE1;
  
            break;
            
        case 0x51E2:
            SendBuf[SendLen++] = 0xE2;
            break;
            
        case 0x51E3:
            SendBuf[SendLen++] = 0xE3;  //??????
            SoftwareVersionLen = 7;//I2C1_ReadByte(EEPROM_ADDRESS, SFVERSION);
            if((0 == SoftwareVersionLen) || (0x32 < SoftwareVersionLen))       
            {
                SendBuf[SendLen++] = 0x01;                                      //?????? 0x00                             
                SendBuf[SendLen++] = 0x00;
            } 
            else
            {
                SendBuf[SendLen++] = SoftwareVersionLen;
                for (uint8_t i = 0; i < SoftwareVersionLen; i++)
                {                             
                    SendBuf[SendLen++] = SensorSoftVersion[i+1];//(EEPROM_ADDRESS, SFVERSION + 1 + i);
                }
            }
            break;
            
        case 0x51E4:
            SendBuf[SendLen++] = 0xE4;
            break;
            
        case 0x51E5:
            SendBuf[SendLen++] = 0xE5;
          
           break;            
            
        default:
            break; 
        }
    }
    MBASC_SendMsg(SendBuf, SendLen);
}


                
//**************************************************************************************************
// 名称         : MBASC_Fun41()
// 创建日期     : 2016-09-19
// 作者         : 梅梦醒
// 功能         : 重启系统（从boot开始执行）
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 
// 修改内容     :
//**************************************************************************************************
void MBASC_Fun41(void)
{
	uint8_t buf[1]={0xFF};
    uint16_t ReadAdr = 0;
    uint16_t DataLength = 0;
    uint8_t ReadData;
      
    ReadAdr = ((uint16_t)(UARTx_RXBuff[2]<< 8)) + UARTx_RXBuff[3];
    DataLength = ((uint16_t)(UARTx_RXBuff[4]<< 8)) + UARTx_RXBuff[5];
        
    SendLen = 0;
//    SendBuf[SendLen++] = UARTx_RXBuff[0] ? UserPara.SlaveAddr : MB_ADDRESS_BROADCAST;
    SendBuf[SendLen++] = MBASC_GetSlaveAddr(UARTx_RXBuff);
	SendBuf[SendLen++] = 0x41;
    for(uint8_t i = 2; i < 6; i++)
    {
        SendBuf[SendLen++] = UARTx_RXBuff[i];
    }
                                                    //???????????boot
    if((0x0001 != ReadAdr) || (0 != DataLength) || (UARTx_RXBuff[0] == MB_ADDRESS_BROADCAST))
    {
        return;
    }
    else 
    {
	//	Flash_Write_OneByte(SOWAYFLASH_SRCPARA_ADDR,0xFF);
		Flash_Write_MultiBytes(RUN_ADDR_BASE, &buf, 1);
        SendBuf[SendLen++] = 0x00;
        MBASC_SendMsg(SendBuf, SendLen);
		Delay_1Ms(20);
        NVIC_SystemReset();

    } 
}     
     

//**************************************************************************************************
// 名称         : MBASC_Function()
// 创建日期     : 2016-09-05
// 作者         : 庄明群
// 功能         : modbus ascii通信处理
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 
// 修改内容     :  
//                                         
//**************************************************************************************************
void MBASC_Function(void)
{
    uint16_t RecvLen = 0;
//    if(UartRecvFrameOK == SET)
//    {
        if(2 == MODBUS_ASCII_RecvData(UARTx_RXBuff, &RecvLen))//????
        {
            return;
        }  
		
		if(UARTx_RXBuff[0] <= 0x29 && UARTx_RXBuff[0] >= 0x26)
        {
            T_H.dev_num = 1;
        }
        else
        {
            T_H.dev_num = 0;
        }
		
        if(RecvLen <= 0)
        {
            return;                                                                 //???????,???
        }

        else if((  (UserPara.SlaveAddr_Temp != UARTx_RXBuff[0]) && (UserPara.SlaveAddr_Humi != UARTx_RXBuff[0])  )&& (MB_ADDRESS_BROADCAST != UARTx_RXBuff[0]))
        {
            return;                                                                 //??????????,???
        }

        else
        {
            switch (UARTx_RXBuff[1])
            {
              case 0x03:
                MBASC_Fun03();	                                                //??????(?????)
                break;

              case 0x04:
                MBASC_Fun04();	                                                //??????(????)
                break;
                
              case 0x10:
                MBASC_Fun10();                                                  //??????
                break;    

              case 0x2B:
                MBASC_Fun2B();                                                  //???????????
                break;    
     
              case 0x41:
                MBASC_Fun41();
                break; 
                
              default:
                SendLen = 0;
                SendBuf[SendLen++] = UARTx_RXBuff[0];
                SendBuf[SendLen++] = 0x80 | UARTx_RXBuff[1];
                SendBuf[SendLen++] = MB_EX_ILLEGAL_FUNCTION;
                MBASC_SendMsg(SendBuf, SendLen);
                break;
             }
         }
//    }
}


void MBASC_AutoUpLoadFrame(void)
{
    uint8_t     i;
    uint8_t     j;
    uint32_t    Data_Buf[3];
    
    SendLen = 0;
    
    if(T_H.dev_num)
    {
        Data_Buf[0] = (uint32_t)T_H.Humi[0]; 
    }
    else
    {     
        Data_Buf[0] = (uint32_t)T_H.Temp[0];      
    }
    Data_Buf[1] = T_H.Duration[T_H.dev_num];                                     

    Data_Buf[2] = T_H.AlarmSta[T_H.dev_num];   
    
    for(j = 0; j < sizeof(Data_Buf) / sizeof(Data_Buf[0]); j++)
    {
        for(i = 4; i > 0; i--)
        {
            SendBuf[SendLen++] = (uint8_t)(Data_Buf[j] >> ((i - 1) * 8));
        }
    }
    MBASC_SendMsg_NoLimit(SendBuf, SendLen);
}



