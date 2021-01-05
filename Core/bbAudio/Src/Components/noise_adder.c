/*
 * noise_adder.c
 *
 *  Created on: Jan 5, 2021
 *      Author: fil
 */

#include "main.h"

static	void static_noiseadder(uint16_t *in_buffer,uint16_t *out_buffer, uint32_t *half_in, uint32_t depth )
{
uint16_t	i,start,end;
double	scaled_in, scaled_noise,in_scaling, noise_scaling;
	get_limits(&start,&end,half_in);
	in_scaling = (double )depth / (double )DAC_BIT;
	noise_scaling = 1.0 - in_scaling;
	for ( i=start;i<end;i++)
	{
		scaled_in = (double )in_buffer[i] * in_scaling;
		scaled_noise = (double )RNG_Buffer[i] * noise_scaling;
		out_buffer[i] = (uint16_t )(scaled_in + scaled_noise) & (DAC_RESOLUTION - 1);
	}
}

uint32_t NoiseAdderInit(uint32_t in_stage, uint32_t in_buffer, uint32_t out_buffer, uint32_t depth, uint32_t channel)
{
	if ( depth > DAC_BIT )
		depth = DAC_BIT;
	setOutStage((uint32_t )&static_noiseadder, in_buffer, out_buffer, get_bufferhalf(channel), depth , 0, 0, 0, 0, 0, channel, in_stage,"Echo");
	stage++;
	return 0;
}

