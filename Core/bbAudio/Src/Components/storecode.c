/*
 * storecode.c
 *
 *  Created on: Jan 4, 2021
 *      Author: fil
 */

DO NOT COMPILE THIS
/*
 * vcf.c
 *
 *  Created on: Jan 3, 2021
 *      Author: fil
 */

#include "main.h"
#include "math.h"
uint16_t lp, hp, bp;

#define	DC_CONSTANT		2047.0f
#define	DC_CONSTANTx2	4095.0f

//#define ALGO2
#ifdef ALGO2
//#define ALGO1

#ifdef ALGO1
float b0=0.0f, b1=0.0f, b2=0.0f, b3=0.0f, b4=0.0f;
float flp, fhp, fbp;
uint16_t	error=0;
static void vcf1(uint16_t inputsample, uint32_t freq , uint32_t res,uint32_t channel )
{
// Lowpass  output:  b4
// Highpass output:  in - b4;
// Bandpass output:  3.0f * (b3 - b4);(float )sampling_frequency / (float )freq
float frequency = (float )freq / (float )SystemParameters.sampling_frequency[channel];
float resonance = (float )res;
float in = (float )(inputsample ) / 2.0f;
float f, p, q;             //filter coefficients
//float b0=0.0f, b1=0.0f, b2=0.0f, b3=0.0f, b4=0.0f;  //filter buffers (beware denormals!)
float t1, t2;              //temporary buffers

  in = (in - DC_CONSTANT) / DC_CONSTANT;

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
  flp = b4*DC_CONSTANT;
  fhp = (in - b4)*DC_CONSTANT;
  fbp = (3.0f * (b3 - b4))*DC_CONSTANT;
  lp = (uint16_t )DC_CONSTANT + (int16_t )flp;
  hp = (uint16_t )DC_CONSTANT + (int16_t )fhp;
  bp = (uint16_t )DC_CONSTANT + (int16_t )fbp;
  if ( lp > 0xfff )
	  error = 1;
}
#else
double in1=0,in2=0,in3=0,in4=0;
double out1=0,out2=0,out3=0,out4=0;

static void vcf1(uint16_t inputsample, uint32_t freq , uint32_t res,uint32_t channel )
//double vcf2(double input, double fcutoff, double resonance)
{
/*
in[x] and out[x] are member variables, init to 0.0 the controls:

fc = cutoff, nearly linear [0,1] -> [0, fs/2]
res = resonance [0, 4] -> [no resonance, self-oscillation]
*/
	double fcutoff = (double )freq / (double )SystemParameters.sampling_frequency[channel];
	double input = (double )inputsample;
	double resonance = (double )res;

  double f = fcutoff * 1.16;
  double fb = resonance * (1.0 - 0.15 * f * f);

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

  lp = (uint16_t )out4;
  hp = (uint16_t )(input - out4);
  bp = (uint16_t )(3 * (out3 - out4));
}
#endif

static	void static_do_vcf(uint16_t *in_buffer,uint16_t *out_buffer, uint32_t *half_in, uint32_t filterCutoff, uint32_t filterResonance, uint32_t filterType, uint32_t channel)
{
uint16_t	i,start,end;
	get_limits(&start,&end,half_in);
#ifdef ALGO1
	//b0=0.0f; b1=0.0f; b2=0.0f; b3=0.0f; b4=0.0f;
#else
	in1=0;in2=0;in3=0;in4=0;
	out1=0;out2=0;out3=0;out4=0;
#endif
	for ( i=start;i<end;i++)
	{
		vcf1(in_buffer[i], filterCutoff , filterResonance ,channel);
		switch ( filterType )
		{
		case	HP : out_buffer[i] = hp;break;
		case	LP : out_buffer[i] = lp;break;
		case	BP : out_buffer[i] = bp;break;
		}
	}
}
#else

#ifdef TEST01
double in1=0,in2=0,in3=0,in4=0;
double out1=0,out2=0,out3=0,out4=0;
double	max=DC_CONSTANT,min=DC_CONSTANT;
uint16_t filter(uint16_t sample, uint32_t filterCutoff, uint32_t resonance, uint32_t channel)
{
double res = (resonance/DC_CONSTANT);
double fc = ((double )filterCutoff * 2.0 )/ (double )SystemParameters.sampling_frequency[channel];
double input = (double )sample;
double f = fc * 1.16;
double fb = res * (1.0 - 0.15 * f * f);
double	eqsample;
	input /= 2048.0;
	input -= 1.0;

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
	eqsample = out4*2048.0;
	eqsample +=2048.0;
	return (uint16_t )eqsample;


}
#else
double b0, b1, b2, b3, b4;  //filter buffers (beware denormals!)
uint16_t filter(uint16_t sample, uint32_t filterCutoff, uint32_t resonance, uint32_t filterType, uint32_t channel)
{
double input = (double )sample;
double res1 = (resonance/DC_CONSTANT);
double fc = (double )filterCutoff / (double )SystemParameters.sampling_frequency[channel] * 2.0;
double f, p, q;             //filter coefficients
double t1, t2;              //temporary buffers
double	eqsample;

	input /= 2048.0;
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
	case	LP	: eqsample = b4; break;
	case	HP	: eqsample = input - b4; break;
	case	BP	: eqsample = 3.0f * (b3 - b4); break;
	default		: eqsample = b4; break;
	}
	eqsample *= 2048.0;
	eqsample += 2048.0;
	return (uint16_t )eqsample;
	// Lowpass  output:  b4
	// Highpass output:  in - b4;
	// Bandpass output:  3.0f * (b3 - b4);
}
#endif

static	void static_do_vcf(uint16_t *in_buffer,uint16_t *out_buffer, uint32_t *half_in, uint32_t filterCutoff, uint32_t filterResonance, uint32_t filterType, uint32_t channel)
{
uint16_t	i,start,end;
	get_limits(&start,&end,half_in);
	for ( i=start;i<end;i++)
		out_buffer[i] = filter(in_buffer[i],filterCutoff,filterResonance,filterType,channel);
}
#endif // ALGO2

uint32_t VCFInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer, uint32_t channel)
{
uint32_t filterResonance = DC_CONSTANT/4;	// max DC_CONSTANT
uint32_t filterType = LP;	// HP, BP or LP
uint32_t frequency = 2000;

	setOutStage((uint32_t )&static_do_vcf,in_buffer, out_buffer, get_bufferhalf(channel), frequency,filterResonance,filterType, channel, in_stage,"VCF");

	stage++;
	return 0;
}

