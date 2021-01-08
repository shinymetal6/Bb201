/*
 * control_init.c
 *
 *  Created on: Dec 3, 2020
 *      Author: fil
 */
#include "main.h"

NO_CACHE_BUFFERS	__attribute__ ((aligned (16))) AF_ControlTypeDef		control_buf;
uint16_t	control_ready;

void ControlInit(void)
{
	HAL_ADCEx_Calibration_Start(CONTROL_ADC, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(CONTROL_ADC, (uint32_t* )&control_buf , CONTROLBUF_LEN);
	control_timer_start();
}

void DoControls(void)
{

}
