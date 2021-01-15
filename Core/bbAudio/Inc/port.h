/*
 * port.h
 *
 *  Created on: Dec 5, 2020
 *      Author: fil
 */

#ifndef BBAUDIO_INC_PORT_H_
#define BBAUDIO_INC_PORT_H_

/* Controls */
extern	LPTIM_HandleTypeDef hlptim3;
/* Audio timers */
extern	TIM_HandleTypeDef htim4;
extern	TIM_HandleTypeDef htim6;
/* ADSR Timer */
extern	TIM_HandleTypeDef htim7;

#define	AUDIO_0_2_LOOP_TIMER	&htim4
#define	AUDIO_1_3_LOOP_TIMER	&htim6
#define	CONTROL_LOOP_TIMER		&hlptim3

extern	void audio_timer_start(void);
extern	void control_timer_start(void);
extern	void envelope_timer_start(void);
extern	void envelope_timer_callback(void);

extern	void change_tim_frequency(uint32_t sampling_frequency , uint32_t channel);


#endif /* BBAUDIO_INC_PORT_H_ */
