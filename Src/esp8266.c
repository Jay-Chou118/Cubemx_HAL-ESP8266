#include "esp8266.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "oled.h"

#define RXBUFFERSIZE 256


#define ESP8266_WIFI_INFO "AT+CWJAP=\"Xiaomi Pad 5 Pro\",\"11111111\"\r\n"

char RxBuffer[RXBUFFERSIZE];   //��������
uint8_t aRxBuffer;                        //�����жϻ���/
uint8_t Uart1_Rx_Cnt = 0;                //���ջ������
uint8_t Uart1_Rx_preCnt = 0;
char wifi[];

uint8_t rst[] = "AT+RST\r\n";
uint8_t cmd[] = "AT+CWMODE=1\r\n";

/*8266��ʼ��*/
//void ESP8266_Init(UART_HandleTypeDef *uart){
//	uart8266=uart;
//	HAL_Delay(2000);//8266�ϵ��Ҫ�ȴ�һ��ʱ�������������
//	HAL_UART_Transmit(uart8266,"AT+RESTORE\r\n",12,0xff);//���ͻָ����������õ�ָ��
//	HAL_Delay(2000);//�ȴ��ָ������������
//	HAL_UARTEx_ReceiveToIdle_DMA(uart8266,ESP8266Rxbuf,sizeof(ESP8266Rxbuf));//��������8266����Ϣ
//	
//}
/*���ATָ���Ƿ�Ϲ�*/
/*0 ���Ϲ� 1 �Ϲ�*/
//uint8_t checkAT(uint8_t *str,int len){
//	int i=0;
//	for(i=0;i<len;i++){
//		if(*(str+i)=='\r'||*(str+i)=='\n'){
//			if(i<len-2){
//				return 0;
//			}
//			else if(*(str+i)=='\r'&&*(str+i+1)=='\n'){
//				return 1;
//			}
//		}
//	}
//	return 0;
//}
///*����ATָ��*/
///*0 ����ʧ�ܣ�1 ���ͳɹ�*/
//uint8_t sendATTo8266(uint8_t *str){
//	int len=strlen((char *)str);
//	if(0==checkAT(str,len)){
//		printf("AT error\n");
//		return 0;
//	}
//	else{
//		printf("AT right\n");
//		if(HAL_OK==HAL_UART_Transmit(uart8266,str,len,0xff)){
//			printf("%.*s send to 8266 ok!\n",len-2,str);
//			return 1;
//		}
//		else{
//			printf("%.*s send to 8266 error!\n",len-2,str);
//			return 0;
//		}
//	}
//}
///*��������8266��AT��Ϣ�ظ�*/
//void recATBy8266(uint16_t Size){
//	HAL_UART_DMAStop(uart8266);//����ͣ����
////		printf("wifibuf:%s\n",WifiRxbuf);
//	int i=0;
//	do{  //�ж���Ϣ����û��OK����ERROR��8266�ظ�����Ϣ�ﶼ�����OK����ERROR
//		if(ESP8266Rxbuf[i]=='O'&&ESP8266Rxbuf[i+1]=='K'){
//			memset(ESP8266Rxbuf,0,sizeof(ESP8266Rxbuf));
//			HAL_Delay(2000);//�յ�OK����ʱ���Ⱥ�������Ϣ�������
//			printf("8266 say OK\r\n");
//			replyFlag=1;//�յ��ظ��ı�־λ��1
////			printf("OK replyFlag:%d\n",replyFlag);
//			HAL_UARTEx_ReceiveToIdle_DMA(uart8266,ESP8266Rxbuf,sizeof(ESP8266Rxbuf));//�����´ν���
//			return;
//			}
//		else if(ESP8266Rxbuf[i]=='E'&&ESP8266Rxbuf[i+1]=='R'&&ESP8266Rxbuf[i+2]=='R'&&ESP8266Rxbuf[i+3]=='O'&&ESP8266Rxbuf[i+4]=='R'){
//			HAL_UART_Transmit(uart8266,"AT\r\n",4,0xff);//���������error����ô��һ�����Ƿ���ʧ�ܣ������ø�ATָ�����ֵ���η���ʧ��
//			HAL_Delay(4000);//��ʱ�ȵ����ɹ�
//			printf("\n8266 say ERROR\r\n");
//			replyFlag=2;//�յ��ظ��ı�־λ��2
////			printf("ERROR replyFlag:%d\n",replyFlag);
//			memset(ESP8266Rxbuf,0,sizeof(ESP8266Rxbuf));
//			HAL_UARTEx_ReceiveToIdle_DMA(uart8266,ESP8266Rxbuf,sizeof(ESP8266Rxbuf));
//			return;
//			}
//		}while(++i<Size);
//	/*8266�ظ�����Ϣ��Щ�Ƿְ����صģ���OK����ERROR�����ں����İ�����*/
//	memset(ESP8266Rxbuf,0,sizeof(ESP8266Rxbuf));//��ս�����һ���ݰ�
//	HAL_UARTEx_ReceiveToIdle_DMA(uart8266,ESP8266Rxbuf,sizeof(ESP8266Rxbuf));//��ʼ������һ���ݰ�
//}
///*��8266�����ַ���*/
///*0������ʧ�ܣ�1�����ͳɹ�*/
//uint8_t sendStringTo8266(uint8_t *str)
//{
//	uint16_t Size = strlen((char *)str);
//	if(CIPMODE==1){//�ж��Ƿ�Ϊ͸������ģʽ���ǵĻ�ֱ�ӽ��з���
//		HAL_UART_Transmit(uart8266,str,Size,0xff);
//		if(!strcmp((char *)str,"+++")){//������͵��ǡ�+++��(����\r\n���ǹر�͸������ģʽ)
//			printf("\nclose CIPSEND\n");
//			CIPMODE=0;
//		}
//		return 1;
//	}
//	/*������ұ�д�ĵ���ģʽ�����÷���8266*/
//	/*����ģʽ�£����൱��PCֱ����8266ͨ��*/
//	else if(!strcmp((char *)str,"AT+TESTMODE\r\n")){
//		if(TESTMODE==0){
//			printf("open testmode\n");
//			TESTMODE=1;
//		}
//		else{
//			printf("close testmode\n");
//			TESTMODE=0;
//		}
//		return 0;
//	}
//	if(HAL_OK==HAL_UART_Transmit(uart8266,str,Size,0xff)){//���ͳɹ�
//		if(!strcmp((char *)str,"AT+CIPSEND\r\n")){//�ж��Ƿ��ǿ���͸������ģʽ��ָ��
//			printf("open CIPSEND\n");
//			uint32_t t=0;
//			while(replyFlag==0){//�ȴ�8266�ظ�
//				HAL_Delay(1000);
//				t++;
//				if(t>40){
//				printf("open CIPSEND error\n");
//				return 0;
//				}
//			}
//			CIPMODE=1;//�յ��ظ��Ͱѱ�־λ��1
//		}
//		return 1;
//	}
//	else{
//		return 0;
//	}
//	
//}
///*��8266������Ϣ����ȷ�ϻظ�*/
///*0������ʧ�ܻ���û�յ��ظ����߻ظ�ERROR*/
///*1�����ͳɹ����յ�OK�ظ�*/
//uint8_t sendTo8266(uint8_t *str){
//	if(sendStringTo8266(str)){
//		uint32_t t=0;
//		

//		while(replyFlag==0){//�ȴ��ظ�
//			HAL_Delay(1000);
//			t++;
//			if(t>40){//��ʱ
//				printf("time out error\n");
//				return 0;
//			}
//		}
//		if(replyFlag==1){//�ж��ǲ���OK�ظ���
//			replyFlag=0;
//			return 1;
//		}
//		else{
//			printf("error\n");
//			replyFlag=0;
//			return 0;
//		}
// 
//	}
//	else{
//		replyFlag=0;
//		return 0;
//	}
//}
///*��������8266����Ϣ*/
//void recStringBy8266(uint16_t Size)
//{
//	if(CIPMODE!=0||TESTMODE!=0){//�����͸������ģʽ���ߵ���ģʽ��ֱ����ʾ
//		printf("%s\n",ESP8266Rxbuf);
//		memset(ESP8266Rxbuf,0,sizeof(ESP8266Rxbuf));
//		HAL_UARTEx_ReceiveToIdle_DMA(uart8266,ESP8266Rxbuf,sizeof(ESP8266Rxbuf));
//	}
//	else{//������Ǵ���AT��Ϣ�ظ�
//		recATBy8266(Size);
//	}
//}
///*����wifi*/
///*����
//1.����ΪSTAģʽ��AT+CWMODE=1\r\n
//2.����8266ģ��ִ�����ã�AT+RST\r\n
//3.����wifi��AT+CWJAP="wifi��","����"\r\n
//*/
//void linkWifi(char *ssid,char *pwd){
//	printf("Start link wifi:\n");
//	printf("Set CWMODE=1\n");
//	if(sendTo8266("AT+CWMODE=1\r\n")){
//		printf("Set CWMODE success!\n");
//	}
//	else{
//		printf("Set CWMODE fail!\n");
//	}
//	printf("8266 RESTART\n");
//	if(sendTo8266("AT+RST\r\n")){
//		printf("8266 RESTART success!\n");
//	}
//	else{
//		printf("8266 RESTART fail!\n");
//	}
//	printf("link wifi:%s %s\n",ssid,pwd);
//	char str[50];//һ��Ҫ��char���͵����飬����sprintf�ᱨ����֪����ô���
//	sprintf(str,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pwd);//��stdio.h����
//	if(sendTo8266(str)){
//		printf("link wifi success!\n");
//	}
//	else{
//		printf("link wifi fail!\n");
//	}
//	
//}
///*����TCP����*/
//void linkTCP(char *addr,char *port){
//	printf("link TCP: %s %s\n",addr,port);
//	char str[50];
//	sprintf(str,"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",addr,port);
//	if(sendTo8266(str)){
//		printf("link tcp success!\n");
//	}
//	else{
//		printf("link tcp fail!\n");
//	}
//}
///*��ʼ͸������*/
///*����
//1.����TCP
//2.����Ϊ͸������ģʽ��AT+CIPMODE=1\r\n
//3.����͸�����䣺AT+CIPSEND\r\n
//*/
//void StartCipsend(char *addr,char *port){
//	linkTCP(addr,port);
//	printf("Set Cipmode=1\n");
//	if(sendTo8266("AT+CIPMODE=1\r\n")){
//		printf("Set Cipmode=1 success!\n");
//	}
//	else{
//		printf("Set Cipmode=1 fail!\n");
//	}
//	printf("Start cipsend\n");
//	if(sendTo8266("AT+CIPSEND\r\n")){
//		printf("Start cipsend success!\n");
//	}
//	else{
//		printf("Start cipsend fail!\n");
//	}
//}


//void Send_AT_Command(UART_HandleTypeDef *huart){
//	char cmd[] ="AT\r\n";
//	if(HAL_UART_Transmit(huart, (uint8_t*)cmd, sizeof(cmd)-1, HAL_MAX_DELAY) != HAL_OK){
//		 Error_Handler();
//	}

//}
//==========================================================
//	�������ƣ�	ESP8266_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Clear(void)
{

	memset(RxBuffer, 0, sizeof(RxBuffer));
	Uart1_Rx_Cnt = 0;

}


//==========================================================
//	�������ƣ�	ESP8266_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(Uart1_Rx_Cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(Uart1_Rx_Cnt == Uart1_Rx_preCnt)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		Uart1_Rx_Cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	Uart1_Rx_preCnt = Uart1_Rx_Cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	uint8_t timeOut = 200;

	//Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd)); //ע�����д���
	HAL_UART_Transmit(&huart1,(uint8_t *)cmd, strlen((uint8_t *)cmd),HAL_MAX_DELAY);
	//OLED_ShowString(0,3,(uint8_t *)cmd,8);
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)RxBuffer, res) != NULL)		//����������ؼ���
			{
				ESP8266_Clear();									//��ջ���
				
				return 0;
			}
		}
		
		HAL_Delay(10);
	}
	
	return 1;

}

//==========================================================
//	�������ƣ�	ESP8266_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
//void ESP8266_SendData(unsigned char *data, unsigned short len)
//{

//	char cmdBuf[32];
//	
//	ESP8266_Clear();								//��ս��ջ���
//	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
//	if(!ESP8266_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
//	{
//		Usart_SendString(USART1, data, len);		//�����豸������������
//	}

//}
void ESP8266_Init(char * ssid,char * pwd)
{
	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);
	ESP8266_Clear();
	
	
	
	//UsartPrintf(USART_DEBUG, "0. AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK"))
		HAL_Delay(1000);
	//OLED_ShowString(0,0,"AT",16);
	
/***************��λESP8266**********************/		
	//UsartPrintf(USART_DEBUG,"1. RST\r\n");
	ESP8266_SendCmd((char *)rst,"");
		HAL_Delay(1000);
	ESP8266_SendCmd("AT+RESTORE\r\n","");
		HAL_Delay(1000);
/*************************************/	
	
	//UsartPrintf(USART_DEBUG, "2. CWMODE\r\n");
	while(ESP8266_SendCmd((char *)cmd, "OK"))
		HAL_Delay(1000);
	//OLED_ShowString(0,0,"MODE",16);
	
	//UsartPrintf(USART_DEBUG, "3. AT+CWDHCP\r\n");
//	while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
//		HAL_Delay(1000);
	
	//UsartPrintf(USART_DEBUG, "4. CWJAP\r\n");
	sprintf(wifi,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pwd);
	while(ESP8266_SendCmd(wifi, "GOT IP"))
		HAL_Delay(2000);
	OLED_ShowString(0,3,"WIFI:",8);
	OLED_ShowString(0,4,(uint8_t *)ssid,8);
	
	//UsartPrintf(USART_DEBUG, "5. CIPSTART\r\n");
//	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
//		HAL_Delay(500);
	//OLED_Display_Off();
//	OLED_Clear();
	OLED_ShowString(0,0,"Finished",8);
	//UsartPrintf(USART_DEBUG, "6. ESP8266 Init OK\r\n");

}


void link()
{
	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);
	uint8_t Senbuff[] = "AT\r\n";  //�������ݷ�������
	uint8_t cmd[] = "AT+CWMODE=1\r\n";
	uint8_t rst[] = "AT+RST\r\n";
	uint8_t wifi[] = "AT+CWJAP=\"Xiaomi Pad 5 Pro\",\"11111111\"\r\n";
//	uint8_t wifi[] = "AT+CWJAP=\"xiaomi\",\"18876322223\"\r\n";
	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)cmd, sizeof(cmd));
	//printf("AT+CWMODE=1\r\n");
	//HAL_Delay(500);
	//printf("Start link wifi:\r\n");
	//printf("connecting ....\r\n");
	HAL_Delay(2000);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)wifi, sizeof(wifi));
	//printf("connecting ...");
	HAL_Delay(2000);


}
 
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file5 p9 }1 L8 U' {) R; `1 o# J
   */
				Uart1_Rx_Cnt++;
        if(Uart1_Rx_Cnt >= 255)  //����ж�
        {
                Uart1_Rx_Cnt = 0;
                memset(RxBuffer,0x00,sizeof(RxBuffer));
                HAL_UART_Transmit(&huart1, (uint8_t *)"�������", 10,0xFFFF);         
					
        }
        else
        {
                RxBuffer[Uart1_Rx_Cnt-1] = aRxBuffer;   //��������ת��
        
//               if((RxBuffer[Uart1_Rx_Cnt-2] == 'K')&&(RxBuffer[Uart1_Rx_Cnt-3] == 'O')) //�жϽ���λ  
//								{				
//												//Uart1_Rx_Cnt = 1;
//												//OLED_ShowString(0,0,(uint8_t *)RxBuffer,8);
//                        //HAL_UART_Transmit(&huart1, (uint8_t *)&RxBuffer, Uart1_Rx_Cnt,0xFFFF); //���յ�����Ϣ���ͳ�ȥ
//												//OLED_Clear();
//												OLED_ShowString(0,0,(uint8_t *)RxBuffer,8);
//												//OLED_ShowNum(0,0,i,3,16);
//										//while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX);//���UART���ͽ���
//                        //Uart1_Rx_Cnt = 0;
                        //memset(RxBuffer,0x00,sizeof(RxBuffer)); //�������
//                }
        }
				
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //�ٿ��������ж�
				//OLED_ShowNum(0,0,Uart1_Rx_Cnt,3,16);
				//OLED_ShowNum(0,0,Uart1_Rx_Cnt,3,16);
				//OLED_Clear();
}
 
 void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_TxCpltCallback can be implemented in the user file.
   */
	
	


}