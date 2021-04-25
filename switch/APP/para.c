#include "para.h"
#include "type.h"
#include "eeprom.h"

UserTypeDef UserPara;
uint8_t Cur_Param[USER_DEFAULT_LEN];
//extern uint16_t LiquidUnit;
//extern uint8_t  LiquidRange;


uint32_t Current_pulse = 0;  //当前脉冲数
uint32_t Current_PositiveTime = 0;  //当前正转脉冲数
uint32_t Current_NegativeTime = 0;  //当前反转脉冲数

//数据的高字节在前，低字节在后（大端模式）
uint8_t  User_Default_Param[USER_DEFAULT_LEN] =
{
    0x01,                       //参数标志
    0x51,                       //设备ID        
    0x03,                       //波特率
    0x03,                       //奇偶校验位
	
	0x01,						//自动发送周期
    
};



//******************************************************************************
// 名称         : ReadPara()
// 创建日期     : 2018-06-08
// 作者         : MMX
// 功能         : 读取EEPROM数据
// 输入参数     : 无
// 输出参数     : 无
// 返回结果     : 无
// 注意和说明   : 无
// 修改内容     : 无 
//******************************************************************************
void ReadPara(void)
{
    uint8_t ParaInitFlag;

    
    Eeprom_ReadNBytes(RUN_ADDR_BASE, &ParaInitFlag, 1);
    
    if(ParaInitFlag != User_Default_Param[0])
    {
        Eeprom_WriteNBytes(RUN_ADDR_BASE, User_Default_Param, USER_DEFAULT_LEN);
    }
//    Eeprom_WriteNBytes(RUN_ADDR_BASE, User_Default_Param, USER_DEFAULT_LEN);
    Eeprom_ReadNBytes(RUN_ADDR_BASE, Cur_Param, USER_DEFAULT_LEN);
    
    
    UserPara.SlaveAddr = Cur_Param[SLAVE_ADDR];  //从机地址
    //UserPara.SlaveAddr = Cur_Param[1];  //从机地址
      
    UserPara.Baudrate = Cur_Param[BAUDRATE];  //波特率
    
    UserPara.Parity = Cur_Param[PARITY];    //奇偶校验
    
	UserPara.AutoUpLoadTime = Cur_Param[AUTO_UPLOAD_TIME];
    

}




