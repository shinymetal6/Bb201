/*
 * oscillators.h
 *
 *  Created on: Jan 8, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_OSCILLATORS_H_
#define BBAUDIO_INC_OSCILLATORS_H_

#define		OSCILLATOR_DISABLED			0
#define		OSCILLATOR_MIDI				1
#define		OSCILLATOR_FREE_RUNNING		2
#define		OSCILLATOR_LOCAL_CONTROL	4
#define		MAX_VOLUME					DAC_RESOLUTION
#define		MIN_VOLUME					0

typedef struct _OscillatorsTypeDef
{
	uint16_t 				current_phase;
	uint16_t 				delta_phase;
	uint16_t 				waveform;
	uint16_t 				channel;
	uint16_t 				midi_note;
	uint16_t 				volume;
	uint16_t 				current_volume;
	uint16_t 				osc_group;
	uint16_t 				flags;
	uint32_t 				local_controller_ptr;
	uint32_t 				buffer_flag_ptr;
	float 					sampling_frequency;
	float 					freq;
	float					new_freq;
	float 					pitch_bend;
	uint16_t				phase;
	uint16_t 				detune;
}OscillatorsTypeDef;

extern	OscillatorsTypeDef	Oscillator[NUMOSCILLATORS];
extern	uint32_t InitOscillators(void);
extern	uint16_t osc_buf[NUMOSCILLATORS][NUMBER_OF_AUDIO_SAMPLES];
extern	uint16_t osc_output_buffer[NUMBER_OF_AUDIO_SAMPLES];
extern	void SetOscillatorParams(uint16_t osc_number, uint32_t sampling_frequency, uint16_t freq, uint8_t waveform, uint16_t volume, uint16_t phase, uint16_t osc_group, uint16_t enabled);
extern	void SetOscillatorFrequency(uint16_t osc_number, uint32_t sampling_frequency, uint16_t freq, uint16_t volume, uint16_t midi_note);
extern	void SetOscillatorWaveform(uint16_t osc_number, uint8_t waveform, uint16_t phase);
extern	void EnableOscillator(uint32_t osc_number);
extern	void DisableOscillator(uint16_t osc_number);
extern	void SetOscillatorGroup(uint16_t osc_number,uint32_t group);
extern	void UnSetOscillatorGroup(uint16_t osc_number );
extern	void SetOscillatorVolume(uint16_t osc_number, uint16_t volume);
extern	void SetOscillatorPhase(uint16_t osc_number, uint8_t phase);
extern	void SetOscillatorDeTune(uint16_t osc_number, uint16_t detune);
extern	void SetOscillatorFlag(uint16_t osc_number,uint32_t flags);

extern	uint32_t FindOscByMidi(uint32_t midi_note);
extern	void DoOscillators(void);
extern	uint32_t InitOscillators(void);

extern	double noteToFreq(uint32_t note);

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



#endif /* BBAUDIO_INC_OSCILLATORS_H_ */
