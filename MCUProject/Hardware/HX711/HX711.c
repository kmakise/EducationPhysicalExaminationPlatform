/**
  ******************************************************************************
  * File Name          : HX711.c
  * Description        : Code for HX711 driver
  ******************************************************************************
  * @author  kmakise-zdd171
  * @version V1.0.0
  * @date    2021-1-19
  * @brief   HX711 driver
	******************************************************************************
  * @attention
  *
  * All rights reserved.
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Hx711.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

#define HX711_SCL_H         HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET)
#define HX711_SCL_L         HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET)
   
#define HX711_SDA_H         HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET)
#define HX711_SDA_L         HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET)

#define HX711_SDA_R      		HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12)

#define HX711_DELAY					osDelay

const uint8_t _pin_dout;
const uint8_t _pin_slk;
long _offset;
float _scale;

void HX711_SCL_delay(void)
{
                
   uint16_t i=1000;
   while(i) 
   { 
     i--; 
   }  
}

void Hx711_Config(void)
{
	HX711_SCL_H;
	HX711_SDA_H;
	//digitalWrite(_pin_slk, HIGH);
	HX711_SCL_H;
	//delayMicroseconds(100);
	HX711_DELAY(100);
	//digitalWrite(_pin_slk, LOW);
	HX711_SCL_L;
	Hx711_averageValue(32);
	Hx711_setOffset(Hx711_averageValue(32));
	Hx711_setScale(17043.f);
}

long Hx711_averageValue(uint8_t times)
{
	//times = 32;
	long sum = 0;
	
	for (int i = 0; i < times; i++)
	{
		long temp = Hx711_getValue();
		if(temp == 0)
		{
			return 0;
		}
		sum += temp;
	}

	return sum / times;
}

long Hx711_getValue()
{
	uint8_t data[3];
	uint8_t lowtime = 0;
	uint16_t timeout = 0;
	
	while (lowtime < 5)
	{
		if(HX711_SDA_R == 0)
		{
			lowtime++;
			timeout = 0;
		}
		else
		{
			lowtime = 0;
			timeout++;
		}
		
		if(timeout > 800)
		{
			return 0;
		}
		
		HX711_DELAY(1);
	}

	
	for (int j = 3; j--;)
	{
		data[j] = 0;
		for (char i = 8; i--;)
		{
			//digitalWrite(_pin_slk, HIGH);
			HX711_SCL_H;
			HX711_SCL_delay();
			//bitWrite(data[j], i, digitalRead(_pin_dout));
			data[j] |= (HX711_SDA_R << i);
			//digitalWrite(_pin_slk, LOW);
			HX711_SCL_L;
			HX711_SCL_delay();
		}
	}

	
	//digitalWrite(_pin_slk, HIGH);
	HX711_SCL_H;
	HX711_SCL_delay();
	//digitalWrite(_pin_slk, LOW);
	HX711_SCL_L;

	data[2] ^= 0x80;

	return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8)
			| (uint32_t) data[0];
}

void Hx711_setOffset(long offset)
{
	_offset = offset;
}

void Hx711_setScale(float scale)
{
	_scale = scale;
}

float Hx711_getGram()
{
	long val = (Hx711_averageValue(32) - _offset);
	return (float) val / _scale;
}
