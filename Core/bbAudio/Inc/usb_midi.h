/*
 * usb_midi.h
 *
 *  Created on: Dec 23, 2020
 *      Author: fil
 */

#ifndef BBAUDIO_INC_USB_MIDI_H_
#define BBAUDIO_INC_USB_MIDI_H_


#define	MIDI_RXBUF_SIZE			2048
#define	USB_TXBUF_SIZE			2048
extern	uint32_t	usb_packet_ready;
extern	uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
extern	uint8_t	USB_RxBuffer[64];
extern	char	USB_TxBuffer[USB_TXBUF_SIZE];
extern	uint32_t USB_RxBufferLen;
extern	void CheckUSB(void);

#define	USBUART_BUFLEN	128
typedef struct {
	uint8_t byte_count;
	uint8_t usb_header;
	uint8_t usb_rx_index;
	uint8_t usb_flag;
	uint8_t packet[USBUART_BUFLEN];
} s_rxbuf;

extern	s_rxbuf		usb_rxbuf;

#define	SYSEX					0xF0
#define	SYSEX_END				0xF7

#define	SYSEX_BB_CMDMASK		0x70

#define	SYSEX_BB_SYSTEM			0x00
#define	SYSEX_BB_CLEAR			0x00
#define	SYSEX_BB_STORESETTINGS	0x01
#define	SYSEX_BB_LOADSETTINGS	0x02
#define	SYSEX_BB_STOREPROGRAMS	0x03
#define	SYSEX_BB_LOADPROGRAMS	0x04
#define	SYSEX_BB_LOADFIRCOEF	0x05
#define	SYSEX_BB_LOADWAVEFORM	0x06

#define	SYSEX_BB_OSC			0x10
#define	SYSEX_BB_SETOSCILLATOR	0x10
/* subcommands */
	#define	SYSEX_BB_SETOSCFREQ		0x00
	#define	SYSEX_BB_SETOSCWAVE		0x01
	#define	SYSEX_BB_SETOSCVOL		0x02
	#define	SYSEX_BB_SETOSCPHASE	0x03
	#define	SYSEX_BB_SETOSCGROUP	0x04
	#define	SYSEX_BB_SETOSCFLAG		0x05


#define	SYSEX_BB_COMPONENTS		0x20
#define	SYSEX_BB_COMP_INSERT	0x20
#define	SYSEX_BB_COMP_MODIN		0x21
#define	SYSEX_BB_COMP_MODOUT	0x22
#define	SYSEX_BB_COMP_DEFCNTRL	0x23

#define	CMD_MASK				0xF0
#define	NOTE_OFF				0x80
#define	NOTE_ON					0x90

extern	void UsbMidiCheck(void);

#endif /* BBAUDIO_INC_USB_MIDI_H_ */
