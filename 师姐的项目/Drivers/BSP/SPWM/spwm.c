#include "spwm.h"
#include "./SYSTEM/usart/usart.h"

uint8_t flag_1HZ = 0;
uint8_t flag_60HZ = 0;
uint8_t flag_550HZ = 0;

//调制度均为0.9
#define SIN_SIZE_1HZ 500

//调制度均为0.9
#define SIN_SIZE_60HZ 500
//1Hz时，占空比表格
static uint16_t sin_table_60Hz[] = {2100,2123,2147,2171,2194,2218,2242,2266,2289,2313,2336,2360,2383,2407,2430,2454,
2477,2500,2523,2546,2570,2592,2615,2638,2661,2684,2706,2729,2751,2773,2795,2817,
2839,2861,2883,2904,2926,2947,2968,2989,3010,3031,3051,3072,3092,3112,3132,3152,
3172,3191,3210,3230,3248,3267,3286,3304,3322,3340,3358,3376,3393,3411,3428,3444,
3461,3477,3493,3509,3525,3541,3556,3571,3586,3600,3614,3629,3642,3656,3669,3682,
3695,3708,3720,3732,3744,3756,3767,3778,3789,3799,3810,3820,3829,3839,3848,3857,
3865,3874,3882,3890,3897,3904,3911,3918,3924,3930,3936,3941,3947,3951,3956,3960,
3964,3968,3971,3975,3977,3980,3982,3984,3986,3987,3988,3989,3989,3990,3989,3989,
3988,3987,3986,3984,3982,3980,3977,3975,3971,3968,3964,3960,3956,3951,3947,3941,
3936,3930,3924,3918,3911,3904,3897,3890,3882,3874,3865,3857,3848,3839,3829,3820,
3810,3799,3789,3778,3767,3756,3744,3732,3720,3708,3695,3682,3669,3656,3642,3629,
3614,3600,3586,3571,3556,3541,3525,3509,3493,3477,3461,3444,3428,3411,3393,3376,
3358,3340,3322,3304,3286,3267,3248,3230,3210,3191,3172,3152,3132,3112,3092,3072,
3051,3031,3010,2989,2968,2947,2926,2904,2883,2861,2839,2817,2795,2773,2751,2729,
2706,2684,2661,2638,2615,2592,2570,2546,2523,2500,2477,2454,2430,2407,2383,2360,
2336,2313,2289,2266,2242,2218,2194,2171,2147,2123,2100,2076,2052,2028,2005,1981,
1957,1933,1910,1886,1863,1839,1816,1792,1769,1745,1722,1699,1676,1653,1629,1607,
1584,1561,1538,1515,1493,1470,1448,1426,1404,1382,1360,1338,1316,1295,1273,1252,
1231,1210,1189,1168,1148,1127,1107,1087,1067,1047,1027,1008,989,969,951,932,
913,895,877,859,841,823,806,788,771,755,738,722,706,690,674,658,
643,628,613,599,585,570,557,543,530,517,504,491,479,467,455,443,
432,421,410,400,389,379,370,360,351,342,334,325,317,309,302,295,
288,281,275,269,263,258,252,248,243,239,235,231,228,224,222,219,
217,215,213,212,211,210,210,210,210,210,211,212,213,215,217,219,
222,224,228,231,235,239,243,248,252,258,263,269,275,281,288,295,
302,309,317,325,334,342,351,360,370,379,389,400,410,421,432,443,
455,467,479,491,504,517,530,543,557,570,585,599,613,628,643,658,
674,690,706,722,738,755,771,788,806,823,841,859,877,895,913,932,
951,969,989,1008,1027,1047,1067,1087,1107,1127,1148,1168,1189,1210,1231,1252,
1273,1295,1316,1338,1360,1382,1404,1426,1448,1470,1493,1515,1538,1561,1584,1607,
1629,1653,1676,1699,1722,1745,1769,1792,1816,1839,1863,1886,1910,1933,1957,1981,
2005,2028,2052,2076};


TIM_HandleTypeDef SPWM_tim1;
TIM_HandleTypeDef SPWM_tim2;


#define CARRIER_WAVE_FREQUENT  40000  //载波的频率 
#define TIM1_PERIOD   ((168000000 / CARRIER_WAVE_FREQUENT) - 1)
//定时器中断的频率，也就是更改SPWM的占空比的时间

/* TIM1 init function */
void SPWM_TIM1_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  __HAL_RCC_TIM1_CLK_ENABLE();

  SPWM_tim1.Instance = TIM1;
  SPWM_tim1.Init.Prescaler = 0;
  SPWM_tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  SPWM_tim1.Init.Period = TIM1_PERIOD;
  SPWM_tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  SPWM_tim1.Init.RepetitionCounter = 0;
  SPWM_tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&SPWM_tim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&SPWM_tim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 123;    //占空比会变化，这里的数值无关
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&SPWM_tim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 10;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&SPWM_tim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  SPWM_TIM_MspPostInit(&SPWM_tim1);

}
/* TIM2 init function */
void SPWM_TIM2_Init(uint32_t period)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  __HAL_RCC_TIM2_CLK_ENABLE();
  
  SPWM_tim2.Instance = TIM2;
  SPWM_tim2.Init.Prescaler = 0;
  SPWM_tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  SPWM_tim2.Init.Period = period;
  SPWM_tim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  SPWM_tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&SPWM_tim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&SPWM_tim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&SPWM_tim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
//SPWM硬件初始化
/**TIM1 GPIO Configuration
    PE12     ------> TIM1_CH3N
    PE13     ------> TIM1_CH3
    */
void SPWM_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM1)
  {
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}
void SPWM_init(Sin_Fre_Typedef status)
{
  uint32_t current_period;

  switch (status)
  {
  case SIN_1Hz:
    current_period = (84000000 / (1 * 500)) - 1; 
    break;
  case SIN_10Hz:
    current_period = (84000000 / (10 * 500)) - 1;  
    break;
  case SIN_30Hz:
    current_period = (84000000 / (30 * 500)) - 1; 
    break;
  case SIN_60Hz:
    current_period = (84000000 / (60 * 500)) - 1;  
    break;
  case SIN_550Hz:
    current_period = (84000000 / (550 * 500)) - 1; 
    break;
  default:
    break;
  }
  SPWM_TIM1_Init();
  SPWM_TIM2_Init(current_period);
  HAL_TIM_PWM_Start(&SPWM_tim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&SPWM_tim1, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&SPWM_tim2);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == SPWM_tim2.Instance)
  {
    static uint16_t num = 0;
	 __HAL_TIM_SET_COMPARE(&SPWM_tim1, TIM_CHANNEL_1, sin_table_60Hz[num]);
      num++;
      if (500 == num) 
        num = 0;
  }
}
