/**
 ****************************************************************************************************
 * @file        mq9.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-15
 * @brief       传感器数据采集
 ****************************************************************************************************
 */

#ifndef _MQ_H
#define _MQ_H

#include "./SYSTEM/sys/sys.h"

#define ADC_DMA_BUF_SIZE        50 * 2      /* ADC DMA采集 BUF大小, 应等于ADC通道数的整数倍 */

extern uint8_t g_adc_dma_sta;               /* DMA传输状态标志, 0,未完成; 1, 已完成 */

/******************************************************************************************/
/* ADC及引脚 定义 */

#define MQ_ADC_GPIO_PORT                    GPIOA
#define MQ9_ADC_GPIO_PIN                    GPIO_PIN_4
#define MQ135_ADC_GPIO_PIN                  GPIO_PIN_5
#define MQ_ADC_GPIO_CLK_ENABLE()            do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)  /* PA口时钟使能 */

#define ADC_ADCX                            ADC1 
#define ADC_ADCX_CHY                        ADC_CHANNEL_1                                /* 通道Y,  0 <= Y <= 17 */ 
#define ADC_ADCX_CHY_CLK_ENABLE()           do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)   /* ADC1 时钟使能 */


/* ADC单通道/多通道 DMA采集 DMA及通道 定义
 * 注意: ADC1的DMA通道只能是: DMA1_Channel1, 因此只要是ADC1, 这里是不能改动的
 *       ADC2不支持DMA采集
 *       ADC3的DMA通道只能是: DMA2_Channel5, 因此如果使用 ADC3 则需要修改
 */
#define ADC_ADCX_DMACx                      DMA1_Channel1
#define ADC_ADCX_DMACx_IRQn                 DMA1_Channel1_IRQn
#define ADC_ADCX_DMACx_IRQHandler           DMA1_Channel1_IRQHandler

#define ADC_ADCX_DMACx_IS_TC()              ( DMA1->ISR & (1 << 1) )    /* 判断 DMA1_Channel1 传输完成标志, 这是一个假函数形式,
                                                                         * 不能当函数使用, 只能用在if等语句里面 
                                                                         */
#define ADC_ADCX_DMACx_CLR_TC()             do{ DMA1->IFCR |= 1 << 1; }while(0) /* 清除 DMA1_Channel1 传输完成标志 */

/******************************************************************************************/
void adc_dma_enable( uint16_t cndtr);                               /* 使能一次ADC DMA采集传输 */

void adc_nch_dma_init(uint32_t mar);                                /* ADC多通道 DMA采集初始化 */

#endif

