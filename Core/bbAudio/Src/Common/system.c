/*
 * system.c
 *
 *  Created on: Dec 9, 2020
 *      Author: fil
 */

#include "main.h"
#include <stdio.h>
#include <string.h>

uint32_t	stage=0;
__attribute__ ((aligned (16))) ProgramTypeDef			Program_ch0[NUMSTAGES];
__attribute__ ((aligned (16))) ProgramTypeDef			Program_ch1[NUMSTAGES];
__attribute__ ((aligned (16))) SystemParametersTypeDef	SystemParameters;
__attribute__ ((aligned (16))) SystemFlagsTypeDef		SystemFlags;

uint32_t setOutStage(uint32_t function_ptr,uint32_t in_buffer1,uint32_t in_buffer2, uint32_t out_buffer, uint32_t control_buffer1, uint32_t control_buffer2, uint32_t aux, uint32_t channel, uint32_t stage_number,char *function)
{
	if ( stage_number >= NUMSTAGES )
		return 1;
	if ( channel == OUTCHANNEL_0 )
	{
		Program_ch0[stage_number].valid =  PROGRAM_VALID;
		Program_ch0[stage_number].FuncPtr =  (void *)function_ptr;
		Program_ch0[stage_number].in_buffer1 =  in_buffer1;
		Program_ch0[stage_number].in_buffer2 =  in_buffer2;
		Program_ch0[stage_number].out_buffer =  out_buffer;
		Program_ch0[stage_number].control_buffer1 =  control_buffer1;
		Program_ch0[stage_number].control_buffer2 =  control_buffer2;
		Program_ch0[stage_number].aux =  aux;
		Program_ch0[stage_number].aux =  channel;
		Program_ch0[stage_number].on_stage =  stage_number;
		sprintf((char *)Program_ch0[stage_number].function,function,strlen(function));
	}
	else
	{
		Program_ch1[stage_number].valid =  PROGRAM_VALID;
		Program_ch1[stage_number].FuncPtr =  (void *)function_ptr;
		Program_ch1[stage_number].in_buffer1 =  in_buffer1;
		Program_ch1[stage_number].in_buffer2 =  in_buffer2;
		Program_ch1[stage_number].out_buffer =  out_buffer;
		Program_ch1[stage_number].control_buffer1 =  control_buffer1;
		Program_ch1[stage_number].control_buffer2 =  control_buffer2;
		Program_ch1[stage_number].aux =  aux;
		Program_ch1[stage_number].channel =  channel;
		Program_ch1[stage_number].on_stage =  stage_number;
		sprintf((char *)Program_ch1[stage_number].function,function,strlen(function));
	}
	return 0;
}


void DoFunnelOut(void)
{
uint32_t	i;
	debug_1();
	DoOscillators();
	debug_0();

	for(i=0;i<NUMSTAGES;i++)
	{
		  if (( Program_ch0[i].FuncPtr != NULL) && ( Program_ch0[i].channel == OUTCHANNEL_0) && ( SystemFlags.audioin_buffer_ready_ch0 == 1 ) && (Program_ch0[i].valid =  PROGRAM_VALID ))
		  {
				(*Program_ch0[i].FuncPtr)(Program_ch0[i].in_buffer1,Program_ch0[i].in_buffer2,Program_ch0[i].out_buffer,Program_ch0[i].control_buffer1,Program_ch0[i].control_buffer2,Program_ch0[i].aux,Program_ch0[i].channel);
		  }
		  else
			  break;
	}
	for(i=0;i<NUMSTAGES;i++)
	{
		  if (( Program_ch1[i].FuncPtr != NULL) && ( Program_ch1[i].channel == OUTCHANNEL_1) && ( SystemFlags.audioin_buffer_ready_ch1 == 1 )&& (Program_ch1[i].valid =  PROGRAM_VALID ))
		  {
				(*Program_ch1[i].FuncPtr)(Program_ch1[i].in_buffer1,Program_ch1[i].in_buffer2,Program_ch1[i].out_buffer,Program_ch1[i].control_buffer1,Program_ch1[i].control_buffer2,Program_ch1[i].aux,Program_ch1[i].channel);
		  }
		  else
			  break;
	}
	clear_buffer_ready_flag();
}

void bbSystem_SystemSetDefaults(void)
{
	sprintf(SystemParameters.Header,bbNAME);
	sprintf(SystemParameters.Version,bbVERSION);
	SystemParameters.sampling_frequency[0] = 96000;
	SystemParameters.sampling_frequency[1] = 96000;
}

void bbSystemInit(void)
{
	bbSystem_SystemSetDefaults();
	SetupFlash();
	AudioInit();
	ControlInit();
	HAL_TIM_PWM_Start_IT(&htim15,TIM_CHANNEL_1);

}

void ChangeSampleFrequency(uint32_t sampling_frequency , uint32_t channel)
{
	if ( channel == OUTCHANNEL_0)
		SystemParameters.sampling_frequency[OUTCHANNEL_0] = sampling_frequency;
	if ( channel == OUTCHANNEL_1)
		SystemParameters.sampling_frequency[OUTCHANNEL_1] = sampling_frequency;
	change_tim_frequency(sampling_frequency , channel);
}

uint32_t ClearFunnelEntries(void)
{
	bzero(&Program_ch0, sizeof(Program_ch0));
	bzero(&Program_ch1, sizeof(Program_ch1));
	bzero(&audio_pipe, sizeof(audio_pipe));
	stage=0;
	return sizeof(Program_ch1);
}

uint32_t ReportFunnelEntries(uint32_t channel)
{
uint32_t elements=0,i;
	for(i=0;i<NUMSTAGES;i++)
	{
		if ( channel == OUTCHANNEL_0)
		{
			if ( Program_ch0[i].FuncPtr != NULL )
				elements++;
			else
				break;
		}
		if ( channel == OUTCHANNEL_1)
		{
			if ( Program_ch1[i].FuncPtr != NULL )
				elements++;
			else
				break;
		}
	}
	return elements;
}

char *ReportFunnelName(uint32_t channel,uint32_t funnel_entry)
{
uint32_t	i;
	for(i=0;i<NUMSTAGES;i++)
	{
		if ( channel == OUTCHANNEL_0)
			return Program_ch0[funnel_entry].function;
		if ( channel == OUTCHANNEL_1)
			return Program_ch1[funnel_entry].function;
	}
	return "Empty";
}

void debug_1(void)
{
	HAL_GPIO_WritePin(LOG_GPIO_Port, LOG_Pin, GPIO_PIN_SET);
}

void debug_0(void)
{
	HAL_GPIO_WritePin(LOG_GPIO_Port, LOG_Pin, GPIO_PIN_RESET);
}


