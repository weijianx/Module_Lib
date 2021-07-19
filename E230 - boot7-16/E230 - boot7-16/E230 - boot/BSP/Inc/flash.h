/*!
    \file  flash_access.h 
    \brief the header file of flash_access.c

    \version 2014-12-26, V1.0.0, demo for GD32F10x
    \version 2017-06-20, V2.0.0, demo for GD32F10x
    \version 2018-07-31, V2.1.0, demo for GD32F10x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef FLASH_H
#define FLASH_H

#include "gd32e230.h"

//Include files
#include "Includes.h"

/*************************************************************************************************
Chip flash started: 0x08000000
Boot occupy falsh:  12K Bytes
Applicatioon falsh: 0x08003000
Back Company infor: 0x08003000 ~ 0x080031FF (512 Bytes)
Back parameter inf: 0x08003200 ~ 0x080037FF

Company infor user: 0x08003800 ~ 0x080039FF (512 Bytes)
Parameter inf user: 0x08003A00 ~ 0x08003FFF
Application start:  0x08004000
*************************************************************************************************/

//#ifdef GD32E230xx
//Define 128k flash operate macro
	#define SOWAYAPP_FLASH_PAGES        48			//128 - 16 = 112
	#define SOWAYFLASH_PAGE_SIZE        1024
	#define SOWAYFLASH_SRCPARA_ADDR     0x08003000  //Backup parameter 2K bytes
	#define SOWAYFLASH_OP_BASE_ADDR     0x08003800  //System parameter 2K bytes
//	#define MAGNET_PARAMETER_ADDR       0x08003A00	//Magnet parameter address
	#define SOWAYAPP_START_ADDR         0x08004000  //Soway application start address
	#define CHIPFLASH_MAXIMUM_ADDR      0x0800FFFF  //According chip type modify
	#define CHIPFLASH_EDGE_ADDR         0X08010000
//Define GD32F103CBT6 Mcro ok
//#endif 

#ifdef GD32F103C8T6
//Define 64k flash operate macro
	#define SOWAYAPP_FLASH_PAGES        48			//64 - 16 = 48
	#define SOWAYFLASH_PAGE_SIZE        1024
	#define SOWAYFLASH_SRCPARA_ADDR     0x08003000  //Backup parameter 2K bytes
	#define SOWAYFLASH_OP_BASE_ADDR     0x08003800  //System parameter 2K bytes
	#define MAGNET_PARAMETER_ADDR       0x08003A00	//Magnet parameter address
	#define SOWAYAPP_START_ADDR         0x08004000  //Soway application start address
	#define CHIPFLASH_MAXIMUM_ADDR      0x0800FFFF  //According chip type modify
	#define CHIPFLASH_EDGE_ADDR         0X08010000
//Define GD32F103CBT6 Mcro ok
#endif

//Functions declare
bool GDFLASHOP_EraseSpecifiedFlashPages( u32 startaddr, u16 npages );
bool GDFLASHOP_WriteData2SpecifiedFlash( u32 startaddr, u32 *psrc, u16 wlen );
bool GDFLASHOP_ReadDataFromChipFlash( u8* pdest, u32 op_addr, u32 dwbufsize, u32 dwlen );


uint8_t Flash_Read_OneByte(uint32_t RWAddr);

void flash_read_multi(uint32_t readAdder, uint8_t *readBuf, uint16_t readLen);

uint32_t  flash_read_MultiBytes(uint32_t read_addr, uint8_t *pBuf, uint16_t len);

uint8_t Flash_Write_OneByte(uint32_t RWAddr, uint8_t WrData);

uint8_t Flash_Write_MultiBytes(uint32_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);

uint32_t flash_write_word (uint32_t write_addr, uint32_t *pBuf, uint16_t wreLen);

uint8_t fmc_erase_for_app(uint32_t startAdder);

uint8_t fmc_erase_a_page(uint32_t startAdder);

uint8_t Flash_Read_OneByte(uint32_t RWAddr);

void flash_read_multi(uint32_t readAdder, uint8_t *readBuf, uint16_t readLen);

uint32_t  flash_read_MultiBytes(uint32_t read_addr, uint8_t *pBuf, uint16_t len);

uint8_t Flash_Write_OneByte(uint32_t RWAddr, uint8_t WrData);

uint8_t Flash_Write_MultiBytes(uint32_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);
//uint8_t Flash_Write_MultiBytes32(uint32_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);

uint32_t flash_write_word (uint32_t write_addr, uint32_t *pBuf, uint16_t wreLen);

uint8_t fmc_erase_for_app(uint32_t startAdder);

uint8_t fmc_erase_a_page(uint32_t startAdder);
uint8_t Flash_Write_32Bytes(uint32_t RWAddr, uint32_t data);

bool GDFLASHOP_EraseSpecifiedFlashPages( uint32_t startaddr, uint16_t npages );

#endif /* FLASH_ACCESS_H */



