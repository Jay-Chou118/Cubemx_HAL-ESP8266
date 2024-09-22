#ifndef _ESP_8266_H
#define _ESP_8266_H


#include "main.h"
#include <stdio.h>

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志


#define LENGTH 1024
 
void ESP8266_Init(char * ssid,char * pwd);//初始化函数
uint8_t sendStringTo8266(uint8_t *str);//向8266发送字符串
void recStringBy8266(uint16_t Size);//接收来自8266的字符串
void linkWifi(char *ssid,char *pwd);//连接wifi，ssid wifi：名字，pwd：wifi密码
void linkTCP(char *addr,char *port);//建立TCP连接，addr：IP地址 port：端口
void StartCipsend(char *addr,char *port);//开始透明传输
uint8_t sendTo8266(uint8_t *str);

void link();

#endif
