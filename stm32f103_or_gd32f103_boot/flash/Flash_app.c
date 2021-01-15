/**@file        Flash_app.c
* @brief        Flashģ���Ӧ��
* @details      ������STM32F10x��GD32F10xϵ�е�Ƭ��,д���ȡFlash����
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
#include "Flash_app.h"



uint32_t FirstPage = 0, NbOfPages = 0;

/**@brief       ���ڲ�Flashָ��λ�ö�ȡһ�ֽ�����
* @param[in]    RWAddr : ��ȡ��ַ
* @return       ����ִ�н�� - 1���ֽ�����
* @note         
*/
uint8_t Flash_Read_OneByte(uint32_t RWAddr)
{
  uint8_t RdData;
  RdData = *(__IO uint8_t *)RWAddr;
  return RdData;
}



/**@brief       ���ڲ�Flashָ��λ�ö�����ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @param[in]    pRdbuf : ���ݻ���ָ��
* @param[in]    Rdlen : �����ݳ���
* @return       ����ִ�н��
* @note        
*/
void Flash_Read_MultiBytes(uint32_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen)
{
  uint16_t FlashCnt;
  for(FlashCnt = 0; FlashCnt < Rdlen; FlashCnt++)
  {
    *(pRdbuf + FlashCnt) = Flash_Read_OneByte(RWAddr);
    RWAddr++;
  }
}




/**@brief       Flash дN������
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    pWrbuf : ���ݻ���ָ��
* @param[in]    Wrlen : д���ݳ���
* @return       ����ִ�н�� 1 �ɹ�  0 ʧ��
*/
uint8_t Flash_Write_HalfWord(uint32_t RWAddr, uint16_t *pWrbuf, uint16_t Wrlen)
{
	uint8_t status=0;
	uint16_t i =0;
    FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
	for(i = 0; i < Wrlen; i++)
	{
		FLASH_ProgramHalfWord((RWAddr + (i * 2)), pWrbuf[i]);
	
	}
 
	return status;

}



uint32_t FLASH_If_Write(uint32_t dwFlashAddress, uint16_t *pWData, uint16_t wDataLength)
{
  	uint16_t i;

  	for (i = 0; (i < wDataLength) && (dwFlashAddress <= (FLASH_LAST_PAGE_ADDR-2)); i++)
  	{
    	//the operation will be done by word 
    	if (FLASH_COMPLETE == FLASH_ProgramHalfWord(dwFlashAddress, *(pWData + i) ) )
    	{
     		//Check the written value 
      		if (*(vu16*)(dwFlashAddress) != *(uint16_t*)(pWData + i) )
      		{
        		//Flash content doesn't match SRAM content 
        		return(2);
      		}
      		//Increment FLASH destination address 
      		dwFlashAddress += 2;
    	}
    	else
    	{
      		// Error occurred while writing data in Flash memory 
      		return (1);
    	}
  	}

  	return (0);
}

/**@brief       ҳ������ΪдAPP��׼��
* @param[in]    dwStartSector : ������ʼ��ַ
* @return       ����ִ�н�� 1 �ɹ�  0 ʧ��
* @note  		Wrlen < 2048,����������д2048���ֽڣ��ɿ�ҳд
*/
uint32_t FLASH_If_Erase(uint32_t dwStartSector)
{
  	uint32_t dwFlashaddress;
  
//  	dwFlashaddress = GetPage(dwStartSector);
  
	dwFlashaddress = (dwStartSector / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
	
	FLASH_Unlock();  //����FLASH��̲���������
  	while (FLASH_LAST_PAGE_ADDR > dwFlashaddress)
  	{
		
		
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
		
	    if (FLASH_COMPLETE == FLASH_ErasePage(dwFlashaddress) )
	    {
	      	dwFlashaddress += FLASH_PAGE_SIZE;
	    }
	    else
	    {
	      	//Error occurred while page erase 
	      	return (1);
	    }
  	}
  	FLASH_Lock();
	
  	return (0);
}


/**@brief       ������ҳ
* @param[in]    startaddress : ������ʼ��ַ
*/
void Flash_erase_apage(uint32_t startaddress)
{
	 uint32_t dwFlashaddress;
  
	dwFlashaddress = (startaddress / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;

	FLASH_Unlock();  //����FLASH��̲���������
	
	FLASH_ErasePage(dwFlashaddress);
	
	FLASH_Lock();
}






