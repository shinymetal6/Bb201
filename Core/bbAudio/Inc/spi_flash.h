/*
 * spi_flash.h
 *
 *  Created on: Dec 19, 2020
 *      Author: fil
 */

#ifndef BBAUDIO_INC_SPI_FLASH_H_
#define BBAUDIO_INC_SPI_FLASH_H_

extern	void 		flash_WriteDisable(void);
extern	uint32_t	flash_SectorToAddress(uint32_t	Sector);
extern	uint32_t	flash_AddressToSector(uint32_t	Address);
extern	void 		flash_EraseSector(uint32_t Sector);
extern	void 		flash_ReadBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size);
extern	void 		flash_WriteBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size);
extern	uint32_t 	flash_ReadID(void);

#endif /* BBAUDIO_INC_SPI_FLASH_H_ */
