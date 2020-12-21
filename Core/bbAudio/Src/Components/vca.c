/*
 * echo.c
 *
 *  Created on: Dec 9, 2020
 *      Author: fil
 */
#include "main.h"

static	void static_do_vca(uint32_t *in_buffer,uint32_t *out_buffer, uint16_t *control, uint32_t *half_in)
{
uint16_t	i,start,end;

	get_limits(&start,&end,half_in);
	for ( i=start;i<end;i++)
		out_buffer[i] = (in_buffer[i] * ( *control )) >> BIT_RESOLUTION;
}

uint32_t VCAInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer,uint32_t control, uint32_t channel)
{
	setOutStage((uint32_t )&static_do_vca,in_buffer, out_buffer,control, get_bufferhalf(channel), 0, 0, channel, in_stage,"VCA");
	stage++;
	return 0;
}
