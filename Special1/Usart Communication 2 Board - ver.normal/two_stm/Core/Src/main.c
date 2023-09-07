/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
char nameT[128];
char nameR[128];
void TransmitData(void)
{
    char rxBuffer[128];
    char rxChar;
    uint16_t index = 0;
    while (1)
    {
    	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_RXNE)==RESET){}
        HAL_UART_Receive(&huart3, (uint8_t *)&rxChar, 1, HAL_MAX_DELAY);

        while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
        HAL_UART_Transmit(&huart3, (uint8_t *)&rxChar, 1, 1000);

        if (rxChar == '\n' || rxChar == '\r')
        {
            rxBuffer[index] = '\0';
            HAL_UART_Transmit(&huart1, (uint8_t *)rxBuffer, strlen(rxBuffer), 1000);
            strcpy(nameT,rxBuffer);
            for (int i = 0; i < sizeof(rxBuffer) / sizeof(rxBuffer[0]); i++)
            {
				rxBuffer[i] = '\0';
			}
            break;
        }
        else
        {
            rxBuffer[index] = rxChar;
            index++;

            if (index >= 127)
            {
            	rxBuffer[127]="";
                break;
            }
        }
    }
}

void ReceiveData(void)
{
	char rxBuffer[128];

	while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE)==RESET){}
	HAL_UART_Receive(&huart1, (uint8_t *)&rxBuffer, 30,1000);

	strcpy(nameR,rxBuffer);

	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
	HAL_UART_Transmit(&huart3, (uint8_t *)rxBuffer, strlen(rxBuffer), 1000);

}

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
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  char str[]="Man from U.A.R.T.2 !\n\rQuit PRESS q\r\n\t";
  char name[]="Name: ";
  char nameTD[128];
  char nameRD[128];

  	HAL_UART_Transmit(&huart3, (uint8_t *)str, strlen(str),1000);

  	ReceiveData();

  	strcpy(nameRD,nameR);

  	HAL_UART_Transmit(&huart3, (uint8_t *)" is Ready\r\n\t", 12,1000);

	HAL_UART_Transmit(&huart3, (uint8_t *)name, strlen(name), 1000);

	TransmitData();

	strcpy(nameTD,nameT);

	HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n\t", strlen("\r\n\t"),1000);


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  HAL_UART_Transmit(&huart3, (uint8_t *)nameRD, strlen(nameRD),1000);

	  HAL_UART_Transmit(&huart3, (uint8_t *)" : ", 3,1000);

	  ReceiveData();

	  if (nameR[0]=='q')
	  {
		break;
	  }

	  HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n\t", strlen("\r\n\t"),1000);

	  HAL_UART_Transmit(&huart3, (uint8_t *)nameTD, strlen(nameTD),1000);

	  HAL_UART_Transmit(&huart3, (uint8_t *)" => ", 4,1000);

	  TransmitData();

	  if (nameT[0]=='q')
	  {
		break;
	  }

	  HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n\t", strlen("\r\n\t"),1000);

  }

  HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n\tQuit", strlen("\r\n\tQuit"),1000);

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
