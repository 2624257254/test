/**
 ****************************************************************************************************
 * @fileg_stepmotor_reversal_sta        motor.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-13
 * @brief       电机驱动代码
 ****************************************************************************************************
 */
 
#include "./BSP/Motor/motor.h"

#define MOTOR_DLTIME    3                               /* 电机延时 */
uint8_t g_window = 0;

/**
 * @brief       初始化MOTOR相关IO口, 并使能时钟
 * @param       无
 * @retval      无
 */
void motor_init(void)
{
    MOTOR_GPIO_CLK_ENABLE();
    
    GPIO_InitTypeDef gpio_init_struct;                  /* 端口结构体 */
    
    gpio_init_struct.Pin = MOTORIN1_GPIO_PIN;           /* IN1引脚*/
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* 推挽输出 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* 高速模式 */
    HAL_GPIO_Init(MOTOR_GPIO_PORT, &gpio_init_struct);  /* 初始化IN1引脚 */
    
    gpio_init_struct.Pin = MOTORIN2_GPIO_PIN;
    HAL_GPIO_Init(MOTOR_GPIO_PORT, &gpio_init_struct);  /* 初始化IN2引脚 */
    
    gpio_init_struct.Pin = MOTORIN3_GPIO_PIN;
    HAL_GPIO_Init(MOTOR_GPIO_PORT, &gpio_init_struct);  /* 初始化IN3引脚 */
    
    gpio_init_struct.Pin = MOTORIN4_GPIO_PIN;
    HAL_GPIO_Init(MOTOR_GPIO_PORT, &gpio_init_struct);  /* 初始化IN4引脚 */
    
    MOTORIN1(0);
    MOTORIN2(0);
    MOTORIN3(0);
    MOTORIN4(0);
}

/**
 * @brief       电机4拍正转
 * @param       无
 * @retval      无
 */
void motor_start_four_bat(void)
{
    MOTORIN1(1); 
    MOTORIN4(0);
    delay_ms(MOTOR_DLTIME);

    MOTORIN1(0);
    MOTORIN2(1);
    delay_ms(MOTOR_DLTIME);

    MOTORIN2(0);
    MOTORIN3(1);
    delay_ms(MOTOR_DLTIME);

    MOTORIN3(0);
    MOTORIN4(1);
    delay_ms(MOTOR_DLTIME);

}

/**
 * @brief       电机4拍反转
 * @param       无
 * @retval      无
 */
void motor_start_four_bat_toggle(void)
{
    MOTORIN1(0);
    MOTORIN4(1);
    delay_ms(MOTOR_DLTIME);
    
    MOTORIN4(0);
    MOTORIN3(1);
    delay_ms(MOTOR_DLTIME);
    
    MOTORIN3(0);
    MOTORIN2(1);
    delay_ms(MOTOR_DLTIME);
    
    MOTORIN2(0);
    MOTORIN1(1);
    delay_ms(MOTOR_DLTIME);
}

/**
 * @brief       电机停转
 * @param       无
 * @retval      无
 */
void motor_stop(void)
{
    MOTORIN1(0);
    MOTORIN2(0);
    MOTORIN3(0);
    MOTORIN4(0);
}








