/*
 * fir.c
 *
 *  Created on: Dec 5, 2020
 *      Author: fil
 */

#include "main.h"
#include "arm_math.h"
#include "fir.h"

#define BLOCK_SIZE      64

static q15_t 			firState[NUM_TAPS+BLOCK_SIZE];
q15_t					input[HALF_NUMBER_OF_AUDIO_SAMPLES],output[HALF_NUMBER_OF_AUDIO_SAMPLES];
arm_fir_instance_q15	S;
q15_t					FIRCoef[NUM_TAPS];

static	void static_do_fir(uint32_t *in_buffer,uint32_t *out_buffer,uint8_t *half_in)
{
uint16_t	i,k,start,end;

	get_limits(&start,&end,half_in);
	for(i=start,k=0;i<end;i++,k++)
		input[k] = (q15_t)(in_buffer[i]);
	arm_fir_fast_q15(&S, input , output, HALF_NUMBER_OF_AUDIO_SAMPLES);
	for(i=start,k=0;i<end;i++,k++)
		out_buffer[i] = (uint32_t)(output[k]+2048);
}

uint32_t q15FirInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer,float fFIRCoef[NUM_TAPS], uint32_t channel)
{
	arm_float_to_q15(fFIRCoef,FIRCoef,NUM_TAPS);
	if ( arm_fir_init_q15(&S, NUM_TAPS, FIRCoef, firState, BLOCK_SIZE) != ARM_MATH_SUCCESS )
		return 1;
	setOutStage((uint32_t )&static_do_fir,in_buffer, out_buffer, get_bufferhalf(channel), 0, 0, 0, 0, in_stage);
	stage++;
	return 0;
}



