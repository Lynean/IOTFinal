/*
 * flashmem.c
 *
 *  Created on: Nov 25, 2024
 *      Author: Admin
 */


#include "flashmem.h"
#include <stdint.h>

#define DATA_SIZE sizeof(flash_datatype)

__IO uint8_t uwData32 = 0;

typedef uint64_t flash_datatype;
uint32_t uwStartSector;
uint32_t uwEndSector;
uint32_t uwSectorCounter;
uint32_t uwAddress = 0;


static uint32_t GetSector(uint32_t Address);

void flashConfig(){
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
	                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);


	//Get available flash range
	uwStartSector = GetSector(FLASH_USER_START_ADDR);
	uwEndSector = GetSector(FLASH_USER_END_ADDR);

	/* Start the erase operation */
	uwSectorCounter = uwStartSector;
	uwAddress = FLASH_USER_START_ADDR;
}

FLASH_Status flashWrite(uint32_t ID, uint32_t data){
	FLASH_Unlock();
	uint32_t currentAddress = uwAddress + ID*4;
	return FLASH_ProgramWord(currentAddress, data);

}

uint32_t flashReadWord(uint32_t ID){
	FLASH_Unlock();
	uint32_t currentAddress = uwAddress + ID*4;
	uwData32 = *(__IO uint32_t*)currentAddress;
	return uwData32;

}
//Must clear before writing
FLASH_Status flashClear(){
	FLASH_Unlock();
	uint32_t sector;
	sector = GetSector(uwAddress);
	return FLASH_EraseSector(sector, VoltageRange_3);

}

static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;
	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_Sector_6;
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_Sector_8;
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_Sector_9;
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_Sector_10;
	}
	else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
	{
		sector = FLASH_Sector_11;
	}
	/* USE_STM324x7I_EVAL or USE_STM324x9I_EVAL */
	return (uint32_t)sector;
}

