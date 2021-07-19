#include "para.h"
#include "type.h"

#include "SHT3x.h"

UserTypeDef UserPara;
uint8_t Cur_Param[USER_DEFAULT_LEN];

static uint8_t User_Default_Param[USER_DEFAULT_LEN] =
{
	0x00,
    0x02,
    0x21,
	0x26,
	0x03,
    0xBF,0x0F,          //?????,4031???130?,???????
    0x47,0x0A,          //?????,2231???-10?,???????        
    0x1E,0x00,          //?????,30s,??????  
    0x1E,0x00,          //?????,30s,??????
    0x20,0x03,         //?????,800???80%,???????
    0xC8,0x00,         //?????,200???20%,???????
	0x00,              //????????(???????)
    0x00               //????????(???????)
    
};
extern SHT3_Typedef T_H;
void ReadPara(void)
{
    uint8_t ParaInitFlag;
    flash_read_MultiBytes(RUN_ADDR_BASE+1, &ParaInitFlag, 1);
   if(ParaInitFlag != User_Default_Param[1])
    {
        Flash_Write_MultiBytes(RUN_ADDR_BASE, User_Default_Param, USER_DEFAULT_LEN);
    }
    flash_read_MultiBytes(RUN_ADDR_BASE, Cur_Param, USER_DEFAULT_LEN);
    UserPara.SlaveAddr_Temp = Cur_Param[2];
    UserPara.SlaveAddr_Humi = Cur_Param[3];
	UserPara.Baudrate       = Cur_Param[4];
   // UserPara.Up_Thr = ((uint16_t)Cur_Param[3] << 8) +Cur_Param[2];
   // UserPara.Do_Thr = ((uint16_t)Cur_Param[5] << 8) +Cur_Param[4];
   // UserPara.Du_Thr = ((uint16_t)Cur_Param[7] << 8) +Cur_Param[6];
    
    T_H.UpThreshold[0] = ((uint16_t)Cur_Param[6] << 8) +Cur_Param[5];
    T_H.DoThreshold[0] = ((uint16_t)Cur_Param[8] << 8) +Cur_Param[7];
    
    T_H.DurThreshold[0] = ((uint16_t)Cur_Param[10] << 8) +Cur_Param[9];
    T_H.DurThreshold[1] = ((uint16_t)Cur_Param[12] << 8) +Cur_Param[11];
    
    T_H.UpThreshold[1] = ((uint16_t)Cur_Param[14] << 8) +Cur_Param[13];
    T_H.DoThreshold[1] = ((uint16_t)Cur_Param[16] << 8) +Cur_Param[15];
    
    T_H.Upload_persist[0] = Cur_Param[17];
    T_H.Upload_persist[1] = Cur_Param[18];  
    
    
}





