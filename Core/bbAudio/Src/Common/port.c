/*
 * port.c
 *
 *  Created on: Dec 5, 2020
 *      Author: fil
 */
#include "main.h"

void audio_timer_start(void)
{
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim6);
}

void control_timer_start(void)
{
	HAL_LPTIM_Counter_Start_IT(CONTROL_LOOP_TIMER,3125);
}

void change_tim_frequency(uint32_t sampling_frequency , uint32_t channel)
{
	if ( channel == OUTCHANNEL_0)
		htim4.Instance->ARR= TIMERS_FREQ / sampling_frequency;
	if ( channel == OUTCHANNEL_1)
		htim6.Instance->ARR= TIMERS_FREQ / sampling_frequency;
}
