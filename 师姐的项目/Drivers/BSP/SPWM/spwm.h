#ifndef SPWM_H
#define SPWM_H


//调制波正弦波的频率
typedef enum {
    SIN_1Hz = 0,
	SIN_10Hz,
	SIN_30Hz,
    SIN_60Hz,
    SIN_550Hz,
}Sin_Fre_Typedef;


#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"



/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */



extern TIM_HandleTypeDef SPWM_tim1;

extern TIM_HandleTypeDef SPWM_tim2;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void SPWM_TIM1_Init(void);
void SPWM_TIM2_Init(uint32_t period);

void SPWM_TIM_MspPostInit(TIM_HandleTypeDef *htim);



void SPWM_init(Sin_Fre_Typedef status);


#endif



