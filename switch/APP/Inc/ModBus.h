/**@file        Modbus.h
* @details      Modbus.c的头文件,声明了 Modbus 通信的API函数
* @author       庄明群
* @date         2020-07-20
* @version      V2.0.0
* @copyright    2020-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author  <th>Maintainer  <th>Description
* <tr><td>2020/07/20  <td>2.0.0    <td>庄明群  <td>杨春林      <td>维护并更新的版本
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


    
#define DATAPACKET_LEN  		64			 //数据包长度

#define AUTOUPLOAD_CYCLE                1000            ///< 自动上传周期,单位 ms
    

#define BROADCASTADDR                   0x00                                  ///< 广播地址
   
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
   
  
//02功能码
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
   



void MBRTU_Fun02(void);  //读单个线圈
void MBRTU_Fun03(void);  //读单个寄存器
void MBRTU_Fun10(void);  //写单个寄存器
void ModbusHandle(void);

void ModbusAutoUpload(void);

#ifdef __cplusplus
}
#endif

#endif // __MODBUS_H
