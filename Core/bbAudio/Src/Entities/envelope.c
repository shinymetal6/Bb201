/*
 * envelope.c
 *
 *  Created on: Jan 10, 2021
 *      Author: fil
 */

#include "main.h"

EnvelopeTypeDef	Envelope[NUMENVELOPES];
uint16_t	envelope_counter=0;

void DoEnvelope(void)
{
uint32_t	i;
	for(i=0;i<NUMENVELOPES;i++)
	{
		Envelope[i].current_counter ++;
		if ( Envelope[i].current_counter == Envelope[envelope_counter].prescaler )
		{
			Envelope[i].current_counter = 0;
			switch(Envelope[i].state)
			{
			case	ENVELOPE_STATE_IDLE	:
				Envelope[i].envelope_value = 0;
				if (( Envelope[i].flags & ENVELOPE_VALID) == ENVELOPE_VALID)
					Envelope[i].state = ENVELOPE_STATE_A;
				break;
			case	ENVELOPE_STATE_A	:
				Envelope[i].A_work++;
				Envelope[i].envelope_value += Envelope[i].A_step;
				if ( Envelope[i].A_work >= Envelope[i].A_time)
				{
					if (( Envelope[i].flags & ENVELOPE_INCLUDE_FLAG_HA ) == ENVELOPE_INCLUDE_FLAG_HA)
					{
						Envelope[i].state = ENVELOPE_STATE_HA;
					}
					else
					{
						Envelope[i].state = ENVELOPE_STATE_D;
					}
				}
				break;
			case	ENVELOPE_STATE_HA	:
				Envelope[i].state = ENVELOPE_STATE_D;
				break;
			case	ENVELOPE_STATE_D	:
				Envelope[i].D_work++;
				Envelope[i].envelope_value -= Envelope[i].D_step;
				if ( Envelope[i].D_work >= Envelope[i].D_time)
				{
					if (( Envelope[i].flags & ENVELOPE_INCLUDE_FLAG_HD ) == ENVELOPE_INCLUDE_FLAG_HD)
					{
						Envelope[i].state = ENVELOPE_STATE_HD;
					}
					else
					{
						Envelope[i].state = ENVELOPE_STATE_S;
					}
				}
				break;
			case	ENVELOPE_STATE_HD	:
				Envelope[i].state = ENVELOPE_STATE_S;
				break;
			case	ENVELOPE_STATE_S	:
				Envelope[i].envelope_value -= Envelope[i].S_step;
				Envelope[i].S_work++;
				if ( Envelope[i].S_work >= Envelope[i].S_time)
				{
					if (( Envelope[i].flags & ENVELOPE_INCLUDE_FLAG_HS ) == ENVELOPE_INCLUDE_FLAG_HS)
					{
						Envelope[i].state = ENVELOPE_STATE_HS;
					}
					else
					{
						Envelope[i].state = ENVELOPE_STATE_R;
					}
				}
				break;
			case	ENVELOPE_STATE_HS	:
				Envelope[i].state = ENVELOPE_STATE_R;
				break;
			case	ENVELOPE_STATE_R	:
				Envelope[i].envelope_value -= Envelope[i].R_step;
				Envelope[i].R_work++;
				if ( Envelope[i].R_work >= Envelope[i].R_time)
				{
					if (( Envelope[i].flags & ENVELOPE_INCLUDE_FLAG_HR ) == ENVELOPE_INCLUDE_FLAG_HR)
					{
						Envelope[i].state = ENVELOPE_STATE_HR;
					}
					else
					{
						Envelope[i].state = ENVELOPE_STATE_END;
					}
				}
				break;
			case	ENVELOPE_STATE_HR	:
				Envelope[i].state = ENVELOPE_STATE_END;
				break;
			case	ENVELOPE_STATE_END	:
				if (( Envelope[i].flags & ENVELOPE_REPEAT ) == ENVELOPE_REPEAT)
					Envelope[i].state = ENVELOPE_STATE_PAUSE;
				else
				{
					Envelope[envelope_counter].A_work = Envelope[envelope_counter].D_work = Envelope[envelope_counter].S_work = Envelope[envelope_counter].R_work = 0;
					Envelope[i].state = ENVELOPE_STATE_IDLE;
				}
				break;
			case	ENVELOPE_STATE_PAUSE	:
				Envelope[i].current_repeat_pause ++;
				if ( Envelope[i].current_repeat_pause  >= Envelope[i].repeat_pause )
				{
					Envelope[i].current_repeat_pause = 0;
					Envelope[i].state = ENVELOPE_STATE_A;
				}
				break;
			}
		}
	}
}

uint32_t EnvelopeInsert(void)
{
	if ( envelope_counter >= NUMENVELOPES )
		return 1;
	Envelope[envelope_counter].A_time = 30;
	Envelope[envelope_counter].D_time = 314;
	Envelope[envelope_counter].S_time = 256;
	Envelope[envelope_counter].R_time = 100;
	Envelope[envelope_counter].A_value = 4096;
	Envelope[envelope_counter].D_value = 3200;
	Envelope[envelope_counter].S_value = 2048;
	Envelope[envelope_counter].R_value = 512;
	Envelope[envelope_counter].A_work = Envelope[envelope_counter].D_work = Envelope[envelope_counter].S_work = Envelope[envelope_counter].R_work = 0;
	Envelope[envelope_counter].state = ENVELOPE_STATE_IDLE;
	Envelope[envelope_counter].prescaler = 1;
	Envelope[envelope_counter].current_counter = 0;
	Envelope[envelope_counter].flags=ENVELOPE_INCLUDE_FLAG_A|ENVELOPE_INCLUDE_FLAG_D|ENVELOPE_INCLUDE_FLAG_S|ENVELOPE_INCLUDE_FLAG_R|ENVELOPE_VALID;
	Envelope[envelope_counter].A_step = Envelope[envelope_counter].A_value / Envelope[envelope_counter].A_time;
	Envelope[envelope_counter].D_step = (Envelope[envelope_counter].A_value - Envelope[envelope_counter].D_value) / Envelope[envelope_counter].D_time;
	Envelope[envelope_counter].S_step = (Envelope[envelope_counter].D_value - Envelope[envelope_counter].S_value) / Envelope[envelope_counter].S_time;
	Envelope[envelope_counter].R_step = (Envelope[envelope_counter].S_value - Envelope[envelope_counter].R_value) / Envelope[envelope_counter].R_time;
	return 0;
}

void EnvelopeInit(void)
{
//uint32_t	i;
	envelope_counter=0;
	bzero(&Envelope, sizeof(Envelope));
}
