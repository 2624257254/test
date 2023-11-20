/**
 ****************************************************************************************************
 * @file        wifi.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-13
 * @brief       WiFi模块驱动代码
 ****************************************************************************************************
 */
#ifndef _WIFI_H
#define _WIFI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LCD/lcd.h"
#include "./cJSON/cJSON.h"


/******************************************************************************************/
/* 接口定义 */

void esp8266_start(void);                                                                           /* esp8266初始化 */
uint8_t esp8266_send_AT_cmd_and_check(uint8_t *command, uint8_t *ack, uint16_t waittime);           /* 发送数据和检查接收 */
uint8_t esp8266_send_tcp_msg_and_ceeck(uint8_t *tcp_msg, uint8_t *buf, uint16_t waittime);          /* 发送请求信息 */
void esp8266_get_time(void);                                                                        /* 获取时间 */
void esp8266_get_weather(void);                                                                     /* 获取心知天气 */
void esp8266_link_aliyun(void);                                                                     /* 连接阿里云 */
void esp8266_unlink_mqtt(void);                                                                     /* 断开mqtt连接 */
uint8_t esp_send_sta_al(char *json_string);                                                         /* 向阿里云发送设备信息 */

char* add_escape_char(const char* str);                                                             /* 在','前插入"\\" */
char* extractJsonString_a(const char* inputString);
cJSON* extractJsonString_b(const char* json, const char* field);                                       /* 解析python字符串 */
void delete_and_concatenate(char *str, uint8_t start, uint8_t end);                                 /* 删除指定字符串 */
uint8_t weekday_to_int(char *weekday);
uint8_t month_to_int(char *month);
char* escape_quotes(char* str);

extern RTC_TimeTypeDef esp_time;
extern RTC_DateTypeDef esp_data;

#endif
