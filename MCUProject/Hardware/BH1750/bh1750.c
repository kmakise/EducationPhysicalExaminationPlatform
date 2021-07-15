/**
  ******************************************************************************
  * File Name          : bh1750.c
  * Description        : Code for bh1750 operation driver
  ******************************************************************************
  * @author  kmakise-zdd171
  * @version V1.0.0
  * @date    2021-1-18
  * @brief   bh1750 driver
	******************************************************************************
  * @attention
  *
  * All rights reserved.
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "bh1750.h"
#include "FreeRTOS.h"
#include "i2c.h"
#include "cmsis_os.h"

#define	 BH1750_ADDR   					0x46
#define  BH1750_DELAY_MS       	osDelay
#define  BH1750_IIC							&hi2c2
#include "cmsis_os.h"
void BH1750_Configure(void)
{
  uint8_t data[1] = {0x01};
	HAL_I2C_Master_Transmit(&hi2c2,BH1750_ADDR, data,1,10);
}

uint16_t Get_Bh_Value(void)
{  
	float temp;
	unsigned int data;
	int dis_data ;
	uint8_t buf[4];  
	uint8_t sendu8[1];
	
	sendu8[0] = 0x01;// power on
	HAL_I2C_Master_Transmit(BH1750_IIC,BH1750_ADDR,sendu8,1,10);
	sendu8[0] = 0x10;// H- resolution mode
	HAL_I2C_Master_Transmit(BH1750_IIC,BH1750_ADDR,sendu8,1,10);
	
	BH1750_DELAY_MS(50);

	HAL_I2C_Master_Receive(BH1750_IIC,BH1750_ADDR,buf,2,10);
	
	dis_data=buf[0];
	dis_data=(dis_data<<8)+buf[1];
	temp=(float)dis_data/1.2;
	data=(int)temp;
	return data;        
}

