#include "esp8266.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "oled.h"

#define RXBUFFERSIZE 256


#define ESP8266_WIFI_INFO "AT+CWJAP=\"Xiaomi Pad 5 Pro\",\"11111111\"\r\n"

char RxBuffer[RXBUFFERSIZE];   //接收数据
uint8_t aRxBuffer;                        //接收中断缓冲/
uint8_t Uart1_Rx_Cnt = 0;                //接收缓冲计数
uint8_t Uart1_Rx_preCnt = 0;
char wifi[];

uint8_t rst[] = "AT+RST\r\n";
uint8_t cmd[] = "AT+CWMODE=1\r\n";

/*8266初始化*/
//void ESP8266_Init(UART_HandleTypeDef *uart){
//	uart8266=uart;
//	HAL_Delay(2000);//8266上电后要等待一段时间才能正常运行
//	HAL_UART_Transmit(uart8266,"AT+RESTORE\r\n",12,0xff);//发送恢复出厂化设置的指令
//	HAL_Delay(2000);//等待恢复出厂设置完成
//	HAL_UARTEx_ReceiveToIdle_DMA(uart8266,ESP8266Rxbuf,sizeof(ESP8266Rxbuf));//开启接收8266的消息
//	
//}
/*检查AT指令是否合规*/
/*0 不合规 1 合规*/
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
///*发送AT指令*/
///*0 发送失败，1 发送成功*/
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
///*接收来自8266的AT消息回复*/
//void recATBy8266(uint16_t Size){
//	HAL_UART_DMAStop(uart8266);//先暂停接收
////		printf("wifibuf:%s\n",WifiRxbuf);
//	int i=0;
//	do{  //判断消息里有没有OK或者ERROR，8266回复的消息里都会带有OK或者ERROR
//		if(ESP8266Rxbuf[i]=='O'&&ESP8266Rxbuf[i+1]=='K'){
//			memset(ESP8266Rxbuf,0,sizeof(ESP8266Rxbuf));
//			HAL_Delay(2000);//收到OK，延时，等后续的消息发送完成
//			printf("8266 say OK\r\n");
//			replyFlag=1;//收到回复的标志位置1
////			printf("OK replyFlag:%d\n",replyFlag);
//			HAL_UARTEx_ReceiveToIdle_DMA(uart8266,ESP8266Rxbuf,sizeof(ESP8266Rxbuf));//开启下次接收
//			return;
//			}
//		else if(ESP8266Rxbuf[i]=='E'&&ESP8266Rxbuf[i+1]=='R'&&ESP8266Rxbuf[i+2]=='R'&&ESP8266Rxbuf[i+3]=='O'&&ESP8266Rxbuf[i+4]=='R'){
//			HAL_UART_Transmit(uart8266,"AT\r\n",4,0xff);//如果返回是error，那么下一条还是发送失败，所以用个AT指令来抵掉这次发送失败
//			HAL_Delay(4000);//延时等抵消成功
//			printf("\n8266 say ERROR\r\n");
//			replyFlag=2;//收到回复的标志位置2
////			printf("ERROR replyFlag:%d\n",replyFlag);
//			memset(ESP8266Rxbuf,0,sizeof(ESP8266Rxbuf));
//			HAL_UARTEx_ReceiveToIdle_DMA(uart8266,ESP8266Rxbuf,sizeof(ESP8266Rxbuf));
//			return;
//			}
//		}while(++i<Size);
//	/*8266回复的消息有些是分包发回的，即OK或者ERROR可能在后续的包里面*/
//	memset(ESP8266Rxbuf,0,sizeof(ESP8266Rxbuf));//清空接收下一数据包
//	HAL_UARTEx_ReceiveToIdle_DMA(uart8266,ESP8266Rxbuf,sizeof(ESP8266Rxbuf));//开始接收下一数据包
//}
///*向8266发送字符串*/
///*0：发送失败，1：发送成功*/
//uint8_t sendStringTo8266(uint8_t *str)
//{
//	uint16_t Size = strlen((char *)str);
//	if(CIPMODE==1){//判断是否为透明传输模式，是的话直接进行发送
//		HAL_UART_Transmit(uart8266,str,Size,0xff);
//		if(!strcmp((char *)str,"+++")){//如果发送的是“+++”(不带\r\n就是关闭透明传输模式)
//			printf("\nclose CIPSEND\n");
//			CIPMODE=0;
//		}
//		return 1;
//	}
//	/*这个是我编写的调试模式，不用发给8266*/
//	/*调试模式下，就相当于PC直接与8266通信*/
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
//	if(HAL_OK==HAL_UART_Transmit(uart8266,str,Size,0xff)){//发送成功
//		if(!strcmp((char *)str,"AT+CIPSEND\r\n")){//判断是否是开启透明传输模式的指令
//			printf("open CIPSEND\n");
//			uint32_t t=0;
//			while(replyFlag==0){//等待8266回复
//				HAL_Delay(1000);
//				t++;
//				if(t>40){
//				printf("open CIPSEND error\n");
//				return 0;
//				}
//			}
//			CIPMODE=1;//收到回复就把标志位置1
//		}
//		return 1;
//	}
//	else{
//		return 0;
//	}
//	
//}
///*向8266发送消息并且确认回复*/
///*0：发送失败或者没收到回复或者回复ERROR*/
///*1：发送成功且收到OK回复*/
//uint8_t sendTo8266(uint8_t *str){
//	if(sendStringTo8266(str)){
//		uint32_t t=0;
//		

//		while(replyFlag==0){//等待回复
//			HAL_Delay(1000);
//			t++;
//			if(t>40){//超时
//				printf("time out error\n");
//				return 0;
//			}
//		}
//		if(replyFlag==1){//判断是不是OK回复。
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
///*处理来自8266的消息*/
//void recStringBy8266(uint16_t Size)
//{
//	if(CIPMODE!=0||TESTMODE!=0){//如果是透明传输模式或者调试模式，直接显示
//		printf("%s\n",ESP8266Rxbuf);
//		memset(ESP8266Rxbuf,0,sizeof(ESP8266Rxbuf));
//		HAL_UARTEx_ReceiveToIdle_DMA(uart8266,ESP8266Rxbuf,sizeof(ESP8266Rxbuf));
//	}
//	else{//否则就是处理AT消息回复
//		recATBy8266(Size);
//	}
//}
///*连接wifi*/
///*过程
//1.设置为STA模式：AT+CWMODE=1\r\n
//2.重启8266模块执行设置：AT+RST\r\n
//3.连接wifi：AT+CWJAP="wifi名","密码"\r\n
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
//	char str[50];//一定要是char类型的数组，否则sprintf会报错，不知道怎么解决
//	sprintf(str,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pwd);//在stdio.h库中
//	if(sendTo8266(str)){
//		printf("link wifi success!\n");
//	}
//	else{
//		printf("link wifi fail!\n");
//	}
//	
//}
///*建立TCP连接*/
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
///*开始透明传输*/
///*过程
//1.连接TCP
//2.设置为透明传输模式：AT+CIPMODE=1\r\n
//3.开启透明传输：AT+CIPSEND\r\n
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
//	函数名称：	ESP8266_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_Clear(void)
{

	memset(RxBuffer, 0, sizeof(RxBuffer));
	Uart1_Rx_Cnt = 0;

}


//==========================================================
//	函数名称：	ESP8266_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(Uart1_Rx_Cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(Uart1_Rx_Cnt == Uart1_Rx_preCnt)				//如果上一次的值和这次相同，则说明接收完毕
	{
		Uart1_Rx_Cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	Uart1_Rx_preCnt = Uart1_Rx_Cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志

}

//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	uint8_t timeOut = 200;

	//Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd)); //注意这行代码
	HAL_UART_Transmit(&huart1,(uint8_t *)cmd, strlen((uint8_t *)cmd),HAL_MAX_DELAY);
	//OLED_ShowString(0,3,(uint8_t *)cmd,8);
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)RxBuffer, res) != NULL)		//如果检索到关键词
			{
				ESP8266_Clear();									//清空缓存
				
				return 0;
			}
		}
		
		HAL_Delay(10);
	}
	
	return 1;

}

//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
//void ESP8266_SendData(unsigned char *data, unsigned short len)
//{

//	char cmdBuf[32];
//	
//	ESP8266_Clear();								//清空接收缓存
//	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
//	if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
//	{
//		Usart_SendString(USART1, data, len);		//发送设备连接请求数据
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
	
/***************复位ESP8266**********************/		
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
	uint8_t Senbuff[] = "AT\r\n";  //定义数据发送数组
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
        if(Uart1_Rx_Cnt >= 255)  //溢出判断
        {
                Uart1_Rx_Cnt = 0;
                memset(RxBuffer,0x00,sizeof(RxBuffer));
                HAL_UART_Transmit(&huart1, (uint8_t *)"数据溢出", 10,0xFFFF);         
					
        }
        else
        {
                RxBuffer[Uart1_Rx_Cnt-1] = aRxBuffer;   //接收数据转存
        
//               if((RxBuffer[Uart1_Rx_Cnt-2] == 'K')&&(RxBuffer[Uart1_Rx_Cnt-3] == 'O')) //判断结束位  
//								{				
//												//Uart1_Rx_Cnt = 1;
//												//OLED_ShowString(0,0,(uint8_t *)RxBuffer,8);
//                        //HAL_UART_Transmit(&huart1, (uint8_t *)&RxBuffer, Uart1_Rx_Cnt,0xFFFF); //将收到的信息发送出去
//												//OLED_Clear();
//												OLED_ShowString(0,0,(uint8_t *)RxBuffer,8);
//												//OLED_ShowNum(0,0,i,3,16);
//										//while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX);//检测UART发送结束
//                        //Uart1_Rx_Cnt = 0;
                        //memset(RxBuffer,0x00,sizeof(RxBuffer)); //清空数组
//                }
        }
				
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
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