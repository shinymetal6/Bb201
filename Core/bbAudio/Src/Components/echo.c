/*
 * echo.c
 *
 *  Created on: Dec 9, 2020
 *      Author: fil
 */
#include "main.h"

static	void static_do_echo(uint32_t *in_buffer,uint32_t *out_buffer, uint8_t *half_in)
{
uint16_t	i,start,end;
	get_limits(&start,&end,half_in);
	for ( i=start;i<end;i++)
		out_buffer[i] = in_buffer[i];
}

uint32_t ECHOInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer, uint32_t channel)
{
	setOutStage((uint32_t )&static_do_echo,in_buffer, out_buffer, get_bufferhalf(channel), 0, 0, 0, channel, in_stage);
	stage++;
	return 0;
}
