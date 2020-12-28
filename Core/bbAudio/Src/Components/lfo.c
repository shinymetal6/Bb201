/*
 * lfo.c
 *
 *  Created on: Dec 28, 2020
 *      Author: fil
 */
#include "main.h"
#include "math.h"

__attribute__ ((aligned (16)))	uint32_t	lfo_sqrtab[CHANNELS][LFOTAB_SIZE];
__attribute__ ((aligned (16)))	uint32_t	lfo_tritab[CHANNELS][LFOTAB_SIZE];

uint32_t	lfotab[LFOTAB_SIZE] = {
		0x800,0x808,0x811,0x81a,0x823,0x82c,0x835,0x83e,
		0x847,0x850,0x859,0x862,0x86b,0x874,0x87c,0x885,
		0x88e,0x897,0x8a0,0x8a9,0x8b2,0x8bb,0x8c4,0x8cd,
		0x8d6,0x8de,0x8e7,0x8f0,0x8f9,0x902,0x90b,0x914,
		0x91c,0x925,0x92e,0x937,0x940,0x949,0x951,0x95a,
		0x963,0x96c,0x975,0x97d,0x986,0x98f,0x998,0x9a0,
		0x9a9,0x9b2,0x9bb,0x9c3,0x9cc,0x9d5,0x9dd,0x9e6,
		0x9ef,0x9f7,0xa00,0xa09,0xa11,0xa1a,0xa23,0xa2b,
		0xa34,0xa3c,0xa45,0xa4e,0xa56,0xa5f,0xa67,0xa70,
		0xa78,0xa81,0xa89,0xa92,0xa9a,0xaa3,0xaab,0xab3,
		0xabc,0xac4,0xacd,0xad5,0xadd,0xae6,0xaee,0xaf6,
		0xaff,0xb07,0xb0f,0xb17,0xb20,0xb28,0xb30,0xb38,
		0xb40,0xb48,0xb51,0xb59,0xb61,0xb69,0xb71,0xb79,
		0xb81,0xb89,0xb91,0xb99,0xba1,0xba9,0xbb1,0xbb9,
		0xbc1,0xbc9,0xbd0,0xbd8,0xbe0,0xbe8,0xbf0,0xbf8,
		0xbff,0xc07,0xc0f,0xc16,0xc1e,0xc26,0xc2d,0xc35,
		0xc3d,0xc44,0xc4c,0xc53,0xc5b,0xc62,0xc6a,0xc71,
		0xc78,0xc80,0xc87,0xc8f,0xc96,0xc9d,0xca4,0xcac,
		0xcb3,0xcba,0xcc1,0xcc9,0xcd0,0xcd7,0xcde,0xce5,
		0xcec,0xcf3,0xcfa,0xd01,0xd08,0xd0f,0xd16,0xd1d,
		0xd24,0xd2a,0xd31,0xd38,0xd3f,0xd46,0xd4c,0xd53,
		0xd5a,0xd60,0xd67,0xd6d,0xd74,0xd7a,0xd81,0xd87,
		0xd8e,0xd94,0xd9b,0xda1,0xda7,0xdae,0xdb4,0xdba,
		0xdc0,0xdc7,0xdcd,0xdd3,0xdd9,0xddf,0xde5,0xdeb,
		0xdf1,0xdf7,0xdfd,0xe03,0xe09,0xe0f,0xe14,0xe1a,
		0xe20,0xe26,0xe2b,0xe31,0xe37,0xe3c,0xe42,0xe47,
		0xe4d,0xe52,0xe58,0xe5d,0xe63,0xe68,0xe6d,0xe73,
		0xe78,0xe7d,0xe82,0xe88,0xe8d,0xe92,0xe97,0xe9c,
		0xea1,0xea6,0xeab,0xeb0,0xeb5,0xeba,0xebe,0xec3,
		0xec8,0xecd,0xed1,0xed6,0xedb,0xedf,0xee4,0xee8,
		0xeed,0xef1,0xef6,0xefa,0xefe,0xf03,0xf07,0xf0b,
		0xf0f,0xf14,0xf18,0xf1c,0xf20,0xf24,0xf28,0xf2c,
		0xf30,0xf34,0xf38,0xf3b,0xf3f,0xf43,0xf47,0xf4a,
		0xf4e,0xf52,0xf55,0xf59,0xf5c,0xf60,0xf63,0xf67,
		0xf6a,0xf6d,0xf71,0xf74,0xf77,0xf7a,0xf7d,0xf80,
		0xf84,0xf87,0xf8a,0xf8d,0xf8f,0xf92,0xf95,0xf98,
		0xf9b,0xf9e,0xfa0,0xfa3,0xfa6,0xfa8,0xfab,0xfad,
		0xfb0,0xfb2,0xfb5,0xfb7,0xfb9,0xfbc,0xfbe,0xfc0,
		0xfc2,0xfc4,0xfc6,0xfc8,0xfcb,0xfcd,0xfce,0xfd0,
		0xfd2,0xfd4,0xfd6,0xfd8,0xfd9,0xfdb,0xfdd,0xfde,
		0xfe0,0xfe1,0xfe3,0xfe4,0xfe6,0xfe7,0xfe9,0xfea,
		0xfeb,0xfec,0xfed,0xfef,0xff0,0xff1,0xff2,0xff3,
		0xff4,0xff5,0xff6,0xff6,0xff7,0xff8,0xff9,0xff9,
		0xffa,0xffb,0xffb,0xffc,0xffc,0xffd,0xffd,0xffd,
		0xffe,0xffe,0xffe,0xfff,0xfff,0xfff,0xfff,0xfff,
		0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xffe,0xffe,
		0xffe,0xffd,0xffd,0xffd,0xffc,0xffc,0xffb,0xffb,
		0xffa,0xff9,0xff9,0xff8,0xff7,0xff6,0xff6,0xff5,
		0xff4,0xff3,0xff2,0xff1,0xff0,0xfef,0xfed,0xfec,
		0xfeb,0xfea,0xfe9,0xfe7,0xfe6,0xfe4,0xfe3,0xfe1,
		0xfe0,0xfde,0xfdd,0xfdb,0xfd9,0xfd8,0xfd6,0xfd4,
		0xfd2,0xfd0,0xfce,0xfcd,0xfcb,0xfc8,0xfc6,0xfc4,
		0xfc2,0xfc0,0xfbe,0xfbc,0xfb9,0xfb7,0xfb5,0xfb2,
		0xfb0,0xfad,0xfab,0xfa8,0xfa6,0xfa3,0xfa0,0xf9e,
		0xf9b,0xf98,0xf95,0xf92,0xf8f,0xf8d,0xf8a,0xf87,
		0xf84,0xf80,0xf7d,0xf7a,0xf77,0xf74,0xf71,0xf6d,
		0xf6a,0xf67,0xf63,0xf60,0xf5c,0xf59,0xf55,0xf52,
		0xf4e,0xf4a,0xf47,0xf43,0xf3f,0xf3b,0xf38,0xf34,
		0xf30,0xf2c,0xf28,0xf24,0xf20,0xf1c,0xf18,0xf14,
		0xf0f,0xf0b,0xf07,0xf03,0xefe,0xefa,0xef6,0xef1,
		0xeed,0xee8,0xee4,0xedf,0xedb,0xed6,0xed1,0xecd,
		0xec8,0xec3,0xebe,0xeba,0xeb5,0xeb0,0xeab,0xea6,
		0xea1,0xe9c,0xe97,0xe92,0xe8d,0xe88,0xe82,0xe7d,
		0xe78,0xe73,0xe6d,0xe68,0xe63,0xe5d,0xe58,0xe52,
		0xe4d,0xe47,0xe42,0xe3c,0xe37,0xe31,0xe2b,0xe26,
		0xe20,0xe1a,0xe14,0xe0f,0xe09,0xe03,0xdfd,0xdf7,
		0xdf1,0xdeb,0xde5,0xddf,0xdd9,0xdd3,0xdcd,0xdc7,
		0xdc0,0xdba,0xdb4,0xdae,0xda7,0xda1,0xd9b,0xd94,
		0xd8e,0xd87,0xd81,0xd7a,0xd74,0xd6d,0xd67,0xd60,
		0xd5a,0xd53,0xd4c,0xd46,0xd3f,0xd38,0xd31,0xd2a,
		0xd24,0xd1d,0xd16,0xd0f,0xd08,0xd01,0xcfa,0xcf3,
		0xcec,0xce5,0xcde,0xcd7,0xcd0,0xcc9,0xcc1,0xcba,
		0xcb3,0xcac,0xca4,0xc9d,0xc96,0xc8f,0xc87,0xc80,
		0xc78,0xc71,0xc6a,0xc62,0xc5b,0xc53,0xc4c,0xc44,
		0xc3d,0xc35,0xc2d,0xc26,0xc1e,0xc16,0xc0f,0xc07,
		0xbff,0xbf8,0xbf0,0xbe8,0xbe0,0xbd8,0xbd0,0xbc9,
		0xbc1,0xbb9,0xbb1,0xba9,0xba1,0xb99,0xb91,0xb89,
		0xb81,0xb79,0xb71,0xb69,0xb61,0xb59,0xb51,0xb48,
		0xb40,0xb38,0xb30,0xb28,0xb20,0xb17,0xb0f,0xb07,
		0xaff,0xaf6,0xaee,0xae6,0xadd,0xad5,0xacd,0xac4,
		0xabc,0xab3,0xaab,0xaa3,0xa9a,0xa92,0xa89,0xa81,
		0xa78,0xa70,0xa67,0xa5f,0xa56,0xa4e,0xa45,0xa3c,
		0xa34,0xa2b,0xa23,0xa1a,0xa11,0xa09,0xa00,0x9f7,
		0x9ef,0x9e6,0x9dd,0x9d5,0x9cc,0x9c3,0x9bb,0x9b2,
		0x9a9,0x9a0,0x998,0x98f,0x986,0x97d,0x975,0x96c,
		0x963,0x95a,0x951,0x949,0x940,0x937,0x92e,0x925,
		0x91c,0x914,0x90b,0x902,0x8f9,0x8f0,0x8e7,0x8de,
		0x8d6,0x8cd,0x8c4,0x8bb,0x8b2,0x8a9,0x8a0,0x897,
		0x88e,0x885,0x87c,0x874,0x86b,0x862,0x859,0x850,
		0x847,0x83e,0x835,0x82c,0x823,0x81a,0x811,0x808,
		0x800,0x7f7,0x7ee,0x7e5,0x7dc,0x7d3,0x7ca,0x7c1,
		0x7b8,0x7af,0x7a6,0x79d,0x794,0x78b,0x783,0x77a,
		0x771,0x768,0x75f,0x756,0x74d,0x744,0x73b,0x732,
		0x729,0x721,0x718,0x70f,0x706,0x6fd,0x6f4,0x6eb,
		0x6e3,0x6da,0x6d1,0x6c8,0x6bf,0x6b6,0x6ae,0x6a5,
		0x69c,0x693,0x68a,0x682,0x679,0x670,0x667,0x65f,
		0x656,0x64d,0x644,0x63c,0x633,0x62a,0x622,0x619,
		0x610,0x608,0x5ff,0x5f6,0x5ee,0x5e5,0x5dc,0x5d4,
		0x5cb,0x5c3,0x5ba,0x5b1,0x5a9,0x5a0,0x598,0x58f,
		0x587,0x57e,0x576,0x56d,0x565,0x55c,0x554,0x54c,
		0x543,0x53b,0x532,0x52a,0x522,0x519,0x511,0x509,
		0x500,0x4f8,0x4f0,0x4e8,0x4df,0x4d7,0x4cf,0x4c7,
		0x4bf,0x4b7,0x4ae,0x4a6,0x49e,0x496,0x48e,0x486,
		0x47e,0x476,0x46e,0x466,0x45e,0x456,0x44e,0x446,
		0x43e,0x436,0x42f,0x427,0x41f,0x417,0x40f,0x407,
		0x400,0x3f8,0x3f0,0x3e9,0x3e1,0x3d9,0x3d2,0x3ca,
		0x3c2,0x3bb,0x3b3,0x3ac,0x3a4,0x39d,0x395,0x38e,
		0x387,0x37f,0x378,0x370,0x369,0x362,0x35b,0x353,
		0x34c,0x345,0x33e,0x336,0x32f,0x328,0x321,0x31a,
		0x313,0x30c,0x305,0x2fe,0x2f7,0x2f0,0x2e9,0x2e2,
		0x2db,0x2d5,0x2ce,0x2c7,0x2c0,0x2b9,0x2b3,0x2ac,
		0x2a5,0x29f,0x298,0x292,0x28b,0x285,0x27e,0x278,
		0x271,0x26b,0x264,0x25e,0x258,0x251,0x24b,0x245,
		0x23f,0x238,0x232,0x22c,0x226,0x220,0x21a,0x214,
		0x20e,0x208,0x202,0x1fc,0x1f6,0x1f0,0x1eb,0x1e5,
		0x1df,0x1d9,0x1d4,0x1ce,0x1c8,0x1c3,0x1bd,0x1b8,
		0x1b2,0x1ad,0x1a7,0x1a2,0x19c,0x197,0x192,0x18c,
		0x187,0x182,0x17d,0x177,0x172,0x16d,0x168,0x163,
		0x15e,0x159,0x154,0x14f,0x14a,0x145,0x141,0x13c,
		0x137,0x132,0x12e,0x129,0x124,0x120,0x11b,0x117,
		0x112,0x10e,0x109,0x105,0x101,0xfc,0xf8,0xf4,
		0xf0,0xeb,0xe7,0xe3,0xdf,0xdb,0xd7,0xd3,
		0xcf,0xcb,0xc7,0xc4,0xc0,0xbc,0xb8,0xb5,
		0xb1,0xad,0xaa,0xa6,0xa3,0x9f,0x9c,0x98,
		0x95,0x92,0x8e,0x8b,0x88,0x85,0x82,0x7f,
		0x7b,0x78,0x75,0x72,0x70,0x6d,0x6a,0x67,
		0x64,0x61,0x5f,0x5c,0x59,0x57,0x54,0x52,
		0x4f,0x4d,0x4a,0x48,0x46,0x43,0x41,0x3f,
		0x3d,0x3b,0x39,0x37,0x34,0x32,0x31,0x2f,
		0x2d,0x2b,0x29,0x27,0x26,0x24,0x22,0x21,
		0x1f,0x1e,0x1c,0x1b,0x19,0x18,0x16,0x15,
		0x14,0x13,0x12,0x10,0xf,0xe,0xd,0xc,
		0xb,0xa,0x9,0x9,0x8,0x7,0x6,0x6,
		0x5,0x4,0x4,0x3,0x3,0x2,0x2,0x2,
		0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,
		0x1,0x2,0x2,0x2,0x3,0x3,0x4,0x4,
		0x5,0x6,0x6,0x7,0x8,0x9,0x9,0xa,
		0xb,0xc,0xd,0xe,0xf,0x10,0x12,0x13,
		0x14,0x15,0x16,0x18,0x19,0x1b,0x1c,0x1e,
		0x1f,0x21,0x22,0x24,0x26,0x27,0x29,0x2b,
		0x2d,0x2f,0x31,0x32,0x34,0x37,0x39,0x3b,
		0x3d,0x3f,0x41,0x43,0x46,0x48,0x4a,0x4d,
		0x4f,0x52,0x54,0x57,0x59,0x5c,0x5f,0x61,
		0x64,0x67,0x6a,0x6d,0x70,0x72,0x75,0x78,
		0x7b,0x7f,0x82,0x85,0x88,0x8b,0x8e,0x92,
		0x95,0x98,0x9c,0x9f,0xa3,0xa6,0xaa,0xad,
		0xb1,0xb5,0xb8,0xbc,0xc0,0xc4,0xc7,0xcb,
		0xcf,0xd3,0xd7,0xdb,0xdf,0xe3,0xe7,0xeb,
		0xf0,0xf4,0xf8,0xfc,0x101,0x105,0x109,0x10e,
		0x112,0x117,0x11b,0x120,0x124,0x129,0x12e,0x132,
		0x137,0x13c,0x141,0x145,0x14a,0x14f,0x154,0x159,
		0x15e,0x163,0x168,0x16d,0x172,0x177,0x17d,0x182,
		0x187,0x18c,0x192,0x197,0x19c,0x1a2,0x1a7,0x1ad,
		0x1b2,0x1b8,0x1bd,0x1c3,0x1c8,0x1ce,0x1d4,0x1d9,
		0x1df,0x1e5,0x1eb,0x1f0,0x1f6,0x1fc,0x202,0x208,
		0x20e,0x214,0x21a,0x220,0x226,0x22c,0x232,0x238,
		0x23f,0x245,0x24b,0x251,0x258,0x25e,0x264,0x26b,
		0x271,0x278,0x27e,0x285,0x28b,0x292,0x298,0x29f,
		0x2a5,0x2ac,0x2b3,0x2b9,0x2c0,0x2c7,0x2ce,0x2d5,
		0x2db,0x2e2,0x2e9,0x2f0,0x2f7,0x2fe,0x305,0x30c,
		0x313,0x31a,0x321,0x328,0x32f,0x336,0x33e,0x345,
		0x34c,0x353,0x35b,0x362,0x369,0x370,0x378,0x37f,
		0x387,0x38e,0x395,0x39d,0x3a4,0x3ac,0x3b3,0x3bb,
		0x3c2,0x3ca,0x3d2,0x3d9,0x3e1,0x3e9,0x3f0,0x3f8,
		0x400,0x407,0x40f,0x417,0x41f,0x427,0x42f,0x436,
		0x43e,0x446,0x44e,0x456,0x45e,0x466,0x46e,0x476,
		0x47e,0x486,0x48e,0x496,0x49e,0x4a6,0x4ae,0x4b7,
		0x4bf,0x4c7,0x4cf,0x4d7,0x4df,0x4e8,0x4f0,0x4f8,
		0x500,0x509,0x511,0x519,0x522,0x52a,0x532,0x53b,
		0x543,0x54c,0x554,0x55c,0x565,0x56d,0x576,0x57e,
		0x587,0x58f,0x598,0x5a0,0x5a9,0x5b1,0x5ba,0x5c3,
		0x5cb,0x5d4,0x5dc,0x5e5,0x5ee,0x5f6,0x5ff,0x608,
		0x610,0x619,0x622,0x62a,0x633,0x63c,0x644,0x64d,
		0x656,0x65f,0x667,0x670,0x679,0x682,0x68a,0x693,
		0x69c,0x6a5,0x6ae,0x6b6,0x6bf,0x6c8,0x6d1,0x6da,
		0x6e3,0x6eb,0x6f4,0x6fd,0x706,0x70f,0x718,0x721,
		0x729,0x732,0x73b,0x744,0x74d,0x756,0x75f,0x768,
		0x771,0x77a,0x783,0x78b,0x794,0x79d,0x7a6,0x7af,
		0x7b8,0x7c1,0x7ca,0x7d3,0x7dc,0x7e5,0x7ee,0x7f7,
};

__attribute__ ((aligned (16))) uint32_t				lfo_buf[CHANNELS][NUM_LFO][NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (16))) uint32_t				lfo_out[CHANNELS][NUMBER_OF_AUDIO_SAMPLES];
__attribute__ ((aligned (16))) OscillatorsTypeDef	Lfo[CHANNELS][NUM_LFO];

uint32_t	lfo_activechannels=0;

static uint32_t LFO_sinFunc(uint32_t pos)
{
	uint32_t *wave = lfotab + pos;
	return *wave;
}

static uint32_t LFO_lookupFunc(uint32_t lookup_ptr,uint32_t pos)
{
	uint32_t *wave = (uint32_t *)lookup_ptr + pos;
	return *wave;
}

static void LFO_triCompile(uint32_t channel,uint32_t phase)
{
float 	step_up,step_down,current;
uint32_t	i;

	step_up = 4095.0f / (float )phase;
	step_down = 4095.0f / (float )( LFOTAB_SIZE - phase);
	current=0;
	for(i=0;i<LFOTAB_SIZE;i++)
	{
		if ( i < phase )
		{
			lfo_tritab[channel][i] = (uint32_t )current;
			current += step_up;
		}
		else
		{
			lfo_tritab[channel][i] = (uint32_t )current;
			current -= step_down;
		}
	}
}

static void LFO_squareCompile(uint32_t channel,uint32_t phase)
{
uint32_t	i;

	for(i=0;i<LFOTAB_SIZE;i++)
	{
		if ( i < phase )
			lfo_sqrtab[channel][i] = 4095;
		else
			lfo_sqrtab[channel][i] = 0;
	}
}

static	void static_process_lfo(uint32_t channel,uint32_t lfo_number)
{
	uint16_t	i,start,end;
	uint32_t	uint32_t_pos;

		get_limits(&start,&end,(uint32_t *)Lfo[channel][lfo_number].buffer_flag_ptr);
		for ( i=start;i<end;i++)
		{
			if ( Lfo[channel][lfo_number].enabled == OSCILLATOR_ENABLED )
			{
				uint32_t_pos = (uint32_t )Lfo[channel][lfo_number].current_phase;
				switch (Lfo[channel][lfo_number].waveform)
				{
				case	SINE:lfo_buf[channel][lfo_number][i] = LFO_sinFunc(uint32_t_pos);
					break;
				case	TRIANGLE:
					lfo_buf[channel][lfo_number][i] = LFO_lookupFunc((uint32_t )&lfo_tritab[channel],uint32_t_pos);
					break;
				case	SQUARE:
					lfo_buf[channel][lfo_number][i] = LFO_lookupFunc((uint32_t )&lfo_sqrtab[channel],uint32_t_pos);
					break;
				}
				lfo_out[channel][i] += ((lfo_buf[channel][lfo_number][i]*Lfo[channel][lfo_number].volume)>>DAC_BIT) / lfo_activechannels;
				Lfo[channel][lfo_number].current_phase += Lfo[channel][lfo_number].delta_phase;
				if ( Lfo[channel][lfo_number].current_phase >= 1440.0f )
				{
					if (( Lfo[channel][lfo_number].flags  & CHANGE_FREQUENCY_FLAG ) == CHANGE_FREQUENCY_FLAG) // Hard sync
					{
						Lfo[channel][lfo_number].freq = Lfo[channel][lfo_number].new_freq;
						Lfo[channel][lfo_number].delta_phase = 1440.0f / ((float )SystemParameters.sampling_frequency[channel] / Lfo[channel][lfo_number].freq);
						Lfo[channel][lfo_number].flags  &= ~CHANGE_FREQUENCY_FLAG;
					}
					if ((( Lfo[channel][lfo_number].flags  & CHANGE_PHASE_FLAG ) == CHANGE_PHASE_FLAG) &&  (Lfo[channel][lfo_number].waveform != SINE ))
					{
						LFO_triCompile(channel,Lfo[channel][lfo_number].phase);
						LFO_squareCompile(channel,Lfo[channel][lfo_number].phase);
						Lfo[channel][lfo_number].flags  &= ~CHANGE_PHASE_FLAG;
					}
					Lfo[channel][lfo_number].current_phase -= 1440.0f;
				}
			}
			else
				lfo_buf[channel][lfo_number][i] = 0;
		}

}

void ChangeLFOWaveform(uint32_t channel,uint32_t lfo_number, uint32_t waveform)
{
	Lfo[channel][lfo_number].waveform = waveform;
}

void ChangeLFOVolume(uint32_t channel,uint32_t lfo_number, uint32_t volume)
{
	Lfo[channel][lfo_number].volume = volume*32;
}

void ChangeLFOPhase(uint32_t channel,uint32_t lfo_number, uint32_t phase)
{
	Lfo[channel][lfo_number].phase = phase;
	Lfo[channel][lfo_number].flags  |= CHANGE_PHASE_FLAG;
}

void EnableLFO(uint32_t channel,uint32_t lfo_number)
{
	Lfo[channel][lfo_number].enabled = OSCILLATOR_ENABLED;
	lfo_activechannels++;
}

void DisableLFO(uint32_t channel,uint32_t lfo_number)
{
	Lfo[channel][lfo_number].enabled = OSCILLATOR_DISABLED;
	Lfo[channel][lfo_number].current_phase = 1440.0f;
	lfo_activechannels--;
}

void ChangeLFOFrequency(uint32_t channel,uint32_t lfo_number, float freq)
{
	lfo_activechannels++;
	Lfo[channel][lfo_number].enabled = OSCILLATOR_ENABLED;
	if ( Lfo[channel][lfo_number].current_phase == 0.0f )
		Lfo[channel][lfo_number].current_phase = 360.0f;
	Lfo[channel][lfo_number].delta_phase = 90.0f;
	Lfo[channel][lfo_number].flags |= CHANGE_FREQUENCY_FLAG;
	Lfo[channel][lfo_number].new_freq = (float )freq;
}

void setLFOParams(uint32_t channel,uint32_t lfo_number,uint32_t freq,uint32_t volume,uint32_t phase,uint32_t waveform,uint32_t osc_group,uint32_t flags)
{
	Lfo[channel][lfo_number].freq = (float )freq;
	Lfo[channel][lfo_number].phase = (float )phase;
	Lfo[channel][lfo_number].volume = volume*32;
	switch ( waveform )
	{
	case	SINE		:	Lfo[channel][lfo_number].waveform = SINE;break;
	case	TRIANGLE	:	Lfo[channel][lfo_number].waveform = TRIANGLE;break;
	case	SQUARE		:	Lfo[channel][lfo_number].waveform = SQUARE;break;
	default				:	Lfo[channel][lfo_number].waveform = SINE;break;
	}
	Lfo[channel][lfo_number].osc_group = osc_group;
	Lfo[channel][lfo_number].flags = flags;
	ChangeLFOFrequency(channel,lfo_number, freq);
}

void DoLfo(void)
{
uint16_t	lfo_number,channel;
uint16_t	i,start,end;

	get_limits(&start,&end,(uint32_t *)Lfo[0][0].buffer_flag_ptr); // Use osc 0 to get limits
	for ( i=start;i<end;i++)
		lfo_out[0][i] = lfo_out[1][i] = 0;
	for(channel=0;channel<CHANNELS;channel++)
	{
		for(lfo_number=0;lfo_number<NUM_LFO;lfo_number++)
		{
			static_process_lfo(channel,lfo_number);
		}
	}
}

uint32_t InitLfo(void)
{
uint16_t	lfo_number,channel;
	for(channel=0;channel<CHANNELS;channel++)
	{
		for(lfo_number=0;lfo_number<NUM_LFO;lfo_number++)
		{
			Lfo[channel][lfo_number].enabled = OSCILLATOR_DISABLED;
			Lfo[channel][lfo_number].freq = 0.0f;
			Lfo[channel][lfo_number].waveform = SINE;
			Lfo[channel][lfo_number].volume = DAC_RESOLUTION;
			Lfo[channel][lfo_number].current_phase = 0.0f;
			Lfo[channel][lfo_number].delta_phase = 1.0f;
			Lfo[channel][lfo_number].buffer_flag_ptr = get_bufferhalf(channel);
			Lfo[channel][lfo_number].flags = NO_FLAG;
		}
		LFO_triCompile(channel,180);
		LFO_squareCompile(channel,180);
	}
	return NUMOSCILLATORS*2;
}





