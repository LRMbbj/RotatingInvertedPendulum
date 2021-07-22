#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "tim.h"

typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t s32;

struct Encoder
{
	TIM_HandleTypeDef* htim;
	u16 ppr;
};

extern struct Encoder encoder0, encoder1;

void EncoderInit(struct Encoder* ecd);
u32 EncoderGetValue(struct Encoder* ecd);
s32 EncoderGetSpeed(struct Encoder* ecd);

#endif
