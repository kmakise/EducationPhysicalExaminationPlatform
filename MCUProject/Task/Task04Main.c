/**
  ******************************************************************************
  * File Name          : Task04Main.c
  * Description        : Code for freeRTOS Task01 Application
  ******************************************************************************
  * @author  kmakise-zdd171
  * @version V1.0.0
  * @date    2021-1-27
  * @brief   task4
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
#include "Task04Main.h"

#include "Task01Main.h"
#include "Task06Main.h"
#include "GP2Y0A02YK0F.h"
#include "Hx711.h"
#include "bh1750.h"
#include "DHT11.h"

//10Hz Task short time

uint16_t g_bh1750data = 0;
uint8_t  g_distance = 0;
uint8_t  g_temperature = 0;
uint8_t  g_humidity = 0;
uint8_t  g_smoking = 0;
float    g_pressure = 0;



// 1s loop dht11_read_data
void Task04_div_Loop1(void)
{
	static int times = 0;
	times ++;
	if(times >= 10)
	{
		times = 0;
		
		vTaskSuspendAll();
		dht11_read_data(&g_temperature,&g_humidity);
		xTaskResumeAll();
	}
}

// 0.3s loop Hx711_getGram
void Task04_div_Loop2(void)
{
	static int times = 0;
	times ++;
	if(times >= 3)
	{
		times = 0;
		
		g_pressure		= Hx711_getGram();
	}
}



void Task4UpdateToUsartBuf(void)
{
	uint16_t prsKG = g_pressure * 100;
	
	setDataToUpdate(LIGHTPW,&g_bh1750data);
	setDataToUpdate(DISTANCE,&g_distance);
	setDataToUpdate(ENVTEMP,&g_temperature);
	setDataToUpdate(ENVHUMT,&g_humidity);
	setDataToUpdate(PRESSURE,&prsKG);
	setDataToUpdate(SMOKING,&g_smoking);
	
}

void Hx711turnZero(void)
{
	if(HAL_GetTick() < 5000)
	{
		Hx711_setOffset(Hx711_averageValue(32));
	}
}


//task main
void Task04Main(void)
{
	
	//wait for hardware configure
	while(getHardwareState());
	
	for(;;)
	{
		Hx711turnZero();
		
		//10Hz
		g_bh1750data 	= Get_Bh_Value();
		g_distance	 	= getIRdis();
		g_smoking			= HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5);
		
		//div
		Task04_div_Loop1();
		Task04_div_Loop2();
		
		Task4UpdateToUsartBuf();
		
		osDelay(100);
	}
}



