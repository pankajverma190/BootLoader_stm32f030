/*
 * flash.c
 *
 *  Created on: 31-May-2021
 *      Author: pankaj.kumar
 */


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

#include "Internal_flash.h"

//Private variables
//1. sector start address
static uint32_t MY_SectorAddrs;
static uint8_t MY_SectorNum;
FLASH_ProcessTypeDef pFlash;
//functions definitions
//1. Erase Sector
void FLASH_EraseSector(uint8_t SECTOR_NO)
{
	uint32_t page_location = 0x08000000 + (SECTOR_NO * 4 * PAGE_SIZE);
	HAL_FLASH_Unlock();

	FLASH_PageErase(page_location);
	page_location = page_location + PAGE_SIZE;
	FLASH_PageErase(page_location);
	page_location = page_location + PAGE_SIZE;
	FLASH_PageErase(page_location);
	page_location = page_location + PAGE_SIZE;
	FLASH_PageErase(page_location);

	HAL_FLASH_Lock();
}

////2. Set Sector Adress
//void MY_FLASH_SetSectorAddrs(uint8_t sector, uint32_t addrs)
//{
//	MY_SectorNum = sector;
//	MY_SectorAddrs = addrs;
//}

//3. Write Flash
//void MY_FLASH_WriteN(uint32_t idx, void *wrBuf, uint32_t Nsize, DataTypeDef dataType)
//{
//	uint32_t flashAddress = MY_SectorAddrs + idx;
//
//	//Erase sector before write
////	MY_FLASH_EraseSector();
//	MY_FLASH_EraseSector1(MY_SectorNum);
//	//Unlock Flash
//	HAL_FLASH_Unlock();
//	//Write to Flash
//	switch(dataType)
//	{
//		case DATA_TYPE_8:
//				for(uint32_t i=0; i<Nsize; i++)
//				{
//					HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, flashAddress , ((uint8_t *)wrBuf)[i]);
//					flashAddress++;
//				}
//			break;
//
//		case DATA_TYPE_16:
//				for(uint32_t i=0; i<Nsize; i++)
//				{
//					HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flashAddress , ((uint16_t *)wrBuf)[i]);
//					flashAddress+=2;
//				}
//			break;
//
//		case DATA_TYPE_32:
//				for(uint32_t i=0; i<Nsize; i++)
//				{
//					HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddress , ((uint32_t *)wrBuf)[i]);
//					flashAddress+=4;
//				}
//			break;
//	}
//	//Lock the Flash space
//	HAL_FLASH_Lock();
//}
//4. Read Flash
//void MY_FLASH_ReadN(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType)
//{
//	uint32_t flashAddress = MY_SectorAddrs + idx;
//
//	switch(dataType)
//	{
//		case DATA_TYPE_8:
//				for(uint32_t i=0; i<Nsize; i++)
//				{
//					*((uint8_t *)rdBuf + i) = *(uint8_t *)flashAddress;
//					flashAddress++;
//				}
//			break;
//
//		case DATA_TYPE_16:
//				for(uint32_t i=0; i<Nsize; i++)
//				{
//					*((uint16_t *)rdBuf + i) = *(uint16_t *)flashAddress;
//					flashAddress+=2;
//				}
//			break;
//
//		case DATA_TYPE_32:
//				for(uint32_t i=0; i<Nsize; i++)
//				{
//					*((uint32_t *)rdBuf + i) = *(uint32_t *)flashAddress;
//					flashAddress+=4;
//				}
//			break;
//	}
//}

void erase_page(uint32_t addr)
{
	 //1, unlock FLASH
	  HAL_FLASH_Unlock();
	  FLASH_EraseInitTypeDef f;
	  f.TypeErase = FLASH_TYPEERASE_PAGES;
	  f.PageAddress = addr;
	  f.NbPages = 1;
	  // Set PageError
	  uint32_t PageError = 0;
	  // call the erase function
	  HAL_FLASHEx_Erase(&f, &PageError);
	  HAL_FLASH_Lock();
}

void Flash_WriteData_With_Erase_Page(uint32_t addr,void *data,uint16_t Size, DataTypeDef dataType)
{
  //1, unlock FLASH
  HAL_FLASH_Unlock();

  //2, erase FLASH
  // Initialize FLASH_EraseInitTypeDef
  FLASH_EraseInitTypeDef f;
  f.TypeErase = FLASH_TYPEERASE_PAGES;
  f.PageAddress = addr;
  f.NbPages = 1;
  // Set PageError
  uint32_t PageError = 0;
  // call the erase function
  HAL_FLASHEx_Erase(&f, &PageError);
  uint8_t loc =0;
  switch(dataType)
  {
	  case DATA_TYPE_16:
	  {
		  Size = Size/2;
		  for(uint32_t i=0; i<Size; i++)
		  {
			  loc = i*4;
			  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , addr +loc, ((uint16_t *)data)[i]);
		  }
	  }
	  break;
	  case DATA_TYPE_32:
	  {
		  Size = Size/4;
		  for(uint32_t i=0; i<Size; i++)
		  {
			  loc = i*4;
			  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD , addr +loc, ((uint32_t *)data)[i]);
		  }
	  }
	  break;
	  case DATA_TYPE_8:
		  break;
  }
  //4, lock FLASH
  HAL_FLASH_Lock();
}

void Flash_WriteData_without_erase(uint32_t addr,void *data,uint16_t Size, DataTypeDef dataType)
{
	//1, unlock FLASH
  HAL_FLASH_Unlock();
  uint8_t loc =0;
  switch(dataType)
  {
  	  case DATA_TYPE_16:
  	  {
  		  Size = Size/2;
  		  for(uint32_t i=0; i<Size; i++)
  		  {
  			  loc = i*4;
  			  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , addr +loc, ((uint16_t *)data)[i]);
  		  }
  	  }
  	  break;
  	  case DATA_TYPE_32:
  	  {
		  Size = Size/4;
		  for(uint32_t i=0; i<Size; i++)
		  {
			  loc = i*4;
			  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD , addr +loc, ((uint32_t *)data)[i]);
		  }
	  }
	  break;
	  case DATA_TYPE_8:
  	  {
  		  //Size = Size;
  		  for(uint32_t i=0; i<Size; i++)
  		  {
  			  loc = i*4;
  			HAL_FLASH_Program_8bit(FLASH_TYPEPROGRAM_HALFWORD , addr +loc, ((uint8_t *)data)[i]);
  		  }
  	  }
		  break;
  	}
  HAL_FLASH_Lock();
}


//void Flash_WriteData(uint32_t addr,uint16_t *data,uint16_t Size)
//{
//         //1, unlock FLASH
//  HAL_FLASH_Unlock();
//
//         //2, erase FLASH
//         // Initialize FLASH_EraseInitTypeDef
//    FLASH_EraseInitTypeDef f;
//    f.TypeErase = FLASH_TYPEERASE_PAGES;
//    f.PageAddress = addr;
//    f.NbPages = 1;
//         // Set PageError
//    uint32_t PageError = 0;
//         // call the erase function
//    HAL_FLASHEx_Erase(&f, &PageError);
//
//         //3, burn to FLASH
//	  uint32_t TempBuf = 0;
//	  for(uint32_t i = 0;i< Size ;i++)
//	 {
//		 TempBuf = ~(*(data+i));
//		 TempBuf <<= 16;
//		  TempBuf += *(data+i); //Reverse check
//		 HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD , addr + i * 4, TempBuf);
//	 }
//
//         //4, lock FLASH
//  HAL_FLASH_Lock();
//}

uint8_t Flash_ReadData(uint32_t addr,void *data,uint32_t Size, DataTypeDef dataType)
{
  uint32_t temp;
	uint8_t result = 1;
	uint8_t loc =0;
//  for(uint32_t i = 0;i< Size ;i++)
//	{
//	  	  loc = i*4;
//			temp = *(__IO uint32_t*)(addr + loc);
//			*(data+i) = (uint16_t)temp;
//	}

  switch(dataType)
   {
   	  case DATA_TYPE_16:
   	  {
   		Size = Size/2;
   		  for(uint32_t i=0; i<Size; i++)
   		  {
   			  loc = i*4;
  			temp = *(__IO uint32_t*)(addr + loc);
  			*(((uint16_t *)data)+i) = (uint16_t)temp;
   		  }
   	  }
   	  break;
   	  case DATA_TYPE_32:
   	  {
   		Size = Size/4;
 		  for(uint32_t i=0; i<Size; i++)
 		  {
 			  loc = i*4;
 				temp = *(__IO uint32_t*)(addr + loc);
 				*(((uint32_t *)data)+i) = (uint32_t)temp;
 		  }
 	  }
 	  break;
 	  case DATA_TYPE_8:
   	  {
   		//Size = Size/4;
   		 		  for(uint32_t i=0; i<Size; i++)
   		 		  {
   		 			  loc = i*4;
   		 				temp = *(__IO uint32_t*)(addr + loc);
   		 				*(((uint8_t *)data)+i) = (uint8_t)temp;
   		 		  }
   	  }
 		  break;
   	}
  return result;
}

//pankaj
HAL_StatusTypeDef HAL_FLASH_Program_8bit(uint32_t TypeProgram, uint32_t Address, uint8_t Data)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  uint8_t index = 0U;
  uint8_t nbiterations = 0U;

  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Check the parameters */
  assert_param(IS_FLASH_TYPEPROGRAM(TypeProgram));
  assert_param(IS_FLASH_PROGRAM_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if(status == HAL_OK)
  {
	  /* Clean the error context */
	  pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;
	  /* Proceed to program the new data */
	  SET_BIT(FLASH->CR, FLASH_CR_PG);
	  /* Write data in the address */
	  *(__IO uint16_t*)Address = (uint8_t)(Data);
	  /* Wait for last operation to be completed */
	  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
	  /* If the program operation is completed, disable the PG Bit */
	  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
  }

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;
}

//uint8_t Flash_ReadData(uint32_t addr,uint16_t *data,uint16_t Size)
//{
//  uint32_t temp;
//	uint8_t result = 1;
//	uint8_t loc =0;
//  for(uint32_t i = 0;i< Size ;i++)
//	{
//	  	  loc = i*4;
//			temp = *(__IO uint32_t*)(addr + loc);
//		  if((uint16_t)temp == (uint16_t)(~(temp>>16)))
//			{
//				*(data+i) = (uint16_t)temp;
//			}
//			else
//			{
//				result = 0;
//			}
//	}
//  return result;
//}
