/*
 * oscd.h
 *
 *  Created on: Jan 8, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_OSCD_H_
#define BBAUDIO_INC_OSCD_H_

#define	VCO_NUMBER_OF_ELEMENTS	4
typedef struct _OscdTypeDef
{
	double	 				current_phase[VCO_NUMBER_OF_ELEMENTS];
	double 					delta_phase[VCO_NUMBER_OF_ELEMENTS];
	double 					frequency[VCO_NUMBER_OF_ELEMENTS];
	double 					sampling_frequency[VCO_NUMBER_OF_ELEMENTS];
	double 					detune[VCO_NUMBER_OF_ELEMENTS];
	uint16_t				dephase[VCO_NUMBER_OF_ELEMENTS];				/* 0 .. 4095, 2048 is no phase change */
	uint16_t 				waveform[VCO_NUMBER_OF_ELEMENTS];
	uint16_t 				volume[VCO_NUMBER_OF_ELEMENTS];
	uint16_t 				channel;
	uint32_t 				local_controller_ptr;
	uint32_t 				buffer_flag_ptr;
}OscdTypeDef;

extern	uint16_t	oscd_buf[NUMOSCILLATORS][NUMBER_OF_AUDIO_SAMPLES];
extern	uint16_t	oscd_output_buffer[NUMBER_OF_AUDIO_SAMPLES];

#endif /* BBAUDIO_INC_OSCD_H_ */
