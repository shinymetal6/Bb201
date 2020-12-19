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
