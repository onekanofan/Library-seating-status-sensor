/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "oled.h"
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
int KE1_Parse_NNMI(char *pcNNMI, char *pcOut)
{
	char ch = 0, lenFlag = 0, dataFlag = 0;
	char acLen[5] = {0};
	int i = 0, pos = 0, dLen = 0;
	//+NNMI:6,010009020000
	for(i=0; i<strlen(pcNNMI); i++){
		ch = pcNNMI[i];
		if(0x0D == ch && 1 == dataFlag) break;

		if(':' == ch){
			lenFlag = 1; pos = 0;
			continue;
		}
		if(',' == ch){
			lenFlag = 0;
			dataFlag = 1; pos = 0;
			dLen = atoi(acLen);
			continue;
		}
		if(1 == lenFlag){
			acLen[pos++] = ch;
		}
		if(1 == dataFlag){
			pcOut[pos++] = ch;
		}
	}
	return dLen;
}

void ascii2hex(char *pcAscii, char *pcHex)
{
	int iDlen = 0, i = 0, pos = 0;
	iDlen = strlen(pcAscii);
	if(128 < iDlen) return;
	for(i=0; i<iDlen; i++){
		sprintf(&pcHex[pos], "%02X", pcAscii[i]);
		pos += 2;
	}
}
extern unsigned char key1, key2;
char acDevInfo[128] = {0}, acHexBuf[256] = {0}, acAtBuf[512] = {0}, acUserCmd[64] = {0};
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	float fTemp = 34.2, fHumi = 0.0;
	unsigned short usLight = 0, usSound = 0;
	int iUserCase = 0, iRet = -1;
	char netFlag = 0, cmdLen = 0;
	unsigned int atLen = 0, dLen = 0, timeout = 1000, upFreq = 0;
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
	MX_ADC1_Init();
	MX_USART1_UART_Init();
	MX_USART3_UART_Init();
	MX_I2C2_Init();
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */
	Beep_Switch(1);
	HAL_Delay(500);
	Beep_Switch(0);

	UART_Enable_Receive_IT();// 使能串口接收中断，开始接收数据

	OLED_Init(); // 初始化OLDE

	OLED_DrawLogo(); // 显示bmp单色logo图片
	HAL_Delay(2000);
	OLED_ShowString(0, 0, (uint8_t *)"OLED Demo", 8);// 显示丿行英文字笿
	HAL_Delay(2000);

	OLED_ShowKE1(); // 显示 小熊座KE1
	HAL_Delay(2000);

	OLED_Clear(); // 清屏

	printf("Hello! i am KE1 south demo V0.0.2\r\n");
	HAL_Delay(500);
	KE1_I2C_SHT31(&fTemp, &fHumi);
	printf("T:%0.2f, H:%0.2f%%\r\n", fTemp, fHumi);
	KE1_ADC_Senser_Get(&usLight, &usSound);

	OLED_ShowT_H(fTemp, fHumi); // OLED显示温湿度
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if(1 == key1){
			key1 = 0;
			printf("key1 touch!\r\n");
		}
		if(1 == key2){
			key2 = 0;
			printf("key2 touch!\r\n");
		}
		printf("case %d\r\n", iUserCase);
		switch(iUserCase){
		case 0:
			KE1_Clear_AT_Buf();
			iUserCase = 1;
			break;
		case 1:
			KE1_Send_AT("AT\r\n"); // AT测试模组
			break;
		case 2:
			KE1_Send_AT("AT+CFUN=0\r\n"); // 关闭模组
			timeout = 10000;
			break;
		case 3:
			KE1_Send_AT("AT+NCDP=180.101.147.115,5683\r\n"); // 设置电信物联网南向接口地�?
			break;
		case 4:
			KE1_Send_AT("AT+CFUN=1\r\n"); // 启动模块
			timeout = 10000;
			break;
		case 5:
			KE1_Send_AT("AT+CGDCONT=1,\"IP\",\"CTNB\"\r\n"); // 设置PDP
			timeout = 1000;
			break;
		case 6:
			KE1_Send_AT("AT+CGATT=1\r\n"); // 模块弿始注册网绿
			break;
		case 7:
			if(0 == upFreq && 1 == netFlag){
				KE1_I2C_SHT31(&fTemp, &fHumi); // 采集温湿度或者其他信息上传到电信物联网平�?
				KE1_ADC_Senser_Get(&usLight, &usSound);
				memset(acDevInfo, 0, sizeof(acDevInfo));
				memset(acAtBuf, 0, sizeof(acAtBuf));
				OLED_ShowT_H(fTemp, fHumi);
				dLen = snprintf(acDevInfo, sizeof(acDevInfo), "{\"T\":\"%0.2f\",\"H\":\"%0.2f\",\"L\":\"%d\",\"S\":\"%d\"}", fTemp, fHumi, usLight,usSound);// 打包用户数据
				printf("%s\r\n", acDevInfo);
				ascii2hex(acDevInfo, acHexBuf);
				snprintf(acAtBuf, sizeof(acAtBuf), "AT+NMGS=%d,00%04X%s\r\n", (dLen+3), dLen, acHexBuf);// 打包COAP数据包AT命令
				printf("%s\r\n", acAtBuf);
				KE1_Send_AT(acAtBuf);
				upFreq = (60*60);
			}
			break;
		}

		atLen = sizeof(acAtBuf);
		iRet = KE1_Recv_AT(acAtBuf, &atLen, timeout);
		//printf("\r\nRAT:%d\r\n",iRet);
		if(0 == iRet){//AT命令响应超时

		}else if(1 == iRet){//AT命令接收到OK响应
			if(7 == iUserCase){
			}else{
				iUserCase++;
			}
			HAL_Delay(1000);
		}else if(2 == iRet){//AT命令接收到ERROR响应
			printf("AT error !\r\n");
			if(7 == iUserCase){
				iUserCase = 0; // 重新初始化模�?
			}
			HAL_Delay(1000);
		}else if(3 == iRet){//接收到网络已注册提示
			printf("Net ready !\r\n");
			netFlag = 1;
			HAL_Delay(5000);
		}else if(4 == iRet){//AT命令接收到电信物联网平台下发数据
			printf("%s", acAtBuf);
			memset(acUserCmd, 0, sizeof(acUserCmd));
			cmdLen = KE1_Parse_NNMI(acAtBuf, acUserCmd);
			printf("user data[%d]:%s\r\n", cmdLen, acUserCmd);
			if(strstr(acUserCmd, "AAAA0000")){
				printf("device info upload successfully\r\n");
			}else{
				/*
				 * 解析用户命令执行对应操作
				 * TO-DO
				 */
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
				HAL_Delay(1000);

				/* 向平台发送命令响�?
				 * AT+NMGS=5,02000A000A
				 */
				acUserCmd[1] = '2';
				acUserCmd[6] = '0';acUserCmd[7] = '0';acUserCmd[8] = '0';acUserCmd[9] = '0';acUserCmd[10] = 0;
				printf("CmdResp:%s\r\n", acUserCmd);
				snprintf(acAtBuf, sizeof(acAtBuf), "AT+NMGS=%d,%s\r\n", 5, acUserCmd);// 打包COAP数据包AT命令
				KE1_Send_AT(acAtBuf);
			}

		}

		if(0 != upFreq) upFreq--;
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 8;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART3
			|RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_ADC;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
	PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
	PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
	PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
	PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
	PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
	PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
	PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
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
void assert_failed(char *file, uint32_t line)
{ 
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/