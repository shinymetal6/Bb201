/*
 * control_functions.h
 *
 *  Created on: Jan 10, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_CONTROL_FUNCTIONS_H_
#define BBAUDIO_INC_CONTROL_FUNCTIONS_H_

#define		CONTROLBUF_LEN					16
#define		CONTROLBUF_CHANNELS				6
extern	ADC_HandleTypeDef hadc2;
typedef struct _AF_ControlTypeDef
{
	uint16_t ain[CONTROLBUF_LEN];
}AF_ControlTypeDef;
extern	AF_ControlTypeDef		control_buf;

uint16_t	control_ready;
#define		CONTROL_ADC				&hadc2
extern	void ControlInit(void);
extern	void DoControls(void);

#endif /* BBAUDIO_INC_CONTROL_FUNCTIONS_H_ */
