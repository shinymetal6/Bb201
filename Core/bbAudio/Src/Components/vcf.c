/*
 * vcf.c
 *
 *  Created on: Jan 3, 2021
 *      Author: fil
 *      Simple Moog VCF implementation
 */

#include "main.h"
#include "math.h"
#define	VCF_FUNCTION_NAME	"VCF"

double b0, b1, b2, b3, b4;  //filter buffers

uint16_t filter(uint16_t sample, uint32_t filterCutoff, uint32_t resonance, uint32_t filterType, uint32_t channel)
{
double input = (double )sample;
double res1 = (resonance/HALF_DAC_RESOLUTION);
double fc = (double )filterCutoff / (double )SystemParameters.sampling_frequency[channel] * 2.0;
double f, p, q;             //filter coefficients
double t1, t2;              //temporary buffers
double	eqsample;

	input /= HALF_DAC_RESOLUTION;
	input -= 1.0;

	q = 1.0 - fc;
	p = fc + 0.8f * fc * q;
	f = p + p - 1.0;
	q = res1 * (1.0 + 0.5 * q * (1.0 - q + 5.6 * q * q));

	input -= q * b4;                          //feedback
	t1 = b1;  b1 = (input + b0) * p - b1 * f;
	t2 = b2;  b2 = (b1 + t1) * p - b2 * f;
	t1 = b3;  b3 = (b2 + t2) * p - b3 * f;
			  b4 = (b3 + t1) * p - b4 * f;
	b4 = b4 - b4 * b4 * b4 * 0.166667f;    //clipping
	b0 = input;
	switch ( filterType)
	{
	case	LP	: eqsample = b4; break;					// Lowpass  output:  b4
	case	HP	: eqsample = input - b4; break;			// Highpass output:  in - b4;
	case	BP	: eqsample = 3.0f * (b3 - b4); break;	// Bandpass output:  3.0f * (b3 - b4);
	default		: eqsample = b4; break;					// Defaults to Lowpass  output:  b4
	}
	eqsample *= HALF_DAC_RESOLUTION;
	eqsample += HALF_DAC_RESOLUTION;
	return (uint16_t )eqsample;
}

/*
static	void static_do_vcf(uint16_t *in_buffer,uint16_t *out_buffer, uint32_t *half_in, uint32_t filterCutoff, uint32_t filterResonance, uint32_t filterType, uint32_t channel)
{
uint16_t	i,start,end;
	get_limits(&start,&end,half_in);
	for ( i=start;i<end;i++)
		out_buffer[i] = filter(in_buffer[i],filterCutoff,filterResonance,filterType,channel);
}

uint32_t VCFInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer, uint32_t channel)
{
uint32_t filterResonance = HALF_DAC_RESOLUTION/2;	// max HALF_DAC_RESOLUTION
uint32_t filterType = LP;	// HP, BP or LP
uint32_t frequency = 2000;

	setOutStage((uint32_t )&static_do_vcf,in_buffer, out_buffer, get_bufferhalf(channel), frequency,filterResonance,filterType,0,0,0, channel, in_stage,"VCF");

	stage++;
	return 0;
}
*/

static	void static_do_vcf(uint16_t stage_number)
{
S_Component		*component = &Component[stage_number];
uint16_t		i,start,end;
uint16_t		channel=component->channel;
uint16_t 		*in_buffer=(uint16_t *)component->in_buffer0;
uint16_t		*out_buffer=(uint16_t *)component->out_buffer0;
uint32_t		*filterCutoff=(uint32_t *)component->volume_ch0_source_ptr;
uint32_t		*filterResonance=(uint32_t *)component->volume_ch1_source_ptr;
uint32_t 		filterType = LP;	// HP, BP or LP

	get_limits(&start,&end,(uint32_t *)component->half_ptr);
	for ( i=start;i<end;i++)
		out_buffer[i] = filter(in_buffer[i],*filterCutoff,*filterResonance,filterType,channel);
}

uint32_t VCFInit(uint32_t in_buffer, uint32_t out_buffer, uint32_t channel, uint32_t stage)
{
S_Component		component;

	bzero(&component,sizeof(S_Component));
	component.FuncPtr =  (void *)&static_do_vcf;
	component.in_buffer0 = in_buffer;
	component.out_buffer0 = out_buffer;
	component.channel = channel;
	component.half_ptr = get_bufferhalf(channel);
	strncat(component.functionName,VCF_FUNCTION_NAME,strlen(VCF_FUNCTION_NAME));
	setOutStage((S_Component *)&component,channel,stage);
	stage++;
	return stage;
}

