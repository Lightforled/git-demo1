#ifndef PWM_H
#define PWM_H

#include "./SYSTEM/sys/sys.h"


extern TIM_HandleTypeDef PWM_tim1;

extern TIM_HandleTypeDef PWM_tim8;


void PWM_TIM1_Init(uint16_t prescaler, uint16_t period, uint16_t deadtime, uint16_t degree);
void PWM_TIM8_Init(uint16_t prescaler, uint16_t period, uint16_t deadtime);
void PWM_PHASE_SHIFT(uint32_t frequent, uint16_t delay_degree);
void PWM_MspPostInit(TIM_HandleTypeDef *htim);


#endif


