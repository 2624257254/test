/**
 ****************************************************************************************************
 * @fileg_stepmotor_reversal_sta        motor.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-13
 * @brief       �����������
 ****************************************************************************************************
 */
 
#include "./BSP/Motor/motor.h"

#define MOTOR_DLTIME    3                               /* �����ʱ */
uint8_t g_window = 0;

/**
 * @brief       ��ʼ��MOTOR���IO��, ��ʹ��ʱ��
 * @param       ��
 * @retval      ��
 */
void motor_init(void)
{
    MOTOR_GPIO_CLK_ENABLE();
    
    GPIO_InitTypeDef gpio_init_struct;                  /* �˿ڽṹ�� */
    
    gpio_init_struct.Pin = MOTORIN1_GPIO_PIN;           /* IN1����*/
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* ������� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* ����ģʽ */
    HAL_GPIO_Init(MOTOR_GPIO_PORT, &gpio_init_struct);  /* ��ʼ��IN1���� */
    
    gpio_init_struct.Pin = MOTORIN2_GPIO_PIN;
    HAL_GPIO_Init(MOTOR_GPIO_PORT, &gpio_init_struct);  /* ��ʼ��IN2���� */
    
    gpio_init_struct.Pin = MOTORIN3_GPIO_PIN;
    HAL_GPIO_Init(MOTOR_GPIO_PORT, &gpio_init_struct);  /* ��ʼ��IN3���� */
    
    gpio_init_struct.Pin = MOTORIN4_GPIO_PIN;
    HAL_GPIO_Init(MOTOR_GPIO_PORT, &gpio_init_struct);  /* ��ʼ��IN4���� */
    
    MOTORIN1(0);
    MOTORIN2(0);
    MOTORIN3(0);
    MOTORIN4(0);
}

/**
 * @brief       ���4����ת
 * @param       ��
 * @retval      ��
 */
void motor_start_four_bat(void)
{
    MOTORIN1(1); 
    MOTORIN4(0);
    delay_ms(MOTOR_DLTIME);

    MOTORIN1(0);
    MOTORIN2(1);
    delay_ms(MOTOR_DLTIME);

    MOTORIN2(0);
    MOTORIN3(1);
    delay_ms(MOTOR_DLTIME);

    MOTORIN3(0);
    MOTORIN4(1);
    delay_ms(MOTOR_DLTIME);

}

/**
 * @brief       ���4�ķ�ת
 * @param       ��
 * @retval      ��
 */
void motor_start_four_bat_toggle(void)
{
    MOTORIN1(0);
    MOTORIN4(1);
    delay_ms(MOTOR_DLTIME);
    
    MOTORIN4(0);
    MOTORIN3(1);
    delay_ms(MOTOR_DLTIME);
    
    MOTORIN3(0);
    MOTORIN2(1);
    delay_ms(MOTOR_DLTIME);
    
    MOTORIN2(0);
    MOTORIN1(1);
    delay_ms(MOTOR_DLTIME);
}

/**
 * @brief       ���ͣת
 * @param       ��
 * @retval      ��
 */
void motor_stop(void)
{
    MOTORIN1(0);
    MOTORIN2(0);
    MOTORIN3(0);
    MOTORIN4(0);
}








