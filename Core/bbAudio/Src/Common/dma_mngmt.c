/*
 * dma_mngmt.c
 *
 *  Created on: Nov 21, 2020
 *      Author: fil
 */

#include "main.h"


void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	if ( hadc == AUDIO_0_2_ADC)
	{
		SystemFlags.half_in_ch0 = HALF_BUFFER_FLAG;
		SystemFlags.audioin_buffer_ready_ch0 = 1;
	}
	if ( hadc == AUDIO_1_3_ADC)
	{
		SystemFlags.half_in_ch1 = HALF_BUFFER_FLAG;
		SystemFlags.audioin_buffer_ready_ch1 = 1;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if ( hadc == AUDIO_0_2_ADC)
	{
		SystemFlags.half_in_ch0 = FULL_BUFFER_FLAG;
		SystemFlags.audioin_buffer_ready_ch0 = 1;
	}
	if ( hadc == AUDIO_1_3_ADC)
	{
		SystemFlags.half_in_ch1 = FULL_BUFFER_FLAG;
		SystemFlags.audioin_buffer_ready_ch1 = 1;
	}
	if ( hadc == CONTROL_ADC)
		SystemFlags.control_ready = 1;
}
