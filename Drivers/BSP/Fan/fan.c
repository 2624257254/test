/**
 ****************************************************************************************************
 * @file        fan.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-14
 * @brief       ������������
 ****************************************************************************************************
 */
 
#include "./BSP/Fan/fan.h"

/* ���ȹ�����־ */
uint8_t g_bluefan_sta = 0;
uint8_t g_redfan_sta = 0;

/**
* @brief       ���ȳ�ʼ������
* @param       ��
* @retval      ��
*/
void fan_init(void)
{
    GPIO_InitTypeDef fan;
    
    FAN_GPIO_CLK_ENABLE();                  /* ����ʱ��ʹ�� */
    
    fan.Pin=BLUEFAN_GPIO_PIN;               /* ��ɫ�������� */
    fan.Mode=GPIO_MODE_OUTPUT_PP;               /* ������� */
    fan.Speed=GPIO_SPEED_FREQ_HIGH;         /* ����ģʽ */
    HAL_GPIO_Init(FAN_GPIO_PORT,&fan);      /* ��ʼ����ɫ�������� */
    
    fan.Pin=REDFAN_GPIO_PIN;                /* ��ɫ�������� */
    HAL_GPIO_Init(FAN_GPIO_PORT,&fan);      /* ��ʼ����ɫ�������� */
    
    BLUEFAN(0);
    REDFAN(0);
}

