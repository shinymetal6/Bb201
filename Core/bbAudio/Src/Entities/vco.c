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

uint16_t	local_vco_master_volume_control[NUMVCO];
uint16_t	local_vco_frequency_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
uint8_t		local_vco_detune_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
uint16_t	local_vco_phase_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
uint16_t	local_vco_volume_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];

uint16_t	midi_vco_master_volume_control[NUMVCO];
uint16_t	midi_vco_frequency_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
uint8_t		midi_vco_detune_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
uint16_t	midi_vco_phase_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
uint16_t	midi_vco_volume_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];

uint16_t	static_vco_master_volume_control[NUMVCO];
uint16_t	static_vco_frequency_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
uint8_t		static_vco_detune_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
uint16_t	static_vco_phase_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
uint16_t	static_vco_volume_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];


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
			angle += *Vco[vco_number].dephase[vco_element];
			vco_out[vco_element] = (vcotab[vco_number][vco_element][angle] * ( *(Vco[vco_number].volume_control[vco_element] )) >> DAC_BIT);
			Vco[vco_number].current_phase[vco_element] += Vco[vco_number].delta_phase[vco_element];
		}
		vco_buf[vco_number][i] = vco_out[0];
		if ( *Vco[vco_number].volume_control[0] != 0 )
			one_volume++;
		if ( *Vco[vco_number].volume_control[1] != 0 )
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
		if ( *Vco[vco_number].volume_control[2] != 0 )
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
		if ( *Vco[vco_number].volume_control[3] != 0 )
		{
			if ( one_volume > 0 )
			{
				vco_buf[vco_number][i] >>= 1;
				vco_buf[vco_number][i] += (vco_out[3] >> 1);
			}
			else
				vco_buf[vco_number][i] = vco_out[3];
		}
		vco_buf[vco_number][i] = (vco_buf[vco_number][i] * ( *(Vco[vco_number].master_volume_control )) >> DAC_BIT);
	}
}

static void VcoSinCompile(uint16_t vco_number,uint16_t vco_element)
{
uint32_t	i;
	for(i=0;i<VCOTAB_SIZE;i++)
	{
		vcotab[vco_number][vco_element][i] = (uint16_t )(HALF_DAC_RESOLUTION + sin((i*M_PI*2)/VCOTAB_SIZE)* (HALF_DAC_RESOLUTION-1));
	}
}

static void VcoTriCompile(uint16_t vco_number,uint16_t vco_element, uint16_t phase)
{
double 	step_up,step_down,current;
uint32_t	i;

	step_up = (double )(DAC_RESOLUTION-1) / (double )phase;
	step_down = (double )(DAC_RESOLUTION-1)  / (double )( VCOTAB_SIZE - phase);
	current=0;
	for(i=0;i<VCOTAB_SIZE;i++)
	{
		if ( i < phase )
		{
			vcotab[vco_number][vco_element][i] = (uint32_t )current;
			current += step_up;
		}
		else
		{
			vcotab[vco_number][vco_element][i] = (uint32_t )current;
			current -= step_down;
		}
	}
}
static void VcoSquareCompile(uint16_t vco_number,uint16_t vco_element, uint16_t phase)
{
uint32_t	i;

	for(i=0;i<VCOTAB_SIZE;i++)
	{
		if ( i < phase )
			vcotab[vco_number][vco_element][i] = DAC_RESOLUTION-1;
		else
			vcotab[vco_number][vco_element][i] = 0;
	}
}

void VcoSetElementWave(uint16_t vco_number, uint8_t vco_element, uint8_t waveform, uint16_t phase)
{
	Vco[vco_number].waveform[vco_element] = waveform;
	switch (waveform)
	{
	case	SINE	:	VcoSinCompile(vco_number,vco_element); break;
	case	TRIANGLE:	VcoTriCompile(vco_number,vco_element,phase);break;
	case	SQUARE	:	VcoSquareCompile(vco_number,vco_element,phase);break;
	}
}

void VcoSetElementVolume(uint16_t	vco_number,uint16_t	vco_element,uint32_t volume_ptr)
{
	Vco[vco_number].volume_control[vco_element] = (uint16_t *)volume_ptr;
}

void VcoElementDetune(uint16_t	vco_number,uint16_t	vco_element,int16_t percentage_dac,uint32_t sampling_frequency)
{
double		delta_phase, frequency_step,frequency_delta, frequency_shift, norm_percent;
	if ( percentage_dac > (DAC_RESOLUTION-1))
		percentage_dac = DAC_RESOLUTION-1;

	norm_percent = percentage_dac - 2048;
	frequency_delta = norm_percent / 2048;
	frequency_step = (Vco[vco_number].frequency - (Vco[vco_number].frequency/2.0))/24.0;
	frequency_shift = frequency_step * frequency_delta;

	//frequency_shift = (((double )Vco[vco_number].frequency - ((double )Vco[vco_number].frequency/2.0)) / 12.0)*(100.0 / (2048.0 - (double )percentage_dac));
	delta_phase = ((double )VCOTAB_SIZE / (double )(Vco[vco_number].sampling_frequency / ((double )Vco[vco_number].frequency +frequency_shift)));
	Vco[vco_number].delta_phase[vco_element]  = (uint16_t )(delta_phase * (double )VCO_PRECISION_SHIFT);
}

void VcoSetFrequency(uint16_t vco_number,uint32_t frequency,uint32_t sampling_frequency)
{
double		delta_phase;
uint16_t	vco_element;
	for(vco_element=0;vco_element<VCO_NUMBER_OF_ELEMENTS;vco_element++)
	{
		static_vco_frequency_control[vco_number][vco_element] = frequency;
		delta_phase = ((double )VCOTAB_SIZE / (double )(Vco[vco_number].sampling_frequency / (double )Vco[vco_number].frequency ));
		Vco[vco_number].delta_phase[vco_element]  = (uint16_t )(delta_phase * (double )VCO_PRECISION_SHIFT);
	}
}

void VcoSetMasterVolume(uint16_t vco_number,uint32_t master_volume_ptr)
{
	Vco[vco_number].master_volume_control = (uint16_t *)master_volume_ptr;
}

void VcoSetMidiNote(uint16_t vco_number,uint16_t midi_note)
{
	Vco[vco_number].midi_note = midi_note;
}

uint32_t FindOscByMidi(uint32_t midi_note)
{
uint32_t	i;
	for(i=0;i<NUMVCO;i++)
	{
		if ( Vco[i].midi_note == midi_note )
		{
			Vco[i].midi_note = 0;
			return i;
		}
	}
	return NUMVCO * 2;
}

uint32_t InitVco(uint32_t sampling_frequency)
{
uint16_t	vco_number;
uint16_t	vco_element;
double		delta_phase;

	for(vco_number=0;vco_number<NUMVCO;vco_number++)
	{
		static_vco_master_volume_control[vco_number] = 4096;
		Vco[vco_number].sampling_frequency = sampling_frequency;
		Vco[vco_number].master_volume_control = &control_buf.ain[0];
		Vco[vco_number].frequency = 880;

		for ( vco_element=0;vco_element<VCO_NUMBER_OF_ELEMENTS;vco_element++)
		{
			static_vco_volume_control[vco_number][vco_element] = 4096;
			Vco[vco_number].detune[vco_element] = 2048;
			Vco[vco_number].dephase[vco_element] = (uint8_t *)&static_vco_phase_control[vco_number][vco_element];
			Vco[vco_number].waveform[vco_element] = SINE;
			Vco[vco_number].current_phase[vco_element] = 0;
			delta_phase = ((double )VCOTAB_SIZE / (double )(Vco[vco_number].sampling_frequency / (double )Vco[vco_number].frequency ));
			Vco[vco_number].delta_phase[vco_element]  = (uint16_t )(delta_phase * (double )VCO_PRECISION_SHIFT);
			Vco[vco_number].volume_control[vco_element] = &static_vco_volume_control[vco_number][vco_element];
		}
		Vco[vco_number].buffer_flag_ptr = get_bufferhalf(0);
		VcoSetElementWave(vco_number, 0, SINE, 0);
		VcoSetElementWave(vco_number, 1, SINE, 0);
		VcoSetElementWave(vco_number, 2, SINE, 0);
		VcoSetElementWave(vco_number, 3, SINE, 0);
	}
	return NUMVCO*2;
}

void DoVco(void)
{
uint16_t	start,end;
uint16_t	vco_number,vco_element=0,percentage_dac=2048,sampling_frequency=SystemParameters.sampling_frequency[0];

	get_limits(&start,&end,(uint32_t *)Vco[0].buffer_flag_ptr);
	percentage_dac=control_buf.ain[1];
	for(vco_number=0;vco_number<NUMVCO;vco_number++)
	{
		VcoSetFrequency(vco_number,Vco[vco_number].frequency, sampling_frequency);
		VcoElementDetune(vco_number,vco_element, percentage_dac, sampling_frequency);
		i_vco(vco_number);
	}
}

