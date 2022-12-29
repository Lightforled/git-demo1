#include "PWM.h"
#include "./SYSTEM/usart/usart.h"

TIM_HandleTypeDef PWM_tim1;
TIM_HandleTypeDef PWM_tim8;

/* TIM1 init function */
void PWM_TIM1_Init(uint16_t prescaler, uint16_t period, uint16_t deadtime, uint16_t degree)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  __HAL_RCC_TIM1_CLK_ENABLE();  //使能定时器1的时钟

  PWM_tim1.Instance = TIM1;
  PWM_tim1.Init.Prescaler = prescaler;
  PWM_tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  PWM_tim1.Init.Period = period;
  PWM_tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  PWM_tim1.Init.RepetitionCounter = 0;
  PWM_tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&PWM_tim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&PWM_tim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&PWM_tim1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&PWM_tim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC4REF;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&PWM_tim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = period / 2;         //50%的占空比
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&PWM_tim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_ACTIVE;
  sConfigOC.Pulse = degree;        //延迟角度
  if (HAL_TIM_OC_ConfigChannel(&PWM_tim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = deadtime;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&PWM_tim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  PWM_MspPostInit(&PWM_tim1);

}
/* TIM8 init function */
void PWM_TIM8_Init(uint16_t prescaler, uint16_t period, uint16_t deadtime)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  __HAL_RCC_TIM8_CLK_ENABLE();    //使能定时器8的时钟

  PWM_tim8.Instance = TIM8;
  PWM_tim8.Init.Prescaler = prescaler;
  PWM_tim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  PWM_tim8.Init.Period = period;
  PWM_tim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  PWM_tim8.Init.RepetitionCounter = 0;
  PWM_tim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&PWM_tim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&PWM_tim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&PWM_tim8) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&PWM_tim8, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&PWM_tim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = period / 2;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&PWM_tim8, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = deadtime;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&PWM_tim8, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  PWM_MspPostInit(&PWM_tim8);
}

//PWM硬件的初始化，对应相关的引脚
/**TIM1 GPIO Configuration
    PA7     ------> TIM1_CH1N
    PA8     ------> TIM1_CH1
    */
/**TIM8 GPIO Configuration
    PB14     ------> TIM8_CH3N
    PC7     ------> TIM8_CH3
    */
void PWM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM1)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
  
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  }
  else if(timHandle->Instance==TIM8)
  {

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }

}


/*
 * 
 *系统工作为168M时可以配置的死区时间范围
 *0~755.952ns(步长为5.95238 ns)
 *761~1511.9ns(步长为5.95238*2 ns)
 *1523.87~3000ns(步长为5.95238*8 ns)
 *3047.62~6000ns(步长为5.95238*16 ns)
 */
void PWM_PHASE_SHIFT(uint32_t frequent, uint16_t delay_degree)
{
  uint32_t uiTIMxCLK = SystemCoreClock;//系统工作的频率
  uint16_t usPeriod = 0;//工作的频率
  uint16_t usPrescaler = 0;//时钟预分频,ARR寄存器的值
  uint16_t deadtime = 0;//死区时间的计算
  float degree = 0;//延迟时间的计算

  if (frequent < 100)
  {
    usPrescaler = 10000 - 1;
    usPeriod = (uiTIMxCLK / 10000) / frequent - 1;
  }
  else if (frequent < 3000)
  {
    usPrescaler  = 100 - 1;
    usPeriod = (uiTIMxCLK / 100) / frequent - 1;
  }
  else
  {
    usPrescaler = 0;					/* 分频比 = 1 */
		usPeriod = uiTIMxCLK / frequent - 1;	/* 自动重装的值 */
  }

  //死区时间的计算,保持1%的死区时间,当频率小于1666.67Hz以下时,死区时间小于1%
  if (frequent < 1666)
    deadtime = 255;//死区时间最大,小于1%
  else if (frequent < 3281)
  {
    deadtime = (uint16_t)((0.01 * 168000000) / (frequent * 16)) - 32 + 224;
  }
  else if (frequent < 6562)
  {
    deadtime = (uint16_t)((0.01 * 168000000) / (frequent * 8)) - 32 + 192;
  }
  else if (frequent < 13140)
  {
    deadtime = (uint16_t)((0.01 * 168000000) / (frequent * 2)) - 64 + 128;
  }
  else
  {
    deadtime = (uint16_t)((0.01 * 168000000) / (frequent));
  }
  degree = 1.0 * delay_degree * (usPeriod + 1) / 360;
  //printf("degree = %g\r\n", degree);
	PWM_TIM1_Init(usPrescaler, usPeriod, deadtime, degree);
 	PWM_TIM8_Init(usPrescaler, usPeriod, deadtime);
	HAL_TIM_PWM_Start(&PWM_tim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&PWM_tim1, TIM_CHANNEL_1);
 	HAL_TIM_PWM_Start(&PWM_tim8, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&PWM_tim8, TIM_CHANNEL_2);
	//printf("死区的时间是 %u\r\n", deadtime);
}






