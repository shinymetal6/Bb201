/*
 * float_fir.c
 *
 *  Created on: Dec 12, 2020
 *      Author: fil
 */
#include "main.h"
#include "arm_math.h"
#include "fir.h"

#define BLOCK_SIZE      64

static float32_t 		firStateF32[BLOCK_SIZE + NUM_TAPS - 1];
arm_fir_instance_f32 	S;
float	input[HALF_NUMBER_OF_AUDIO_SAMPLES];
float	output[HALF_NUMBER_OF_AUDIO_SAMPLES];

static	void static_do_float_fir(uint32_t *in_buffer,uint32_t *out_buffer,uint8_t *half_in)
{
uint16_t	i,start,end;

	get_limits(&start,&end,half_in);
	arm_q31_to_float((q31_t *)&in_buffer[start],input,HALF_NUMBER_OF_AUDIO_SAMPLES);
	arm_fir_f32(&S, input , output, BLOCK_SIZE);
	arm_float_to_q31(output,(q31_t *)&out_buffer[start],HALF_NUMBER_OF_AUDIO_SAMPLES);
	for(i=start;i<end;i++)
		out_buffer[i] = (uint32_t )(out_buffer[i]+2048);


}

uint32_t FloatFirInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer,float fFIRCoef[NUM_TAPS], uint32_t channel)
{
	arm_fir_init_f32(&S, NUM_TAPS, fFIRCoef, &firStateF32[0], BLOCK_SIZE);
	//setOutStage(in_stage , (uint32_t )&static_do_float_fir,in_buffer,out_buffer,get_bufferhalf(channel),0,channel);
	setOutStage((uint32_t )&static_do_float_fir,in_buffer, out_buffer, get_bufferhalf(channel), 0, 0, 0, 0, in_stage,"FloatFIR");
	stage++;
	return 0;
}
