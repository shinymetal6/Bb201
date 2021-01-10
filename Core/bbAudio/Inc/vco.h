/*
 * vco.h
 *
 *  Created on: Jan 9, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_VCO_H_
#define BBAUDIO_INC_VCO_H_

#define	VCO_NUMBER_OF_ELEMENTS	4
#define	VCO_MAX_VOLUME			(DAC_RESOLUTION-1)
extern	uint16_t	vco_buf[NUMVCO][NUMBER_OF_AUDIO_SAMPLES];
extern	uint16_t	vco_output_buffer[NUMBER_OF_AUDIO_SAMPLES];

extern	uint32_t InitVco(uint32_t sampling_frequency);
extern	void DoVco(void);
extern	void VcoElementDetune(uint16_t	vco_number,uint16_t	vco_element,int16_t delta_frequency,uint32_t sampling_frequency);



typedef struct _VCOTypeDef
{
	uint32_t				sampling_frequency;
	uint32_t 				current_phase[VCO_NUMBER_OF_ELEMENTS];
	uint32_t 				delta_phase[VCO_NUMBER_OF_ELEMENTS];
	uint16_t				frequency[VCO_NUMBER_OF_ELEMENTS];
	uint16_t				detune[VCO_NUMBER_OF_ELEMENTS];
	uint8_t					dephase[VCO_NUMBER_OF_ELEMENTS];
	uint16_t 				waveform[VCO_NUMBER_OF_ELEMENTS];
	uint16_t 				*volume_control_ptr[VCO_NUMBER_OF_ELEMENTS];	/* range is 0 .. 4095 */
	uint16_t				*master_volume_control_ptr;
	uint32_t 				buffer_flag_ptr;
}VCOTypeDef;


#endif /* BBAUDIO_INC_VCO_H_ */
