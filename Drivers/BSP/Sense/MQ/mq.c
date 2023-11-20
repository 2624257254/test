/**
 ****************************************************************************************************
 * @file        mq9.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-15
 * @brief       ���������ݲɼ�
 ****************************************************************************************************
 */
 
#include "./BSP/Sense/MQ/mq.h"
#include "./SYSTEM/delay/delay.h"


/***************************************************************************************************/

DMA_HandleTypeDef g_dma_nch_adc_handle = {0};                               /* ����Ҫ����ADC��ͨ�����ݵ�DMA��� */
ADC_HandleTypeDef g_adc_nch_dma_handle = {0};                               /* ����ADC����ͨ��DMA��ȡ����� */
uint8_t g_adc_dma_sta = 0;                                                  /* DMA����״̬��־, 0,δ���; 1, ����� */

/**
 * @brief       ADC Nͨ��(2ͨ��) DMA��ȡ ��ʼ������
 *   @note      ����������ʹ��adc_init��ADC���д󲿷�����,�в���ĵط��ٵ�������
 *              ����,���ڱ������õ���6��ͨ��, �궨���Ƚ϶�����, ���,�������Ͳ����ú궨��ķ�ʽ���޸�ͨ����,
 *              ֱ���ڱ����������޸�, ��������Ĭ��ʹ��PA0~PA5��6��ͨ��.
 *
 *              ע��: ����������ʹ�� ADC_ADCX(Ĭ��=ADC1) �� ADC_ADCX_DMACx( DMA1_Channel1 ) ������ض���
 *              ��Ҫ���޸�adc.h�����������������, ����������ԭ���Ļ����Ͻ����޸�, ������ܵ����޷�����ʹ��.
 *
 * @param       mar         : �洢����ַ 
 * @retval      ��
 */
void adc_nch_dma_init(uint32_t mar)
{
    GPIO_InitTypeDef gpio_init_struct;
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
    ADC_ChannelConfTypeDef adc_ch_conf = {0};

    ADC_ADCX_CHY_CLK_ENABLE();                                                /* ʹ��ADCxʱ�� */
    __HAL_RCC_GPIOA_CLK_ENABLE();                                             /* ����GPIOAʱ�� */

    if ((uint32_t)ADC_ADCX_DMACx > (uint32_t)DMA1_Channel7)                   /* ����DMA1_Channel7, ��ΪDMA2��ͨ���� */
    {
        __HAL_RCC_DMA2_CLK_ENABLE();                                          /* DMA2ʱ��ʹ�� */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();                                          /* DMA1ʱ��ʹ�� */
    }

    /* ����ADCʱ�� */
    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;                    /* ADC����ʱ�� */
    adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;                       /* ��Ƶ����6ʱ��Ϊ72M/6=12MHz */
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);                                 /* ����ADCʱ�� */

    /* 
        ����ADC1ͨ��0~5��Ӧ��IO��ģ������
        AD�ɼ�����ģʽ����,ģ������
        PA0��Ӧ ADC1_IN0
        PA1��Ӧ ADC1_IN1
        PA2��Ӧ ADC1_IN2
        PA3��Ӧ ADC1_IN3
        PA4��Ӧ ADC1_IN4
        PA5��Ӧ ADC1_IN5
    */
    gpio_init_struct.Pin = MQ9_ADC_GPIO_PIN|MQ135_ADC_GPIO_PIN;                              /* GPIOA4��5 */
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;                                 /* ģ�� */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    /* ��ʼ��DMA */
    g_dma_nch_adc_handle.Instance = ADC_ADCX_DMACx;                           /* ����DMAͨ�� */
    g_dma_nch_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;               /* �����赽�洢��ģʽ */
    g_dma_nch_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;                   /* ���������ģʽ */
    g_dma_nch_adc_handle.Init.MemInc = DMA_MINC_ENABLE;                       /* �洢������ģʽ */
    g_dma_nch_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;  /* �������ݳ���:16λ */
    g_dma_nch_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;     /* �洢�����ݳ���:16λ */
    g_dma_nch_adc_handle.Init.Mode = DMA_NORMAL;                              /* ��������ģʽ */
    g_dma_nch_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                 /* �е����ȼ� */
    HAL_DMA_Init(&g_dma_nch_adc_handle);

    __HAL_LINKDMA(&g_adc_nch_dma_handle, DMA_Handle, g_dma_nch_adc_handle);   /* ��DMA��adc��ϵ���� */

    /* ��ʼ��ADC */
    g_adc_nch_dma_handle.Instance = ADC_ADCX;                                 /* ѡ���ĸ�ADC */
    g_adc_nch_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                /* ���ݶ��뷽ʽ���Ҷ��� */
    g_adc_nch_dma_handle.Init.ScanConvMode = ADC_SCAN_ENABLE;                 /* ʹ��ɨ��ģʽ */
    g_adc_nch_dma_handle.Init.ContinuousConvMode = ENABLE;                    /* ʹ������ת�� */
    g_adc_nch_dma_handle.Init.NbrOfConversion = 2;                            /* ��ֵ��Χ��1~16����ʵ���õ�6������ͨ������ */
    g_adc_nch_dma_handle.Init.DiscontinuousConvMode = DISABLE;                /* ��ֹ����ͨ������ģʽ */
    g_adc_nch_dma_handle.Init.NbrOfDiscConversion = 0;                        /* ���ü��ģʽ�Ĺ���ͨ����������ֹ����ͨ������ģʽ�󣬴˲������� */
    g_adc_nch_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;          /* �������� */
    HAL_ADC_Init(&g_adc_nch_dma_handle);                                      /* ��ʼ�� */

    HAL_ADCEx_Calibration_Start(&g_adc_nch_dma_handle);                       /* У׼ADC */

    /* ����ADCͨ�� */
    adc_ch_conf.Channel = ADC_CHANNEL_4;                                      /* ����ʹ�õ�ADCͨ�� */
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;                                    /* ����������ĵ�5�� */
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);               /* ����ADCͨ�� */

    adc_ch_conf.Channel = ADC_CHANNEL_5;                                      /* ����ʹ�õ�ADCͨ�� */
    adc_ch_conf.Rank = ADC_REGULAR_RANK_2;                                    /* ����������ĵ�6�� */
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);               /* ����ADCͨ�� */

    /* ����DMA�����������ж����ȼ� */
    HAL_NVIC_SetPriority(ADC_ADCX_DMACx_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(ADC_ADCX_DMACx_IRQn);

    HAL_DMA_Start_IT(&g_dma_nch_adc_handle, (uint32_t)&ADC1->DR, mar, 0);     /* ����DMA���������ж� */
    HAL_ADC_Start_DMA(&g_adc_nch_dma_handle, &mar, 0);                        /* ����ADC��ͨ��DMA������ */
}

/********************************************************/

/**
 * @brief       ʹ��һ��ADC DMA����
 *   @note      �ú����üĴ�������������ֹ��HAL������������������޸�,ҲΪ�˼�����
 * @param       ndtr: DMA����Ĵ���
 * @retval      ��
 */
void adc_dma_enable(uint16_t cndtr)
{
    ADC_ADCX->CR2 &= ~(1 << 0);                 /* �ȹر�ADC */

    ADC_ADCX_DMACx->CCR &= ~(1 << 0);           /* �ر�DMA���� */
    while (ADC_ADCX_DMACx->CCR & (1 << 0));     /* ȷ��DMA���Ա����� */
    ADC_ADCX_DMACx->CNDTR = cndtr;              /* DMA���������� */
    ADC_ADCX_DMACx->CCR |= 1 << 0;              /* ����DMA���� */

    ADC_ADCX->CR2 |= 1 << 0;                    /* ��������ADC */
    ADC_ADCX->CR2 |= 1 << 22;                   /* ��������ת��ͨ�� */
}

/**
 * @brief       ADC DMA�ɼ��жϷ�����
 * @param       �� 
 * @retval      ��
 */
void ADC_ADCX_DMACx_IRQHandler(void)
{
    if (ADC_ADCX_DMACx_IS_TC())
    {
        g_adc_dma_sta = 1;                      /* ���DMA������� */
        ADC_ADCX_DMACx_CLR_TC();                /* ���DMA1 ������7 ��������ж� */
    }
}