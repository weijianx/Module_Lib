/**@file        Flash_app.h
* @brief        Flash_app.c��ͷ�ļ�,������Flash��ַ�궨��,������FlashӦ�õ�API����
* @details      ������STM32F10x��GD32F103ϵ�е�Ƭ��,д���ȡFlash����
* @author       Τ����
* @date         2021-01-10
* @version      V1.0.0
* @copyright    2021-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2021/01/10  <td>1.0.0    <td>Τ����    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/
#ifndef Flash_APP_H
#define Flash_APP_H

#include "BSP.h"


//#define USER_FLASH_LAST_PAGE_ADDRESS  0x08004000


#define FLASH_PAGE_SIZE                 0x00000800U    /*!< FLASHҳ���С��2K�ֽ� */


#define FLASH_LAST_PAGE_ADDR		              0x0801F800					// Flash���һ�������׵�ַ
#define APPLICATION_ADDRESS                       (uint32_t)0x08008000      /* Start user code address: ADDR_FLASH_PAGE_8 */

//#define IN_FLASH_START                  0x0000                              ///< Flash��ʼ��ַ
//#define IN_FLASH_END                    0x07FF                              ///< Flash������ַ


#define USER_FLASH_PAGE_SIZE          FLASH_PAGE_SIZE
//#define USER_FLASH_END_ADDRESS        0x08020000
//#define USER_FLASH_LAST_PAGE_ADDRESS  0x0801FC00

#define STANDBY_OFFSET_ADDR					 0x00000800							// ���ݲ���ƫ�Ƶ�ַ(���FLASH_BASE_ADDR)
#define STANDBY_BASE_ADDR					 0x08006000							// �������ݻ���ַ

#define FLASH_BASE_ADDR						 0x08004800					        // Flash�洢�����Ļ���ַ
#define COMMON_OFFSET						 0x00000000							// ͨ�ò����洢��ַƫ����
#define COMMON_BASE_ADDR					(FLASH_BASE_ADDR + COMMON_OFFSET)	// FLASH������ʼ��ַ
#define COMMON_UPGRADE_ADDR					(COMMON_BASE_ADDR + 0)				// ͨ�ò���ϵͳ������־��ַ
#define COMMON_SLAVE_ADDR					(COMMON_BASE_ADDR + 1)				// ͨ�ò����ӻ���ַ��ַ
#define COMMON_BAUD_ADDR					(COMMON_BASE_ADDR + 2)				// ͨ�ò��������ʵ�ַ
#define COMMON_PARITY_ADDR					(COMMON_BASE_ADDR + 3)				// ͨ�ò�����żУ���ַ
#define COMMON_OUTPUTMOD_ADDR				(COMMON_BASE_ADDR + 4)				// ͨ�ò������ģʽ��ַ
#define COMMON_RESERVED_ADDR				(COMMON_BASE_ADDR + 5)				// ͨ�ò���������ַ

// ����ʹ��
#define UPGRADE_DISABLE							0x00							// ������־ʧ��(Ĭ��ֵ)
#define UPGRADE_ENABLE							0xFF							// ������־ʹ��


#define EN_INT                  	__enable_irq();				// ϵͳ��ȫ���ж�  
#define DIS_INT                 	__disable_irq();			// ϵͳ��ȫ���ж�


/**@brief       ���ڲ�Flashָ��λ�ö�ȡһ�ֽ�����
* @param[in]    RWAddr : ��ȡ��ַ
* @return       ����ִ�н�� - 1���ֽ�����
* @note         
*/
uint8_t Flash_Read_OneByte(uint32_t RWAddr);



/**@brief       ���ڲ�Flashָ��λ�ö�����ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @param[in]    pRdbuf : ���ݻ���ָ��
* @param[in]    Rdlen : �����ݳ���
* @return       ����ִ�н��
* @note        
*/
void Flash_Read_MultiBytes(uint32_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);


/**@brief       Flash дN������
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    pWrbuf : ���ݻ���ָ��
* @param[in]    Wrlen : д���ݳ���
* @return       ����ִ�н�� 1 �ɹ�  0 ʧ��
*/
uint8_t Flash_Write_HalfWord(uint32_t RWAddr, uint16_t *pWrbuf, uint16_t Wrlen);

/**@brief       ҳ������ΪдAPP��׼��
* @param[in]    dwStartSector : ������ʼ��ַ
* @return       ����ִ�н�� 1 �ɹ�  0 ʧ��
* @note  		Wrlen < 2048,����������д2048���ֽڣ��ɿ�ҳд
*/
uint32_t FLASH_If_Erase(uint32_t dwStartSector);

/**@brief       ������ҳ
* @param[in]    startaddress : ������ʼ��ַ
*/
void Flash_erase_apage(uint32_t startaddress);


#endif


