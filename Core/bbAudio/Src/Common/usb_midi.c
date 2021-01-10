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

double noteToFreq(uint32_t note)
{
    return (440.0 * pow( 2.0, ((double)note - 69.0) / 12.0 ));
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

#define	MIDI_MAX_VOLUME	127
static void OscCmdsSYSEX(uint8_t cmd)
{
uint32_t osc_number=midi_rx_buf[5]&0x0f,parameter=midi_rx_buf[7];
uint32_t ref_freq,freq;

	if (midi_rx_buf[4] == SYSEX_BB_SETOSCILLATOR )
	{
		switch (midi_rx_buf[6])
		{
		case	SYSEX_BB_SETOSCFREQ	:
			ref_freq = SystemParameters.sampling_frequency[(parameter & 0x03)];
			freq =	(((midi_rx_buf[8]&0x0f)*1000)+((midi_rx_buf[9]&0x0f)*100)+((midi_rx_buf[10]&0x0f)*10)+((midi_rx_buf[11]&0x0f)));
			SetOscillatorFrequency(osc_number, ref_freq, freq, MIDI_MAX_VOLUME, 0);
			break;
		case	SYSEX_BB_SETOSCWAVE	:
			SetOscillatorWaveform(osc_number, parameter & 0x07, 127);
			break;
		case	SYSEX_BB_SETOSCVOL	:
			SetOscillatorVolume( osc_number, parameter);
			break;
		case	SYSEX_BB_SETOSCPHASE	:
			SetOscillatorPhase( osc_number, parameter);
			break;
		case	SYSEX_BB_SETOSCGROUP	:
			SetOscillatorGroup( osc_number, parameter);
			break;
		case	SYSEX_BB_SETOSCFLAG	:
			SetOscillatorFlag( osc_number, parameter);
			break;
		}
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
	case	SYSEX_BB_OSC			:	OscCmdsSYSEX(midi_rx_buf[4]); break;
	case	SYSEX_BB_COMPONENTS		:	CompCmdsSYSEX(midi_rx_buf[4]); break;
	default : break;
	}
}

#define	MIDI_CHANNEL	midi_rx_buffer[1]&0x0f
#define	MIDI_NOTE		midi_rx_buffer[2]
#define	MIDI_FREQUENCY	noteToFreq(midi_rx_buffer[2])
#define	MIDI_VELOCITY	midi_rx_buffer[3]

int32_t	active_oscillators = -1;

static void UsbMidiParseNoteOFF(void)
{
uint32_t	oscnum;
	if ( active_oscillators < 0 )
		return;
	oscnum = FindOscByMidi( MIDI_NOTE);
	if ( oscnum < NUMOSCILLATORS )
	{
		DisableOscillator(oscnum);
		active_oscillators--;
	}
}

static void UsbMidiParseNoteON(void)
{
uint32_t	channel , midi_note;
double		freq;
	midi_note = midi_rx_buffer[2];
	channel = midi_rx_buffer[1]&0x0f;
	freq = noteToFreq(midi_note);
	if ( freq > 0.0 )
	{
		active_oscillators++;
		SetOscillatorFrequency(active_oscillators, SystemParameters.sampling_frequency[channel], freq, MAX_VOLUME, midi_note);
	}
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


