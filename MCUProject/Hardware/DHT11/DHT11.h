#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f4xx_hal.h"

#define DHT11_GPIO_TYPE  GPIOC
#define DHT11_GPIO_PIN   GPIO_PIN_4


#define DHT11_OUT_H HAL_GPIO_WritePin(DHT11_GPIO_TYPE,DHT11_GPIO_PIN,GPIO_PIN_SET)
#define DHT11_OUT_L HAL_GPIO_WritePin(DHT11_GPIO_TYPE,DHT11_GPIO_PIN,GPIO_PIN_RESET)
#define DHT11_IN    HAL_GPIO_ReadPin(DHT11_GPIO_TYPE,DHT11_GPIO_PIN)

uint16_t dht11_read_data(uint8_t *temp,uint8_t *humi);

#endif

