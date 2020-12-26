/*
 * audio_init.c
 *
 *  Created on: Nov 21, 2020
 *      Author: fil
 */

#include "main.h"
#define	DUAL


DMA_BUFFER __attribute__ ((aligned (4))) uint16_t	audio_buf_0_2_in[AUDIOBUF_LEN];
DMA_BUFFER __attribute__ ((aligned (4))) uint16_t	audio_buf_1_3_in[AUDIOBUF_LEN];
DMA_BUFFER __attribute__ ((aligned (4))) uint32_t	audio_pipe[NUMSTAGES][NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (4))) uint32_t	osc_buf[CHANNELS][NUMOSCILLATORS][NUMBER_OF_AUDIO_SAMPLES];

/* Helper audio functions */
uint32_t get_bufferhalf(uint32_t channel)
{
	return  (channel == OUTCHANNEL_0 ? (uint32_t )&SystemFlags.half_in_ch0 : (uint32_t )&SystemFlags.half_in_ch1);
}

uint32_t get_limits(uint16_t *start,uint16_t *end, uint32_t *half_in)
{
	if ( *half_in == HALF_BUFFER_FLAG)
	{
		*start=0;
		*end = HALF_NUMBER_OF_AUDIO_SAMPLES;
	}
	else
	{
		*start=HALF_NUMBER_OF_AUDIO_SAMPLES;
		*end = NUMBER_OF_AUDIO_SAMPLES;
	}
	return *half_in;
}

void clear_buffer_ready_flag(void)
{
	if ( SystemFlags.audioin_buffer_ready_ch0 == 1 )
		SystemFlags.audioin_buffer_ready_ch0 = 0;
	if ( SystemFlags.audioin_buffer_ready_ch1 == 1 )
		SystemFlags.audioin_buffer_ready_ch1 = 0;
}

/* Functions */
void GetBufferIn(void)
{
uint16_t	i,start,end;
	if ( SystemFlags.audioin_buffer_ready_ch0 == 1 )
	{
		get_limits(&start,&end,&SystemFlags.half_in_ch0);
		for ( i=start;i<end;i++)
		{
			audio_pipe[CH0_IN][i] = audio_buf_0_2_in[i*2];
			audio_pipe[CH1_IN][i] = audio_buf_0_2_in[(i*2)+1];
		}
	}
	if ( SystemFlags.audioin_buffer_ready_ch1 == 1 )
	{
		get_limits(&start,&end,&SystemFlags.half_in_ch1);
		for ( i=start;i<end;i++)
		{
			audio_pipe[CH2_IN][i] = audio_buf_1_3_in[i*2];
			audio_pipe[CH3_IN][i] = audio_buf_1_3_in[(i*2)+1];
		}
	}
}

void AudioCh0Init(void)
{
	HAL_ADCEx_Calibration_Start(AUDIO_0_2_ADC, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	HAL_OPAMP_SelfCalibrate(&hopamp1);
	HAL_ADC_Start_DMA(AUDIO_0_2_ADC, (uint32_t* )&audio_buf_0_2_in , AUDIOBUF_LEN);
	HAL_DAC_Start_DMA(AUDIO_0_DAC, DAC_CHANNEL_1, (uint32_t* )AUDIO_BUFOUT_CH0, NUMBER_OF_AUDIO_SAMPLES, DAC_ALIGN_12B_R);
	HAL_OPAMP_Start(&hopamp1);
}

#ifdef DUAL
void AudioCh1Init(void)
{
	HAL_ADCEx_Calibration_Start(AUDIO_1_3_ADC, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	HAL_OPAMP_SelfCalibrate(&hopamp2);
	HAL_ADC_Start_DMA(AUDIO_1_3_ADC, (uint32_t* )&audio_buf_1_3_in , AUDIOBUF_LEN);
	HAL_DAC_Start_DMA(AUDIO_0_DAC, DAC_CHANNEL_2, (uint32_t* )AUDIO_BUFOUT_CH1, NUMBER_OF_AUDIO_SAMPLES, DAC_ALIGN_12B_R);
	HAL_OPAMP_Start(&hopamp2);
}
#endif
void AudioInit(void)
{
	SystemFlags.half_in_ch0 = 0;
	SystemFlags.audioin_buffer_ready_ch0 = 0;
	SystemFlags.half_in_ch1 = 0;
	SystemFlags.audioin_buffer_ready_ch1 = 0;

	AudioCh0Init();
	#ifdef DUAL
	AudioCh1Init();
	#endif
	audio_timer_start();
}

