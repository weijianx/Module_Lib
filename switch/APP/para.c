#include "para.h"
#include "type.h"
#include "eeprom.h"

UserTypeDef UserPara;
uint8_t Cur_Param[USER_DEFAULT_LEN];
//extern uint16_t LiquidUnit;
//extern uint8_t  LiquidRange;


uint32_t Current_pulse = 0;  //��ǰ������
uint32_t Current_PositiveTime = 0;  //��ǰ��ת������
uint32_t Current_NegativeTime = 0;  //��ǰ��ת������

//���ݵĸ��ֽ���ǰ�����ֽ��ں󣨴��ģʽ��
uint8_t  User_Default_Param[USER_DEFAULT_LEN] =
{
    0x01,                       //������־
    0x51,                       //�豸ID        
    0x03,                       //������
    0x03,                       //��żУ��λ
	
	0x01,						//�Զ���������
    
};



//******************************************************************************
// ����         : ReadPara()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : ��ȡEEPROM����
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : �� 
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
    
    
    UserPara.SlaveAddr = Cur_Param[SLAVE_ADDR];  //�ӻ���ַ
    //UserPara.SlaveAddr = Cur_Param[1];  //�ӻ���ַ
      
    UserPara.Baudrate = Cur_Param[BAUDRATE];  //������
    
    UserPara.Parity = Cur_Param[PARITY];    //��żУ��
    
	UserPara.AutoUpLoadTime = Cur_Param[AUTO_UPLOAD_TIME];
    

}




