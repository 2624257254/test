/**
 ******************************************************************************
 * @file     main.c
 * @author   gyd
 * @version  V1.0
 * @date     2020-08-20
 * @brief    智能检测系统
 ******************************************************************************
 */

#include <time.h>
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/Motor/motor.h"
#include "./BSP/Fan/fan.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/LED/led.h"
#include "./BSP/Sense/dht11/dht11.h"
#include "./BSP/Sense/MQ/MQ.h"
#include "./BSP/Wifi/wifi.h"

uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];           /* ADC DMA BUF */
uint8_t temperature;                                /* DHT11温度 */
uint8_t humidity;                                   /* DHT11湿度 */
uint8_t g_dht11_sta = 0;                            /* DHT11状态标志位 0正常 1异常*/
uint16_t mq135_ppm = 0;                             /* 环境污染指数 */
uint16_t mq9_ppm = 0;                               /* 可燃气体浓度 */
uint8_t alarm = 0;
uint8_t window = 0;
uint8_t heating = 0;
uint8_t bluefan = 0;
uint8_t redfan = 0; 
uint8_t MANAL_MODE = 0; 

void data_get(void);
void send_data(void);
void sub_recv(void);
void judge(void);
void motor_judge(void);

int main(void)
{
    uint16_t timer = 0;
    
    //初始化
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟, 72Mhz */
    usart_init(115200);                         /* 串口初始化为115200（LCD屏要用） */
    delay_init(72);                             /* 延时初始化 */
    lcd_init();                                 /* 初始化LCD */
    led_init();                                 /* 初始化led */
    
    /* DHT11初始化 */
    if (dht11_init())                        
    {
        lcd_show_string(20, 10, 200, 12, 12, "DHT11 Error", RED);
        g_dht11_sta = 1;
    }
    else
    {
        lcd_show_string(20, 10, 240, 12, 12, "DHT11 OK", RED);
        lcd_show_string(20, 25, 240, 12, 12, "Temp:  C", BLUE);
        lcd_show_string(20, 40, 240, 12, 12, "Humi:  %", BLUE);
    }
    
    /* 初始化ADC DMA采集 */
    adc_nch_dma_init((uint32_t)&g_adc_dma_buf); 
    
    lcd_show_string(20, 55, 240, 12, 12, "MQ9", RED);
    lcd_show_string(20, 70, 240, 12, 12, "MQ9_VAL:000", BLUE);
    lcd_show_string(20, 85, 240, 12, 12, "MQ9_VOL:0.000V", BLUE);
    
    lcd_show_string(20, 100, 240, 12, 12, "MQ135", RED);
    lcd_show_string(20, 115, 240, 12, 12, "MQ135_VAL:000", BLUE);
    lcd_show_string(20, 130, 240, 12, 12, "MQ135_VOL:0.000V", BLUE);
    
    fan_init();                                 /* 风扇初始化 */
    
    motor_init();                               /* motor初始化 */
    
    //lcd显示-风扇
    lcd_show_string(20, 145, 240, 12, 12, "FAN", RED);
    lcd_show_string(20, 160, 240, 12, 12, "BLUEFAN:OFF", BLUE);
    lcd_show_string(20, 175, 240, 12, 12, "REDFAN:OFF", BLUE);
    
    //lcd显示-步进电机
    lcd_show_string(20, 190, 240, 12, 12, "STEP_NOTOR", RED);
    lcd_show_string(20, 205, 240, 12, 12, "FOREWARD:OFF", BLUE);
    lcd_show_string(20, 220, 240, 12, 12, "REVERSAL:OFF", BLUE);
    
    //lcd显示-步进电机
    lcd_show_string(20, 235, 240, 12, 12, "HEATING_SYS", RED);
    lcd_show_string(20, 250, 240, 12, 12, "HEATING_STA:OFF", BLUE);
    
    /* esp8266初始化 */
    esp8266_start();
    lcd_show_string(20, 265, 240, 12, 12, "WIFI OK", RED);
    
    /* esp连接阿里云 */
    esp8266_link_aliyun();
    lcd_show_string(20, 280, 240, 12, 12, "ALY LINKED", RED);
    lcd_show_string(20, 295, 240, 12, 12, "MODE:AUTO  ", BLUE);
    
    adc_dma_enable(ADC_DMA_BUF_SIZE);   /* 启动ADC DMA采集 */
    
    while(1)
    {
        if (g_usart_rx_recv_sta & 0X8000)
        {
            sub_recv();
            send_data();
        }
        
        if (timer%1000 == 0)
        {
            data_get();
        }
        if (timer%3000 == 0)
        {
            send_data();
        }
        if(!MANAL_MODE)
        {
            
            if (timer%2000 == 0)
            {
                judge();
            }
        }
        if(timer >= 100000000)
        {
            timer = 0;
        }
        timer++;
        delay_ms(10);
    }
}

void sub_recv(void)
{
    cJSON* root;
    cJSON *value;
    char *buf;
    
    
    /* 解析json字符串，找出items */
    root = extractJsonString_b(extractJsonString_a((const char*)g_usart_rx_buf),"items");
    value = root->child->child;
    buf = cJSON_PrintUnformatted(root);
    if (strstr((const char*)buf, "BFS") != NULL)
    {
        bluefan = value->valuedouble;
        
        /* 执行操作 */
        if (bluefan)
        {
            BLUEFAN(1);
            lcd_show_string(20, 160, 240, 12, 12, "BLUEFAN:ON ", BLUE);
        }
        else
        {
            BLUEFAN(0);
            lcd_show_string(20, 160, 240, 12, 12, "BLUEFAN:OFF", BLUE);
        }
    }
    if (strstr((const char*)buf, "RFS") != NULL)
    {
        redfan = value->valuedouble;
        
        /* 执行操作 */
        if (redfan)
        {
            REDFAN(1);
            lcd_show_string(20, 175, 240, 12, 12, "REDFAN:ON ", BLUE);
        }
        else
        {
            REDFAN(0);
            lcd_show_string(20, 175, 240, 12, 12, "REDFAN:OFF", BLUE);
        }
    }
    if (strstr((const char*)buf, "HS") != NULL)
    {
        heating = value->valuedouble;
        
        /* 执行操作 */
        if (heating)
        {
            HEATING(0);
            lcd_show_string(20, 250, 240, 12, 12, "HEATING_STA:ON ", BLUE);
        }
        else
        {
            HEATING(1);
            lcd_show_string(20, 250, 240, 12, 12, "HEATING_STA:OFF", BLUE);
        }
    }
    if (strstr((const char*)buf, "WS") != NULL)
    {
        window = value->valuedouble;
        
        motor_judge();
    }
    if (strstr((const char*)buf, "MM") != NULL)
    {
        MANAL_MODE = value->valuedouble;
        if (MANAL_MODE)
        {
            lcd_show_string(20, 295, 240, 12, 12, "MODE:HANDLE", BLUE);
        }
        else 
        {
            lcd_show_string(20, 295, 240, 12, 12, "MODE:AUTO  ", BLUE);
        }
    }
    free(buf);
    cJSON_Delete(root);
    memset(g_usart_rx_buf,0,USART_REC_LEN-1);
    g_usart_rx_recv_sta = 0;
}

void motor_judge(void)
{
    uint32_t i;
    if(window == g_window)
    {
        return ;
    }
    else
    {
        lcd_show_string(20 + 9 * 6, 220, 240, 12, 12, "ON ", BLUE);
        for(i = 0; i < 1000;i++)
        {
            if(window)
            {
                motor_start_four_bat();
            }
            else
            {
                motor_start_four_bat_toggle();
            }
            
        }
        lcd_show_string(20 + 9 * 6, 220, 240, 12, 12, "OFF", BLUE);
        g_window = window;
    }
}

void send_data(void)
{
    cJSON* root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "AS",     alarm);
    cJSON_AddNumberToObject(root, "WS",     window);
    cJSON_AddNumberToObject(root, "HS",     heating);
    cJSON_AddNumberToObject(root, "RFS",    redfan);
    cJSON_AddNumberToObject(root, "BFS",    bluefan);
    cJSON_AddNumberToObject(root, "MQ9P",   mq9_ppm);
    cJSON_AddNumberToObject(root, "MQ135P", mq135_ppm);
    cJSON_AddNumberToObject(root, "DH",     humidity);
    cJSON_AddNumberToObject(root, "DT",     temperature);
    cJSON_AddNumberToObject(root, "MM",     MANAL_MODE);
    char *add_char_json = add_escape_char(cJSON_PrintUnformatted(root));
    cJSON_Delete(root);
    esp_send_sta_al(add_char_json);
    
    free(add_char_json);
    
}

void judge(void)
{
    if (temperature < 25)
    {
        /* 开启加热系统,关闭风扇 */
        BLUEFAN(0);
        lcd_show_string(20, 160, 240, 12, 12, "BLUEFAN:OFF", BLUE);
        HEATING(1);
        lcd_show_string(20, 205, 240, 12, 12, "HEATING_STA:ON ", BLUE);
        bluefan = 0;
        heating = 1;
    }
    else if (temperature > 30)
    {
        /* 关闭加热系统，开启风扇 */
        BLUEFAN(1);
        lcd_show_string(20, 160, 240, 12, 12, "BLUEFAN:ON ", BLUE);
        HEATING(0);
        lcd_show_string(20, 205, 240, 12, 12, "HEATING_STA:OFF", BLUE);
        heating = 0;
        bluefan = 1;
    }
    else if (temperature >= 27 && temperature <= 28)
    {
        
        BLUEFAN(0);
        lcd_show_string(20, 160, 240, 12, 12, "BLUEFAN:OFF", BLUE);
        bluefan = 0;
    }
    
    if (mq135_ppm >= 3000)
    {
        /* 开始通风,开启窗户 */
        REDFAN(1);
        lcd_show_string(20, 175, 240, 12, 12, "REDFAN:ON ", BLUE);
        redfan = 1;
        window = 1;
        motor_judge();
    }
    else 
    {
        REDFAN(0);
        lcd_show_string(20, 175, 240, 12, 12, "REDFAN:OFF", BLUE);
        redfan = 0;
        window = 0;
        motor_judge();
    }
    
    if (mq9_ppm >= 2000)
    {
        /* 可燃气体预警 */
        alarm = 1;
    }
    else
    {
        alarm = 0;
    }
}
void data_get(void)
{
    uint16_t i,j;
    uint16_t adcx;
    uint32_t sum;
    float temp;
    
    /* DHT11*/ 
    lcd_show_string(20, 10, 240, 12, 12, "DHT11 OK", RED);

    dht11_read_data(&temperature, &humidity);                   /* 读取温湿度值 */
    
    lcd_show_num(20 + 30, 25, temperature, 2, 12, BLUE);        /* 显示温度 */
    lcd_show_num(20 + 30, 40, humidity, 2, 12, BLUE);           /* 显示湿度 */
    lcd_show_string(20, 10, 240, 12, 12, "DHT11 OK", RED);
    
    /* MQ */
    if (g_adc_dma_sta == 1)
    {
        lcd_show_string(20, 55, 240, 12, 12, "MQ9", RED);
        lcd_show_string(20, 100, 240, 12, 12, "MQ135", RED);
        /* 循环显示通道0~通道1的结果 */
        for(j = 0; j < 2; j++)                                                              /* 遍历2个通道 */
        {
            sum = 0;                                                                        /* 清零 */
            for (i = 0; i < ADC_DMA_BUF_SIZE / 2; i++)                                      /* 每个通道采集了10次数据,进行10次累加 */
            {
                sum += g_adc_dma_buf[(2 * i) + j];                                          /* 相同通道的转换数据累加 */
            }
            adcx = sum / (ADC_DMA_BUF_SIZE / 2);                                            /* 取平均值 */
            
            /* 显示结果 */
            lcd_show_xnum(20 + 48 + (j * 12), 70 + (j * 45), adcx, 4, 12, 0, BLUE);         /* 显示ADCC采样后的原始值 */

            temp = (float)adcx * (3.3 / 4096);                                              /* 获取计算后的带小数的实际电压值，比如3.1111 */
            adcx = temp;                                                                    /* 赋值整数部分给adcx变量，因为adcx为u16整形 */
            lcd_show_xnum(20 + 48 + (j * 12), 85 + (j * 45), adcx, 1, 12, 0, BLUE);         /* 显示电压值的整数部分，3.1111的话，这里就是显示3 */

            temp -= adcx;                                                                   /* 把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111 */
            temp *= 1000;                                                                   /* 小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。 */
            lcd_show_xnum(20 + 60 + (j * 12), 85 + (j * 45), temp, 3, 12, 0X80, BLUE);      /* 显示小数部分（前面转换为了整形显示），这里显示的就是111. */
            
            if (j)
            {
                mq135_ppm = ((temp * 3300) / 4095);
            }
            else 
            {
                mq9_ppm = ((temp * 3300) / 4095);
            }
        }
        g_adc_dma_sta = 0;                                                                  /* 清除DMA采集完成状态标志 */
        adc_dma_enable(ADC_DMA_BUF_SIZE);                                                   /* 启动下一次ADC DMA采集 */
    }
    lcd_show_string(20, 55, 240, 12, 12, "MQ9", RED);
    lcd_show_string(20, 100, 240, 12, 12, "MQ135", RED);
    
}















