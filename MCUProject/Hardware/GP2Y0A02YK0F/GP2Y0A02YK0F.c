/**
  ******************************************************************************
  * File Name          : Task03Main.c
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
#include "GP2Y0A02YK0F.h"
#include "adc.h"

uint16_t ADC1_Value[6];

const uint16_t IRA02DISMAP[15][2] = {
//cm adc
	10,3265,
	15,3225,
	20,2920,
	25,2670,
	30,2290,
	35,1950,
	40,1720,
	45,1600,
	50,1445,
	55,1320,
	60,1200,
	65,1100,
	70,1030,
	75,965,
	80,890,
};

void getSensorADCval(void)
{
	for(int i = 0;i < 6;i++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,0xffff);
		ADC1_Value[i]=HAL_ADC_GetValue(&hadc1);
	}
	HAL_ADC_Stop(&hadc1);
}

float tf_ADCtoDis(uint16_t adcval)
{
	uint16_t index = 0;
	float x,y,x1,x2,y1,y2;
	float k,b,dis;
	
	index = 0xFF;
	//find adc val from table
	for(int i = 0;i < 14;i++)
	{
		if(IRA02DISMAP[i][1] >= adcval && IRA02DISMAP[i+1][1] < adcval)
		{
			index = i;
			break;
		}
	}
	
	if(index == 0xFF)
	{
		return 0;
	}
	
	x1 = IRA02DISMAP[index][0];
	x2 = IRA02DISMAP[index + 1][0];
	y1 = IRA02DISMAP[index][1];
	y2 = IRA02DISMAP[index + 1][1];

	k = (y2 - y1)/(x2 - x1);
	b = y1 - k * x1;
	
	y = adcval;
	x = (y - b) / k;

	return x;
}

uint16_t getIRdis(void)
{
	float temp = 0;
	uint16_t dis;
	
	for(int i = 0;i < 100;i++)
	{
		getSensorADCval();
		temp += ADC1_Value[4];
	}
	ADC1_Value[4] = temp /100;
	
	dis = tf_ADCtoDis(ADC1_Value[4]);
	
	return dis;
}
