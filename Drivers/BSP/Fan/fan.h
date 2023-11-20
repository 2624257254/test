/**
 ****************************************************************************************************
 * @file        fan.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-14
 * @brief       风扇驱动代码
 ****************************************************************************************************
 */
 
#ifndef _FAN_H
#define _FAN_H
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* 风扇工作标志 */
extern uint8_t g_bluefan_sta;
extern uint8_t g_redfan_sta;

/******************************************************************************************/
/* 引脚 定义 */

#define FAN_GPIO_PORT                   GPIOE
#define BLUEFAN_GPIO_PIN                GPIO_PIN_1
#define REDFAN_GPIO_PIN                 GPIO_PIN_2
#define FAN_GPIO_CLK_ENABLE()             do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)

/******************************************************************************************/
/* FAN端口操作定义 */

#define BLUEFAN(x)      do{ x ? \
                            HAL_GPIO_WritePin(FAN_GPIO_PORT, BLUEFAN_GPIO_PIN, GPIO_PIN_SET): \
                            HAL_GPIO_WritePin(FAN_GPIO_PORT, BLUEFAN_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)

#define REDFAN(x)      do{ x ? \
                            HAL_GPIO_WritePin(FAN_GPIO_PORT, REDFAN_GPIO_PIN, GPIO_PIN_SET): \
                            HAL_GPIO_WritePin(FAN_GPIO_PORT, REDFAN_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)

/* FAN端口反转 */
#define BLUEFAN_TOGGLE()    do{ HAL_GPIO_TogglePin(FAN_GPIO_PORT, BLUEFAN_GPIO_PIN);\
                                g_bluefan_sta = !g_bluefan_sta; }while(0)
#define REDFAN_TOGGLE()     do{ HAL_GPIO_TogglePin(FAN_GPIO_PORT, REDFAN_GPIO_PIN);\
                                g_redfan_sta = !g_redfan_sta; }while(0)

/******************************************************************************************/
/* 外部接口函数 */
void fan_init(void);                                /* 风扇初始化 */



#endif






