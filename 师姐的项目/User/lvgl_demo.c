/**
 ****************************************************************************************************
 * @file        lvgl_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-11
 * @brief       LVGL V8����ϵͳ��ֲ
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
 
#include "lvgl_demo.h"
#include "./BSP/LED/led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "./BSP/BEEP/beep.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_demo_stress.h"
#include "./BSP/MY_GUI/my_gui.h"

/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO     1           /* �������ȼ� */
#define START_STK_SIZE      128         /* �����ջ��С */
TaskHandle_t StartTask_Handler;         /* ������ */
void start_task(void *pvParameters);    /* ������ */

/* LV_DEMO_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LV_DEMO_TASK_PRIO   3           /* �������ȼ� */
#define LV_DEMO_STK_SIZE    1024        /* �����ջ��С */
TaskHandle_t LV_DEMOTask_Handler;       /* ������ */
void lv_demo_task(void *pvParameters);  /* ������ */

/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO       4           /* �������ȼ� */
#define LED_STK_SIZE        128         /* �����ջ��С */
TaskHandle_t LEDTask_Handler;           /* ������ */
void led_task(void *pvParameters);      /* ������ */
/******************************************************************************************************/
#include "motor.h"
#include "spwm.h"


/**
 * @brief       lvgl_demo��ں���
 * @param       ��
 * @retval      ��
 */
void lvgl_demo(void)
{
    lv_init();                                          /* lvglϵͳ��ʼ�� */
    lv_port_disp_init();                                /* lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ��� */
    lv_port_indev_init();                               /* lvgl����ӿڳ�ʼ��,����lv_init()�ĺ��� */

    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ���ݸ��������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */
	printf("��ʼ�������\r\n");
    vTaskStartScheduler();                              /* ����������� */
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    taskENTER_CRITICAL();           /* �����ٽ��� */

    /* ����LVGL���� */
    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);

    /* LED�������� */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
}

/**
 * @brief       LVGL��������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void lv_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
	my_gui();
    
   
    while(1)
    {
        lv_timer_handler(); /* LVGL��ʱ�� */
        vTaskDelay(5);
    }
}


/**
 * @brief       led_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while(1)
    {
        //HAL_TIM_PWM_Start(&Motor_tim4, TIM_CHANNEL_3);
        vTaskDelay(10);
    }
}


