/**@file        Flash_app.c
* @brief        Flash模块的应用
* @details      适用于STM32F10x、GD32F10x系列单片机,写入读取Flash函数
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
#include "Flash_app.h"



uint32_t FirstPage = 0, NbOfPages = 0;

/**@brief       向内部Flash指定位置读取一字节数据
* @param[in]    RWAddr : 读取地址
* @return       函数执行结果 - 1个字节数据
* @note         
*/
uint8_t Flash_Read_OneByte(uint32_t RWAddr)
{
  uint8_t RdData;
  RdData = *(__IO uint8_t *)RWAddr;
  return RdData;
}



/**@brief       从内部Flash指定位置读多个字节
* @param[in]    RWAddr : 读起始地址
* @param[in]    pRdbuf : 数据缓存指针
* @param[in]    Rdlen : 读数据长度
* @return       函数执行结果
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




/**@brief       Flash 写N个半字
* @param[in]    RWAddr : 写起始地址
* @param[in]    pWrbuf : 数据缓存指针
* @param[in]    Wrlen : 写数据长度
* @return       函数执行结果 1 成功  0 失败
*/
uint8_t Flash_Write_HalfWord(uint32_t RWAddr, uint16_t *pWrbuf, uint16_t Wrlen)
{
	uint8_t status=0;
	uint16_t i =0;
    FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//清除标志位
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

/**@brief       页擦除，为写APP做准备
* @param[in]    dwStartSector : 擦除起始地址
* @return       函数执行结果 1 成功  0 失败
* @note  		Wrlen < 2048,单次最大可以写2048个字节，可跨页写
*/
uint32_t FLASH_If_Erase(uint32_t dwStartSector)
{
  	uint32_t dwFlashaddress;
  
//  	dwFlashaddress = GetPage(dwStartSector);
  
	dwFlashaddress = (dwStartSector / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
	
	FLASH_Unlock();  //解锁FLASH编程擦除控制器
  	while (FLASH_LAST_PAGE_ADDR > dwFlashaddress)
  	{
		
		
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//清除标志位
		
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


/**@brief       擦除单页
* @param[in]    startaddress : 擦除起始地址
*/
void Flash_erase_apage(uint32_t startaddress)
{
	 uint32_t dwFlashaddress;
  
	dwFlashaddress = (startaddress / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;

	FLASH_Unlock();  //解锁FLASH编程擦除控制器
	
	FLASH_ErasePage(dwFlashaddress);
	
	FLASH_Lock();
}






