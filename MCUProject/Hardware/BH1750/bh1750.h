#ifndef __BH1750_H
#define __BH1750_H	 

#include "stm32f4xx_hal.h"

void BH1750_Configure(void);
uint16_t Get_Bh_Value(void);

#endif
