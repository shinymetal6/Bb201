/*
 * echo.c
 *
 *  Created on: Dec 9, 2020
 *      Author: fil
 */
#include "main.h"

#define	VCA_FUNCTION_NAME	"VCA"

static	void static_do_vca(uint16_t stage_number)
{
uint16_t	i,start,end;
S_Component		*component = &Component[stage_number];
uint16_t 		*in_buffer=(uint16_t *)component->in_buffer0;
uint16_t		*out_buffer=(uint16_t *)component->out_buffer0;
uint16_t		*volume_ch0=(uint16_t *)component->volume_ch0_source_ptr;

	get_limits(&start,&end,(uint32_t *)component->half_ptr);
	for ( i=start;i<end;i++)
		out_buffer[i] = (in_buffer[i] * ( *volume_ch0 )) >> BIT_RESOLUTION;
}

uint32_t VCAInit(uint32_t in_buffer, uint32_t out_buffer,uint32_t volume_ch0_source_ptr, uint32_t channel,uint32_t stage)
{
S_Component		component;

	bzero(&component,sizeof(S_Component));
	component.FuncPtr =  (void *)&static_do_vca;
	component.in_buffer0 = in_buffer;
	component.out_buffer0 = out_buffer;
	component.volume_ch0_source_ptr= volume_ch0_source_ptr;
	component.channel = channel;
	component.half_ptr = get_bufferhalf(channel);
	strncat(component.functionName,VCA_FUNCTION_NAME,strlen(VCA_FUNCTION_NAME));
	setOutStage((S_Component *)&component,channel,stage);
	stage++;
	return stage;
}
