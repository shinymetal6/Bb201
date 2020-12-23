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

__attribute__ ((aligned (4)))	uint8_t		midirx_buff[MIDI_RXBUF_SIZE];
uint8_t		midirx_flag;
uint16_t 	midirx_len;

void UsbMidiInit(void)
{

}

static void UsbMidiParseSYSEX(void)
{
	switch (midirx_buff[4])
	{
		case	SYSEX_BB_CLEAR	:	break;
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
	cmd = midirx_buff[0];

	switch(cmd)
	{
	case SYSEX	:	if ( midirx_buff[midirx_len-1] != SYSEX_END )
						return;
					if ( (midirx_buff[1] == 0) && (midirx_buff[2] == 'b') && (midirx_buff[3] == 'B'))
						UsbMidiParseSYSEX();
					break;
	default:	break;
	}
}

void UsbMidiCheck(void)
{
	if ( midirx_flag == 1 )
	{
		midirx_flag = 0;
		UsbMidiParser();
	}

}

