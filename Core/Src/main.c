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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "interrupt.h"
#include "lcd.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
char B1=64,B2=64,B3=64;
char key_B1=48 , key_B2=50 , key_B3=52;
int F=2000;
int D=10;
uint8_t view=0;
extern struct Keys key[4];
extern char rx_buffer[30];
extern uint8_t rx_data;
extern uint8_t rx_pointer;
extern bool uart_flag;
bool password_flag = 0;
bool ledON = 0;
bool false_ledON=0;
uint8_t false_time=0;
int led_time;
uint8_t false_flag;
bool ledState=0;
bool clear_flag=0;
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
void lcd_control(void);
void key_control(void);
void uart_control(void);
void led_control(void);
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
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_UART_Receive_IT(&huart1 , &rx_data , 1);
	HAL_GPIO_WritePin(GPIOC , GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 , GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_RESET);
	HAL_TIM_PWM_Start(&htim2 , TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		lcd_control();
		key_control();
		uart_control();
		led_control();
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
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void lcd_control()
{
	if(view==0)
	{
		if(clear_flag == 1)
		{
			B1=64,B2=64,B3=64;
			LCD_Clear(Black);
			clear_flag = 0;
		}
		LCD_DisplayStringLine(Line1 , (uint8_t *)"       PSD          ");
		char text[30];
		sprintf(text ,"    B1:%c" , B1);
		LCD_DisplayStringLine(Line3 , (uint8_t *)text);
		sprintf(text ,"    B2:%c" , B2);
		LCD_DisplayStringLine(Line4 , (uint8_t *)text);
		sprintf(text ,"    B3:%c" , B3);
		LCD_DisplayStringLine(Line5 , (uint8_t *)text);	
	}
	
	if(view==1)
	{
		LCD_DisplayStringLine(Line1 , (uint8_t *)"       STA          ");
		char text[30];
		sprintf(text ,"    F:%dHz" , F);
		LCD_DisplayStringLine(Line3 , (uint8_t *)text);
		sprintf(text ,"    D:%d%%" , D);
		LCD_DisplayStringLine(Line4 , (uint8_t *)text);		
	}

}

void key_control()
{
	if(B1 == 64 && key[0].single_flag == 1)
	{
		key[0].single_flag = 0;
		B1 = B1-16;
	}else if(key[0].single_flag == 1)
					{
						B1++;
						if(B1 == 58)	B1=48;
						key[0].single_flag = 0;
					}
					
	if(B2 == 64 && key[1].single_flag == 1)
	{
		key[1].single_flag = 0;
		B2 = B2-16;
	}else if(key[1].single_flag == 1)		
					{
						B2++;
						if(B2 == 58)	B2=48;
						key[1].single_flag = 0;
					}
					
	if(B3 == 64 && key[2].single_flag == 1)
	{
		key[2].single_flag = 0;
		B3 = B3-16;
	}else if(key[2].single_flag == 1)
					{
						B3++;
						if(B3 == 58)	B3=48;
						key[2].single_flag = 0;
					}
					
	if(key[3].single_flag == 1)
	{
		if(B1==key_B1 && B2==key_B2	&& B3==key_B3)
		{
			password_flag = 1;
			LCD_Clear(Black);
			key[3].single_flag = 0;
			view = 1;
		}else
				{
					B1 = 64;
					B2 = 64;
					B3 = 64;
					key[3].single_flag = 0;
					HAL_TIM_Base_Start_IT(&htim2);
					false_flag++;
					if(false_flag>=3)	false_ledON=1;
				}
		
	}
	
}

void uart_control()
{
	char puB1 , puB2 , puB3 , nuB1 , nuB2 , nuB3;
	if(uart_flag == 1)
	{
		sscanf(rx_buffer , "%c%c%c-%c%c%c" , &puB1 , &puB2 , &puB3 , &nuB1 , &nuB2 , &nuB3);
		if(puB1==key_B1 && puB2==key_B2 && puB3==key_B3)
		{
			key_B1 = nuB1;
			key_B2 = nuB2;
			key_B3 = nuB3;
			uart_flag = 0;
			HAL_UART_Transmit(&huart1 , (uint8_t *)"Modified Successfully\r\n" , strlen("Modified Successfully\r\n") , 50);
		}else 
				{
					HAL_UART_Transmit(&huart1 , (uint8_t *)"Modified Failed\r\n" , strlen("Modified Failed\r\n") , 50);
					uart_flag = 0;
				}
		
	}

}

void led_control()
{
	
	if(password_flag == 1)
	{
		__HAL_TIM_SET_COMPARE(&htim2 , TIM_CHANNEL_2 , 10);
		__HAL_TIM_SET_PRESCALER(&htim2 , 40-1);
		__HAL_TIM_SET_AUTORELOAD(&htim2 , 1000-1);
		ledON = 1;
		HAL_TIM_Base_Start_IT(&htim2);
		password_flag = 0;
	}
}

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
