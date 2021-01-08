/*
 * oscd.h
 *
 *  Created on: Jan 8, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_OSCD_H_
#define BBAUDIO_INC_OSCD_H_

typedef struct _OscdTypeDef
{
	double	 				current_phase;
	double 					delta_phase;
	double 					sampling_frequency;
	double 					freq;
	double					new_freq;
	double 					pitch_bend;
	uint16_t 				waveform;
	uint16_t 				channel;
	uint16_t 				midi_note;
	uint16_t 				volume;
	uint16_t 				current_volume;
	uint16_t 				osc_group;
	uint16_t 				flags;
	uint32_t 				local_controller_ptr;
	uint32_t 				buffer_flag_ptr;
	uint16_t				phase;
	uint16_t 				detune;
}OscdTypeDef;


#endif /* BBAUDIO_INC_OSCD_H_ */
