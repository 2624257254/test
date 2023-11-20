/**
 ****************************************************************************************************
 * @file        usart3.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-13
 * @brief       esp8266使用的串口3驱动
 ****************************************************************************************************
 */

#ifndef _USART3_H
#define _USART3_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
//#include "timer.h"
/******************************************************************************************/
/* 引脚 和 串口 定义 */

#define USART3_TX_GPIO_PORT                  GPIOB
#define USART3_TX_GPIO_PIN                   GPIO_PIN_11
#define USART3_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART3_RX_GPIO_PORT                  GPIOB
#define USART3_RX_GPIO_PIN                   GPIO_PIN_10
#define USART3_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART3_UX                            USART3
#define USART3_UX_IRQn                       USART3_IRQn
#define USART3_UX_IRQHandler                 USART3_IRQHandler
#define USART3_UX_CLK_ENABLE()               do{ __HAL_RCC_USART3_CLK_ENABLE(); }while(0)  /* USART1 时钟使能 */

/******************************************************************************************/

#define USART3_REC_LEN               1024         /* 定义最大接收字节数 200 */
#define USART3_EN_RX                 1           /* 使能（1）/禁止（0）串口1接收 */

extern UART_HandleTypeDef g_uart3_handle;       /* HAL UART句柄 */

extern uint8_t  g_usart3_rx_buf[USART3_REC_LEN];  /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
extern uint16_t g_usart3_rx_sta;                 /* 接收状态标记 */
extern uint8_t g_3_rx_buffer[RXBUFFERSIZE];       /* HAL库USART接收Buffer */


void usart3_init(uint32_t bound);                /* 串口初始化函数 */

#endif





