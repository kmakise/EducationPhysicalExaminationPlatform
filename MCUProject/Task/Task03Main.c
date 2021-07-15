/**
  ******************************************************************************
  * File Name          : Task03Main.c
  * Description        : Code for freeRTOS Task01 Application
  ******************************************************************************
  * @author  kmakise-zdd171
  * @version V1.0.0
  * @date    2021-1-18
  * @brief   task3
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
#include "Task03Main.h"

#include "Task01Main.h"
#include "HRSpO2tf.h"



//10Hz Task Long Time

void Task03Main(void)
{

	//wait for hardware configure
	while(getHardwareState());
	
	for(;;)
	{		
		blood_data_translate();
		osDelay(100);
	}
}



