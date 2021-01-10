/*
 * control_functions.h
 *
 *  Created on: Jan 10, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_CONTROL_FUNCTIONS_H_
#define BBAUDIO_INC_CONTROL_FUNCTIONS_H_

#define		CONTROLBUF_LEN					6
extern	ADC_HandleTypeDef hadc2;
typedef struct _AF_ControlTypeDef
{
	uint16_t ain0;
	uint16_t ain1;
	uint16_t ain2;
	uint16_t ain3;
	uint16_t ain4;
	uint16_t ain5;
}AF_ControlTypeDef;
extern	AF_ControlTypeDef		control_buf;

uint16_t	control_ready;
#define		CONTROL_ADC				&hadc2
extern	void ControlInit(void);

#endif /* BBAUDIO_INC_CONTROL_FUNCTIONS_H_ */
