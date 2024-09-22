#ifndef _ESP_8266_H
#define _ESP_8266_H


#include "main.h"
#include <stdio.h>

#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־


#define LENGTH 1024
 
void ESP8266_Init(char * ssid,char * pwd);//��ʼ������
uint8_t sendStringTo8266(uint8_t *str);//��8266�����ַ���
void recStringBy8266(uint16_t Size);//��������8266���ַ���
void linkWifi(char *ssid,char *pwd);//����wifi��ssid wifi�����֣�pwd��wifi����
void linkTCP(char *addr,char *port);//����TCP���ӣ�addr��IP��ַ port���˿�
void StartCipsend(char *addr,char *port);//��ʼ͸������
uint8_t sendTo8266(uint8_t *str);

void link();

#endif
