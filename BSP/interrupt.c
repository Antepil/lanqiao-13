#include "interrupt.h"
#include "usart.h"
#include "tim.h"
#include "lcd.h"
struct Keys key[4]={0,0,0,0};
extern bool ledON;
extern bool false_ledON;
extern int led_time;
extern uint8_t false_time;
extern bool ledState;
extern uint8_t view;
extern char B1,B2,B3;
extern bool clear_flag;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3){
		key[0].key_sta = HAL_GPIO_ReadPin(GPIOB , GPIO_PIN_0);
		key[1].key_sta = HAL_GPIO_ReadPin(GPIOB , GPIO_PIN_1);
		key[2].key_sta = HAL_GPIO_ReadPin(GPIOB , GPIO_PIN_2);
	  key[3].key_sta = HAL_GPIO_ReadPin(GPIOA , GPIO_PIN_0);
		
		for(int i=0;i<4;i++){
			switch(key[i].key_judage){
				case 0:{
					if(key[i].key_sta == 0){
						key[i].key_judage = 1;
						key[i].key_time = 0;
					}
				}break;
				case 1:{
					if(key[i].key_sta == 0){
						key[i].key_judage = 2;
					}else
							key[i].key_judage = 0;
				}break;
				case 2:{
					if(key[i].key_sta == 1){
						key[i].key_judage = 0;
						if(key[i].key_time<=80){
							key[i].single_flag = 1;
						}
					}else{
						key[i].key_time++;
						if(key[i].key_time>80){
							key[i].key_judage = 0;
							key[i].long_flag = 1;
						}
					}
				}break;			
			}
		}
	}
	
	if(htim->Instance == TIM4)
	{
		if(ledON == 1)
		{
			HAL_GPIO_WritePin(GPIOC , GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 , GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC , GPIO_PIN_8 , GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_RESET);
			led_time++;
			if(led_time == 50)	
			{
				__HAL_TIM_SET_COMPARE(&htim2 , TIM_CHANNEL_2 , 100);
				__HAL_TIM_SET_PRESCALER(&htim2 , 80-1);
				__HAL_TIM_SET_AUTORELOAD(&htim2 , 1000-1);
				clear_flag = 1;
				view = 0;
				led_time = 0;
				ledON = 0;
			}
			//HAL_TIM_Base_Stop_IT(&htim2);
		}
		
		
						
		if(false_ledON == 1)
		{
			ledState = !ledState;
			HAL_GPIO_WritePin(GPIOC , GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 , GPIO_PIN_SET);
				if (ledState) {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
			} else {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			}
  
			//HAL_GPIO_TogglePin(GPIOC , GPIO_PIN_9);
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_RESET);
			led_time++;
			if(led_time == 50)	
			{
				led_time = 0;
				false_ledON = 0;
			}
		}
		
		if(ledON == 0 && false_ledON == 0)
		{
			HAL_GPIO_WritePin(GPIOC , GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 , GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD , GPIO_PIN_2 , GPIO_PIN_RESET);
			//HAL_TIM_Base_Stop_IT(&htim2);
		}
	}
	
	if(htim->Instance == TIM2)
	{
		
	
	
	}
}

char rx_buffer[30];
uint8_t rx_data;
uint8_t rx_pointer=0;
bool uart_flag=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		rx_buffer[rx_pointer++] = rx_data;
		HAL_UART_Receive_IT(&huart1 , &rx_data , 1);
		if(rx_pointer == 7)
		{
			uart_flag = 1;
		}
	
	}
	
}
