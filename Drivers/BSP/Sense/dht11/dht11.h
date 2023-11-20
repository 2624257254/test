/**
 ****************************************************************************************************
 * @file        dht11.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-14
 * @brief       传感器数据采集
 ****************************************************************************************************
 */

#ifndef _DHT11_H
#define _DHT11_H

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LCD/lcd.h"

/******************************************************************************************/
/* 引脚 定义 */

/* DHT11温湿度传感器 */
#define DHT11_GPIO_PORT                 GPIOG
#define DHT11_GPIO_PIN                  GPIO_PIN_11
#define DHT11_GPIO_CLK_ENABLE()         do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

/******************************************************************************************/
/* IO操作函数 */

/* dht11数据输出 */
#define DHT11_OUT(x)        do{ x ? \
                                HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET) : \
                                HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET); \
                            }while(0)
/* dht11数据读入 */
#define DHT11_IN            HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

/* 外部接口函数 */
uint8_t dht11_init(void);                                   /* 初始化dht11 */
uint8_t dht11_check(void);                                  /* 检测dht11是否存在 */
uint8_t dht11_read_data(uint8_t *temp, uint8_t *humi);      /* 读取温湿度 */

#endif





