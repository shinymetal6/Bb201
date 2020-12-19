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
	bbSystemInit();
	flash_EraseSector(HEADER_SECTOR);
	flash_WriteBytes((uint8_t *)&System ,flash_SectorToAddress(HEADER_SECTOR),sizeof(System));
}

void InitPeripherals(void)
{
	ChangeSampleFrequency(System.sampling_frequency[OUTCHANNEL_0] , OUTCHANNEL_0);
	ChangeSampleFrequency(System.sampling_frequency[OUTCHANNEL_1] , OUTCHANNEL_1);
}

uint32_t SetupFlash(void)
{
	LoadSettingsFromFlash();
	if ( strncmp(System.Header,"B201_a",6 ) != 0 )
	{
		FlashFormat();
		LoadSettingsFromFlash();
	}
	InitPeripherals();
	return   flash_ReadID();
}

void StoreSettingsInFlash(void)
{
	flash_EraseSector(HEADER_SECTOR);
	flash_WriteBytes((uint8_t *)&System ,flash_SectorToAddress(HEADER_SECTOR),sizeof(System));
}

void LoadSettingsFromFlash(void)
{
	flash_ReadBytes((uint8_t *)&System,flash_SectorToAddress(HEADER_SECTOR),sizeof(System));
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

