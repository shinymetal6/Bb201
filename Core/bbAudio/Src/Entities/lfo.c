/*
 * lfo.c
 *
 *  Created on: Jan 10, 2021
 *      Author: fil
 */

#include "main.h"
#include "math.h"
#include "lfo.h"

#define	LFOTAB_SIZE_IN_BITS		8
#define	LFOTAB_SIZE				(1 << LFOTAB_SIZE_IN_BITS)
#define	LFO_PRECISION			14
#define	LFO_PRECISION_SHIFT		(1<<LFO_PRECISION)

AUDIO_BUFFERS __attribute__ ((aligned (16))) uint16_t	lfo_buf[NUMLFO][NUMBER_OF_AUDIO_SAMPLES];

__attribute__ ((aligned (16))) LFOTypeDef	Lfo[NUMLFO];
uint16_t	lfotab[NUMLFO][LFOTAB_SIZE];

static	void i_lfo(uint32_t lfo_number)
{
uint16_t	i,start,end;
uint16_t	angle;
	get_limits(&start,&end,(uint32_t *)Lfo[lfo_number].buffer_flag_ptr);
	for ( i=start;i<end;i++)
	{
		angle = (uint8_t )(Lfo[lfo_number].current_phase >> LFO_PRECISION);
		lfo_buf[lfo_number][i] = (lfotab[lfo_number][angle] * ( *(Lfo[lfo_number].volume_control_ptr )) >> DAC_BIT);
		Lfo[lfo_number].current_phase += Lfo[lfo_number].delta_phase;
	}
}

static void LfoSinCompile(uint16_t lfo_number)
{
uint32_t	i;
	for(i=0;i<LFOTAB_SIZE;i++)
	{
		lfotab[lfo_number][i] = (uint16_t )(HALF_DAC_RESOLUTION + sin((i*M_PI*2)/LFOTAB_SIZE)* (HALF_DAC_RESOLUTION-1));
	}
}

void DoLfo(void)
{
uint16_t	start,end,lfo_number;

	get_limits(&start,&end,(uint32_t *)Lfo[0].buffer_flag_ptr);
	for(lfo_number=0;lfo_number<NUMLFO;lfo_number++)
	{
		i_lfo(lfo_number);
	}
}

void LfoSetFrequency(uint16_t	lfo_number,uint32_t frequency)
{
double		delta_phase;
	Lfo[lfo_number].frequency = frequency;
	delta_phase = ((double )LFOTAB_SIZE / (double )(Lfo[lfo_number].sampling_frequency / (double )Lfo[lfo_number].frequency ));
	Lfo[lfo_number].delta_phase  = (uint16_t )(delta_phase * (double )LFO_PRECISION_SHIFT);
}

uint16_t	fake_lfo_volume_control[NUMLFO];

uint32_t InitLfo(uint32_t sampling_frequency)
{
uint16_t	lfo_number;
double		delta_phase;

	for(lfo_number=0;lfo_number<NUMLFO;lfo_number++)
	{
		fake_lfo_volume_control[lfo_number] = 4096;

		Lfo[lfo_number].sampling_frequency = sampling_frequency;
		Lfo[lfo_number].frequency = 375;
		Lfo[lfo_number].waveform = SINE;
		Lfo[lfo_number].volume = VCO_MAX_VOLUME;
		Lfo[lfo_number].current_phase = 0;
		delta_phase = ((double )LFOTAB_SIZE / (double )(Lfo[lfo_number].sampling_frequency / (double )Lfo[lfo_number].frequency ));
		Lfo[lfo_number].delta_phase  = (uint16_t )(delta_phase * (double )LFO_PRECISION_SHIFT);
		Lfo[lfo_number].buffer_flag_ptr = get_bufferhalf(0);
		Lfo[lfo_number].volume_control_ptr = &fake_lfo_volume_control[lfo_number];
		LfoSinCompile(lfo_number);
	}

	return NUMLFO*2;
}


