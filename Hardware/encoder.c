#include "encoder.h"
#include "tim.h"

#define FAULT_VAL 1000

struct Encoder encoder0 = { &htim3, 2400}, encoder1 = {&htim4, 40000};

void EncoderInit(struct Encoder* ecd)
{
	
	HAL_TIM_Encoder_Start(ecd->htim, TIM_CHANNEL_ALL);
	
}

u32 EncoderGetValue(struct Encoder* ecd)
{
	return __HAL_TIM_GET_COUNTER(ecd->htim);
}

s32 EncoderGetSpeed(struct Encoder* ecd)
{
    u32 pos = __HAL_TIM_GET_COUNTER(ecd->htim);
    //if (pos < ecd->ppr / FAULT_VAL  || pos > ecd->ppr - ecd->ppr / FAULT_VAL) return 0;
    s32 spd = pos - ecd->ppr * __HAL_TIM_IS_TIM_COUNTING_DOWN(ecd->htim);
    __HAL_TIM_SET_COUNTER(ecd->htim, 0);
    return spd > ecd->ppr / 2 || spd < -ecd->ppr / 2 ? 0 : spd;
}
