/*****************************************************************************
* @file    Project/includes.h 
* @author  ktfe
* @version V1.0.0
* @date    24/01/2014
* @brief   include
*****************************************************************************/

#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "stm32f10x_sys.h"
#include "stm32f10x_gpio.h"


typedef  void (*pFunction)(void);

/*Set the number of ticks in one second*/
#define TICKS_PER_SEC 1000


typedef struct
{
	uint8_t UpgradeFlag;														// 系统升级标志 存储物理地址 Addr_0x08003000 
	uint8_t Addr;																    // 从机ID 		存储物理地址 Addr_0x08003001
	uint8_t Baud;																    // 波特率		存储物理地址 Addr_0x08003002
	uint8_t Parity;																  // 奇偶校验 	存储物理地址 Addr_0x08003003
	uint8_t OutPutMod;															// 输出模式 	存储物理地址 Addr_0x08003004
	uint8_t ProgEraseFlag;
	uint8_t Reserved[3];														// 保留 留做备用 方便添加更多参数 按16字节对齐
}CommonPara_Typedef;															// 通用参数结构体 通用参数与客户可操作参数结构体部分参数重叠 仅存储格式不同
																				// 目的是为了兼容BootLoad和Modbus等定义
typedef struct
{
	uint8_t Flag;								  // Flash写标志
//  uint16_t TempH;                             	  // Addr_0x02 数据高16位 
//  uint16_t TempL;                             	  // Addr_0x03 数据低16位
	uint8_t Addr;                             		  // Addr_0x30 设备地址
	uint8_t Baud;                             		  // Addr_0x31 波特率
	uint8_t Parity;                            	  // Addr_0x32 奇偶校验
	uint8_t Compensation;                      	  // Addr_0x34 补偿使能
	uint8_t UploadFreq;                      		  // Addr_0x3F 自动上传时间
	uint8_t UpgradeFlag;								// Addr_0x40 系统升级标志
	uint8_t Reset;                             	  // Addr_0x51 恢复出厂设置
	uint8_t OutPutMod;                         	  // Addr_0x60 输出模式
//	uint8_t Reserved[5];													  // 保留 留做备用 方便添加更多参数 按16字对齐
}CustomerPara_Typedef;													  // 客户可操作参数结构体



typedef struct
{
	CommonPara_Typedef		Common;												// 通用参数结构体 通用参数与客户可操作参数结构体部分参数重叠 仅存储格式不同 为兼容BootLoad和Modbus规范 
	CustomerPara_Typedef	Customer;											// 客户可操作参数结构体 

}FlashPara_Typedef;																// Flash保存参数结构体 结构体中的Data数组用作对齐


/*启动初始化*/
void Target_Init(void);


#endif
