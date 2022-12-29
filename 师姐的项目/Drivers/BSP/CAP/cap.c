#include "./BSP/CAP/cap.h"



//��ʼ���̵������ŵ�GPIO
void cap_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    //����ʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    //��ʼ��GPIO�ĵ�ƽ,�ܹ�20��
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_9, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_RESET);

    GPIO_InitStructure.Pin = GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_8;                   /* B������ */
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    GPIO_InitStructure.Pull = GPIO_NOPULL;                    /* ���� */
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;          /* ���� */
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);       /* ��ʼ������ */

    GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_13;                   /* B������ */
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);       /* ��ʼ������ */

    GPIO_InitStructure.Pin = GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_2;                   /* B������ */
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);       /* ��ʼ������ */

    GPIO_InitStructure.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_5;                   /* B������ */
    HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);       /* ��ʼ������ */

    GPIO_InitStructure.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_9;                   /* B������ */
    HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);       /* ��ʼ������ */

    GPIO_InitStructure.Pin = GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_15;                   /* B������ */
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);       /* ��ʼ������ */
}


void cap_change(uint8_t flag, uint8_t id)
{
    switch (id)
    {
        case 1: CAP1(flag);break;
        case 2: CAP2(flag);break;
        case 3: CAP3(flag);break;
        case 4: CAP4(flag);break;
        case 5: CAP5(flag);break;
        case 6: CAP6(flag);break;
        case 7: CAP7(flag);break;
        case 8: CAP8(flag);break;
        case 9: CAP9(flag);break;
        case 10: CAP10(flag);break;
        case 11: CAP11(flag);break;
        case 12: CAP12(flag);break;
        case 13: CAP13(flag);break;
        case 14: CAP14(flag);break;
        case 15: CAP15(flag);break;
        case 16: CAP16(flag);break;
        case 17: CAP17(flag);break;
        case 18: CAP18(flag);break;
        case 19: CAP19(flag);break;
        case 20: CAP20(flag);break;
        default:break;
    }
}
