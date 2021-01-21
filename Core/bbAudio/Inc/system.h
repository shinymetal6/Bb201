/*
 * system.h
 *
 *  Created on: Jan 10, 2021
 *      Author: fil
 */

#ifndef BBAUDIO_INC_SYSTEM_H_
#define BBAUDIO_INC_SYSTEM_H_

typedef struct _S_Component
{
	uint32_t (* FuncPtr)(uint16_t channel,uint16_t index);
	uint32_t in_buffer0;
	uint32_t in_buffer1;
	uint32_t in_buffer2;
	uint32_t in_buffer3;
	uint32_t out_buffer0;
	uint32_t out_buffer1;
	uint8_t  volumes_ch0_source;
	uint8_t  volumes_ch1_source;
	uint8_t  volumes_ch2_source;
	uint8_t  volumes_ch3_source;
	uint32_t volume_ch0_source_ptr;
	uint32_t volume_ch1_source_ptr;
	uint32_t volume_ch2_source_ptr;
	uint32_t volume_ch3_source_ptr;
	uint16_t aux_ch0[4];
	uint16_t aux_ch1[4];
	uint16_t aux_ch2[4];
	uint16_t aux_ch3[4];
	uint32_t half_ptr;
	uint16_t flags;
	uint16_t channel;
	uint16_t stage_number;
	char 	 functionName[32];
}S_Component;

#define	PROGRAM_VALID	0xc1a0c1a0
typedef struct _SystemParametersTypeDef
{
	char					Header[8];
	char					Version[8];
	uint32_t 				sampling_frequency[2];
	uint32_t 				global_detune[2];
	uint32_t 				flash_capacity;
}SystemParametersTypeDef;

typedef struct _SystemFlagsTypeDef
{
	uint32_t 				half_in_ch0;
	uint32_t 				half_in_ch1;
	uint32_t 				audioin_buffer_ready_ch0;
	uint32_t 				audioin_buffer_ready_ch1;
	uint32_t 				control_ready;
	uint32_t 				systick_ready;
}SystemFlagsTypeDef;

extern	uint32_t		stage;

extern	S_Component				Component[NUMSTAGES];
extern	SystemParametersTypeDef	SystemParameters;
extern	SystemFlagsTypeDef		SystemFlags;
extern	uint32_t setOutStage(S_Component *component,uint16_t channel,uint32_t stage_number);
extern	void DoFunnelOut(void);
extern	void bbSystemInit(void);
extern	void debug_0(void);
extern	void debug_1(void);
extern	void ChangeSampleFrequency(uint32_t sampling_frequency , uint32_t channel);
extern	void bbSystem_SystemSetDefaults(void);

#endif /* BBAUDIO_INC_SYSTEM_H_ */
