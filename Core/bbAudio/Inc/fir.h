/*
 * fir.h
 *
 *  Created on: Dec 13, 2020
 *      Author: fil
 */

#ifndef BBAUDIO_INC_FIR_H_
#define BBAUDIO_INC_FIR_H_

#include "main.h"
#include "arm_math.h"

#define 	NUM_TAPS		64
extern		float32_t ffirCoeffs32[NUM_TAPS];

#endif /* BBAUDIO_INC_FIR_H_ */
