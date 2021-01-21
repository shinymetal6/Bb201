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
AUDIO_BUFFERS	__attribute__ ((aligned (16)))	S_Component		Component[NUMSTAGES];
__attribute__ ((aligned (16))) SystemParametersTypeDef			SystemParameters;
__attribute__ ((aligned (16))) SystemFlagsTypeDef				SystemFlags;

uint32_t setOutStage(S_Component *component,uint16_t channel,uint32_t stage_number)
{
S_Component		*ComponentStruct = &Component[stage_number];

	memcpy(ComponentStruct,component,sizeof(S_Component));
	stage_number++;
	return stage_number;
}

static void	EmptyComponent(uint16_t channel,uint16_t index)
{
	return;
}

static void InitNFunnel(void)
{
uint32_t	i;
	for(i=0;i<NUMSTAGES;i++)
	{
		bzero(&Component[i],sizeof(S_Component));
		Component[i].FuncPtr =  (void *)EmptyComponent;
	}
}

void DoFunnelOut(void)
{
uint16_t	index;
	debug_1();
	DoVco();
	DoLfo();
	for(index=0;index<NUMSTAGES;index++)
		(*Component[index].FuncPtr)(OUTCHANNEL_0, index);
	clear_buffer_ready_flag();
	debug_0();
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
	LcdInit();
	InitNFunnel();
	SetupFlash();
	AudioInit();
	ControlInit();
	InitVco(SystemParameters.sampling_frequency[0]);
	//InitLfo(SystemParameters.sampling_frequency[0]);
	//EnvelopeInit();
	//EnvelopeInsert();
	system_timer_start();
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
	return 0;
}

uint32_t ReportFunnelEntries(uint32_t channel)
{
	return 0;
}

char *ReportFunnelName(uint32_t channel,uint32_t funnel_entry)
{
	return 0;
}

void debug_1(void)
{
	HAL_GPIO_WritePin(LOG_GPIO_Port, LOG_Pin, GPIO_PIN_SET);
}

void debug_0(void)
{
	HAL_GPIO_WritePin(LOG_GPIO_Port, LOG_Pin, GPIO_PIN_RESET);
}


