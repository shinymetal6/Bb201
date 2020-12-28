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
#define	DAC_RESOLUTION		(1<<12)
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

extern	void AudioInit(void);
#define		AUDIO_0_2_ADC			&hadc3
#define		AUDIO_1_3_ADC			&hadc1
#define		AUDIO_0_DAC				&hdac1

#define		SINE					0
#define		TRIANGLE				1
#define		SQUARE					2
#define		SAW						3
#define DMA_BUFFER __attribute__((section(".audio_buffers")))
#define OSC_BUFFER __attribute__((section(".osc_buffers")))

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
extern	__attribute__ ((aligned (4))) AF_ControlTypeDef		control_buf;

uint16_t	control_ready;
#define		CONTROL_ADC				&hadc2
extern	void ControlInit(void);

/* system.c */
#define	NUMSTAGES		96
typedef struct _ProgramTypeDef
{
	uint32_t valid;
	uint32_t (* FuncPtr)(uint32_t in_buffer1,uint32_t in_buffer2, uint32_t out_buffer, uint32_t control_buffer1, uint32_t control_buffer2, uint32_t aux, uint32_t channel);
	uint32_t in_buffer1;
	uint32_t in_buffer2;
	uint32_t out_buffer;
	uint32_t control_buffer1;
	uint32_t control_buffer2;
	uint32_t aux;
	uint32_t channel;
	uint32_t on_stage;
	char 	 function[32];
}ProgramTypeDef;

#define	PROGRAM_VALID	0xc1a0c1a0
typedef struct _SystemParametersTypeDef
{
	char					Header[8];
	char					Version[8];
	uint32_t 				sampling_frequency[2];
	uint32_t 				flash_capacity;
}SystemParametersTypeDef;

typedef struct _SystemFlagsTypeDef
{
	char					Header[8];
	char					Version[8];
	uint32_t 				half_in_ch0;
	uint32_t 				half_in_ch1;
	uint32_t 				audioin_buffer_ready_ch0;
	uint32_t 				audioin_buffer_ready_ch1;
	uint32_t 				control_ready;
}SystemFlagsTypeDef;

extern	uint32_t		stage;
extern	ProgramTypeDef	Program_ch0[NUMSTAGES];
extern	ProgramTypeDef	Program_ch1[NUMSTAGES];
extern	SystemParametersTypeDef	SystemParameters;
extern	SystemFlagsTypeDef		SystemFlags;
extern uint32_t setOutStage(uint32_t function_ptr,uint32_t in_buffer1,uint32_t in_buffer2, uint32_t out_buffer, uint32_t control_buffer1, uint32_t control_buffer2, uint32_t aux, uint32_t channel, uint32_t stage_number,char *function);
extern	void DoFunnelOut(void);
extern	uint32_t ClearFunnelEntries(void);
extern	uint32_t ReportFunnelEntries(uint32_t channel);
extern	char *ReportFunnelName(uint32_t channel,uint32_t funnel_entry);
extern	void bbSystemInit(void);
extern	void debug_0(void);
extern	void debug_1(void);
extern	void ChangeSampleFrequency(uint32_t sampling_frequency , uint32_t channel);
extern	void bbSystem_SystemSetDefaults(void);

/* audio_init.c */
extern	void GetBufferIn(void);
extern	void do_workbuf_out(void);
/* mixer.c */
extern	uint32_t MixerInit(uint32_t in_stage,uint32_t in_buffer1,uint32_t in_buffer2, uint32_t out_buffer,uint32_t control0,uint32_t control1, uint32_t channel);
/* fir.c */
extern	uint32_t q15FirInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer,float fFIRCoef[64], uint32_t channel);
/* float_fir.c */
extern	uint32_t FloatFirInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer,float fFIRCoef[128], uint32_t channel);
/* vco.c */
extern	uint32_t VCOInit(uint32_t in_stage,uint16_t waveform_type, uint32_t out_buffer, uint32_t freq_control, uint32_t channel);
/* echo.c */
extern	uint32_t ECHOInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer, uint32_t channel);
/* vca.c */
#define	INTERNAL_VCACONTROL	0
#define	EXTERNAL_VCACONTROL	1
extern	uint32_t VCAInit(uint32_t in_stage,uint32_t in_buffer, uint32_t out_buffer,uint32_t control, uint32_t channel);
/* lfo.c */
#define	LFOTAB_SIZE		1440
#define	NUM_LFO			4
extern	uint32_t InitLfo(void);
extern	void DoLfo(void);
extern	void ChangeLFOPhase(uint32_t channel,uint32_t lfo_number, uint32_t phase);
extern	void ChangeLFOVolume(uint32_t channel,uint32_t lfo_number, uint32_t volume);
extern	void ChangeLFOWaveform(uint32_t channel,uint32_t lfo_number, uint32_t waveform);
extern	void setLFOParams(uint32_t channel,uint32_t lfo_number,uint32_t freq,uint32_t volume,uint32_t phase,uint32_t waveform,uint32_t osc_group,uint32_t flags);
extern	void ChangeLFOFrequency(uint32_t channel,uint32_t lfo_number, float freq);
extern	__attribute__ ((aligned (16))) uint32_t				lfo_buf[CHANNELS][NUM_LFO][128];
extern	__attribute__ ((aligned (16))) uint32_t				lfo_out[CHANNELS][128];

#define	LFO_0_MIX				(uint32_t )&osc_out[OUTCHANNEL_0]
#define	LFO_1_MIX				(uint32_t )&osc_out[OUTCHANNEL_1]

#define	LFO_0_0_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][0]
#define	LFO_0_1_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][1]
#define	LFO_0_2_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][2]
#define	LFO_0_3_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][3]

#define	LFO_1_0_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][0]
#define	LFO_1_1_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][1]
#define	LFO_1_2_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][2]
#define	LFO_1_3_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][3]

/* ring_mod.c */
extern	uint32_t RINGInit(uint32_t in_stage,uint32_t in_buffer1,uint32_t in_buffer2, uint32_t out_buffer,uint32_t control, uint32_t channel);
/* oscillators.c */
#define		NUMOSCILLATORS			64
#define		OSCILLATOR_ENABLED		1
#define		OSCILLATOR_DISABLED		0
#define		FIXED_FREQUENCY_FLAG	0x80000000
#define		CHANGE_FREQUENCY_FLAG	0x40000000
#define		CHANGE_PHASE_FLAG		0x20000000
#define		NO_FLAG					0x00000000
#define		MAX_VOLUME				DAC_RESOLUTION

typedef struct _OscillatorsTypeDef
{
	uint32_t 				enabled;
	uint32_t 				waveform;
	uint32_t 				channel;
	uint32_t 				midi_note;
	uint32_t 				volume;
	uint32_t 				osc_group;
	uint32_t 				flags;
	uint32_t 				local_controller_ptr;
	uint32_t 				buffer_flag_ptr;
	float 					freq;
	float 					new_freq;
	float 					pitch_bend;
	uint32_t				phase;
	float 					current_phase;
	float 					delta_phase;
}OscillatorsTypeDef;

extern	OscillatorsTypeDef	Oscillator[CHANNELS][NUMOSCILLATORS];
extern	uint32_t InitOscillators(void);
extern	uint32_t osc_buf[CHANNELS][NUMOSCILLATORS][128];
extern	uint32_t osc_out[CHANNELS][128];
extern	void DoOscillators(void);
extern	void EnableOscillator(uint32_t channel,uint32_t osc_number);
extern	void DisableOscillator(uint32_t channel,uint32_t osc_number);
extern	void ChangeOscillatorFrequency(uint32_t channel,uint32_t osc_number, float freq,uint32_t midi_note);
extern	void ChangeOscillatorWaveform(uint32_t channel,uint32_t osc_number, uint32_t waveform);
extern	void ChangeOscillatorPhase(uint32_t channel,uint32_t osc_number, uint32_t phase);
extern	void ChangeOscillatorPitchBend(uint32_t channel,uint32_t osc_number, uint32_t percent);
extern	void ChangeOscillatorVolume(uint32_t channel,uint32_t osc_number, uint32_t volume);

extern	void setOscillatorParams(uint32_t channel,uint32_t osc_number,uint32_t freq,uint32_t volume,uint32_t phase,uint32_t waveform,uint32_t osc_group,uint32_t flags);
extern	uint32_t FindOscByMidi(uint32_t channel, uint32_t midi_note);
extern	float noteToFreq(uint32_t note);

#define	OSCILLATOR_0_MIX				(uint32_t )&osc_out[OUTCHANNEL_0]
#define	OSCILLATOR_1_MIX				(uint32_t )&osc_out[OUTCHANNEL_1]

#define	OSCILLATOR_0_0_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][0]
#define	OSCILLATOR_0_1_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][1]
#define	OSCILLATOR_0_2_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][2]
#define	OSCILLATOR_0_3_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][3]
#define	OSCILLATOR_0_4_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][4]
#define	OSCILLATOR_0_5_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][5]
#define	OSCILLATOR_0_6_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][6]
#define	OSCILLATOR_0_7_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][7]
#define	OSCILLATOR_0_8_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][8]
#define	OSCILLATOR_0_9_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][9]
#define	OSCILLATOR_0_10_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][10]
#define	OSCILLATOR_0_11_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][11]
#define	OSCILLATOR_0_12_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][12]
#define	OSCILLATOR_0_13_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][13]
#define	OSCILLATOR_0_14_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][14]
#define	OSCILLATOR_0_15_BUF				(uint32_t )&osc_buf[OUTCHANNEL_0][15]

#define	OSCILLATOR_1_0_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][0]
#define	OSCILLATOR_1_1_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][1]
#define	OSCILLATOR_1_2_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][2]
#define	OSCILLATOR_1_3_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][3]
#define	OSCILLATOR_1_4_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][4]
#define	OSCILLATOR_1_5_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][5]
#define	OSCILLATOR_1_6_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][6]
#define	OSCILLATOR_1_7_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][7]
#define	OSCILLATOR_1_8_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][8]
#define	OSCILLATOR_1_9_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][9]
#define	OSCILLATOR_1_10_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][10]
#define	OSCILLATOR_1_11_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][11]
#define	OSCILLATOR_1_12_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][12]
#define	OSCILLATOR_1_13_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][13]
#define	OSCILLATOR_1_14_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][14]
#define	OSCILLATOR_1_15_BUF				(uint32_t )&osc_buf[OUTCHANNEL_1][15]


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
#include "audio_buffers.h"
#include "port.h"
#include "flash_manager.h"

#endif /* BBAUDIO_INC_BBAUDIO_H_ */
