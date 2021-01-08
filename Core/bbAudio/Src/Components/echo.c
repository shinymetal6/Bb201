/*
 * n_echo.c
 *
 *  Created on: Jan 8, 2021
 *      Author: fil
 */


#include "main.h"

#define	ECHO_FUNCTION_NAME	"Echo"

static	void static_do_echo(uint16_t stage_number)
{
S_Component		*component = &Component[stage_number];
uint16_t		i,start,end;
uint16_t 		*in_buffer=(uint16_t *)component->in_buffer0;
uint16_t		*out_buffer=(uint16_t *)component->out_buffer0;

	get_limits(&start,&end,(uint32_t *)component->half_ptr);
	for ( i=start;i<end;i++)
		out_buffer[i] = in_buffer[i];
}


uint32_t ECHOInit(uint32_t in_buffer, uint32_t out_buffer, uint32_t channel, uint32_t stage)
{
S_Component		component;

	bzero(&component,sizeof(S_Component));
	component.FuncPtr =  (void *)&static_do_echo;
	component.in_buffer0 = in_buffer;
	component.out_buffer0 = out_buffer;
	component.channel = channel;
	component.half_ptr = get_bufferhalf(channel);
	strncat(component.functionName,ECHO_FUNCTION_NAME,strlen(ECHO_FUNCTION_NAME));
	setOutStage((S_Component *)&component,channel,stage);
	stage++;
	return stage;
}
