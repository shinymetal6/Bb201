/*
 * vcf.c
 *
 *  Created on: Jan 3, 2021
 *      Author: fil
 */

#include "main.h"

void vcf1(uint16_t freq , uint16_t res, uint16_t inputsample)
{
// Lowpass  output:  b4
// Highpass output:  in - b4;
// Bandpass output:  3.0f * (b3 - b4);
float frequency = (float )freq, resonance = (float )res, in = (float )inputsample;
float f, p, q;             //filter coefficients
float b0=0.0f, b1=0.0f, b2=0.0f, b3=0.0f, b4=0.0f;  //filter buffers (beware denormals!)
float t1, t2;              //temporary buffers

// Set coefficients given frequency & resonance [0.0...1.0]

  q = 1.0f - frequency;
  p = frequency + 0.8f * frequency * q;
  f = p + p - 1.0f;
  q = resonance * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));

// Filter (in [-1.0...+1.0])

  in -= q * b4;                          //feedback
  t1 = b1;  b1 = (in + b0) * p - b1 * f;
  t2 = b2;  b2 = (b1 + t1) * p - b2 * f;
  t1 = b3;  b3 = (b2 + t2) * p - b3 * f;
            b4 = (b3 + t1) * p - b4 * f;
  b4 = b4 - b4 * b4 * b4 * 0.166667f;    //clipping
  b0 = in;
}

double vcf2(double input, double fcutoff, double resonance)
{
/*
in[x] and out[x] are member variables, init to 0.0 the controls:

fc = cutoff, nearly linear [0,1] -> [0, fs/2]
res = resonance [0, 4] -> [no resonance, self-oscillation]
*/
  double f = fcutoff * 1.16;
  double fb = resonance * (1.0 - 0.15 * f * f);
  double in1=0,in2=0,in3=0,in4=0;
  double out1=0,out2=0,out3=0,out4=0;
  input -= out4 * fb;
  input *= 0.35013 * (f*f)*(f*f);
  out1 = input + 0.3 * in1 + (1 - f) * out1; // Pole 1
  in1  = input;
  out2 = out1 + 0.3 * in2 + (1 - f) * out2;  // Pole 2
  in2  = out1;
  out3 = out2 + 0.3 * in3 + (1 - f) * out3;  // Pole 3
  in3  = out2;
  out4 = out3 + 0.3 * in4 + (1 - f) * out4;  // Pole 4
  in4  = out3;
  return out4;
}

static	void static_do_vcf(uint16_t *in_buffer,uint16_t *out_buffer, uint32_t *half_in)
{
uint16_t	i,start,end;
	get_limits(&start,&end,half_in);
	for ( i=start;i<end;i++)
		out_buffer[i] = in_buffer[i];
}

uint32_t VCFInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer, uint32_t channel)
{
	setOutStage((uint32_t )&static_do_vcf,in_buffer, out_buffer, get_bufferhalf(channel), 0, 0, 0, channel, in_stage,"VCF");
	stage++;
	return 0;
}

