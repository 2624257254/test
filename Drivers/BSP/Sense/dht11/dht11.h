/**
 ****************************************************************************************************
 * @file        dht11.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-14
 * @brief       ���������ݲɼ�
 ****************************************************************************************************
 */

#ifndef _DHT11_H
#define _DHT11_H

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LCD/lcd.h"

/******************************************************************************************/
/* ���� ���� */

/* DHT11��ʪ�ȴ����� */
#define DHT11_GPIO_PORT                 GPIOG
#define DHT11_GPIO_PIN                  GPIO_PIN_11
#define DHT11_GPIO_CLK_ENABLE()         do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

/******************************************************************************************/
/* IO�������� */

/* dht11������� */
#define DHT11_OUT(x)        do{ x ? \
                                HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET) : \
                                HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET); \
                            }while(0)
/* dht11���ݶ��� */
#define DHT11_IN            HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

/* �ⲿ�ӿں��� */
uint8_t dht11_init(void);                                   /* ��ʼ��dht11 */
uint8_t dht11_check(void);                                  /* ���dht11�Ƿ���� */
uint8_t dht11_read_data(uint8_t *temp, uint8_t *humi);      /* ��ȡ��ʪ�� */

#endif





