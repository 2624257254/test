/**
 ****************************************************************************************************
 * @file        mq9.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-15
 * @brief       ���������ݲɼ�
 ****************************************************************************************************
 */

#ifndef _MQ_H
#define _MQ_H

#include "./SYSTEM/sys/sys.h"

#define ADC_DMA_BUF_SIZE        50 * 2      /* ADC DMA�ɼ� BUF��С, Ӧ����ADCͨ������������ */

extern uint8_t g_adc_dma_sta;               /* DMA����״̬��־, 0,δ���; 1, ����� */

/******************************************************************************************/
/* ADC������ ���� */

#define MQ_ADC_GPIO_PORT                    GPIOA
#define MQ9_ADC_GPIO_PIN                    GPIO_PIN_4
#define MQ135_ADC_GPIO_PIN                  GPIO_PIN_5
#define MQ_ADC_GPIO_CLK_ENABLE()            do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)  /* PA��ʱ��ʹ�� */

#define ADC_ADCX                            ADC1 
#define ADC_ADCX_CHY                        ADC_CHANNEL_1                                /* ͨ��Y,  0 <= Y <= 17 */ 
#define ADC_ADCX_CHY_CLK_ENABLE()           do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)   /* ADC1 ʱ��ʹ�� */


/* ADC��ͨ��/��ͨ�� DMA�ɼ� DMA��ͨ�� ����
 * ע��: ADC1��DMAͨ��ֻ����: DMA1_Channel1, ���ֻҪ��ADC1, �����ǲ��ܸĶ���
 *       ADC2��֧��DMA�ɼ�
 *       ADC3��DMAͨ��ֻ����: DMA2_Channel5, ������ʹ�� ADC3 ����Ҫ�޸�
 */
#define ADC_ADCX_DMACx                      DMA1_Channel1
#define ADC_ADCX_DMACx_IRQn                 DMA1_Channel1_IRQn
#define ADC_ADCX_DMACx_IRQHandler           DMA1_Channel1_IRQHandler

#define ADC_ADCX_DMACx_IS_TC()              ( DMA1->ISR & (1 << 1) )    /* �ж� DMA1_Channel1 ������ɱ�־, ����һ���ٺ�����ʽ,
                                                                         * ���ܵ�����ʹ��, ֻ������if��������� 
                                                                         */
#define ADC_ADCX_DMACx_CLR_TC()             do{ DMA1->IFCR |= 1 << 1; }while(0) /* ��� DMA1_Channel1 ������ɱ�־ */

/******************************************************************************************/
void adc_dma_enable( uint16_t cndtr);                               /* ʹ��һ��ADC DMA�ɼ����� */

void adc_nch_dma_init(uint32_t mar);                                /* ADC��ͨ�� DMA�ɼ���ʼ�� */

#endif

