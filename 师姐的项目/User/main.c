/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-28
 * @brief       LVGL V8����ϵͳ��ֲ ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7); /* ����ʱ��,168Mhz */
    delay_init(168);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    key_init();                         /* ��ʼ������ */
    sram_init();                        /* SRAM��ʼ�� */
    BEEP_Init();                        /* ��ʼ�������� */
    cap_init();                         /* ��ʼ�����ݼ̵����� */
	
	//SPWM_init(SIN_550Hz);
	MOTOR_TIM4_Init();					/* ��ʼ����� */
	Motor_Dir_Init();					/* ��ʼ���������GPIO */
    
	//SPWM_init(SIN_550Hz);
    //HAL_TIM_PWM_Start(&Motor_tim4, TIM_CHANNEL_3);
	//SPWM_init(SIN_550Hz);
    lvgl_demo();                        /* ����FreeRTOS���� */
	while(1)
	{
	}
}


