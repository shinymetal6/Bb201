/*
 * oscillators.c
 *
 *  Created on: Dec 26, 2020
 *      Author: fil
 */


#include "main.h"
#include "math.h"
#include "arm_math.h"

#define	WAVETABLE_SIZE	360

__attribute__ ((aligned (16))) uint32_t	osc_buf[CHANNELS][NUMOSCILLATORS][NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (16))) uint32_t	osc_out[CHANNELS][NUMBER_OF_AUDIO_SAMPLES];

__attribute__ ((aligned (16)))	uint32_t	sqrtab[CHANNELS][WAVETABLE_SIZE];
__attribute__ ((aligned (16)))	uint32_t	tritab[CHANNELS][WAVETABLE_SIZE];

__attribute__ ((aligned (16))) OscillatorsTypeDef	Oscillator[CHANNELS][NUMOSCILLATORS];

uint32_t	sintab[WAVETABLE_SIZE] =
{
		0x800,0x823,0x847,0x86b,0x88e,0x8b2,0x8d6,0x8f9,
		0x91c,0x940,0x963,0x986,0x9a9,0x9cc,0x9ef,0xa11,
		0xa34,0xa56,0xa78,0xa9a,0xabc,0xadd,0xaff,0xb20,
		0xb40,0xb61,0xb81,0xba1,0xbc1,0xbe0,0xbff,0xc1e,
		0xc3d,0xc5b,0xc78,0xc96,0xcb3,0xcd0,0xcec,0xd08,
		0xd24,0xd3f,0xd5a,0xd74,0xd8e,0xda7,0xdc0,0xdd9,
		0xdf1,0xe09,0xe20,0xe37,0xe4d,0xe63,0xe78,0xe8d,
		0xea1,0xeb5,0xec8,0xedb,0xeed,0xefe,0xf0f,0xf20,
		0xf30,0xf3f,0xf4e,0xf5c,0xf6a,0xf77,0xf84,0xf8f,
		0xf9b,0xfa6,0xfb0,0xfb9,0xfc2,0xfcb,0xfd2,0xfd9,
		0xfe0,0xfe6,0xfeb,0xff0,0xff4,0xff7,0xffa,0xffc,
		0xffe,0xfff,0xfff,0xfff,0xffe,0xffc,0xffa,0xff7,
		0xff4,0xff0,0xfeb,0xfe6,0xfe0,0xfd9,0xfd2,0xfcb,
		0xfc2,0xfb9,0xfb0,0xfa6,0xf9b,0xf8f,0xf84,0xf77,
		0xf6a,0xf5c,0xf4e,0xf3f,0xf30,0xf20,0xf0f,0xefe,
		0xeed,0xedb,0xec8,0xeb5,0xea1,0xe8d,0xe78,0xe63,
		0xe4d,0xe37,0xe20,0xe09,0xdf1,0xdd9,0xdc0,0xda7,
		0xd8e,0xd74,0xd5a,0xd3f,0xd24,0xd08,0xcec,0xcd0,
		0xcb3,0xc96,0xc78,0xc5b,0xc3d,0xc1e,0xbff,0xbe0,
		0xbc1,0xba1,0xb81,0xb61,0xb40,0xb20,0xaff,0xadd,
		0xabc,0xa9a,0xa78,0xa56,0xa34,0xa11,0x9ef,0x9cc,
		0x9a9,0x986,0x963,0x940,0x91c,0x8f9,0x8d6,0x8b2,
		0x88e,0x86b,0x847,0x823,0x800,0x7dc,0x7b8,0x794,
		0x771,0x74d,0x729,0x706,0x6e3,0x6bf,0x69c,0x679,
		0x656,0x633,0x610,0x5ee,0x5cb,0x5a9,0x587,0x565,
		0x543,0x522,0x500,0x4df,0x4bf,0x49e,0x47e,0x45e,
		0x43e,0x41f,0x400,0x3e1,0x3c2,0x3a4,0x387,0x369,
		0x34c,0x32f,0x313,0x2f7,0x2db,0x2c0,0x2a5,0x28b,
		0x271,0x258,0x23f,0x226,0x20e,0x1f6,0x1df,0x1c8,
		0x1b2,0x19c,0x187,0x172,0x15e,0x14a,0x137,0x124,
		0x112,0x101,0xf0,0xdf,0xcf,0xc0,0xb1,0xa3,
		0x95,0x88,0x7b,0x70,0x64,0x59,0x4f,0x46,
		0x3d,0x34,0x2d,0x26,0x1f,0x19,0x14,0xf,
		0xb,0x8,0x5,0x3,0x1,0x0,0x0,0x0,
		0x1,0x3,0x5,0x8,0xb,0xf,0x14,0x19,
		0x1f,0x26,0x2d,0x34,0x3d,0x46,0x4f,0x59,
		0x64,0x70,0x7b,0x88,0x95,0xa3,0xb1,0xc0,
		0xcf,0xdf,0xf0,0x101,0x112,0x124,0x137,0x14a,
		0x15e,0x172,0x187,0x19c,0x1b2,0x1c8,0x1df,0x1f6,
		0x20e,0x226,0x23f,0x258,0x271,0x28b,0x2a5,0x2c0,
		0x2db,0x2f7,0x313,0x32f,0x34c,0x369,0x387,0x3a4,
		0x3c2,0x3e1,0x400,0x41f,0x43e,0x45e,0x47e,0x49e,
		0x4bf,0x4df,0x500,0x522,0x543,0x565,0x587,0x5a9,
		0x5cb,0x5ee,0x610,0x633,0x656,0x679,0x69c,0x6bf,
		0x6e3,0x706,0x729,0x74d,0x771,0x794,0x7b8,0x7dc,
};

uint32_t	activechannels=0;

static uint32_t sinFunc(uint32_t pos)
{
	uint32_t *wave = sintab + pos;
	return *wave;
}

static uint32_t lookupFunc(uint32_t lookup_ptr,uint32_t pos)
{
	uint32_t *wave = (uint32_t *)lookup_ptr + pos;
	return *wave;
}


static void triCompile(uint32_t channel,uint32_t phase)
{
float 	step_up,step_down,current;
uint32_t	i;

	step_up = 4095.0f / (float )phase;
	step_down = 4095.0f / (float )( WAVETABLE_SIZE - phase);
	current=0;
	for(i=0;i<WAVETABLE_SIZE;i++)
	{
		if ( i < phase )
		{
			tritab[channel][i] = (uint32_t )current;
			current += step_up;
		}
		else
		{
			tritab[channel][i] = (uint32_t )current;
			current -= step_down;
		}
	}
}

static void squareCompile(uint32_t channel,uint32_t phase)
{
uint32_t	i;

	for(i=0;i<WAVETABLE_SIZE;i++)
	{
		if ( i < phase )
			sqrtab[channel][i] = 4095;
		else
			sqrtab[channel][i] = 0;
	}
}
#ifdef	SW_SINE
static void sinCompile(void)
{
uint32_t	i;

	for(i=0;i<WAVETABLE_SIZE;i++)
        sintab[i] = 2048 + (unsigned int )(sinf(((float )i*M_PI*2)/360.0f)* 2047.0f);
}
#endif

static	void static_process_oscillator(uint32_t channel,uint32_t osc_number)
{
uint16_t	i,start,end;
uint32_t	uint32_t_pos;

	get_limits(&start,&end,(uint32_t *)Oscillator[channel][osc_number].buffer_flag_ptr);
	for ( i=start;i<end;i++)
	{
		if ( Oscillator[channel][osc_number].enabled == OSCILLATOR_ENABLED )
		{
			uint32_t_pos = (uint32_t )Oscillator[channel][osc_number].current_phase;
			switch (Oscillator[channel][osc_number].waveform)
			{
			case	SINE	:
				osc_buf[channel][osc_number][i] = sinFunc(uint32_t_pos);
				break;
			case	TRIANGLE:
				osc_buf[channel][osc_number][i] = lookupFunc((uint32_t )&tritab[channel],uint32_t_pos);
				break;
			case	SQUARE:
				osc_buf[channel][osc_number][i] = lookupFunc((uint32_t )&sqrtab[channel],uint32_t_pos);
				break;
			}
			osc_out[channel][i] += ((osc_buf[channel][osc_number][i]*Oscillator[channel][osc_number].volume)>>DAC_BIT) / activechannels;
			Oscillator[channel][osc_number].current_phase += Oscillator[channel][osc_number].delta_phase;
			if ( Oscillator[channel][osc_number].current_phase >= 360.0f )
			{
				if (( Oscillator[channel][osc_number].flags  & CHANGE_FREQUENCY_FLAG ) == CHANGE_FREQUENCY_FLAG) // Hard sync
				{
					Oscillator[channel][osc_number].freq = Oscillator[channel][osc_number].new_freq;
					Oscillator[channel][osc_number].delta_phase = 360.0f / ((float )SystemParameters.sampling_frequency[channel] / Oscillator[channel][osc_number].freq);
					Oscillator[channel][osc_number].flags  &= ~CHANGE_FREQUENCY_FLAG;
				}
				if ((( Oscillator[channel][osc_number].flags  & CHANGE_PHASE_FLAG ) == CHANGE_PHASE_FLAG) &&  (Oscillator[channel][osc_number].waveform != SINE ))
				{
					triCompile(channel,Oscillator[channel][osc_number].phase);
					squareCompile(channel,Oscillator[channel][osc_number].phase);
					Oscillator[channel][osc_number].flags  &= ~CHANGE_PHASE_FLAG;
				}
				Oscillator[channel][osc_number].current_phase -= 360.0f;
			}
		}
		else
			osc_buf[channel][osc_number][i] = 0;
	}
}

void DoOscillators(void)
{
uint16_t	osc_number,channel;
uint16_t	i,start,end;

	get_limits(&start,&end,(uint32_t *)Oscillator[0][0].buffer_flag_ptr); // Use osc 0 to get limits
	for ( i=start;i<end;i++)
		osc_out[0][i] = osc_out[1][i] = 0;
	for(channel=0;channel<CHANNELS;channel++)
	{
		for(osc_number=0;osc_number<NUMOSCILLATORS;osc_number++)
		{
			static_process_oscillator(channel,osc_number);
		}
	}
}

void EnableOscillator(uint32_t channel,uint32_t osc_number)
{
	Oscillator[channel][osc_number].enabled = OSCILLATOR_ENABLED;
	activechannels++;
}

void DisableOscillator(uint32_t channel,uint32_t osc_number)
{
	Oscillator[channel][osc_number].enabled = OSCILLATOR_DISABLED;
	Oscillator[channel][osc_number].current_phase = 360.0f;
	activechannels--;
}

void ChangeOscillatorFrequency(uint32_t channel,uint32_t osc_number, float freq,uint32_t midi_note)
{
	activechannels++;
	Oscillator[channel][osc_number].enabled = OSCILLATOR_ENABLED;
	if ( Oscillator[channel][osc_number].current_phase == 0.0f )
		Oscillator[channel][osc_number].current_phase = 360.0f;
	Oscillator[channel][osc_number].delta_phase = 90.0f;
	Oscillator[channel][osc_number].flags |= CHANGE_FREQUENCY_FLAG;
	Oscillator[channel][osc_number].new_freq = (float )freq;
	Oscillator[channel][osc_number].midi_note = midi_note;
}

void SetOscillatorGroup(uint32_t channel,uint32_t osc_number,uint32_t group)
{
	Oscillator[channel][osc_number].osc_group = group;
}

void UnSetOscillatorGroup(uint32_t channel,uint32_t osc_number,uint32_t group)
{
	Oscillator[channel][osc_number].osc_group = (channel << 16) | osc_number;
}

void SetOscillatorFixedF(uint32_t channel,uint32_t osc_number)
{
	Oscillator[channel][osc_number].flags |= FIXED_FREQUENCY_FLAG;
}

void SetOscillatorFloatF(uint32_t channel,uint32_t osc_number)
{
	Oscillator[channel][osc_number].flags &= ~FIXED_FREQUENCY_FLAG;
}

uint32_t FindOscByMidi(uint32_t channel, uint32_t midi_note)
{
uint32_t	i;
	for(i=0;i<NUMOSCILLATORS;i++)
		if ( Oscillator[channel][i].midi_note == midi_note )
			return i;
	return NUMOSCILLATORS * 2;
}

void ChangeOscillatorWaveform(uint32_t channel,uint32_t osc_number, uint32_t waveform)
{
	Oscillator[channel][osc_number].waveform = waveform;
}

void ChangeOscillatorVolume(uint32_t channel,uint32_t osc_number, uint32_t volume)
{
	Oscillator[channel][osc_number].volume = volume*32;
}

void ChangeOscillatorPhase(uint32_t channel,uint32_t osc_number, uint32_t phase)
{
	Oscillator[channel][osc_number].phase = phase;
	Oscillator[channel][osc_number].flags  |= CHANGE_PHASE_FLAG;
}

void ChangeOscillatorPitchBend(uint32_t channel,uint32_t osc_number, uint32_t percent)
{
	Oscillator[channel][osc_number].new_freq = Oscillator[channel][osc_number].freq + ((Oscillator[channel][osc_number].freq/100.0f)* (float )percent);
}

void setOscillatorParams(uint32_t channel,uint32_t osc_number,uint32_t freq,uint32_t volume,uint32_t phase,uint32_t waveform,uint32_t osc_group,uint32_t flags)
{
	Oscillator[channel][osc_number].freq = (float )freq;
	Oscillator[channel][osc_number].phase = (float )phase;
	Oscillator[channel][osc_number].volume = volume*32;
	switch ( waveform )
	{
	case	SINE		:	Oscillator[channel][osc_number].waveform = SINE;break;
	case	TRIANGLE	:	Oscillator[channel][osc_number].waveform = TRIANGLE;break;
	case	SQUARE		:	Oscillator[channel][osc_number].waveform = SQUARE;break;
	default				:	Oscillator[channel][osc_number].waveform = SINE;break;
	}
	Oscillator[channel][osc_number].osc_group = osc_group;
	Oscillator[channel][osc_number].flags = flags;
}

uint32_t InitOscillators(void)
{
uint16_t	osc_number,channel;
	for(channel=0;channel<CHANNELS;channel++)
	{
		for(osc_number=0;osc_number<NUMOSCILLATORS;osc_number++)
		{
			Oscillator[channel][osc_number].enabled = OSCILLATOR_DISABLED;
			Oscillator[channel][osc_number].freq = 0.0f;
			Oscillator[channel][osc_number].waveform = SINE;
			Oscillator[channel][osc_number].volume = DAC_RESOLUTION;
			Oscillator[channel][osc_number].current_phase = 0.0f;
			Oscillator[channel][osc_number].delta_phase = 1.0f;
			Oscillator[channel][osc_number].buffer_flag_ptr = get_bufferhalf(channel);
			Oscillator[channel][osc_number].flags = NO_FLAG;
			Oscillator[channel][osc_number].osc_group = (channel << 16) | osc_number;
		}
		triCompile(channel,180);
		squareCompile(channel,180);
	}
	//sinCompile();
	return NUMOSCILLATORS*2;
}
