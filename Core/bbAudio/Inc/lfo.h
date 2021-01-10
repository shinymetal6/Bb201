/*
 * lfo.h
 *
 *  Created on: Jan 10, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_LFO_H_
#define BBAUDIO_INC_LFO_H_

extern	uint16_t	lfo_buf[NUMLFO][NUMBER_OF_AUDIO_SAMPLES];
extern	uint32_t InitLfo(uint32_t sampling_frequency);
extern	void DoLfo(void);

typedef struct _LFOTypeDef
{
	uint32_t				sampling_frequency;
	uint32_t 				current_phase;
	uint32_t 				delta_phase;
	uint16_t				frequency;
	uint16_t 				waveform;
	uint16_t 				volume;	/* range is 0 .. 4095 */
	uint16_t 				*volume_control_ptr;
	uint32_t 				buffer_flag_ptr;
}LFOTypeDef;

#endif /* BBAUDIO_INC_LFO_H_ */
