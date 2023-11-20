/**
 ****************************************************************************************************
 * @file        usart3.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-13
 * @brief       esp8266ʹ�õĴ���3����
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
/* ���� �� ���� ���� */

#define USART3_TX_GPIO_PORT                  GPIOB
#define USART3_TX_GPIO_PIN                   GPIO_PIN_11
#define USART3_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define USART3_RX_GPIO_PORT                  GPIOB
#define USART3_RX_GPIO_PIN                   GPIO_PIN_10
#define USART3_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define USART3_UX                            USART3
#define USART3_UX_IRQn                       USART3_IRQn
#define USART3_UX_IRQHandler                 USART3_IRQHandler
#define USART3_UX_CLK_ENABLE()               do{ __HAL_RCC_USART3_CLK_ENABLE(); }while(0)  /* USART1 ʱ��ʹ�� */

/******************************************************************************************/

#define USART3_REC_LEN               1024         /* �����������ֽ��� 200 */
#define USART3_EN_RX                 1           /* ʹ�ܣ�1��/��ֹ��0������1���� */

extern UART_HandleTypeDef g_uart3_handle;       /* HAL UART��� */

extern uint8_t  g_usart3_rx_buf[USART3_REC_LEN];  /* ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint16_t g_usart3_rx_sta;                 /* ����״̬��� */
extern uint8_t g_3_rx_buffer[RXBUFFERSIZE];       /* HAL��USART����Buffer */


void usart3_init(uint32_t bound);                /* ���ڳ�ʼ������ */

#endif





