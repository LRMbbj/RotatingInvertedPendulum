#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "tim.h"

typedef uint16_t u16;

struct Encoder
{
	TIM_HandleTypeDef* htim;
	u16 ppr;
};

extern struct Encoder encoder0, encoder1;

void EncoderInit(struct Encoder* ecd);
u16 EncoderGetValue(struct Encoder* ecd);

#endif
