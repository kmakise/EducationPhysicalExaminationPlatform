/**
  ******************************************************************************
  * File Name          : Task02Main.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @author  kmakise-zdd171
  * @version V1.0.0
  * @date    2021-1-18
  * @brief   task2
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
#include "Task02Main.h"

#include "Task01Main.h"
#include "Task06Main.h"
#include "HRSpO2tf.h"

void Task2UpdateToUsartBuf(void)
{
	setDataToUpdate(HRTIMES,&(g_blooddata.heart));
	setDataToUpdate(SPO2PCT,&(g_blooddata.SpO2 ));
	setDataToUpdate(HBDATA1,&(g_blooddata.hb   ));
	setDataToUpdate(HBDATA2,&(g_blooddata.hbo2 ));
}


//50Hz Task

void Task02Main(void)
{

	//wait for hardware configure
	while(getHardwareState());
		
	for(;;)
	{
		blood_data_update();
		
		Task2UpdateToUsartBuf();
		
		osDelay(10);;
	}
}



