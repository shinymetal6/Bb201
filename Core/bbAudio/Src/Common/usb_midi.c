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

__attribute__ ((aligned (4)))	uint8_t		midi_rx_buffer[MIDI_RXBUF_SIZE];
__attribute__ ((aligned (4)))	uint8_t		midi_rx_buff[MIDI_RXBUF_SIZE];
uint8_t		midi_rx_flag;
uint16_t 	midi_rx_length;
uint16_t 	midi_rx_len;

static void CleanMidiBuf(void)
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
			midi_rx_buff[midi_rx_len] = midi_rx_buffer[i];
			midi_rx_len++;
		}
	}
	if ( rx7 == 1 )
	{
		midi_rx_buff[midi_rx_len] = SYSEX_END;
		midi_rx_len++;
	}
}

static void UsbMidiParseSYSEX(void)
{
char tt[12];
	sprintf(tt," Ciao ");
	tt[0] = SYSEX;
	tt[5] = SYSEX_END;
	if ( midi_rx_buff[midi_rx_len-1] != SYSEX_END )
		return;
	if ( (midi_rx_buff[1] != 0) || (midi_rx_buff[2] != 'b') || (midi_rx_buff[3] != 'B'))
		return;
	switch (midi_rx_buff[4])
	{
		case	SYSEX_BB_CLEAR			:	MIDI_Transmit_FS(midi_rx_buffer,midi_rx_length); break;
		case	SYSEX_BB_STORESETTINGS	:	StoreSettingsInFlash();break;
		case	SYSEX_BB_LOADSETTINGS	:	LoadSettingsFromFlash();break;
		case	SYSEX_BB_STOREPROGRAMS	:	StoreProgramInFlash();break;
		case	SYSEX_BB_LOADPROGRAMS	:	LoadProgramFromFlash();break;
		default : break;
	}
}

static void UsbMidiParser(void)
{
uint8_t	cmd;
	CleanMidiBuf();
	cmd = midi_rx_buff[0];
	switch(cmd)
	{
		case SYSEX	:	UsbMidiParseSYSEX();break;
		default		:	break;
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


