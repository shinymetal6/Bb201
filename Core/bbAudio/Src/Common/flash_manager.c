/*
 * flash_manager.c
 *
 *  Created on: Dec 15, 2020
 *      Author: fil
 */

#include "main.h"
#include <stdio.h>
#include <string.h>
#include "flash_manager.h"
/*
 * Each sector is 4096 bytes
 * Sector 0 : header + setup data , see SystemParametersTypeDef
 * Sector 1 : program
 * Sector 2 : fir_table
 * Sector 3 : iir_table
 * Sector 4 ..  : user data
 */
#define	HEADER_SECTOR			0
#define	PROGRAM_SECTOR			1
#define	FIRTAB_SECTOR			2
#define	IIRTAB_SECTOR			3
#define	USERDATA_SECTOR			4

void FlashFormat(void)
{
	flash_EraseSector(HEADER_SECTOR);
	flash_WriteBytes((uint8_t *)&SystemParameters ,flash_SectorToAddress(HEADER_SECTOR),sizeof(SystemParameters));
}

void InitPeripherals(void)
{
	ChangeSampleFrequency(SystemParameters.sampling_frequency[OUTCHANNEL_0] , OUTCHANNEL_0);
	ChangeSampleFrequency(SystemParameters.sampling_frequency[OUTCHANNEL_1] , OUTCHANNEL_1);
}


uint32_t	FlashIs256	= 0;

uint32_t SetupFlash(void)
{
uint32_t	FlashID;
	FlashID = flash_ReadID();
	if (( FlashID & 0xff ) == F128 )	/* 128 MBits */
		SystemParameters.flash_capacity = F128;
	else if (( FlashID & 0xff ) == F256 )	/* 256 MBits */
		SystemParameters.flash_capacity = F256;
	else
		return -1;
	LoadSettingsFromFlash();
	if ( strncmp(SystemParameters.Header,bbNAME,6 ) != 0 )
	{
		bbSystem_SystemSetDefaults();
		FlashFormat();
		LoadSettingsFromFlash();
	}
	InitPeripherals();
	return FlashIs256;
}

void StoreSettingsInFlash(void)
{
	flash_EraseSector(HEADER_SECTOR);
	flash_WriteBytes((uint8_t *)&SystemParameters ,flash_SectorToAddress(HEADER_SECTOR),sizeof(SystemParameters));
}

void LoadSettingsFromFlash(void)
{
	flash_ReadBytes((uint8_t *)&SystemParameters,flash_SectorToAddress(HEADER_SECTOR),sizeof(SystemParameters));
}

void StoreProgramInFlash(void)
{
	flash_EraseSector(PROGRAM_SECTOR);
	flash_WriteBytes((uint8_t *)Program_ch0,flash_SectorToAddress(PROGRAM_SECTOR),sizeof(Program_ch0));
	flash_WriteBytes((uint8_t *)Program_ch1,flash_SectorToAddress(PROGRAM_SECTOR)+sizeof(Program_ch0),sizeof(Program_ch1));
}
void LoadProgramFromFlash(void)
{
	flash_ReadBytes((uint8_t *)&Program_ch0,flash_SectorToAddress(PROGRAM_SECTOR),sizeof(Program_ch0));
	flash_ReadBytes((uint8_t *)&Program_ch1,flash_SectorToAddress(PROGRAM_SECTOR)+sizeof(Program_ch0),sizeof(Program_ch1));
}

