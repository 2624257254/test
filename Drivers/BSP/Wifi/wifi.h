/**
 ****************************************************************************************************
 * @file        wifi.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-13
 * @brief       WiFiģ����������
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
/* �ӿڶ��� */

void esp8266_start(void);                                                                           /* esp8266��ʼ�� */
uint8_t esp8266_send_AT_cmd_and_check(uint8_t *command, uint8_t *ack, uint16_t waittime);           /* �������ݺͼ����� */
uint8_t esp8266_send_tcp_msg_and_ceeck(uint8_t *tcp_msg, uint8_t *buf, uint16_t waittime);          /* ����������Ϣ */
void esp8266_get_time(void);                                                                        /* ��ȡʱ�� */
void esp8266_get_weather(void);                                                                     /* ��ȡ��֪���� */
void esp8266_link_aliyun(void);                                                                     /* ���Ӱ����� */
void esp8266_unlink_mqtt(void);                                                                     /* �Ͽ�mqtt���� */
uint8_t esp_send_sta_al(char *json_string);                                                         /* �����Ʒ����豸��Ϣ */

char* add_escape_char(const char* str);                                                             /* ��','ǰ����"\\" */
char* extractJsonString_a(const char* inputString);
cJSON* extractJsonString_b(const char* json, const char* field);                                       /* ����python�ַ��� */
void delete_and_concatenate(char *str, uint8_t start, uint8_t end);                                 /* ɾ��ָ���ַ��� */
uint8_t weekday_to_int(char *weekday);
uint8_t month_to_int(char *month);
char* escape_quotes(char* str);

extern RTC_TimeTypeDef esp_time;
extern RTC_DateTypeDef esp_data;

#endif
