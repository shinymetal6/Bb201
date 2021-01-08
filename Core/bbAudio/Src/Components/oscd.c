/*
 * oscd.c
 *
 *  Created on: Jan 8, 2021
 *      Author: fil
 */
#include "main.h"
#include "math.h"
#include "oscd.h"

#define	WAVETABLE_SIZE	256

AUDIO_BUFFERS __attribute__ ((aligned (16))) uint16_t	oscd_buf[NUMOSCILLATORS][NUMBER_OF_AUDIO_SAMPLES];
AUDIO_BUFFERS __attribute__ ((aligned (16))) uint16_t	oscd_output_buffer[NUMBER_OF_AUDIO_SAMPLES];

__attribute__ ((aligned (16))) OscdTypeDef	Oscd[NUMOSCILLATORS];
uint16_t	oscd_wavetab[NUMOSCILLATORS][WAVETABLE_SIZE];
uint32_t	oscd_activechannels=0;

static	void i_oscd(uint32_t oscd_number)
{
uint16_t	i,start,end;
uint8_t		angle;

	get_limits(&start,&end,(uint32_t *)Oscd[oscd_number].buffer_flag_ptr);
	for ( i=start;i<end;i++)
	{
		angle = (uint8_t )(Oscd[oscd_number].current_phase);
		if ( Oscd[oscd_number].current_volume != 0 )
		{
			oscd_buf[oscd_number][i] = (oscd_wavetab[oscd_number][angle] * Oscd[oscd_number].current_volume ) >> (DAC_BIT-1);
			if ( oscd_activechannels > 1 )
				oscd_output_buffer[i] = ((oscd_output_buffer[i] >> 1 ) + (oscd_buf[oscd_number][i] >> 1));
			else
				oscd_output_buffer[i] = oscd_buf[oscd_number][i];
		}
		Oscd[oscd_number].current_phase += Oscd[oscd_number].delta_phase;
		if ( Oscd[oscd_number].current_phase >= 256.0 )
			Oscd[oscd_number].current_phase -= 256.0;
	}
}

static void OscdSinCompile(uint32_t oscd_number)
{
uint32_t	i;
	for(i=0;i<WAVETABLE_SIZE;i++)
		oscd_wavetab[oscd_number][i] = (uint16_t )(HALF_DAC_RESOLUTION + sin((i*M_PI*2)/WAVETABLE_SIZE)* (HALF_DAC_RESOLUTION-1));
}

void SetOscdVolume(uint16_t osc_number, uint16_t volume)
{
	Oscillator[osc_number].current_volume = volume;
}

void DoOscds(void)
{
uint16_t	i,start,end;
uint16_t	oscd_number;

	get_limits(&start,&end,(uint32_t *)Oscd[0].buffer_flag_ptr);
	for ( i=start;i<end;i++)
		oscd_output_buffer[i] = 0;
	for(oscd_number=0;oscd_number<NUMOSCILLATORS;oscd_number++)
		i_oscd(oscd_number);
}

uint32_t InitOscd(void)
{
uint16_t	oscd_number;
	for(oscd_number=0;oscd_number<NUMOSCILLATORS;oscd_number++)
	{
		Oscd[oscd_number].freq = 0.0;
		Oscd[oscd_number].waveform = SINE;
		Oscd[oscd_number].volume = (DAC_RESOLUTION-1);
		Oscd[oscd_number].current_phase = 0.0;
		Oscd[oscd_number].delta_phase = 1.0;
		Oscd[oscd_number].buffer_flag_ptr = get_bufferhalf(0);
		Oscd[oscd_number].flags = OSCILLATOR_DISABLED;
		OscdSinCompile(oscd_number);
	}
	return NUMOSCILLATORS*2;
}
