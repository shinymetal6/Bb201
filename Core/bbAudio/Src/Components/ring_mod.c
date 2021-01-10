/*
 * ring_mod.c
 *
 *  Created on: Dec 19, 2020
 *      Author: fil
 */

#include "main.h"

#define	RING_FUNCTION_NAME	"Ring"

static	void static_do_ring(uint16_t stage_number)
{
S_Component		*component = &Component[stage_number];
uint16_t		i,start,end;
uint16_t 		*in_buffer0=(uint16_t *)component->in_buffer0;
uint16_t 		*in_buffer1=(uint16_t *)component->in_buffer1;
uint16_t		*out_buffer=(uint16_t *)component->out_buffer0;
uint16_t		*volume_ch0=(uint16_t *)component->volume_ch0_source_ptr;
uint16_t		*volume_ch1=(uint16_t *)component->volume_ch1_source_ptr;

	get_limits(&start,&end,(uint32_t *)component->half_ptr);
	for ( i=start;i<end;i++)
		out_buffer[i] = (((in_buffer0[i]* ( *volume_ch0 )) >> BIT_RESOLUTION ) * ((in_buffer1[i]* ( *volume_ch1 )) >> BIT_RESOLUTION)) >> BIT_RESOLUTION;
}

uint32_t RINGInit(uint32_t in_buffer0, uint32_t in_buffer1, uint32_t out_buffer, uint32_t volume_ch0_source_ptr, uint32_t volume_ch1_source_ptr, uint32_t channel, uint32_t stage)
{
S_Component		component;

	bzero(&component,sizeof(S_Component));
	component.FuncPtr =  (void *)&static_do_ring;
	component.in_buffer0 = in_buffer0;
	component.in_buffer1 = in_buffer1;
	component.out_buffer0 = out_buffer;
	component.volume_ch0_source_ptr= volume_ch0_source_ptr;
	component.volume_ch1_source_ptr= volume_ch1_source_ptr;
	component.channel = channel;
	component.half_ptr = get_bufferhalf(channel);
	strncat(component.functionName,RING_FUNCTION_NAME,strlen(RING_FUNCTION_NAME));
	setOutStage((S_Component *)&component,channel,stage);
	stage++;
	return stage;
}
