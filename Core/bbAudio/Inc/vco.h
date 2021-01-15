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
#define	VCO_MIN_VOLUME			0
extern	uint16_t	vco_buf[NUMVCO][NUMBER_OF_AUDIO_SAMPLES];
extern	uint16_t	vco_output_buffer[NUMBER_OF_AUDIO_SAMPLES];

extern	uint16_t	static_vco_master_volume_control[NUMVCO];
extern	uint16_t	static_vco_frequency_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
extern	uint8_t		static_vco_detune_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
extern	uint16_t	static_vco_phase_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
extern	uint16_t	static_vco_volume_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];

extern	uint16_t	midi_vco_master_volume_control[NUMVCO];
extern	uint16_t	midi_vco_frequency_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
extern	uint8_t		midi_vco_detune_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
extern	uint16_t	midi_vco_phase_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];
extern	uint16_t	midi_vco_volume_control[NUMVCO][VCO_NUMBER_OF_ELEMENTS];

extern	uint32_t InitVco(uint32_t sampling_frequency);
extern	void DoVco(void);
extern	void VcoElementDetune(uint16_t	vco_number,uint16_t	vco_element,int16_t delta_frequency,uint32_t sampling_frequency);
extern	void VcoSetFrequency(uint16_t vco_number,uint32_t frequency,uint32_t sampling_frequency);
extern	void VcoSetMasterVolume(uint16_t vco_number,uint32_t master_volume_ptr);
extern	void VcoSetMidiNote(uint16_t vco_number,uint16_t midi_note);
extern	uint32_t FindOscByMidi(uint32_t midi_note);
extern	void VcoSetElementWave(uint16_t vco_number, uint8_t vco_element, uint8_t waveform, uint16_t phase);
extern	void VcoSetElementVolume(uint16_t	vco_number,uint16_t	vco_element,uint32_t volume_ptr);


typedef struct _VCOTypeDef
{
	uint32_t				sampling_frequency;
	uint16_t				frequency;
	uint32_t 				current_phase[VCO_NUMBER_OF_ELEMENTS];
	uint32_t 				delta_phase[VCO_NUMBER_OF_ELEMENTS];
	uint16_t				detune[VCO_NUMBER_OF_ELEMENTS];
	uint8_t					*dephase[VCO_NUMBER_OF_ELEMENTS];
	uint16_t 				waveform[VCO_NUMBER_OF_ELEMENTS];
	uint16_t 				*volume_control[VCO_NUMBER_OF_ELEMENTS];	/* range is 0 .. 4095 */
	uint16_t				*master_volume_control;
	uint16_t 				midi_note;
	uint32_t 				buffer_flag_ptr;
}VCOTypeDef;


#endif /* BBAUDIO_INC_VCO_H_ */
