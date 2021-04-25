/**@file        Modbus.c
* @brief        Modbus 通信协议
* @details      标定功能、采集数据交互
* @author       庄明群
* @date         2021-04-07
* @version      V1.0.0
* @copyright    2020-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2021/04/07  <td>1.0.0    <td>庄明群  <td>韦剑祥      <td>删除了自动上传线程, 
* 使用 ModBus 处理线程处理消息帧和自动上传
* </table>
*
**********************************************************************************
*/

#include "ModBus.h"
#include "type.h"

#include "eeprom.h"
#include "para.h"
#include "main.h"


uint8_t RcvDataBuf[64];
uint8_t SendBuf[DATAPACKET_LEN];   //存储从机响应消息帧
uint8_t SendLen;

extern UserTypeDef UserPara;
//extern uint32_t sta;


//CRC_16正规查询表
/* CRC低位字节值表*/
const uint8_t auchCRCLo[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* CRC高位字节值表*/
const uint8_t auchCRCHi[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};


uint16_t MBRTU_GetCRC16(uint8_t *uint8_tMsg, uint32_t uint8_tMsgLen);



//////////////////////////////////////////////////////////////////////////////////////////////
//description描述：MBRTU_SendMsg() 发送数据帧 modbus RTU模式
//input输入      ：
//output输出     ：
//////////////////////////////////////////////////////////////////////////////////////////////

void MBRTU_SendMsg(uint8_t *uint8_tMsg, uint32_t uint8_tMsgLen)
{
    if(BROADCASTADDR != RcvDataBuf[0])
    {
        uint16_t uint16_tCRC = MBRTU_GetCRC16( uint8_tMsg, uint8_tMsgLen );
        uint8_tMsg[uint8_tMsgLen] = ( uint8_t )( uint16_tCRC & 0x00FF );         //低位在前
        uint8_tMsg[uint8_tMsgLen+1] = ( uint8_t )(( uint16_tCRC >> 8 ) & 0x00FF ); //高位在后
        UART1_SendData( uint8_tMsg, uint8_tMsgLen + 2 );       //发送数据
    }
}

void MBRTU_SendMsg_NoLimit(uint8_t *uint8_tMsg, uint32_t uint8_tMsgLen)
{
//    if (UartComm.Echo == ECHO_ENABLE)
//    {
        uint16_t uint16_tCRC = MBRTU_GetCRC16(uint8_tMsg, uint8_tMsgLen);
        uint8_tMsg[uint8_tMsgLen] = (uint8_t)(uint16_tCRC & 0x00FF);             //低位在前
        uint8_tMsg[uint8_tMsgLen+1] = (uint8_t)((uint16_tCRC >> 8) & 0x00FF);      //高位在后
        UART1_SendData(uint8_tMsg, uint8_tMsgLen + 2);           //发送数据
//    }
}

void MBRTU_SendErr(uint8_t ErrCode)
{
    SendLen = 1;
    SendBuf[SendLen++] |= 0x80;
    SendBuf[SendLen++] = ErrCode;

    MBRTU_SendMsg(SendBuf, SendLen);
}


//////////////////////////////////////////////////////////////////////////////////////////
//description描述：MBRTU_GetCRC16() 获取字符序列的CRC校验码
//input输入      ：Msg，MsgLen十六进制字符序列及其长度
//output输出     ：16位CRC码
///////////////////////////////////////////////////////////////////////////////////////////
uint16_t MBRTU_GetCRC16(uint8_t *uint8_tMsg, uint32_t uint8_tMsgLen)
{
    uint8_t CRCLo = 0xFF ; // 高CRC字节初始化
    uint8_t CRCHi = 0xFF ; // 低CRC字节初始化
    unsigned uIndex ; // CRC循环中的索引

    while (uint8_tMsgLen--)    // 传输消息缓冲区
    {
        uIndex = CRCLo ^ *uint8_tMsg++; // 计算CRC
        CRCLo  = CRCHi ^ auchCRCLo[uIndex];
        CRCHi  = auchCRCHi[uIndex];
    }

    return (CRCHi << 8) | CRCLo;
}


void MBRTU_Fun02(void)  //读单个寄存器
{
    uint32_t i;
    uint32_t Data_Buf;
    uint16_t ReadAdr = (uint16_t)RcvDataBuf[2] * 256 + RcvDataBuf[3];
    uint16_t Register_Num = (uint16_t)RcvDataBuf[4] * 256 + RcvDataBuf[5];

    SendLen = 0;
    SendBuf[SendLen++] = RcvDataBuf[0] ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = RcvDataBuf[1];     //功能码
    SendBuf[SendLen++] = Register_Num * 2;      //数据长度

    //如果读取范围溢出

    if (!(((ReadAdr <= HOLDING_REG_REGION01_END)
            && (ReadAdr + Register_Num <= (HOLDING_REG_REGION01_END + 1)))
            && (0 != Register_Num)))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    for (long k = 0; k < Register_Num; ReadAdr++, k++)
    {
        switch (ReadAdr)
        {

        case 0x0000:
			Data_Buf = Get_SwitchSta();

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

    MBRTU_SendMsg(SendBuf, SendLen);

}


void MBRTU_Fun03(void)  //读单个寄存器
{
    uint32_t i;
    uint32_t Data_Buf;
    uint16_t ReadAdr = (uint16_t)RcvDataBuf[2] * 256 + RcvDataBuf[3];
    uint16_t Register_Num = (uint16_t)RcvDataBuf[4] * 256 + RcvDataBuf[5];

    SendLen = 0;
    SendBuf[SendLen++] = RcvDataBuf[0] ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = RcvDataBuf[1];     //功能码
    SendBuf[SendLen++] = Register_Num * 2;      //数据长度

    //如果读取范围溢出

    if (!(((ReadAdr >= HOLDING_REG_REGION1_BGEIN) && (ReadAdr <= HOLDING_REG_REGION1_END)
            && (ReadAdr + Register_Num <= (HOLDING_REG_REGION1_END + 1)))
            || ((ReadAdr >= HOLDING_REG_REGION2_BGEIN) && (ReadAdr <= HOLDING_REG_REGION2_END)
                && (ReadAdr + Register_Num <= (HOLDING_REG_REGION2_END + 1)))
            && (0 != Register_Num)))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    for (long k = 0; k < Register_Num; ReadAdr++, k++)
    {
        switch (ReadAdr)
        {

        case 0x0030:
            Data_Buf = UserPara.SlaveAddr;              //设备地址
            break;

        case 0x0031:
            Data_Buf = UserPara.Baudrate;               //波特率
            break;

        case 0x0032:
            Data_Buf = UserPara.Parity;                 //奇偶校验
            break;

        case 0x0033:
            Data_Buf = 0;                               //NC
            break;

        case 0x0034:
            Data_Buf = 0;                               //NC
            break;

//        case 0x0035:
//            Data_Buf = UserParam.FilterLevel;           //滤波系数
//            break;
//
        case 0x0036:
            Data_Buf = UserPara.AutoUpLoadTime;      //自动发送周期
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

    MBRTU_SendMsg(SendBuf, SendLen);

}


void MBRTU_Fun10(void)  //写单个寄存器
{
    uint32_t i = 0;
    uint16_t ReadAdr = (uint16_t)RcvDataBuf[2] * 256 + RcvDataBuf[3];
    uint16_t Register_Num = (uint16_t)RcvDataBuf[4] * 256 + RcvDataBuf[5];

    SendLen = 0;
    SendBuf[SendLen++] = RcvDataBuf[0] ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = RcvDataBuf[1];     //功能码
    SendBuf[SendLen++] = Register_Num * 2;      //数据长度

    //如果读取范围溢出
    if (!(((ReadAdr >= MUL_REG_REGION1_BGEIN) && (ReadAdr <= MUL_REG_REGION1_END)
            && (ReadAdr + Register_Num <= (MUL_REG_REGION1_END + 1)))
            || ((ReadAdr >= MUL_REG_REGION2_BGEIN) && (ReadAdr <= MUL_REG_REGION2_END)
                && (ReadAdr + Register_Num <= (MUL_REG_REGION2_END + 1)))
            && (0 != Register_Num) && ((Register_Num * 2) == RcvDataBuf[6])))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    for (long k = 0; k < Register_Num; ReadAdr++, k++)
    {
        switch (ReadAdr)
        {

        case 0x0030:
            UserPara.SlaveAddr = RcvDataBuf[8 + i];              //设备地址
			Eeprom_WriteNBytes(SLAVE_ADDR, &UserPara.SlaveAddr, 1);
			i += 2;
            break;

        case 0x0031:
            UserPara.Baudrate = RcvDataBuf[8 + i];               //波特率
			Eeprom_WriteNBytes(BAUDRATE, &UserPara.Baudrate, 1);
			i += 2;
            break;

        case 0x0032:
            UserPara.Parity = RcvDataBuf[8 + i];                 //奇偶校验
			Eeprom_WriteNBytes(PARITY, &UserPara.Parity, 1);
			i += 2;
            break;

        case 0x0033:
            i += 2;         		                      		//NC
            break;

        case 0x0034:
            i += 2;                              				//NC
            break;

        case 0x0035:
            i += 2;          
            break;

        case 0x0036:
            UserPara.AutoUpLoadTime = RcvDataBuf[8 + i];      //自动发送周期
			Eeprom_WriteNBytes(AUTO_UPLOAD_TIME, &UserPara.AutoUpLoadTime, 1);
            break;


        default:
            break;
        }

    }

    MBRTU_SendMsg(RcvDataBuf, 6);

}


/**@brief       Modbus消息帧处理函数
* @param[in]    ModBusBaseParam : ModBus处理的基本参数结构体;
* @param[in]    arg : 用户自定义的参数,例如设备参数
* @return       函数执行结果
* - None
*/
void ModbusHandle(void)
{
    uint16_t crc;
    int32_t RecvLen = 0;
    
    RecvLen = UART1_RecvData(RcvDataBuf, sizeof(RcvDataBuf) / sizeof(RcvDataBuf[0]));
	UART1_SendData(RcvDataBuf, RecvLen);
	
    crc = RcvDataBuf[RecvLen - 1] << 8;
    crc |= RcvDataBuf[RecvLen - 2]; 
    if (RecvLen <= 0)
    {
        return;
    }
    else if ((UserPara.SlaveAddr != RcvDataBuf[0]) &&
             (BROADCASTADDR != RcvDataBuf[0]))
    {
        return;
    }
    else if (crc != MBRTU_GetCRC16(RcvDataBuf, RecvLen - 2))
    {
        if ((RcvDataBuf[0] == UserPara.SlaveAddr))
        {
            SendLen = 0;
            SendBuf[SendLen++] = RcvDataBuf[0];
            SendBuf[SendLen++] = 0x80 | RcvDataBuf[1];
            SendBuf[SendLen++] = MB_EX_MEMORY_PARITY_ERROR;

            MBRTU_SendMsg(SendBuf, SendLen);
        }
    }
    else
    {
        switch (RcvDataBuf[1])
        {

			case 0x02:
				MBRTU_Fun02();	//读单个线圈
				break;
			
			case 0x03:
				MBRTU_Fun03();  //读单个寄存器
				break;

			case 0x10:
				MBRTU_Fun10();  //读单个寄存器
				break;

			default:
				SendLen = 0;
				SendBuf[SendLen++] = RcvDataBuf[0];
				SendBuf[SendLen++] = 0x80 | RcvDataBuf[1];
				SendBuf[SendLen++] = 0x01;

				MBRTU_SendMsg(SendBuf, SendLen);
				break;

        }
		
    }

}



/**@brief       Modbus 消息帧自动上传处理
* @param[in]    None
* @return       None
* - None
*/
void ModbusAutoUpload(void)
{
    uint32_t i = 0;
	uint32_t j;
    uint32_t Data_Buf;

    SendBuf[i++] = UserPara.SlaveAddr;
    SendBuf[i++] = 0x02;
    SendBuf[i++] = 0x02;

    Data_Buf = Get_SwitchSta();
	for (j = 2; j > 0; j--)
	{
		SendBuf[i++] = (uint8_t)(Data_Buf >> ((j - 1) * 8));
	}

	MBRTU_SendMsg_NoLimit(SendBuf, i);
}



