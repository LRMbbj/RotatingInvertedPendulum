#include "motor.h"
#include <math.h>

const struct Motor motor0 = {&htim3, TIM_CHANNEL_2, 1000, GPIOC, GPIO_PIN_2, GPIO_PIN_3};

void MotorStatus(const struct Motor *motor, u8 state)
{
	
	if(state)
	{
		HAL_TIM_PWM_Start(motor->htim, motor->Channel);
		HAL_GPIO_WritePin(motor->DirGPIO, motor->DirA|motor->DirB, GPIO_PIN_SET );
	}
	else
	{
		HAL_TIM_PWM_Stop(motor->htim, motor->Channel);
		HAL_GPIO_WritePin(motor->DirGPIO, motor->DirA|motor->DirB, GPIO_PIN_RESET );
	}
	
}
void MotorControl(const struct Motor *motor, int16_t val)
{
	uint16_t ctrl;
	
	if(val >= 0)
	{
		HAL_GPIO_WritePin(motor->DirGPIO, motor->DirA, GPIO_PIN_SET );
		HAL_GPIO_WritePin(motor->DirGPIO, motor->DirB, GPIO_PIN_RESET );
		ctrl = val;
	}
	else
	{
		HAL_GPIO_WritePin(motor->DirGPIO, motor->DirA, GPIO_PIN_RESET );
		HAL_GPIO_WritePin(motor->DirGPIO, motor->DirB, GPIO_PIN_SET );
		ctrl = -val;
	}
	
	ctrl = ctrl > motor->Counter ? motor->Counter : ctrl;
	
	__HAL_TIM_SET_COMPARE(motor->htim, motor->Channel, ctrl);
	
}

