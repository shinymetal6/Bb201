/*
 * audio_buffers.h
 *
 *  Created on: Nov 21, 2020
 *      Author: fil
 */

#ifndef BBAUDIO_INC_AUDIO_BUFFERS_H_
#define BBAUDIO_INC_AUDIO_BUFFERS_H_

#define		NUMBER_OF_AUDIO_SAMPLES			128
#define		HALF_NUMBER_OF_AUDIO_SAMPLES	NUMBER_OF_AUDIO_SAMPLES/2
#define		AUDIOBUF_LEN					NUMBER_OF_AUDIO_SAMPLES*2

#define		NUMBER_OF_DELAY_LINES			256

extern	__attribute__ ((aligned (4))) uint16_t		audio_buf_0_2_in[AUDIOBUF_LEN];
extern	__attribute__ ((aligned (4))) uint16_t		audio_buf_1_3_in[AUDIOBUF_LEN];

extern	__attribute__ ((aligned (4))) uint32_t		audio_pipe[NUMSTAGES][NUMBER_OF_AUDIO_SAMPLES];

extern	uint8_t	half_in_ch0 , audioin_buffer_ready_ch0;
extern	uint8_t	half_in_ch1 , audioin_buffer_ready_ch1;
#define	CH0_IN				0
#define	CH1_IN				1
#define	CH2_IN				2
#define	CH3_IN				3
#define	CH0_OUT				30
#define	CH1_OUT				31
#define	AUDIO_BUFIN_CH0		(uint32_t )&audio_pipe[CH0_IN]
#define	AUDIO_BUFIN_CH1		(uint32_t )&audio_pipe[CH0_IN]
#define	AUDIO_BUFOUT_CH0	(uint32_t )&audio_pipe[CH0_OUT]
#define	AUDIO_BUFOUT_CH1	(uint32_t )&audio_pipe[CH1_OUT]



#endif /* BBAUDIO_INC_AUDIO_BUFFERS_H_ */