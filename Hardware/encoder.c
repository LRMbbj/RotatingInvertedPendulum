#include "encoder.h"
#include "tim.h"

struct Encoder encoder0 = { &htim3, 2400}, encoder1 = {&htim4, 40000};

void EncoderInit(struct Encoder* ecd)
{
	
	HAL_TIM_Encoder_Start(ecd->htim, TIM_CHANNEL_ALL);
	
}

u16 EncoderGetValue(struct Encoder* ecd)
{
	return __HAL_TIM_GET_COUNTER(ecd->htim);
}
