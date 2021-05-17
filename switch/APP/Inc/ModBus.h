/**@file        Modbus.h
* @details      Modbus.c��ͷ�ļ�,������ Modbus ͨ�ŵ�API����
* @author       ׯ��Ⱥ
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>ׯ��Ⱥ  <td>���      <td>ά�������µİ汾
* </table>
*
**********************************************************************************
*/

#ifndef __MODBUS_H
#define __MODBUS_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l0xx.h"
#include "stm32l031xx.h"
   
#include "usart.h"


    
#define DATAPACKET_LEN  		64			 //���ݰ�����

#define AUTOUPLOAD_CYCLE                1000            ///< �Զ��ϴ�����,��λ ms
    

#define BROADCASTADDR                   0x00                                  ///< �㲥��ַ
   
#define MB_EX_NONE 					(0x00)
#define MB_EX_ILLEGAL_FUNCTION 		(0x01)
#define MB_EX_ILLEGAL_DATA_ADDRESS	(0x02)
#define MB_EX_ILLEGAL_DATA_VALUE	(0x03)
#define MB_EX_SLAVE_DEVICE_FAILURE	(0x04)
#define MB_EX_ACKNOWLEDGE			(0x05)
#define MB_EX_SLAVE_BUSY			(0x06)
#define MB_EX_MEMORY_PARITY_ERROR	(0x08)
#define MB_EX_GATEWAY_PATH_FAILED	(0x0A)
#define MB_EX_GATEWAY_TGT_FAILED	(0x0B)
   
  
//02������
#define HOLDING_REG_REGION01_BGEIN		0x0000
#define HOLDING_REG_REGION01_END		0x00FF



#define HOLDING_REG_REGION1_BGEIN		0x0030
#define HOLDING_REG_REGION1_END			0x0042
#define HOLDING_REG_REGION2_BGEIN		0X0060
#define HOLDING_REG_REGION2_END			0X0080
 
#define MUL_REG_REGION1_BGEIN          HOLDING_REG_REGION1_BGEIN
#define MUL_REG_REGION1_END            HOLDING_REG_REGION1_END
#define MUL_REG_REGION2_BGEIN          HOLDING_REG_REGION2_BGEIN
#define MUL_REG_REGION2_END            HOLDING_REG_REGION2_END
   



void MBRTU_Fun02(void);  //��������Ȧ
void MBRTU_Fun03(void);  //�������Ĵ���
void MBRTU_Fun10(void);  //д�����Ĵ���
void ModbusHandle(void);

void ModbusAutoUpload(void);

#ifdef __cplusplus
}
#endif

#endif // __MODBUS_H
