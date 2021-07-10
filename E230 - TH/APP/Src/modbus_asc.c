
#include "modbus_asc.h"
#include "modbus_ascii.h"
#include "type.h"
#include "para.h"
#include "string.h"

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


uint8_t const SensorSoftVersion[10] = {0x09, 'S', 'V', '1', '.', '0', '.', '0', 'G', 'D'};      //�����汾  20200413

extern SHT3_Typedef T_H;
//**************************************************************************************************
// ����         	: MBASCII_GetSlaveAddr()
// ��������   	        : 2015-10-29
// ����         	: wang
// ����         	:����豸��ַ
// �������     	:���յĻ�������(uint8_t *uint8_tMsg)
// �������     	: �豸��ַ
// ���ؽ��     	: ��
// ע���˵��   	:
// �޸�����     	:
//**************************************************************************************************

uint32_t MBASC_GetSlaveAddr(uint8_t *uint8_tMsg)
{
    return(uint8_tMsg[0]);
}


//******************************************************************************
// ����         : MBASC_SendMsg()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : ��ASCII��ʽ������Ϣ
// �������     : ֡������(uint8_t *uint8_tMsg),֡��(uint8_t uint8_tMsgLen)
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��
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
// ����         : MBASC_SendMsg_NoLimit()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : �������Ե���ASCII��ʽ������Ϣ
// �������     : ֡������(uint8_t *uint8_tMsg),֡��(uint8_t uint8_tMsgLen)
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��
//******************************************************************************
void MBASC_SendMsg_NoLimit(uint8_t *uint8_tMsg, uint8_t uint8_tMsgLen)
{
    Delay_Ms(50);
    MODBUS_ASCII_SendData(uint8_tMsg, uint8_tMsgLen);
}

//******************************************************************************
// ����         : MBASC_SendErr()
// ��������     : 2016-06-08
// ����         : MMX
// ����         : ���ʹ��������Ӧ֡
// �������     : ������(uint8_t ErrCode)
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��
//******************************************************************************
void MBASC_SendErr(uint8_t ErrCode)
{
    SendLen = 1;
    SendBuf[SendLen++] |= 0x80;
    SendBuf[SendLen++] = ErrCode;
    MBASC_SendMsg(SendBuf, SendLen);
}

//**************************************************************************************************
// ??         : MBASC_Fun03()
// ????     : 2016-09-05
// ??         : ???
// ??         : 03???,????????
// ????     : ?
// ????     : ?
// ????     : ?
// ?????   : 
// ????     :  1.????????0x45,???????????0x45xx   2016.09.08
//                 2.???????,???????????,?:???0x46,?????0x4630????
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
// ??         : MBASC_Fun04()
// ????     : 2016-09-05
// ??         : ???
// ??         : 04???,????????
// ????     : ?
// ????     : ?
// ????     : ?
// ?????   : 
// ????     :  ????????????,????????????     2016.09.12
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
// ??         : MBASC_Fun10()
// ????     : 2016-09-05
// ??         : ???
// ??         : 10???,????????,??????
// ????     : ?
// ????     : ?
// ????     : ?
// ?????   : 
// ????     : 1.???????????,?????????,??????????????????
//                ??????????      2016.09.10
//                2.???????(??????????????)
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
                if(upload_persist < MBASC_AUTO_UPLOAD_NONE || upload_persist > MBASC_AUTO_UPLOAD_30S)
                {
                    break;
                }
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
                break; 
                
            default:
                break;
        }
        index += 2;
    }
    
    MBASC_SendMsg_NoLimit(UARTx_RXBuff, 6);
}





//**************************************************************************************************
// ??         : MBASC_Fun2B()
// ????     : 2020-04-17
// ??         : LHL
// ??         : ???????????
// ????     : ?
// ????     : ?
// ????     : ?
// ?????   : ???5?????????,??SendBuf???
// ????     :
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
            SoftwareVersionLen = 9;//I2C1_ReadByte(EEPROM_ADDRESS, SFVERSION);
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
// ??         : MBASC_Fun41()
// ????     : 2016-09-19
// ??         : ???
// ??         : ????(?boot????)
// ????     : ?
// ????     : ?
// ????     : ?
// ?????   : 
// ????     :
//**************************************************************************************************
void MBASC_Fun41(void)
{
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
        Flash_Write_MultiBytes(1023,"\x0C", 1);
        flash_read_MultiBytes(1023, &ReadData, 1);
        if(ReadData == 0x0C)
        {
            SendBuf[SendLen++] = 0x00;
            MBASC_SendMsg(SendBuf, SendLen);
            while(0 != u8SendNum);                                              //????????,?????,????????????
            NVIC_SystemReset();
        }
        else
        {
            SendBuf[SendLen++] = 0x01;
            MBASC_SendMsg(SendBuf, SendLen); 
        }
    } 
}     
     

//**************************************************************************************************
// ??         : MBASC_Function()
// ????     : 2016-09-05
// ??         : ???
// ??         : modbus ascii????
// ????     : ?
// ????     : ?
// ????     : ?
// ?????   : 
// ????     :  
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


