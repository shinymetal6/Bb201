/*
 * usb_midi.c
 *
 *  Created on: Dec 23, 2020
 *      Author: fil
 */

#include "main.h"
#include "usb_midi.h"
#include <stdio.h>
#include <string.h>
#include <usbd_midi_if.h>

__attribute__ ((aligned (4)))	uint8_t		midi_rx_buffer[MIDI_RXBUF_SIZE];	/* filled by MIDI_Receive_FS in usbd_midi_if.c */
__attribute__ ((aligned (4)))	uint8_t		midi_rx_buf[MIDI_RXBUF_SIZE];
__attribute__ ((aligned (4)))	uint8_t		midi_tx_buf[MIDI_RXBUF_SIZE];
uint8_t		midi_rx_flag;
uint16_t 	midi_rx_length;
uint16_t 	midi_rx_len;

float noteToFreq(uint32_t note)
{
    return (float )(440.0 * pow( 2.0, ((double)note - 69.0) / 12.0 ));
}

static uint16_t SYSEX_ConvertMidiBufOut(uint8_t *buf_out , uint16_t len)
{
uint32_t	i,k=0;
	midi_tx_buf[0] = SYSEX;
	for(i=1;i<len;i++)
	{
		if ( ((i & 0x0f) == 0x0c) || ((i & 0x0f) == 0x08) || ((i & 0x0f) == 0x04) || ((i & 0x0f) == 0x00) )
		{
			midi_tx_buf[k] = 0x04;
			k++;
		}
		midi_tx_buf[k] = buf_out[i];
		k++;
	}
	midi_tx_buf[k] = SYSEX_END;
	return k;
}

static uint32_t SYSEX_ConvertMidiBufIn(void)
{
uint32_t	i;

	midi_rx_len=0;
	for(i=1;i<midi_rx_length;i++)
	{
		if ( ((i & 0x0f) != 0x0c) && ((i & 0x0f) != 0x08) && ((i & 0x0f) != 0x04) && ((i & 0x0f) != 0x00) )
		{
			midi_rx_buf[midi_rx_len] = midi_rx_buffer[i];
			midi_rx_len++;
			if (midi_rx_buffer[i] == SYSEX_END)
			{
				if ( (midi_rx_buf[1] == 0) && (midi_rx_buf[2] == 'b') && (midi_rx_buf[3] == 'B'))
					return 1;
			}
		}
	}
	return 0;
}


static void SysCmdsSYSEX(uint8_t cmd)
{
	switch (midi_rx_buf[4])
	{
	case	SYSEX_BB_CLEAR			:	MIDI_Transmit_FS(midi_rx_buffer,midi_rx_length); break;
	case	SYSEX_BB_STORESETTINGS	:	StoreSettingsInFlash();break;
	case	SYSEX_BB_LOADSETTINGS	:	LoadSettingsFromFlash();break;
	case	SYSEX_BB_STOREPROGRAMS	:	StoreProgramInFlash();break;
	case	SYSEX_BB_LOADPROGRAMS	:	LoadProgramFromFlash();break;
	default : break;
	}
}
static void SetOscillator(void)
{
uint32_t channel, osc_number, freq, volume, phase, waveform, osc_group, flags;
	channel = midi_rx_buf[5]&0x0f;
	osc_number = midi_rx_buf[6]&0x0f;
	freq =
			((midi_rx_buf[7]&0x0f)*10000)+
			((midi_rx_buf[8]&0x0f)*1000)+
			((midi_rx_buf[9]&0x0f)*100)+
			((midi_rx_buf[10]&0x0f)*10)+
			((midi_rx_buf[11]&0x0f));
	volume =
			((midi_rx_buf[12]&0x0f)*100)+
			((midi_rx_buf[13]&0x0f)*10)+
			((midi_rx_buf[14]&0x0f));
	phase =
			((midi_rx_buf[15]&0x0f)*100)+
			((midi_rx_buf[16]&0x0f)*10)+
			((midi_rx_buf[17]&0x0f));
	waveform = midi_rx_buf[18]&0x0f;
	osc_group = midi_rx_buf[19]&0x0f;
	flags = midi_rx_buf[20]&0x0f;

	setOscillatorParams( channel, osc_number, freq, volume, phase, waveform, osc_group, flags);
}

static void SetLfo(void)
{
uint32_t channel, lfo_number, freq, volume, phase, waveform, osc_group, flags;
	channel = midi_rx_buf[5]&0x0f;
	lfo_number = midi_rx_buf[6]&0x0f;
	freq =
			((midi_rx_buf[7]&0x0f)*10000)+
			((midi_rx_buf[8]&0x0f)*1000)+
			((midi_rx_buf[9]&0x0f)*100)+
			((midi_rx_buf[10]&0x0f)*10)+
			((midi_rx_buf[11]&0x0f));
	volume =
			((midi_rx_buf[12]&0x0f)*100)+
			((midi_rx_buf[13]&0x0f)*10)+
			((midi_rx_buf[14]&0x0f));
	phase =
			((midi_rx_buf[15]&0x0f)*100)+
			((midi_rx_buf[16]&0x0f)*10)+
			((midi_rx_buf[17]&0x0f));
	waveform = midi_rx_buf[18]&0x0f;
	osc_group = midi_rx_buf[19]&0x0f;
	flags = midi_rx_buf[20]&0x0f;

	setLFOParams( channel, lfo_number, freq, volume, phase, waveform, osc_group, flags);
	EnableLFO(channel, lfo_number);
}

static void OscCmdsSYSEX(uint8_t cmd)
{
uint32_t channel = midi_rx_buf[5]&0x0f, osc_number=midi_rx_buf[6]&0x0f,parameter;

	parameter =	(((midi_rx_buf[7]&0x0f)*100)+((midi_rx_buf[8]&0x0f)*10)+((midi_rx_buf[9]&0x0f))); // default parameter
	switch (midi_rx_buf[4])
	{
	case	SYSEX_BB_SETOSCILLATOR	:	SetOscillator();break;
	case	SYSEX_BB_SETOSCWAVE		:
		parameter = midi_rx_buf[7]&0x0f;
		ChangeOscillatorWaveform( channel, osc_number, parameter );break;
	case	SYSEX_BB_SETOSCVOL		:
		ChangeOscillatorVolume( channel, osc_number, parameter);break;
	case	SYSEX_BB_SETOSCPHASE		:
		ChangeOscillatorPhase( channel, osc_number, parameter);break;
	case	SYSEX_BB_SETLFO	:	SetLfo();break;
	case	SYSEX_BB_SETLFOWAVE		:
		parameter = midi_rx_buf[7]&0x0f;
		ChangeOscillatorWaveform( channel, osc_number, parameter );break;
	case	SYSEX_BB_SETLFOVOL		:
		ChangeOscillatorVolume( channel, osc_number, parameter);break;
	case	SYSEX_BB_SETLFOPHASE		:
		ChangeOscillatorPhase( channel, osc_number, parameter);break;
	default : break;
	}
}

static void ComponentInsert(void)
{

}

static void ComponentModifyInput(void)
{

}

static void ComponentModifyOutput(void)
{

}

static void ComponentDefineController(void)
{

}

static void CompCmdsSYSEX(uint8_t cmd)
{
	switch (midi_rx_buf[4])
	{
	case	SYSEX_BB_COMP_INSERT	:	ComponentInsert(); break;
	case	SYSEX_BB_COMP_MODIN		:	ComponentModifyInput();break;
	case	SYSEX_BB_COMP_MODOUT	:	ComponentModifyOutput();break;
	case	SYSEX_BB_COMP_DEFCNTRL	:	ComponentDefineController();break;
	default : break;
	}
}

static void UsbMidiParseSYSEX(void)
{
	if ( SYSEX_ConvertMidiBufIn() == 0 )
		return;
	switch (midi_rx_buf[4] & SYSEX_BB_CMDMASK)
	{
	case	SYSEX_BB_SYSTEM			:	SysCmdsSYSEX(midi_rx_buf[4]); break;
	case	SYSEX_BB_OSCLFO			:	OscCmdsSYSEX(midi_rx_buf[4]); break;
	case	SYSEX_BB_COMPONENTS		:	CompCmdsSYSEX(midi_rx_buf[4]); break;
	default : break;
	}
}

#define	MIDI_CHANNEL	midi_rx_buffer[1]&0x0f
#define	MIDI_NOTE		midi_rx_buffer[2]
#define	MIDI_FREQUENCY	noteToFreq(midi_rx_buffer[2])
#define	MIDI_VELOCITY	midi_rx_buffer[3]

uint32_t	active_oscillators = 0;

static void UsbMidiParseNoteOFF(void)
{
uint32_t	oscnum;
	oscnum = FindOscByMidi(MIDI_CHANNEL, MIDI_NOTE);
	if ( oscnum < NUMOSCILLATORS )
	{
		active_oscillators--;
		DisableOscillator(MIDI_CHANNEL,oscnum);
	}
}

static void UsbMidiParseNoteON(void)
{
uint32_t	channel , midi_note;
float		freq;
	midi_note = midi_rx_buffer[2];
	channel = midi_rx_buffer[1]&0x0f;
	freq = noteToFreq(midi_note);
	if ( freq > 0.0f )
		ChangeOscillatorFrequency(channel,active_oscillators, freq, midi_note);
	active_oscillators++;
}

static void UsbMidiParser(void)
{
uint8_t	cmd;

	cmd = midi_rx_buffer[1] & CMD_MASK;

	switch(cmd)
	{
		case SYSEX		:	UsbMidiParseSYSEX();break;
		case NOTE_OFF	:	UsbMidiParseNoteOFF();break;
		case NOTE_ON	:	UsbMidiParseNoteON();break;
		default			:	break;
	}
}

void UsbMidiCheck(void)
{
	if ( midi_rx_flag == 1 )
	{
		midi_rx_flag = 0;
		UsbMidiParser();
	}
}


