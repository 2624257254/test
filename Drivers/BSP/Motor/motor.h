/**
 ****************************************************************************************************
 * @file        motor.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-13
 * @brief       �����������
 ****************************************************************************************************
 */
 
#ifndef _MOTOR_H
#define _MOTOR_H
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/KEY/key.h"

/******************************************************************************************/
/* ���������־λ */
extern uint8_t g_window;

/******************************************************************************************/
/* ���� ���� */

#define MOTOR_GPIO_PORT                     GPIOF
#define MOTORIN1_GPIO_PIN                   GPIO_PIN_1
#define MOTORIN2_GPIO_PIN                   GPIO_PIN_2
#define MOTORIN3_GPIO_PIN                   GPIO_PIN_3
#define MOTORIN4_GPIO_PIN                   GPIO_PIN_4
#define MOTOR_GPIO_CLK_ENABLE()             do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

/******************************************************************************************/
/* MOTOR�˿ڲ������� */
    /* ����ֵ */
#define MOTORIN1(x)     do{ x ? \
                            HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTORIN1_GPIO_PIN, GPIO_PIN_SET): \
                            HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTORIN1_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)

#define MOTORIN2(x)     do{ x ? \
                            HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTORIN2_GPIO_PIN, GPIO_PIN_SET): \
                            HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTORIN2_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)

#define MOTORIN3(x)     do{ x ? \
                            HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTORIN3_GPIO_PIN, GPIO_PIN_SET): \
                            HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTORIN3_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)

#define MOTORIN4(x)     do{ x ? \
                            HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTORIN4_GPIO_PIN, GPIO_PIN_SET): \
                            HAL_GPIO_WritePin(MOTOR_GPIO_PORT, MOTORIN4_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)

    /* ȡ�� */
#define MOTORIN1_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTOR_GPIO_PORT, MOTORIN1_GPIO_PIN); }while(0)
#define MOTORIN2_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTOR_GPIO_PORT, MOTORIN2_GPIO_PIN); }while(0)
#define MOTORIN3_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTOR_GPIO_PORT, MOTORIN3_GPIO_PIN); }while(0)
#define MOTORIN4_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTOR_GPIO_PORT, MOTORIN4_GPIO_PIN); }while(0)

/******************************************************************************************/
/* �ⲿ�ӿں��� */
void motor_init(void);                          /* ���������ʼ�� */

void motor_start_four_bat(void);                /* �������4����ת */
void motor_start_four_bat_toggle(void);         /* �������4�ķ�ת */

void motor_stop(void);                          /* �������ͣת */

void motor_control_chain(void);                 /* ��������ת�� */

void motor_test(void);                          /* ����������� */

#endif








