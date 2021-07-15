/**
  ******************************************************************************
  * File Name          : Task01Main.c
  * Description        : Code for freeRTOS Task01 Application
  ******************************************************************************
  * @author  kmakise-zdd171
  * @version V1.0.0
  * @date    2021-1-18
  * @brief   task1
	******************************************************************************
  * @attention
  *
  * All rights reserved.
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Task01Main.h"

#include "MAX30100.h"
#include "GP2Y0A02YK0F.h"
#include "oled.h"
#include "bmp.h"
#include "Hx711.h"
#include "bh1750.h"
#include "usart.h"

/*Golbal data space ----------------------------------------------------------*/
uint8_t g_HardwareState = 0;

int getHardwareState(void)
{
	return g_HardwareState;
}

void HardwareConfigure(void)
{
	uint8_t str[3] = {0x45,0x45,0x45};
	
	g_HardwareState = 0;
	OLED_Init();
	OLED_DrawBMP(0,0,128,8,(uint8_t *)BMP_Start);
	OLED_ShowString(50,7,"Ver:1.3.2",8);
	
	Hx711_Config();
	BH1750_Configure();
	max30100_init();
	
	HAL_UART_Transmit(&huart6,str,1,100);
	g_HardwareState = 1;
}

//hardware configure , led flash

void Task01Main(void)
{
	//hard ware init
	HardwareConfigure();
	
	for(;;)
	{
		osDelay(500);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
		osDelay(500);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	}
}



