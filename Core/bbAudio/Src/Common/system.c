/*
 * system.c
 *
 *  Created on: Dec 9, 2020
 *      Author: fil
 */

#include "main.h"

uint32_t	stage=0;
AF_OutTypeDef	Af_OUT_ch0[NUMSTAGES];
AF_OutTypeDef	Af_OUT_ch1[NUMSTAGES];

uint32_t setOutStage(uint32_t function_ptr,uint32_t in_buffer1,uint32_t in_buffer2, uint32_t out_buffer, uint32_t control_buffer1, uint32_t control_buffer2, uint32_t aux, uint32_t channel, uint32_t stage_number)
{
	if ( stage_number >= NUMSTAGES )
		return 1;
	if ( channel == OUTCHANNEL_0 )
	{
		Af_OUT_ch0[stage_number].FuncPtr =  (void *)function_ptr;
		Af_OUT_ch0[stage_number].in_buffer1 =  in_buffer1;
		Af_OUT_ch0[stage_number].in_buffer2 =  in_buffer2;
		Af_OUT_ch0[stage_number].out_buffer =  out_buffer;
		Af_OUT_ch0[stage_number].control_buffer1 =  control_buffer1;
		Af_OUT_ch0[stage_number].control_buffer2 =  control_buffer2;
		Af_OUT_ch0[stage_number].aux =  aux;
		Af_OUT_ch0[stage_number].aux =  channel;
		Af_OUT_ch0[stage_number].on_stage =  stage_number;
	}
	else
	{
		Af_OUT_ch1[stage_number].FuncPtr =  (void *)function_ptr;
		Af_OUT_ch1[stage_number].in_buffer1 =  in_buffer1;
		Af_OUT_ch1[stage_number].in_buffer2 =  in_buffer2;
		Af_OUT_ch1[stage_number].out_buffer =  out_buffer;
		Af_OUT_ch1[stage_number].control_buffer1 =  control_buffer1;
		Af_OUT_ch1[stage_number].control_buffer2 =  control_buffer2;
		Af_OUT_ch1[stage_number].aux =  aux;
		Af_OUT_ch1[stage_number].channel =  channel;
		Af_OUT_ch1[stage_number].on_stage =  stage_number;
	}
	return 0;
}

void DoFunnelOut(void)
{
uint32_t	i;
	debug_1();
	if ( audioin_buffer_ready_ch0 == 1 )
	{
		for(i=0;i<NUMSTAGES;i++)
		{
			  if ( Af_OUT_ch0[i].FuncPtr != NULL)
			  {
					(*Af_OUT_ch0[i].FuncPtr)(Af_OUT_ch0[i].in_buffer1,Af_OUT_ch0[i].in_buffer2,Af_OUT_ch0[i].out_buffer,Af_OUT_ch0[i].control_buffer1,Af_OUT_ch0[i].control_buffer2,Af_OUT_ch0[i].aux,Af_OUT_ch0[i].channel);
			  }
			  else
				  break;
		}
	}
	if ( audioin_buffer_ready_ch1 == 1 )
	{
		for(i=0;i<NUMSTAGES;i++)
		{
			  if ( Af_OUT_ch1[i].FuncPtr != NULL)
					(*Af_OUT_ch1[i].FuncPtr)(Af_OUT_ch1[i].in_buffer1,Af_OUT_ch1[i].in_buffer2,Af_OUT_ch1[i].out_buffer,Af_OUT_ch1[i].control_buffer1,Af_OUT_ch1[i].control_buffer2,Af_OUT_ch1[i].aux,Af_OUT_ch1[i].channel);
			  else
				break;
		}
	}
	debug_0();
}

void DoControl(void)
{

}

void debug_1(void)
{
	HAL_GPIO_WritePin(LOG_GPIO_Port, LOG_Pin, GPIO_PIN_SET);
}

void debug_0(void)
{
	HAL_GPIO_WritePin(LOG_GPIO_Port, LOG_Pin, GPIO_PIN_RESET);
}

