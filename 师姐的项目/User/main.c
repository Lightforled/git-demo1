/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-28
 * @brief       LVGL V8操作系统移植 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/SRAM/sram.h"
#include "lvgl_demo.h"
#include "./BSP/CAP/cap.h"
#include "motor.h"
#include "spwm.h"
#include "PWM.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */
    delay_init(168);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    key_init();                         /* 初始化按键 */
    sram_init();                        /* SRAM初始化 */
    BEEP_Init();                        /* 初始化蜂鸣器 */
    cap_init();                         /* 初始化电容继电器组 */
	
	//SPWM_init(SIN_550Hz);
	MOTOR_TIM4_Init();					/* 初始化电机 */
	Motor_Dir_Init();					/* 初始化电机方向GPIO */
    
	//SPWM_init(SIN_550Hz);
    //HAL_TIM_PWM_Start(&Motor_tim4, TIM_CHANNEL_3);
	//SPWM_init(SIN_550Hz);
    lvgl_demo();                        /* 运行FreeRTOS例程 */
	while(1)
	{
	}
}


