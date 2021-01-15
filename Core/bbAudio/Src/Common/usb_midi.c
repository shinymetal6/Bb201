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
static void VcoCmdsSYSEX(uint8_t cmd)
{
uint32_t command=midi_rx_buf[4],vco_number=midi_rx_buf[5]&0x0f,subcmd=midi_rx_buf[6],parameter1=midi_rx_buf[7],parameter2=midi_rx_buf[8],parameter3=midi_rx_buf[9],parameter4=midi_rx_buf[10],parameter5=midi_rx_buf[11];
uint32_t ref_freq,freq;

	if ( command == SYSEX_BB_SETVCO )
	{
		switch (subcmd)
		{
		case	SYSEX_BB_SETVCOFREQ	:
			ref_freq = SystemParameters.sampling_frequency[(parameter1 & 0x03)];
			freq =	(((parameter2 & 0x0f)*1000)+((parameter3 & 0x0f)*100)+((parameter4 & 0x0f)*10)+((parameter5 & 0x0f)));
			VcoSetFrequency(vco_number,freq,ref_freq);
			break;
		case	SYSEX_BB_SETVCOMASTERVOL	:
			if ( parameter1 <= LOCAL_CONTROLLER_MASK )
			{
				VcoSetMasterVolume(vco_number,(uint32_t )&control_buf.ain[parameter1]);
			}
			if ( parameter1 == MIDI_CONTROLLER_MASK )
			{
				VcoSetMasterVolume(vco_number,(uint32_t )&midi_vco_master_volume_control[vco_number]);
			}
			if ( parameter1 == STATIC_CONTROLLER_MASK )
			{
				static_vco_master_volume_control[vco_number] = parameter2;
				VcoSetMasterVolume(vco_number,(uint32_t )&static_vco_master_volume_control[vco_number]);
			}
			break;
		case	SYSEX_BB_SETVCOWxVOL	:
			if ( parameter1 <= LOCAL_CONTROLLER_MASK )
			{
				VcoSetElementVolume(vco_number,parameter2,(uint32_t )&control_buf.ain[parameter1]);
			}
			if ( parameter1 == MIDI_CONTROLLER_MASK )
			{
				VcoSetElementVolume(vco_number,parameter2,(uint32_t )&midi_vco_master_volume_control[vco_number]);
			}
			if ( parameter1 == STATIC_CONTROLLER_MASK )
			{
				static_vco_master_volume_control[vco_number] = midi_rx_buf[8]&0x0f;
				VcoSetElementVolume(vco_number,parameter2,(uint32_t )&static_vco_master_volume_control[vco_number]);
			}
			break;
		case	SYSEX_BB_SETVCOWxWAVE	:
			VcoSetElementWave(vco_number, parameter2,parameter3,parameter4);
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
	case	SYSEX_BB_VCO			:	VcoCmdsSYSEX(midi_rx_buf[4]); break;
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
	if ( oscnum < NUMVCO )
	{
		VcoSetMasterVolume(oscnum,VCO_MAX_VOLUME);
		VcoSetMidiNote(active_oscillators,0);
		active_oscillators--;
	}
}

static void UsbMidiParseNoteON(void)
{
uint32_t	channel , midi_note,ref_freq;
double		freq;
	midi_note = midi_rx_buffer[2];
	channel = midi_rx_buffer[1]&0x0f;
	ref_freq=SystemParameters.sampling_frequency[channel];
	freq = noteToFreq(midi_note);
	if ( freq > 0.0 )
	{
		active_oscillators++;
		VcoSetMidiNote(active_oscillators,midi_note);
		VcoSetFrequency(active_oscillators,freq,ref_freq);
		VcoSetMasterVolume(active_oscillators,VCO_MAX_VOLUME);
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


