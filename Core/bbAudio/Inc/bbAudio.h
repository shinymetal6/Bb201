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
#define NO_CACHE_BUFFERS	__attribute__((section(".no_cache_buffers")))
#define AUDIO_BUFFERS 		__attribute__((section(".audio_buffers")))

/* Controls */
#define		CONTROLBUF_LEN					6
extern	ADC_HandleTypeDef hadc2;
typedef struct _AF_ControlTypeDef
{
	uint16_t ain0;
	uint16_t ain1;
	uint16_t ain2;
	uint16_t ain3;
	uint16_t ain4;
	uint16_t ain5;
}AF_ControlTypeDef;
extern	AF_ControlTypeDef		control_buf;

uint16_t	control_ready;
#define		CONTROL_ADC				&hadc2
extern	void ControlInit(void);

/* system.c */

/* index to volume controls */
#define	VOLUME_SOURCE_LOCAL		0
#define	VOLUME_SOURCE_MIDI		1
#define	VOLUME_SOURCE_EXTERNAL	2
#define	VOLUME_SOURCE_FROMCOMP	3
/* Flags meaning */
#define	COMP_HAS_LOCALVOLUME			1 << 0
#define	COMP_HAS_MIDIVOLUME				1 << 1
#define	COMP_HAS_EXTERNALVOLUME			1 << 2
#define	COMP_HAS_FROMCOMPVOLUME			1 << 3

typedef struct _S_Component
{
	uint32_t (* FuncPtr)(uint16_t channel,uint16_t index);
	uint32_t in_buffer0;
	uint32_t in_buffer1;
	uint32_t in_buffer2;
	uint32_t in_buffer3;
	uint32_t out_buffer0;
	uint32_t out_buffer1;
	uint8_t  volumes_ch0_source;
	uint8_t  volumes_ch1_source;
	uint8_t  volumes_ch2_source;
	uint8_t  volumes_ch3_source;
	uint32_t volume_ch0_source_ptr;
	uint32_t volume_ch1_source_ptr;
	uint32_t volume_ch2_source_ptr;
	uint32_t volume_ch3_source_ptr;
	uint16_t aux_ch0[4];
	uint16_t aux_ch1[4];
	uint16_t aux_ch2[4];
	uint16_t aux_ch3[4];
	uint32_t half_ptr;
	uint16_t flags;
	uint16_t channel;
	uint16_t stage_number;
	char 	 functionName[32];
}S_Component;

#define	PROGRAM_VALID	0xc1a0c1a0
typedef struct _SystemParametersTypeDef
{
	char					Header[8];
	char					Version[8];
	uint32_t 				sampling_frequency[2];
	uint32_t 				global_detune[2];
	uint32_t 				flash_capacity;
}SystemParametersTypeDef;

typedef struct _SystemFlagsTypeDef
{
	uint32_t 				half_in_ch0;
	uint32_t 				half_in_ch1;
	uint32_t 				audioin_buffer_ready_ch0;
	uint32_t 				audioin_buffer_ready_ch1;
	uint32_t 				control_ready;
}SystemFlagsTypeDef;

extern	uint32_t		stage;
/*
extern	ProgramTypeDef	Program_ch0[NUMSTAGES];
extern	ProgramTypeDef	Program_ch1[NUMSTAGES];
*/
extern	S_Component				Component[NUMSTAGES];
extern	SystemParametersTypeDef	SystemParameters;
extern	SystemFlagsTypeDef		SystemFlags;
//extern uint32_t setOutStage(uint32_t function_ptr,uint32_t in_buffer1,uint32_t in_buffer2, uint32_t out_buffer, uint32_t control_buffer1, uint32_t control_buffer2, uint32_t aux1, uint32_t aux2, uint32_t aux3, uint32_t aux4, uint32_t channel, uint32_t stage_number,char *function);
extern	uint32_t setOutStage(S_Component *component,uint16_t channel,uint32_t stage_number);
extern	void DoFunnelOut(void);
/*
extern	uint32_t ClearFunnelEntries(void);
extern	uint32_t ReportFunnelEntries(uint32_t channel);
extern	char *ReportFunnelName(uint32_t channel,uint32_t funnel_entry);
*/
extern	void bbSystemInit(void);
extern	void debug_0(void);
extern	void debug_1(void);
extern	void ChangeSampleFrequency(uint32_t sampling_frequency , uint32_t channel);
extern	void bbSystem_SystemSetDefaults(void);
/* noise_generator.c */
extern	uint32_t NoiseGeneratorInit(uint32_t in_stage, uint32_t out_buffer, uint32_t channel);

/* audio_functions.c */
extern	void GetBufferIn(void);
extern	void do_workbuf_out(void);
extern	uint16_t	RNG_Buffer[RNG_ELEMENTS];
extern	void RNG_callback(void);

/* audio_buffers.c */
/* No cache on this areas */
extern	__attribute__ ((aligned (4))) uint16_t	audio_buf_0_2_in[AUDIOBUF_LEN];
extern	__attribute__ ((aligned (4))) uint16_t	audio_buf_1_3_in[AUDIOBUF_LEN];
extern	__attribute__ ((aligned (4))) uint16_t	audio_buf_out[CHANNELS][NUMBER_OF_AUDIO_SAMPLES];

#define	CH0_IN				0
#define	CH1_IN				1
#define	CH2_IN				2
#define	CH3_IN				3

#define	AUDIO_BUFIN_CH0		(uint32_t )&audio_pipe[CH0_IN]
#define	AUDIO_BUFIN_CH1		(uint32_t )&audio_pipe[CH1_IN]
#define	AUDIO_BUFIN_CH2		(uint32_t )&audio_pipe[CH2_IN]
#define	AUDIO_BUFIN_CH3		(uint32_t )&audio_pipe[CH3_IN]
#define	AUDIO_BUFOUT_CH0	(uint32_t )&audio_buf_out[0]
#define	AUDIO_BUFOUT_CH1	(uint32_t )&audio_buf_out[1]

/* This area can be cached */
extern	__attribute__ ((aligned (4))) uint16_t	audio_pipe[NUMSTAGES][NUMBER_OF_AUDIO_SAMPLES];
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

/* ring_mod.c */
extern	uint32_t RINGInit(uint32_t in_stage,uint32_t in_buffer1,uint32_t in_buffer2, uint32_t out_buffer,uint32_t control, uint32_t channel);
/* vcf.c */
uint32_t VCFInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer, uint32_t channel);
#define	HP	1
#define	LP	2
#define	BP	3

/* oscillators.c */

/* usb_commands.c */
#define	USB_TXBUF_SIZE	2048
extern	uint32_t	usb_packet_ready;
extern	uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
extern	uint8_t	USB_RxBuffer[64];
extern	char	USB_TxBuffer[USB_TXBUF_SIZE];
extern	uint32_t USB_RxBufferLen;
extern	void CheckUSB(void);

#define	USBUART_BUFLEN	128
typedef struct {
	uint8_t byte_count;
	uint8_t usb_header;
	uint8_t usb_rx_index;
	uint8_t usb_flag;
	uint8_t packet[USBUART_BUFLEN];
} s_rxbuf;

extern	s_rxbuf		usb_rxbuf;

/* Debug and utilities functions */
extern	void change_tim_frequency(uint32_t sampling_frequency , uint32_t channel);

extern	uint32_t get_bufferhalf(uint32_t channel);
extern	uint32_t get_limits(uint16_t *start,uint16_t *end, uint32_t *half_in);
extern	void clear_buffer_ready_flag(void);

/* Includes, here for back refs */
#include "port.h"
#include "flash_manager.h"
#include "oscillators.h"
#include "oscd.h"
#include "vco.h"
#include "lfo.h"

#endif /* BBAUDIO_INC_BBAUDIO_H_ */
