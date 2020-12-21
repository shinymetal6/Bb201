/*
 * flash_manager.h
 *
 *  Created on: Dec 19, 2020
 *      Author: fil
 */

#ifndef BBAUDIO_INC_FLASH_MANAGER_H_
#define BBAUDIO_INC_FLASH_MANAGER_H_

#include "SpiFlash.h"

extern	uint32_t SetupFlash(void);
extern	void StoreSettingsInFlash(void);
extern	void LoadSettingsFromFlash(void);
extern	void StoreProgramInFlash(void);
extern	void LoadProgramFromFlash(void);


#endif /* BBAUDIO_INC_FLASH_MANAGER_H_ */
