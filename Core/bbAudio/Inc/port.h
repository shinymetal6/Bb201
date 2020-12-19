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

#define	AUDIO_0_2_LOOP_TIMER	&htim4
#define	AUDIO_1_3_LOOP_TIMER	&htim6
#define	CONTROL_LOOP_TIMER		&hlptim3


#define	KHZ352800			680
#define	KHZ176400			1360
#define	KHZ88200			2721
#define	KHZ44100			5442
#define	KHZ22050			10884

extern	void audio_timer_start(void);
extern	void control_timer_start(void);


#endif /* BBAUDIO_INC_PORT_H_ */
