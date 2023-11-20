/**
 ****************************************************************************************************
 * @file        motor.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-13
 * @brief       电机驱动代码
 ****************************************************************************************************
 */
 
#ifndef _MOTOR_H
#define _MOTOR_H
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/KEY/key.h"

/******************************************************************************************/
/* 步进电机标志位 */
extern uint8_t g_window;

/******************************************************************************************/
/* 引脚 定义 */

#define MOTOR_GPIO_PORT                     GPIOF
#define MOTORIN1_GPIO_PIN                   GPIO_PIN_1
#define MOTORIN2_GPIO_PIN                   GPIO_PIN_2
#define MOTORIN3_GPIO_PIN                   GPIO_PIN_3
#define MOTORIN4_GPIO_PIN                   GPIO_PIN_4
#define MOTOR_GPIO_CLK_ENABLE()             do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

/******************************************************************************************/
/* MOTOR端口操作定义 */
    /* 设置值 */
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

    /* 取反 */
#define MOTORIN1_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTOR_GPIO_PORT, MOTORIN1_GPIO_PIN); }while(0)
#define MOTORIN2_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTOR_GPIO_PORT, MOTORIN2_GPIO_PIN); }while(0)
#define MOTORIN3_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTOR_GPIO_PORT, MOTORIN3_GPIO_PIN); }while(0)
#define MOTORIN4_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTOR_GPIO_PORT, MOTORIN4_GPIO_PIN); }while(0)

/******************************************************************************************/
/* 外部接口函数 */
void motor_init(void);                          /* 步进电机初始化 */

void motor_start_four_bat(void);                /* 步进电机4拍正转 */
void motor_start_four_bat_toggle(void);         /* 步进电机4拍反转 */

void motor_stop(void);                          /* 步进电机停转 */

void motor_control_chain(void);                 /* 控制链条转动 */

void motor_test(void);                          /* 步进电机测试 */

#endif








