#ifndef	MOTOR_H
#define MOTOR_H


#include "./SYSTEM/sys/sys.h"




extern TIM_HandleTypeDef Motor_tim4;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void Motor_Dir_Init(void);
void MOTOR_TIM4_Init(void);

void MOTOR_TIM_MspPostInit(TIM_HandleTypeDef *htim);

 


#endif

