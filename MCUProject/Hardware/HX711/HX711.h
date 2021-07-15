#ifndef HX711_H_
#define HX711_H_

#include "stm32f4xx_hal.h"

void Hx711_Config(void);
long Hx711_getValue(void);
long Hx711_averageValue(uint8_t times);
void Hx711_setOffset(long offset);
void Hx711_setScale(float scale); // У׼
float Hx711_getGram(void);


#endif /* HX711_H_ */
