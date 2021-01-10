/*
 * vco.c
 *
 *  Created on: Jan 9, 2021
 *      Author: fil
 */

#include "main.h"
#include "math.h"
#include "vco.h"

#define	VCOTAB_SIZE_IN_BITS		8
#define	VCOTAB_SIZE				(1 << VCOTAB_SIZE_IN_BITS)
#define	VCO_PRECISION			14
#define	VCO_PRECISION_SHIFT		(1<<VCO_PRECISION)

AUDIO_BUFFERS __attribute__ ((aligned (16))) uint16_t	vco_buf[NUMVCO][NUMBER_OF_AUDIO_SAMPLES];

__attribute__ ((aligned (16))) VCOTypeDef	Vco[NUMVCO];
uint16_t	vcotab[NUMVCO][VCO_NUMBER_OF_ELEMENTS][VCOTAB_SIZE];

static	void i_vco(uint32_t vco_number)
{
uint16_t	i,start,end;
uint8_t		angle,one_volume=0;
uint32_t	vco_element,vco_out[4]={0,0,0,0};

	get_limits(&start,&end,(uint32_t *)Vco[vco_number].buffer_flag_ptr);
	for ( i=start;i<end;i++)
	{
		one_volume=0;
		vco_buf[vco_number][i] = 0;
		for ( vco_element=0;vco_element<VCO_NUMBER_OF_ELEMENTS;vco_element++)
		{
			angle = (uint8_t )(Vco[vco_number].current_phase[vco_element] >> VCO_PRECISION);
			angle += Vco[vco_number].dephase[vco_element];
			vco_out[vco_element] = (vcotab[vco_number][vco_element][angle] * ( *(Vco[vco_number].volume_control_ptr[vco_element] )) >> DAC_BIT);
			Vco[vco_number].current_phase[vco_element] += Vco[vco_number].delta_phase[vco_element];
		}
		vco_buf[vco_number][i] = vco_out[0];
		if ( *Vco[vco_number].volume_control_ptr[0] != 0 )
			one_volume++;
		if ( *Vco[vco_number].volume_control_ptr[1] != 0 )
		{
			if ( one_volume > 0 )
			{
				vco_buf[vco_number][i] >>= 1;
				vco_buf[vco_number][i] += (vco_out[1] >> 1);
			}
			else
				vco_buf[vco_number][i] = vco_out[1];
			one_volume++;
		}
		if ( *Vco[vco_number].volume_control_ptr[2] != 0 )
		{
			if ( one_volume > 0 )
			{
				vco_buf[vco_number][i] >>= 1;
				vco_buf[vco_number][i] += (vco_out[2] >> 1);
			}
			else
				vco_buf[vco_number][i] = vco_out[2];
			one_volume++;
		}
		if ( *Vco[vco_number].volume_control_ptr[3] != 0 )
		{
			if ( one_volume > 0 )
			{
				vco_buf[vco_number][i] >>= 1;
				vco_buf[vco_number][i] += (vco_out[3] >> 1);
			}
			else
				vco_buf[vco_number][i] = vco_out[3];
		}
		vco_buf[vco_number][i] = (vco_buf[vco_number][i] * ( *(Vco[vco_number].master_volume_control_ptr )) >> DAC_BIT);
	}
}

static void VcoSinCompile(uint16_t vco_number,uint16_t vco_element)
{
uint32_t	i;
	for(i=0;i<VCOTAB_SIZE;i++)
	{
		vcotab[vco_number][0][i] = vcotab[vco_number][1][i] = vcotab[vco_number][2][i] = vcotab[vco_number][3][i] = (uint16_t )(HALF_DAC_RESOLUTION + sin((i*M_PI*2)/VCOTAB_SIZE)* (HALF_DAC_RESOLUTION-1));
	}
}

void DoVco(void)
{
uint16_t	start,end;
uint16_t	vco_number;

	get_limits(&start,&end,(uint32_t *)Vco[0].buffer_flag_ptr);
	for(vco_number=0;vco_number<NUMVCO;vco_number++)
	{
		i_vco(vco_number);
	}
}

void VcoElementDetune(uint16_t	vco_number,uint16_t	vco_element,int16_t delta_frequency,uint32_t sampling_frequency)
{
double		delta_phase;
	Vco[vco_number].frequency[vco_element] += delta_frequency;
	delta_phase = ((double )VCOTAB_SIZE / (double )(Vco[vco_number].sampling_frequency / (double )Vco[vco_number].frequency[vco_element] ));
	Vco[vco_number].delta_phase[vco_element]  = (uint16_t )(delta_phase * (double )VCO_PRECISION_SHIFT);
}

void VcoSetFrequency(uint16_t vco_number,uint32_t frequency,uint32_t sampling_frequency)
{
double		delta_phase;
uint16_t	i;
	for(i=0;i<VCO_NUMBER_OF_ELEMENTS;i++)
	{
		Vco[vco_number].frequency[i] = frequency;
		delta_phase = ((double )VCOTAB_SIZE / (double )(Vco[vco_number].sampling_frequency / (double )Vco[vco_number].frequency[i] ));
		Vco[vco_number].delta_phase[i]  = (uint16_t )(delta_phase * (double )VCO_PRECISION_SHIFT);
	}
}

uint16_t	internal_vco_volume_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
uint16_t	internal_vco_master_volume_control[NUMVCO];

void VcoSetMasterVolume(uint16_t vco_number,uint16_t master_volume)
{
	internal_vco_master_volume_control[vco_number] = master_volume;
}


uint32_t InitVco(uint32_t sampling_frequency)
{
uint16_t	vco_number;
uint16_t	vco_element;
double		delta_phase;

	for(vco_number=0;vco_number<NUMVCO;vco_number++)
	{
		internal_vco_master_volume_control[vco_number] = 4096;
		Vco[vco_number].sampling_frequency = sampling_frequency;
		Vco[vco_number].master_volume_control_ptr = &internal_vco_master_volume_control[vco_number];

		for ( vco_element=0;vco_element<VCO_NUMBER_OF_ELEMENTS;vco_element++)
		{
			internal_vco_volume_control[vco_number][vco_element] = 4096;
			Vco[vco_number].frequency[vco_element] = 375*2;
			Vco[vco_number].detune[vco_element] = 0;
			Vco[vco_number].dephase[vco_element] = 0;
			Vco[vco_number].waveform[vco_element] = SINE;
			Vco[vco_number].current_phase[vco_element] = 0;
			delta_phase = ((double )VCOTAB_SIZE / (double )(Vco[vco_number].sampling_frequency / (double )Vco[vco_number].frequency[vco_element] ));
			Vco[vco_number].delta_phase[vco_element]  = (uint16_t )(delta_phase * (double )VCO_PRECISION_SHIFT);
			Vco[vco_number].volume_control_ptr[vco_element] = &internal_vco_volume_control[vco_number][vco_element];
		}
		Vco[vco_number].buffer_flag_ptr = get_bufferhalf(0);
		VcoSinCompile(vco_number,0);
	}
/*
	//VcoElementDetune(0,1,50,96000);
	Vco[0].volume[0]=0;
	//Vco[0].volume[1]=0;
	Vco[0].volume[2]=0;
	Vco[0].volume[3]=0;
	Vco[0].dephase[1] = 32;
	*/
	return NUMVCO*2;
}

