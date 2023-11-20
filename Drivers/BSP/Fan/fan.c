/**
 ****************************************************************************************************
 * @file        fan.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-14
 * @brief       风扇驱动代码
 ****************************************************************************************************
 */
 
#include "./BSP/Fan/fan.h"

/* 风扇工作标志 */
uint8_t g_bluefan_sta = 0;
uint8_t g_redfan_sta = 0;

/**
* @brief       风扇初始化程序
* @param       无
* @retval      无
*/
void fan_init(void)
{
    GPIO_InitTypeDef fan;
    
    FAN_GPIO_CLK_ENABLE();                  /* 风扇时钟使能 */
    
    fan.Pin=BLUEFAN_GPIO_PIN;               /* 蓝色风扇引脚 */
    fan.Mode=GPIO_MODE_OUTPUT_PP;               /* 推挽输出 */
    fan.Speed=GPIO_SPEED_FREQ_HIGH;         /* 高速模式 */
    HAL_GPIO_Init(FAN_GPIO_PORT,&fan);      /* 初始化蓝色风扇引脚 */
    
    fan.Pin=REDFAN_GPIO_PIN;                /* 红色风扇引脚 */
    HAL_GPIO_Init(FAN_GPIO_PORT,&fan);      /* 初始化红色风扇引脚 */
    
    BLUEFAN(0);
    REDFAN(0);
}

