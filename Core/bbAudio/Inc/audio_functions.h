/*
 * audio_functions.h
 *
 *  Created on: Jan 10, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_AUDIO_FUNCTIONS_C_
#define BBAUDIO_INC_AUDIO_FUNCTIONS_C_

extern	void GetBufferIn(void);
extern	void do_workbuf_out(void);
extern	uint16_t	RNG_Buffer[RNG_ELEMENTS];
extern	void RNG_callback(void);
extern	uint32_t get_bufferhalf(uint32_t channel);
extern	uint32_t get_limits(uint16_t *start,uint16_t *end, uint32_t *half_in);
extern	void clear_buffer_ready_flag(void);

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

#endif /* BBAUDIO_INC_AUDIO_FUNCTIONS_C_ */
