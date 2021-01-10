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
#define	PHASE_RATIO		(HALF_DAC_RESOLUTION/WAVETABLE_SIZE)

AUDIO_BUFFERS __attribute__ ((aligned (16))) uint16_t	oscd_buf[NUMOSCILLATORS][NUMBER_OF_AUDIO_SAMPLES];
AUDIO_BUFFERS __attribute__ ((aligned (16))) uint16_t	oscd_output_buffer[NUMBER_OF_AUDIO_SAMPLES];

__attribute__ ((aligned (16))) OscdTypeDef	Oscd[NUMOSCILLATORS];
uint16_t	oscd_wavetab[NUMOSCILLATORS][OSC_NUMBER_IN_GROUP][WAVETABLE_SIZE];
uint16_t	angle;

static	void i_oscd(uint32_t oscd_number)
{
uint16_t	i,start,end;
uint32_t	oscd_element,osc_out[4];

	get_limits(&start,&end,(uint32_t *)Oscd[oscd_number].buffer_flag_ptr);
	for ( i=start;i<end;i++)
	{
		for ( oscd_element=0;oscd_element<OSC_NUMBER_IN_GROUP;oscd_element++)
		{
			angle = (uint16_t )(Oscd[oscd_number].current_phase[oscd_element]);
			if ( angle > 255 )
				Oscd[oscd_number].current_phase[oscd_element] -= 256.0;
			angle += Oscd[oscd_number].dephase[oscd_element];
			angle &= (WAVETABLE_SIZE-1);
			osc_out[oscd_element] = (oscd_wavetab[oscd_number][oscd_element][angle] * Oscd[oscd_number].volume[oscd_element] ) >> (DAC_BIT);
			Oscd[oscd_number].current_phase[oscd_element] += (Oscd[oscd_number].delta_phase[oscd_element] * Oscd[oscd_number].detune[oscd_element]);
		}
		oscd_buf[oscd_number][i] = (osc_out[0]+osc_out[1]+osc_out[2]+osc_out[3]) >> 2;
		//oscd_buf[oscd_number][i] = osc_out[0];
		//oscd_buf[oscd_number][i] = (osc_out[0]+osc_out[1]) >> 1;
	}
}

static void OscdSinCompile(uint32_t oscd_number)
{
uint32_t	i;
	for(i=0;i<WAVETABLE_SIZE;i++)
	{
		oscd_wavetab[oscd_number][0][i] = oscd_wavetab[oscd_number][1][i] = oscd_wavetab[oscd_number][2][i] = oscd_wavetab[oscd_number][3][i] = (uint16_t )(HALF_DAC_RESOLUTION + sin((i*M_PI*2)/WAVETABLE_SIZE)* (HALF_DAC_RESOLUTION-1));
	}
}

void SetOscdMasterVolume(uint16_t osc_number, uint16_t volume)
{
uint32_t	oscd_element;
	for ( oscd_element=0;oscd_element<OSC_NUMBER_IN_GROUP;oscd_element++)
		Oscd[osc_number].volume[oscd_element] = volume;
}

void SetOscdElementVolume(uint16_t osc_number,uint16_t element_number, uint16_t volume)
{
	Oscd[osc_number].volume[element_number] = volume;
}

void DoOscds(void)
{
uint16_t	i,start,end;
uint16_t	oscd_number;

	get_limits(&start,&end,(uint32_t *)Oscd[0].buffer_flag_ptr);
	for ( i=start;i<end;i++)
		oscd_output_buffer[i] = 0;
	for(oscd_number=0;oscd_number<NUMOSCILLATORS;oscd_number++)
	{
		i_oscd(oscd_number);
		for ( i=start;i<end;i++)
			oscd_output_buffer[i] += oscd_buf[oscd_number][i];
	}
}

void setOscdDetune(uint16_t oscd_number,uint32_t oscd_element,uint16_t detune)
{
	if ( detune >= DAC_RESOLUTION )
		return;
	Oscd[oscd_number].detune[oscd_element] = ((double )detune - HALF_DAC_RESOLUTION) / DAC_RESOLUTION;
}

void setOscdDephase(uint16_t oscd_number,uint32_t oscd_element,uint16_t dephase)
{
	if ( dephase >= DAC_RESOLUTION )
		return;
	Oscd[oscd_number].dephase[oscd_element] = dephase / (PHASE_RATIO -1);
}

uint32_t InitOscd(uint32_t sampling_frequency)
{
uint16_t	oscd_number;
uint32_t	oscd_element;

	for(oscd_number=0;oscd_number<NUMOSCILLATORS;oscd_number++)
	{
		for ( oscd_element=0;oscd_element<OSC_NUMBER_IN_GROUP;oscd_element++)
		{
			Oscd[oscd_number].frequency[oscd_element] = 375.0;
			Oscd[oscd_number].sampling_frequency[oscd_element] = (double )sampling_frequency;
			Oscd[oscd_number].detune[oscd_element] = 1.0;
			Oscd[oscd_number].dephase[oscd_element] = 0;
			Oscd[oscd_number].waveform[oscd_element] = SINE;
			Oscd[oscd_number].volume[oscd_element] = (DAC_RESOLUTION-1);
			Oscd[oscd_number].current_phase[oscd_element] = 0.0;
			Oscd[oscd_number].delta_phase[oscd_element]  = (double )WAVETABLE_SIZE / ( Oscd[oscd_number].sampling_frequency[oscd_element] / Oscd[oscd_number].frequency[oscd_element]);
		}
		Oscd[oscd_number].buffer_flag_ptr = get_bufferhalf(0);
		OscdSinCompile(oscd_number);
	}
	Oscd[0].dephase[0] = 1536;
	Oscd[0].dephase[1] = 2048;
	Oscd[0].dephase[2] = 2048;
	Oscd[0].dephase[3] = 2048;
	return NUMOSCILLATORS*2;
}
