/*
 * oscillators.c
 *
 *  Created on: Dec 26, 2020
 *      Author: fil
 */


#include "main.h"
#include "math.h"
#include "arm_math.h"

#define	WAVETABLE_SIZE	256
#define	INT_PRECISION	256
#define	INT_PRECISION_SHIFT	(INT_PRECISION >> 5)
#define	VOLUME_MULT	16


OSC_BUFFER __attribute__ ((aligned (16))) uint16_t	osc_buf[NUMOSCILLATORS][NUMBER_OF_AUDIO_SAMPLES];
OSC_BUFFER __attribute__ ((aligned (16))) uint16_t	osc_output_buffer[NUMBER_OF_AUDIO_SAMPLES];

__attribute__ ((aligned (16))) OscillatorsTypeDef	Oscillator[NUMOSCILLATORS];

uint16_t	wavetab[NUMOSCILLATORS][WAVETABLE_SIZE];

uint32_t	osc_activechannels=0;

static	void i_oscillator(uint32_t osc_number)
{
uint16_t	i,start,end;
uint8_t		angle;

	get_limits(&start,&end,(uint32_t *)Oscillator[osc_number].buffer_flag_ptr);
	for ( i=start;i<end;i++)
	{
		angle = (uint8_t )(Oscillator[osc_number].current_phase >> 8);
		if ( Oscillator[osc_number].current_volume != 0 )
		{
			osc_buf[osc_number][i] = (wavetab[osc_number][angle] * Oscillator[osc_number].current_volume ) >> (DAC_BIT-1);
			osc_output_buffer[i] = ((osc_output_buffer[i] >> 1 ) + (osc_buf[osc_number][i] >> 1));
		}
		Oscillator[osc_number].current_phase += Oscillator[osc_number].delta_phase;
	}
}

static void sinCompile(uint32_t osc_number)
{
uint32_t	i;
	for(i=0;i<WAVETABLE_SIZE;i++)
		wavetab[osc_number][i] = (uint16_t )(HALF_DAC_RESOLUTION + sin((i*M_PI*2)/WAVETABLE_SIZE)* (HALF_DAC_RESOLUTION-1));
}

static void squareCompile(uint32_t osc_number,uint32_t phase)
{
uint32_t	i;

	for(i=0;i<WAVETABLE_SIZE;i++)
	{
		if ( i < phase )
			wavetab[osc_number][i] = DAC_RESOLUTION-1;
		else
			wavetab[osc_number][i] = 0;
	}
}

static void triCompile(uint32_t osc_number,uint32_t phase)
{
float 	step_up,step_down,current;
uint32_t	i;

	step_up = (float )(DAC_RESOLUTION-1) / (float )phase;
	step_down = (float )(DAC_RESOLUTION-1)  / (float )( WAVETABLE_SIZE - phase);
	current=0;
	for(i=0;i<WAVETABLE_SIZE;i++)
	{
		if ( i < phase )
		{
			wavetab[osc_number][i] = (uint32_t )current;
			current += step_up;
		}
		else
		{
			wavetab[osc_number][i] = (uint32_t )current;
			current -= step_down;
		}
	}
}

void SetOscillatorWaveform(uint16_t osc_number, uint8_t waveform, uint16_t phase)
{
	switch (Oscillator[osc_number].waveform)
	{
	case	SINE	:	sinCompile(osc_number); break;
	case	TRIANGLE:	triCompile(osc_number,phase);break;
	case	SQUARE	:	squareCompile(osc_number,phase);break;
	}
}

void SetOscillatorVolume(uint16_t osc_number, uint16_t volume)
{
	Oscillator[osc_number].current_volume = volume*VOLUME_MULT;
	if ( volume > 0 )
		osc_activechannels++;
	else
	{
		if ( osc_activechannels != 0 )
			osc_activechannels--;
	}
}

void SetOscillatorPhase(uint16_t osc_number, uint8_t phase)
{
	Oscillator[osc_number].phase = phase;
	SetOscillatorWaveform(osc_number, Oscillator[osc_number].waveform, Oscillator[osc_number].phase);
}


void SetOscillatorParams(uint16_t osc_number, uint32_t sampling_frequency, uint16_t freq, uint8_t waveform, uint16_t volume, uint16_t phase, uint16_t osc_group, uint16_t control)
{
float	delta_phase;
	SetOscillatorWaveform(osc_number,waveform,phase);
	Oscillator[osc_number].freq = (float )freq;
	delta_phase = (float )WAVETABLE_SIZE / ((float )sampling_frequency / (float )freq);
	Oscillator[osc_number].delta_phase = (uint16_t )(delta_phase * 256.0f);
	Oscillator[osc_number].current_phase = 0;
	Oscillator[osc_number].volume = volume*VOLUME_MULT;
	if ( control == OSCILLATOR_FREE_RUNNING)
		Oscillator[osc_number].current_volume = volume*VOLUME_MULT;
	else
		Oscillator[osc_number].current_volume = 0;
	Oscillator[osc_number].phase = phase;
	Oscillator[osc_number].osc_group = osc_group;
	Oscillator[osc_number].detune = 0;
}

void SetOscillatorFrequency(uint16_t osc_number, uint32_t sampling_frequency, uint16_t freq, uint16_t volume, uint16_t midi_note)
{
float	delta_phase;
	Oscillator[osc_number].sampling_frequency = (float )sampling_frequency;
	Oscillator[osc_number].freq = (float )freq;
	delta_phase = (float )WAVETABLE_SIZE / (Oscillator[osc_number].sampling_frequency / Oscillator[osc_number].freq);
	Oscillator[osc_number].delta_phase = (uint16_t )(delta_phase * (float )INT_PRECISION);
	Oscillator[osc_number].current_phase = 0;
	Oscillator[osc_number].midi_note = midi_note;
	Oscillator[osc_number].volume = volume*VOLUME_MULT;
	Oscillator[osc_number].current_volume = volume*VOLUME_MULT;
}

void SetOscillatorDeTune(uint16_t osc_number, uint16_t detune)
{
float	delta_phase;
	delta_phase = (float )WAVETABLE_SIZE / ((Oscillator[osc_number].sampling_frequency+ (float )detune) / Oscillator[osc_number].freq);
	Oscillator[osc_number].delta_phase = (uint16_t )(delta_phase * (float )INT_PRECISION);
	Oscillator[osc_number].osc_group = detune;
}

void EnableOscillator(uint32_t osc_number)
{
	Oscillator[osc_number].current_volume = Oscillator[osc_number].volume;
}

void DisableOscillator(uint16_t osc_number)
{
	Oscillator[osc_number].current_volume = 0;
}

void SetOscillatorGroup(uint16_t osc_number,uint32_t group)
{
	Oscillator[osc_number].osc_group = group;
}

void SetOscillatorFlag(uint16_t osc_number,uint32_t flags)
{
	Oscillator[osc_number].flags = flags;
	if ( flags != OSCILLATOR_FREE_RUNNING )
		Oscillator[osc_number].current_volume = 0;
	else
		Oscillator[osc_number].current_volume = Oscillator[osc_number].volume;
}

uint32_t FindOscByMidi(uint32_t midi_note)
{
uint32_t	i;
	for(i=0;i<NUMOSCILLATORS;i++)
	{
		if ( Oscillator[i].midi_note == midi_note )
		{
			Oscillator[i].midi_note = 0;
			return i;
		}
	}
	return NUMOSCILLATORS * 2;
}

void DoOscillators(void)
{
uint16_t	i,start,end;
uint16_t	osc_number;

	get_limits(&start,&end,(uint32_t *)Oscillator[0].buffer_flag_ptr);
	for ( i=start;i<end;i++)
		osc_output_buffer[i] = 0;
	for(osc_number=0;osc_number<NUMOSCILLATORS;osc_number++)
		i_oscillator(osc_number);
}

uint32_t InitOscillators(void)
{
uint16_t	osc_number;
	for(osc_number=0;osc_number<NUMOSCILLATORS;osc_number++)
	{
		Oscillator[osc_number].freq = 0.0f;
		Oscillator[osc_number].waveform = SINE;
		Oscillator[osc_number].volume = DAC_RESOLUTION;
		Oscillator[osc_number].current_phase = 0.0f;
		Oscillator[osc_number].delta_phase = 1.0f;
		Oscillator[osc_number].buffer_flag_ptr = get_bufferhalf(0);
		Oscillator[osc_number].flags = OSCILLATOR_DISABLED;
		sinCompile(osc_number);
	}
	return NUMOSCILLATORS*2;
}
