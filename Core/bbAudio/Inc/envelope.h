/*
 * envelope.h
 *
 *  Created on: Jan 10, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_ENVELOPE_H_
#define BBAUDIO_INC_ENVELOPE_H_

extern	void DoEnvelope(void);
extern	void EnvelopeInit(void);
extern	uint32_t EnvelopeInsert(void);

#define	ENVELOPE_STATE_IDLE		0
#define	ENVELOPE_STATE_A		1
#define	ENVELOPE_STATE_HA		2
#define	ENVELOPE_STATE_D		3
#define	ENVELOPE_STATE_HD		4
#define	ENVELOPE_STATE_S		5
#define	ENVELOPE_STATE_HS		6
#define	ENVELOPE_STATE_R		7
#define	ENVELOPE_STATE_HR		8
#define	ENVELOPE_STATE_PAUSE	9
#define	ENVELOPE_STATE_END		10

#define	ENVELOPE_INCLUDE_FLAG_A		1
#define	ENVELOPE_INCLUDE_FLAG_D		1 << 1
#define	ENVELOPE_INCLUDE_FLAG_S		1 << 2
#define	ENVELOPE_INCLUDE_FLAG_R		1 << 3
#define	ENVELOPE_INCLUDE_FLAG_HA	1 << 4
#define	ENVELOPE_INCLUDE_FLAG_HD	1 << 5
#define	ENVELOPE_INCLUDE_FLAG_HS	1 << 6
#define	ENVELOPE_INCLUDE_FLAG_HR	1 << 7
#define	ENVELOPE_REPEAT				1 << 30
#define	ENVELOPE_VALID				1 << 31

typedef struct _EnvelopeTypeDef
{
	double					envelope_value;
	uint32_t				A_time,D_time,S_time,R_time;
	uint32_t				A_value,D_value,S_value,R_value;
	double					A_step,D_step,S_step,R_step;
	uint32_t				HA,HD,HS,HR;
	uint32_t				A_work,D_work,S_work,R_work;
	uint32_t				HA_work,HD_work,HS_work,HR_work;
	uint32_t				state;
	uint32_t				flags;
	uint16_t				prescaler;
	uint16_t				current_counter;
	uint16_t				repeat_pause;
	uint16_t				current_repeat_pause;
	uint32_t 				controlled_object_ptr;
}EnvelopeTypeDef;



#endif /* BBAUDIO_INC_ENVELOPE_H_ */
