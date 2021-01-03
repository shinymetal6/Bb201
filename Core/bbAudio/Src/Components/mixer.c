/*
 * mixer.c
 *
 *  Created on: Dec 13, 2020
 *      Author: fil
 */

#include "main.h"

static	void static_do_mixer(uint16_t *in_buffer1,uint16_t *in_buffer2,uint16_t *out_buffer, uint16_t *control, uint32_t *half_in)
{
uint16_t	i,start,end;

	get_limits(&start,&end,half_in);
	for ( i=start;i<end;i++)
		out_buffer[i] = (in_buffer1[i] + in_buffer2[i] ) >> 1;
}

uint32_t MixerInit(uint32_t in_stage,uint32_t in_buffer1,uint32_t in_buffer2, uint32_t out_buffer,uint32_t control0,uint32_t control1, uint32_t channel)
{
	setOutStage((uint32_t )&static_do_mixer, in_buffer1, in_buffer2 , out_buffer, control0, control1, get_bufferhalf(channel), channel, in_stage,"Mixer");

	stage++;
	return 0;
}
