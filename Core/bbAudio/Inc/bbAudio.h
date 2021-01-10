/*
 * bbAudio.h
 *
 *  Created on: Nov 21, 2020
 *      Author: fil
 */

#ifndef BBAUDIO_INC_BBAUDIO_H_
#define BBAUDIO_INC_BBAUDIO_H_

#include "arm_math.h"


#define	ARM_MATH_CM7
#define	TIMERS_FREQ	240000000
#define	DAC_BIT				12
#define	DAC_RESOLUTION		(1<<DAC_BIT)
#define	HALF_DAC_RESOLUTION	(DAC_RESOLUTION/2)

#define	bbNAME		"B201_a"
#define	bbVERSION	"0.0.1"
/* Audio */
#define	INCHANNEL_0		0
#define	INCHANNEL_1		1
#define	OUTCHANNEL_0	0
#define	OUTCHANNEL_1	1
#define	CHANNELS		2
#define	HALF_BUFFER_FLAG	1
#define	FULL_BUFFER_FLAG	0
#define	BIT_RESOLUTION		12

#define	NUMSTAGES				16
//#define	NUMOSCILLATORS			4
#define	NUMOSCILLATORS			16
#define	NUMVCO					16
#define	NUMLFO					4

#define	NUMBER_OF_AUDIO_SAMPLES			512
#define	HALF_NUMBER_OF_AUDIO_SAMPLES	NUMBER_OF_AUDIO_SAMPLES/2
#define	AUDIOBUF_LEN					NUMBER_OF_AUDIO_SAMPLES*2
#define	RNG_ELEMENTS					NUMBER_OF_AUDIO_SAMPLES

extern	ADC_HandleTypeDef hadc1;
extern	ADC_HandleTypeDef hadc3;
extern	DMA_HandleTypeDef hdma_adc1;
extern	DMA_HandleTypeDef hdma_adc3;
extern	DAC_HandleTypeDef hdac1;
extern	DMA_HandleTypeDef hdma_dac1_ch1;
extern	DMA_HandleTypeDef hdma_dac1_ch2;
extern	OPAMP_HandleTypeDef hopamp1;
extern	OPAMP_HandleTypeDef hopamp2;
extern	SPI_HandleTypeDef hspi4;
extern	TIM_HandleTypeDef htim15;
extern	RNG_HandleTypeDef hrng;

extern	void AudioInit(void);
#define		AUDIO_0_2_ADC			&hadc3
#define		AUDIO_1_3_ADC			&hadc1
#define		AUDIO_0_DAC				&hdac1

#define		SINE					0
#define		TRIANGLE				1
#define		SQUARE					2
#define 	NO_CACHE_BUFFERS	__attribute__((section(".no_cache_buffers")))
#define 	AUDIO_BUFFERS 		__attribute__((section(".audio_buffers")))



/* system.c */


/* noise_generator.c */
extern	uint32_t NoiseGeneratorInit(uint32_t in_stage, uint32_t out_buffer, uint32_t channel);

/* audio_functions.c */

/* echo.c */
extern	uint32_t ECHOInit(uint32_t in_buffer, uint32_t out_buffer, uint32_t channel, uint32_t stage);
/* mixer_2ch.c */
//extern	uint32_t Mixer2CHInit(uint32_t in_buffer, uint32_t out_buffer, uint32_t channel, uint32_t stage);

/* fir.c */
extern	uint32_t q15FirInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer,float fFIRCoef[64], uint32_t channel);
/* vco.c */
extern	uint32_t VCOInit(uint32_t in_stage,uint16_t waveform_type, uint32_t out_buffer, uint32_t freq_control, uint32_t channel);
/* vca.c */
#define	INTERNAL_VCACONTROL	0
#define	EXTERNAL_VCACONTROL	1
extern	uint32_t VCAInit(uint32_t in_buffer, uint32_t out_buffer,uint32_t volume_ch0_source_ptr, uint32_t channel,uint32_t in_stage);

/* vcf.c */
uint32_t VCFInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer, uint32_t channel);
#define	HP	1
#define	LP	2
#define	BP	3





/* Includes, here for back refs */
#include "system.h"
#include "audio_functions.h"
#include "control_functions.h"
#include "port.h"
#include "flash_manager.h"
#include "oscillators.h"
//#include "oscd.h"
#include "vco.h"
#include "lfo.h"
#include "ring_mod.h"
#include "envelope.h"

#endif /* BBAUDIO_INC_BBAUDIO_H_ */
