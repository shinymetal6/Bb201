/*
 * audio_init.c
 *
 *  Created on: Nov 21, 2020
 *      Author: fil
 */

#include "main.h"
#define	DUAL

__attribute__ ((aligned (4))) uint16_t		audio_buf_0_2_in[AUDIOBUF_LEN];
__attribute__ ((aligned (4))) uint16_t		audio_buf_1_3_in[AUDIOBUF_LEN];

__attribute__ ((aligned (4))) uint32_t		audio_workbuf_0in[NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (4))) uint32_t		audio_workbuf_1in[NUMBER_OF_AUDIO_SAMPLES];

__attribute__ ((aligned (4))) uint32_t		audio_pipe0[NUMSTAGES][NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (4))) uint32_t		audio_pipe1[NUMSTAGES][NUMBER_OF_AUDIO_SAMPLES];

__attribute__ ((aligned (4))) uint32_t		audio_delay_ch0[NUMBER_OF_DELAY_LINES*NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (4))) uint32_t		audio_delay_ch1[NUMBER_OF_DELAY_LINES*NUMBER_OF_AUDIO_SAMPLES];

__attribute__ ((aligned (4))) uint32_t		audio_buf_0_out[NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (4))) uint32_t		audio_buf_1_out[NUMBER_OF_AUDIO_SAMPLES];


uint8_t	half_in_ch0=0 , audioin_buffer_ready_ch0=0;
uint8_t	half_in_ch1=1 , audioin_buffer_ready_ch1=0;
uint8_t	audio_delay_index_ch0 = 0 ,audio_delay_index_ch1 = 0;

/* Helper audio functions */
uint32_t get_bufferhalf(uint32_t channel)
{
	if ( channel == OUTCHANNEL_0 )
		return (uint32_t )&half_in_ch0;
	else
		return (uint32_t )&half_in_ch1;
}

uint32_t get_limits(uint16_t *start,uint16_t *end, uint8_t *half_in)
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
	if ( audioin_buffer_ready_ch0 == 1 )
		audioin_buffer_ready_ch0 = 0;
	if ( audioin_buffer_ready_ch1 == 1 )
		audioin_buffer_ready_ch1 = 0;
}

/* Functions */

#ifdef SIMPLE_ECHO
void do_workbuf_out(void)
{
uint16_t	i,start,end;

		get_limits(&start,&end,&half_in_ch0);
		for ( i=start;i<end;i++)
			audio_buf_0_out[i] = audio_workbuf_0in[i];

		get_limits(&start,&end,&half_in_ch1);
		for ( i=start;i<end;i++)
			audio_buf_1_out[i] = audio_workbuf_1in[i];
}
#endif

void GetBufferIn(void)
{
uint16_t	i,start,end;
uint32_t	*ptr_audio_delay_ch0,*ptr_audio_workbuf_0in;
uint32_t	*ptr_audio_delay_ch1,*ptr_audio_workbuf_1in;
	debug_1();
	if ( audioin_buffer_ready_ch0 == 1 )
	{
		get_limits(&start,&end,&half_in_ch0);
		ptr_audio_delay_ch0 = &audio_delay_ch0[audio_delay_index_ch0];
		ptr_audio_workbuf_0in = &audio_workbuf_0in[start];
		for ( i=start;i<end;i++)
			*ptr_audio_delay_ch0++ = *ptr_audio_workbuf_0in++ = audio_buf_0_2_in[i*2];
			//audio_delay_ch0[audio_delay_index_ch0] = audio_workbuf_0in[i] = audio_buf_0_2_in[i*2];
		audio_delay_index_ch0+=HALF_NUMBER_OF_AUDIO_SAMPLES;
	}
	if ( audioin_buffer_ready_ch1 == 1 )
	{
		get_limits(&start,&end,&half_in_ch1);
		ptr_audio_delay_ch1 = &audio_delay_ch1[audio_delay_index_ch1];
		ptr_audio_workbuf_1in = &audio_workbuf_1in[start];
		for ( i=start;i<end;i++)
			*ptr_audio_delay_ch1++ = *ptr_audio_workbuf_1in++ = audio_buf_0_2_in[(i*2)+1];
			//audio_delay_ch1[audio_delay_index_ch1] = audio_workbuf_1in[i] = audio_buf_0_2_in[(i*2)+1];
		audio_delay_index_ch1+=HALF_NUMBER_OF_AUDIO_SAMPLES;
	}
	debug_0();

}

void AudioCh0Init(void)
{
	HAL_ADCEx_Calibration_Start(AUDIO_0_2_ADC, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	HAL_OPAMP_SelfCalibrate(&hopamp1);
	HAL_ADC_Start_DMA(AUDIO_0_2_ADC, (uint32_t* )&audio_buf_0_2_in , AUDIOBUF_LEN);
	HAL_DAC_Start_DMA(AUDIO_0_DAC, DAC_CHANNEL_1, (uint32_t* )&audio_buf_0_out, NUMBER_OF_AUDIO_SAMPLES, DAC_ALIGN_12B_R);
	HAL_OPAMP_Start(&hopamp1);
}

#ifdef DUAL
void AudioCh1Init(void)
{
	HAL_ADCEx_Calibration_Start(AUDIO_1_3_ADC, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	HAL_OPAMP_SelfCalibrate(&hopamp2);
	HAL_ADC_Start_DMA(AUDIO_1_3_ADC, (uint32_t* )&audio_buf_1_3_in , AUDIOBUF_LEN);
	HAL_DAC_Start_DMA(AUDIO_0_DAC, DAC_CHANNEL_2, (uint32_t* )&audio_buf_1_out, NUMBER_OF_AUDIO_SAMPLES, DAC_ALIGN_12B_R);
	HAL_OPAMP_Start(&hopamp2);
}
#endif
void AudioInit(void)
{
	half_in_ch0 = 0;
	audioin_buffer_ready_ch0 = 0;
	half_in_ch1 = 0;
	audioin_buffer_ready_ch1 = 0;

	AudioCh0Init();
	#ifdef DUAL
	AudioCh1Init();
	#endif
	audio_timer_start();
}

