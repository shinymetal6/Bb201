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

extern	__attribute__ ((aligned (4))) uint32_t		audio_workbuf_0in[NUMBER_OF_AUDIO_SAMPLES];
extern	__attribute__ ((aligned (4))) uint32_t		audio_workbuf_1in[NUMBER_OF_AUDIO_SAMPLES];

extern	__attribute__ ((aligned (4))) uint32_t		audio_pipe0[NUMSTAGES][NUMBER_OF_AUDIO_SAMPLES];
extern	__attribute__ ((aligned (4))) uint32_t		audio_pipe1[NUMSTAGES][NUMBER_OF_AUDIO_SAMPLES];

extern	__attribute__ ((aligned (4))) uint32_t		audio_delay_ch0[NUMBER_OF_DELAY_LINES*NUMBER_OF_AUDIO_SAMPLES];
extern	__attribute__ ((aligned (4))) uint32_t		audio_delay_ch1[NUMBER_OF_DELAY_LINES*NUMBER_OF_AUDIO_SAMPLES];

extern	__attribute__ ((aligned (4))) uint32_t		audio_buf_0_out[NUMBER_OF_AUDIO_SAMPLES];
extern	__attribute__ ((aligned (4))) uint32_t		audio_buf_1_out[NUMBER_OF_AUDIO_SAMPLES];

extern	uint8_t	half_in_ch0 , audioin_buffer_ready_ch0;
extern	uint8_t	half_in_ch1 , audioin_buffer_ready_ch1;

#define	AUDIO_BUFIN_CH0		(uint32_t )&audio_workbuf_0in[0]
#define	AUDIO_BUFIN_CH1		(uint32_t )&audio_workbuf_1in[0]
#define	AUDIO_BUFOUT_CH0	(uint32_t )&audio_buf_0_out[0]
#define	AUDIO_BUFOUT_CH1	(uint32_t )&audio_buf_1_out[0]

#define	AUDIOPIPE_0_CH0		(uint32_t )&audio_pipe0[0]
#define	AUDIOPIPE_1_CH0		(uint32_t )&audio_pipe0[1]
#define	AUDIOPIPE_2_CH0		(uint32_t )&audio_pipe0[2]
#define	AUDIOPIPE_3_CH0		(uint32_t )&audio_pipe0[3]
#define	AUDIOPIPE_4_CH0		(uint32_t )&audio_pipe0[4]
#define	AUDIOPIPE_5_CH0		(uint32_t )&audio_pipe0[5]
#define	AUDIOPIPE_6_CH0		(uint32_t )&audio_pipe0[6]
#define	AUDIOPIPE_7_CH0		(uint32_t )&audio_pipe0[7]
#define	AUDIOPIPE_8_CH0		(uint32_t )&audio_pipe0[8]
#define	AUDIOPIPE_9_CH0		(uint32_t )&audio_pipe0[9]
#define	AUDIOPIPE_10_CH0	(uint32_t )&audio_pipe0[10]
#define	AUDIOPIPE_11_CH0	(uint32_t )&audio_pipe0[11]
#define	AUDIOPIPE_12_CH0	(uint32_t )&audio_pipe0[12]
#define	AUDIOPIPE_13_CH0	(uint32_t )&audio_pipe0[13]
#define	AUDIOPIPE_14_CH0	(uint32_t )&audio_pipe0[14]
#define	AUDIOPIPE_15_CH0	(uint32_t )&audio_pipe0[15]


#endif /* BBAUDIO_INC_AUDIO_BUFFERS_H_ */
