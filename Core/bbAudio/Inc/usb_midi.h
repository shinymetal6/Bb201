/*
 * usb_midi.h
 *
 *  Created on: Dec 23, 2020
 *      Author: fil
 */

#ifndef BBAUDIO_INC_USB_MIDI_H_
#define BBAUDIO_INC_USB_MIDI_H_

#define	MIDI_RXBUF_SIZE		2048

#define	SYSEX			0xF0
#define	SYSEX_END		0xF7
#define	SYSEX_BB_CLEAR			0x00
#define	SYSEX_BB_STORESETTINGS	0x01
#define	SYSEX_BB_LOADSETTINGS	0x02
#define	SYSEX_BB_STOREPROGRAMS	0x03
#define	SYSEX_BB_LOADPROGRAMS	0x04

extern	void UsbMidiCheck(void);

#endif /* BBAUDIO_INC_USB_MIDI_H_ */
