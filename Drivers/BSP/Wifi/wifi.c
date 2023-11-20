/**
 ****************************************************************************************************
 * @file        wifi.c
 * @author      gyd
 * @version     V1.0
 * @date        2023-09-13
 * @brief       WiFi模块驱动代码
 ****************************************************************************************************
 */
 
#include "./BSP/Wifi/wifi.h"

/* 0正常，1异常 */
uint16_t g_esp_sta = 0;

uint8_t weather[500];
uint8_t esp_get_time[50];
RTC_TimeTypeDef esp_time;
RTC_DateTypeDef esp_data;

/**
 * @brief       esp8266发送设备信息到阿里云
 * @param       json_string：json字符串
 * @retval      0 发送成功，1 发送失败
 */
uint8_t esp_send_sta_al(char *json_string)
{
    json_string = escape_quotes(json_string);
    uint8_t esp_message[512] = "AT+MQTTPUB=0,\"/sys/k0aft1MYYGf/esp8266/thing/event/property/post\",\"{\\\"params\\\":";

    strcat((char*)esp_message,json_string);
    strcat((char*)esp_message,"}\",0,0\r\n");
    free(json_string);
    while(esp8266_send_AT_cmd_and_check(esp_message, "OK", 50));                                                /* 发送信息到阿里云 */
    delay_ms(50);
    
    free(esp_message);
    memset(esp_message,0,511);
    return 0;
}
 
/**
 * @brief       esp8266断开mqtt连接
 * @param       无
 * @retval      无
 */
void esp8266_unlink_mqtt(void)
{
    while(esp8266_send_AT_cmd_and_check("AT+MQTTCLEAN=0\r\n", "OK", 50));                                                /* 断开mqtt连接 */
    delay_ms(50);
}

/**
 * @brief       esp8266连接阿里云
 * @param       无
 * @retval      无
 */
void esp8266_link_aliyun(void)
{
    uint8_t *esp_message;
    
    esp_message = "AT+MQTTUSERCFG=0,1,\"NULL\",\"esp8266&k0aft1MYYGf\",\"486089bae2a9a57fe1b7ee58fb37be64bdc64c43b876a12ba3a2f7678627e1b0\",0,0,\"\"\r\n";
    while(esp8266_send_AT_cmd_and_check(esp_message, "OK", 50));                                                /* 设置阿里云账户密码 */
    delay_ms(50);
    
    esp_message = "AT+MQTTCLIENTID=0,\"k0aft1MYYGf.esp8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1697185150215|\"\r\n";
    while(esp8266_send_AT_cmd_and_check(esp_message, "OK", 50));                                                /* 设置clientId */
    delay_ms(50);
    
    esp_message = "AT+MQTTCONN=0,\"iot-06z00fgvdtixio3.mqtt.iothub.aliyuncs.com\",1883,1\r\n";
    while(esp8266_send_AT_cmd_and_check(esp_message, "OK", 50));                                                /* 连接阿里云 */
    delay_ms(50);
    
    esp_message = "AT+MQTTSUB=0,\"/sys/k0aft1MYYGf/esp8266/thing/service/property/set\",1\r\n";
    while(esp8266_send_AT_cmd_and_check(esp_message, "OK", 50));                                               /* 发送订阅消息 */
    delay_ms(50);
}



/**
 * @brief       esp8266初始化
 * @param       无
 * @retval      无
 */
void esp8266_start(void)
{
    while(esp8266_send_AT_cmd_and_check("AT+RST\r\n", "ready", 500));                                                       /* 重启esp8266 */
    delay_ms(100);
    
    while(esp8266_send_AT_cmd_and_check("ATE0\r\n", "OK", 50));                                                             /* 清除回显 */
    delay_ms(100);
    
    while(esp8266_send_AT_cmd_and_check("AT+CWMODE=1\r\n", "OK", 50));                                                      /* 设置esp8266工作模式为station */
    delay_ms(100);
    
    while(esp8266_send_AT_cmd_and_check("AT+CWJAP=\"UNIONMAN-5633\",\"15623hggd\"\r\n", "WIFI GOT IP", 200));               /* 连接热点 */
    delay_ms(100);
}


/**
 * @brief       向esp8266发送AT指令，并接受返回值
 * @param       command：要发送的命令
                ack：期待应答
                waittime：等待时间
 * @retval      0：发送成功，1：发送失败
 */
uint8_t esp8266_send_AT_cmd_and_check(uint8_t *command, uint8_t *ack, uint16_t waittime)
{
    uint8_t res = 1;
    g_usart_rx_sta = 0;
    memset(g_usart_rx_buf,0,USART_REC_LEN-1);
    printf("%s",command);
    
    if (waittime)
    {
        while (--waittime)
        {
            delay_ms(10);
            if (g_usart_rx_sta & 0X8000)                                                        /* 接收完成 */
            {

                if (strstr((const char*)g_usart_rx_buf, (const char*)ack) != NULL)              /* 比较接收缓冲区与ack */
                {
                    res = 0;
                    break;
                }
                else if (strstr((const char*)g_usart_rx_buf, "busy p...") != NULL)
                {
                    delay_ms(500);
                    printf("%s",command);
                }
                else 
                {
                    g_usart_rx_sta = 0;
                    memset(g_usart_rx_buf,0,USART_REC_LEN-1);
                    res = 1;
                }
            }
        }
        if (waittime == 0) res = 1;
    }
    g_usart_rx_sta = 0;
    memset(g_usart_rx_buf,0,USART_REC_LEN-1);
    return res;
}

/**
 * @brief       传入一个字符串后在每一个‘”’前加上"\\"
 * @param       要添加的字符串
 * @retval      添加完成后的字符串
 */
char* escape_quotes(char* str)
{
    int len = strlen(str);
    char* new_str = malloc(len * 2 + 1);
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] == '\"') {
            new_str[j++] = '\\';
        }
        new_str[j++] = str[i];
    }
    free(str);
    new_str[j] = '\0';
    return new_str;
}

/**
 * @brief       解析json字符串
 * @param       json原始JSON字符串
 * @param       field提取的字段名称
 * @retval      添加完成后的字符串
 */
cJSON* extractJsonString_b(const char* json, const char* field)
{
    cJSON* root = cJSON_Parse(json);
    if (root == NULL) {
        cJSON_Delete(root);
        return NULL;
    }

    cJSON* value = cJSON_GetObjectItemCaseSensitive(root, field);
    if (value == NULL) {
        cJSON_Delete(root);
        return NULL;
    }

    return value;
}

/**
 * @brief       从字符串中截取json字符串
 * @param       字符串
 * @retval      添加完成后的字符串
 */
char* extractJsonString_a(const char* inputString)
{
    const char* start = strchr(inputString, '{');
    const char* end = strrchr(inputString, '}');
    if (start != NULL && end != NULL && end > start) {
        int length = end - start + 1;
        char* jsonString = malloc(length + 1);
        strncpy(jsonString, start, length);
        jsonString[length] = '\0';
        return jsonString;
    }
    return NULL;
}

/**
 * @brief       将字符串内所有','前加上"\\"
 * @param       字符串
 * @retval      添加完成后的字符串
 */
char* add_escape_char(const char* str)
{
    int len = strlen(str);
    char* new_str = (char*)malloc(sizeof(char) * (len * 2 + 1));
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] == ',') {
            new_str[j++] = '\\';
        }
        new_str[j++] = str[i];
    }
    new_str[j] = '\0';
    return new_str;
}



