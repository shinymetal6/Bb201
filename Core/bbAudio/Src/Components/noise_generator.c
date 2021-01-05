/*
 * noise_generator.c
 *
 *  Created on: Jan 5, 2021
 *      Author: fil
 */

#include "main.h"

static	void static_noise_generator(uint16_t *out_buffer, uint32_t *half_in)
{
uint16_t	i,start,end;
	get_limits(&start,&end,half_in);
	for ( i=start;i<end;i++)
		out_buffer[i] = RNG_Buffer[i];
}

uint32_t NoiseGeneratorInit(uint32_t in_stage, uint32_t out_buffer, uint32_t channel)
{
	setOutStage((uint32_t )&static_noise_generator, out_buffer, get_bufferhalf(channel), 0, 0, 0, 0, 0, 0, 0, channel, in_stage,"NoiseGen");
	stage++;
	return 0;
}

