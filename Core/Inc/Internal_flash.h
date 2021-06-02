/*
 * flash.h
 *
 *  Created on: 31-May-2021
 *      Author: pankaj.kumar
 */

#ifndef INC_INTERNAL_FLASH_H_
#define INC_INTERNAL_FLASH_H_


/*
Library:				STM32F40x Internal FLASH read/write
Written by:			Mohamed Yaqoob (MYaqoobEmbedded YouTube Channel)
Last modified:	15/03/2019
Description:
							MY_FLASH library implements the following basic functionalities
								- Set sectos address
								- Flash Sector Erase
								- Flash Write
								- Flash Read

* Copyright (C) 2019 - M. Yaqoob
   This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
   of the GNU General Public Licenseversion 3 as published by the Free Software Foundation.

   This software library is shared with puplic for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
   or indirectly by this software, read more about this on the GNU General Public License.
*/

#include "stm32f0xx_hal.h"

//Typedefs
//1. data size
typedef enum
{
	DATA_TYPE_8=0,
	DATA_TYPE_16,
	DATA_TYPE_32,
}DataTypeDef;

#define FLASH_TYPEPROGRAM_BYTE  	(0x00U)
#define PAGE_SIZE	1024

//functions prototypes
//1. Erase Sector
static void MY_FLASH_EraseSector(void);

//2. Set Sector Adress
void MY_FLASH_SetSectorAddrs(uint8_t sector, uint32_t addrs);
//3. Write Flash
void MY_FLASH_WriteN(uint32_t idx, void *wrBuf, uint32_t Nsize, DataTypeDef dataType);
//4. Read Flash
void MY_FLASH_ReadN(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType);
//pankaj
HAL_StatusTypeDef HAL_FLASH_Program_8bit(uint32_t TypeProgram, uint32_t Address, uint8_t Data);

void Flash_WriteData_With_Erase_Page(uint32_t addr,void *data,uint16_t Size, DataTypeDef dataType);
uint8_t Flash_ReadData(uint32_t addr,void *data,uint32_t Size, DataTypeDef dataType);
void Flash_WriteData_without_erase(uint32_t addr,void *data,uint16_t Size, DataTypeDef dataType);
void erase_page(uint32_t addr);
void FLASH_EraseSector(uint8_t SECTOR_NO);
#endif /* INC_INTERNAL_FLASH_H_ */
