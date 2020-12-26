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

uint32_t noteToFreq(uint32_t note)
{
	uint32_t a = 440; //frequency of A
    return (uint32_t)((a / 32) * pow(2, ((note - 9) / 12)));
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

static void SYSEX_ConvertMidiBufIn(void)
{
uint32_t	i,rx7=0;

	midi_rx_len=0;
	for(i=1;i<midi_rx_length;i++)
	{
		if ( ((i & 0x0f) == 0x0c) || ((i & 0x0f) == 0x08) || ((i & 0x0f) == 0x04) || ((i & 0x0f) == 0x00) )
		{
			if (midi_rx_buffer[i] == 0x07)
				rx7=1;
		}
		else
		{
			midi_rx_buf[midi_rx_len] = midi_rx_buffer[i];
			midi_rx_len++;
		}
	}
	if ( rx7 == 1 )
	{
		midi_rx_buf[midi_rx_len] = SYSEX_END;
		midi_rx_len++;
	}
}

static void UsbMidiParseSYSEX(void)
{
	if ( midi_rx_buf[midi_rx_len-1] != SYSEX_END )
		return;
	if ( (midi_rx_buf[1] != 0) || (midi_rx_buf[2] != 'b') || (midi_rx_buf[3] != 'B'))
		return;
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

#define	MIDI_CHANNEL	midi_rx_buf[1] & 0x0f
#define	MIDI_NOTE		midi_rx_buf[2]
uint32_t	current_oscillator = 0;

static void UsbMidiParseNoteOFF(void)
{
	DisableOscillator(MIDI_CHANNEL);
	current_oscillator = 0;
}

static void UsbMidiParseNoteON(void)
{
uint32_t	next_oscillator;
	ChangeOscillatorFrequency(MIDI_CHANNEL,current_oscillator, noteToFreq(MIDI_NOTE));
	next_oscillator = EnableOscillator(MIDI_CHANNEL,current_oscillator);
	current_oscillator = next_oscillator;
}

static void UsbMidiParser(void)
{
uint8_t	cmd;
	SYSEX_ConvertMidiBufIn();
	cmd = midi_rx_buf[0] & CMD_MASK;

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


