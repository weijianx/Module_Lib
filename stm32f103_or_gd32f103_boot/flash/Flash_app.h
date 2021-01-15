/**@file        Flash_app.h
* @brief        Flash_app.c的头文件,定义了Flash地址宏定义,声明了Flash应用的API函数
* @details      适用于STM32F10x、GD32F103系列单片机,写入读取Flash函数
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
#ifndef Flash_APP_H
#define Flash_APP_H

#include "BSP.h"


//#define USER_FLASH_LAST_PAGE_ADDRESS  0x08004000


#define FLASH_PAGE_SIZE                 0x00000800U    /*!< FLASH页面大小，2K字节 */


#define FLASH_LAST_PAGE_ADDR		              0x0801F800					// Flash最后一个扇区首地址
#define APPLICATION_ADDRESS                       (uint32_t)0x08008000      /* Start user code address: ADDR_FLASH_PAGE_8 */

//#define IN_FLASH_START                  0x0000                              ///< Flash起始地址
//#define IN_FLASH_END                    0x07FF                              ///< Flash结束地址


#define USER_FLASH_PAGE_SIZE          FLASH_PAGE_SIZE
//#define USER_FLASH_END_ADDRESS        0x08020000
//#define USER_FLASH_LAST_PAGE_ADDRESS  0x0801FC00

#define STANDBY_OFFSET_ADDR					 0x00000800							// 备份参数偏移地址(相对FLASH_BASE_ADDR)
#define STANDBY_BASE_ADDR					 0x08006000							// 参数备份基地址

#define FLASH_BASE_ADDR						 0x08004800					        // Flash存储参数的基地址
#define COMMON_OFFSET						 0x00000000							// 通用参数存储地址偏移量
#define COMMON_BASE_ADDR					(FLASH_BASE_ADDR + COMMON_OFFSET)	// FLASH参数开始地址
#define COMMON_UPGRADE_ADDR					(COMMON_BASE_ADDR + 0)				// 通用参数系统升级标志地址
#define COMMON_SLAVE_ADDR					(COMMON_BASE_ADDR + 1)				// 通用参数从机地址地址
#define COMMON_BAUD_ADDR					(COMMON_BASE_ADDR + 2)				// 通用参数波特率地址
#define COMMON_PARITY_ADDR					(COMMON_BASE_ADDR + 3)				// 通用参数奇偶校验地址
#define COMMON_OUTPUTMOD_ADDR				(COMMON_BASE_ADDR + 4)				// 通用参数输出模式地址
#define COMMON_RESERVED_ADDR				(COMMON_BASE_ADDR + 5)				// 通用参数保留地址

// 升级使能
#define UPGRADE_DISABLE							0x00							// 升级标志失能(默认值)
#define UPGRADE_ENABLE							0xFF							// 升级标志使能


#define EN_INT                  	__enable_irq();				// 系统开全局中断  
#define DIS_INT                 	__disable_irq();			// 系统关全局中断


/**@brief       向内部Flash指定位置读取一字节数据
* @param[in]    RWAddr : 读取地址
* @return       函数执行结果 - 1个字节数据
* @note         
*/
uint8_t Flash_Read_OneByte(uint32_t RWAddr);



/**@brief       从内部Flash指定位置读多个字节
* @param[in]    RWAddr : 读起始地址
* @param[in]    pRdbuf : 数据缓存指针
* @param[in]    Rdlen : 读数据长度
* @return       函数执行结果
* @note        
*/
void Flash_Read_MultiBytes(uint32_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);


/**@brief       Flash 写N个半字
* @param[in]    RWAddr : 写起始地址
* @param[in]    pWrbuf : 数据缓存指针
* @param[in]    Wrlen : 写数据长度
* @return       函数执行结果 1 成功  0 失败
*/
uint8_t Flash_Write_HalfWord(uint32_t RWAddr, uint16_t *pWrbuf, uint16_t Wrlen);

/**@brief       页擦除，为写APP做准备
* @param[in]    dwStartSector : 擦除起始地址
* @return       函数执行结果 1 成功  0 失败
* @note  		Wrlen < 2048,单次最大可以写2048个字节，可跨页写
*/
uint32_t FLASH_If_Erase(uint32_t dwStartSector);

/**@brief       擦除单页
* @param[in]    startaddress : 擦除起始地址
*/
void Flash_erase_apage(uint32_t startaddress);


#endif


