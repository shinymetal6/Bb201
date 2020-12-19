/*
 * waveform.c
 *
 *  Created on: Nov 28, 2020
 *      Author: fil
 */


/*
 * app_vco.c
 *
 *  Created on: Apr 2, 2020
 *      Author: fil
 */
#include "main.h"

uint16_t sine[NUMBER_OF_AUDIO_SAMPLES] =
{
		0x800,0x864,0x8c8,0x92c,0x98f,0x9f1,0xa52,0xab1,
		0xb0f,0xb6b,0xbc5,0xc1c,0xc71,0xcc3,0xd12,0xd5f,
		0xda7,0xded,0xe2e,0xe6c,0xea6,0xedc,0xf0d,0xf3a,
		0xf63,0xf87,0xfa7,0xfc2,0xfd8,0xfe9,0xff5,0xffd,
		0xfff,0xffd,0xff5,0xfe9,0xfd8,0xfc2,0xfa7,0xf87,
		0xf63,0xf3a,0xf0d,0xedc,0xea6,0xe6c,0xe2e,0xded,
		0xda7,0xd5f,0xd12,0xcc3,0xc71,0xc1c,0xbc5,0xb6b,
		0xb0f,0xab1,0xa52,0x9f1,0x98f,0x92c,0x8c8,0x864,
		0x800,0x79b,0x737,0x6d3,0x670,0x60e,0x5ad,0x54e,
		0x4f0,0x494,0x43a,0x3e3,0x38e,0x33c,0x2ed,0x2a0,
		0x258,0x212,0x1d1,0x193,0x159,0x123,0xf2,0xc5,
		0x9c,0x78,0x58,0x3d,0x27,0x16,0xa,0x2,
		0x0,0x2,0xa,0x16,0x27,0x3d,0x58,0x78,
		0x9c,0xc5,0xf2,0x123,0x159,0x193,0x1d1,0x212,
		0x258,0x2a0,0x2ed,0x33c,0x38e,0x3e3,0x43a,0x494,
		0x4f0,0x54e,0x5ad,0x60e,0x670,0x6d3,0x737,0x79b,
};

uint16_t triangle[NUMBER_OF_AUDIO_SAMPLES] =
{
		0x40,0x80,0xc0,0x100,0x140,0x180,0x1c0,0x200,
		0x240,0x280,0x2c0,0x300,0x340,0x380,0x3c0,0x400,
		0x440,0x480,0x4c0,0x500,0x540,0x580,0x5c0,0x600,
		0x640,0x680,0x6c0,0x700,0x740,0x780,0x7c0,0x800,
		0x83f,0x87f,0x8bf,0x8ff,0x93f,0x97f,0x9bf,0x9ff,
		0xa3f,0xa7f,0xabf,0xaff,0xb3f,0xb7f,0xbbf,0xbff,
		0xc3f,0xc7f,0xcbf,0xcff,0xd3f,0xd7f,0xdbf,0xdff,
		0xe3f,0xe7f,0xebf,0xeff,0xf3f,0xf7f,0xfbf,0xfff,
		0xfbf,0xf7f,0xf3f,0xeff,0xebf,0xe7f,0xe3f,0xdff,
		0xdbf,0xd7f,0xd3f,0xcff,0xcbf,0xc7f,0xc3f,0xbff,
		0xbbf,0xb7f,0xb3f,0xaff,0xabf,0xa7f,0xa3f,0x9ff,
		0x9bf,0x97f,0x93f,0x8ff,0x8bf,0x87f,0x83f,0x800,
		0x7c0,0x780,0x740,0x700,0x6c0,0x680,0x640,0x600,
		0x5c0,0x580,0x540,0x500,0x4c0,0x480,0x440,0x400,
		0x3c0,0x380,0x340,0x300,0x2c0,0x280,0x240,0x200,
		0x1c0,0x180,0x140,0x100,0xc0,0x80,0x40,0x0,
};

uint16_t square[NUMBER_OF_AUDIO_SAMPLES] =
{
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,
		0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,
		0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,
		0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,
		0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,
		0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,
		0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,
		0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,
};
static	void static_do_vco(uint16_t *wave,uint32_t *out_buffer, uint8_t *half_in, uint32_t freq_control)
{
uint16_t	i,start,end;

	get_limits(&start,&end,half_in);
	for ( i=start;i<end;i++)
		out_buffer[i] = (uint32_t )wave[i];
}

uint32_t VCOInit(uint32_t in_stage,uint16_t waveform_type, uint32_t out_buffer, uint32_t freq_control, uint32_t channel)
{
uint16_t 	*wave;
	switch ( waveform_type )
	{
		case	SINE :		wave = sine; break;
		case	TRIANGLE :	wave = triangle; break;
		case	SQUARE :	wave = square; break;
		default : 			wave = sine; break;
	}
	setOutStage((uint32_t )&static_do_vco, (uint32_t )wave , out_buffer, get_bufferhalf(channel), freq_control, 0, 0, channel, in_stage);

	stage++;
	return 0;
}
