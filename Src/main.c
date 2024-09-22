/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "esp8266.h"
#include "oled.h"
#include <string.h>



/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

	uint8_t Senbuff[] = "AT\r\n";  //定义数据发送数组
	uint8_t cmd[] = "AT+CWMODE=1\r\n";
	uint8_t rst[] = "AT+RST\r\n";
	uint8_t wifi[] = "AT+CWJAP=\"Xiaomi Pad 5 Pro\",\"11111111\"\r\n";
	uint8_t heartbeat[] = "working\r\n";
	uint8_t ssid[] = "xiaomi";
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
	
	
  /* USER CODE BEGIN 2 */
	//ESP8266_Init(&huart1);
	//Send_AT_Command(&huart1);
//	printf("Link\n");
////	
		OLED_Init();
  	OLED_Clear();
		//ESP8266_SendCmd((uint8_t *)cmd,"OK");
		//OLED_ShowString(0,0,"Wifi: ",16);
		//HAL_UART_Transmit(&huart1,(uint8_t *)cmd, strlen((uint8_t *)cmd),HAL_MAX_DELAY);
		//link();
		ESP8266_Init("Xiaomi Pad 5 Pro","11111111");
//	OLED_ShowString(0,0,"Wifi: ",16);
//	OLED_ShowString(48,0,ssid,16);
	
	 //OLED_Display_On();
	//OLED_Reverse();
	//OLED_Update();
  
  /*延时3000ms，观察现象*/
  //HAL_Delay(3000);
	
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	//HAL_UART_Transmit_DMA(&huart1, (uint8_t *)heartbeat, sizeof(heartbeat));
//		OLED_Clear();
//		OLED_Update();
		//printf("TEST\n");
		//HAL_UART_Transmit(&huart1, (uint8_t *)rst, sizeof(rst),0xff);
		//HAL_Delay(3000);  
		//HAL_UART_Transmit(&huart1, (uint8_t *)cmd, sizeof(cmd),0xff);
		HAL_Delay(3000);
		//HAL_UART_Transmit(&huart1,(uint8_t *)"AT",sizeof("AT"),0xff);
		//HAL_UART_Transmit(&huart1,(uint8_t *)"AT\r\n",sizeof("AT\r\n"),0xff);
		
	//OLED_ShowStr(0,0,"wifi",48);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart1)
//{
//		if (__HAL_UART_GET_FLAG(huart1, UART_FLAG_RXNE) != RESET)
//    {
//        esp8266_cnt = 1;
//				if (esp8266_cnt >= sizeof(esp8266_buf))
//        {
//            esp8266_cnt = 0; // 防止缓冲区溢出
//        }

//        // 使用HAL库接收单个字节
//        uint8_t data;
//        if (HAL_OK == HAL_UART_Receive_IT(huart1, &data, 1))
//        {
//            esp8266_buf[esp8266_cnt++] = data;
//        }
//				//OLED_ShowString(0,0,(uint8_t *)esp8266_buf,8);
//        // 清除接收中断标志
//        __HAL_UART_CLEAR_FLAG(huart1, UART_FLAG_RXNE);
////				HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
//    }
//		OLED_ShowString(0,0,(uint8_t *)esp8266_buf,8);


//}




//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//  /* Prevent unused argument(s) compilation warning */
//  UNUSED(huart);
//  /* NOTE: This function Should not be modified, when the callback is needed,
//           the HAL_UART_TxCpltCallback could be implemented in the user file5 p9 }1 L8 U' {) R; `1 o# J
//   */
//				Uart1_Rx_Cnt++;
//        if(Uart1_Rx_Cnt >= sizeof(RxBuffer))  //溢出判断
//        {
//                Uart1_Rx_Cnt = 0;
//                memset(RxBuffer,0x00,sizeof(RxBuffer));
//                //HAL_UART_Transmit(&huart1, (uint8_t *)"数据溢出", 10,0xFFFF);         
//					
//        }
//        else
//        {
//                RxBuffer[Uart1_Rx_Cnt-1] = aRxBuffer;   //接收数据转存
//        
//                if((RxBuffer[Uart1_Rx_Cnt-2] == 'K')&&(RxBuffer[Uart1_Rx_Cnt-3] == 'O')) //判断结束位  
//								{				
////												//Uart1_Rx_Cnt = 1;
////												//OLED_ShowString(0,0,(uint8_t *)RxBuffer,8);
////                        HAL_UART_Transmit(&huart1, (uint8_t *)&RxBuffer, Uart1_Rx_Cnt,0xFFFF); //将收到的信息发送出去
////												//OLED_Clear();
//												OLED_ShowString(0,0,(uint8_t *)RxBuffer,8);
////												//OLED_ShowNum(0,0,i,3,16);
////										   //while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX);//检测UART发送结束
//                        //Uart1_Rx_Cnt = 0;
//                        //memset(RxBuffer,0x00,sizeof(RxBuffer)); //清空数组
//                }
//					
//							
//					
//					
//        }
//				OLED_ShowNum(0,4,Uart1_Rx_Cnt,3,8);
//        HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
//				//OLED_ShowNum(0,0,Uart1_Rx_Cnt,3,16);
//				//OLED_Clear();
//}				

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
