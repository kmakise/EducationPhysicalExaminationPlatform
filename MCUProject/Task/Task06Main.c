/**
  ******************************************************************************
  * File Name          : Task05Main.c
  * Description        : Code for freeRTOS Task06 Application
  ******************************************************************************
  * @author  kmakise-zdd171
  * @version V1.0.0
  * @date    2021-1-28
  * @brief   task5
	******************************************************************************
  * @attention
  *
  * All rights reserved.
  *
  * Communication of sensor data update of Android Board 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Task06Main.h"

#include "Task01Main.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "crc.h"

uint8_t g_SendDataTemp[100];  //change temp
uint8_t g_SendDataRead[100];  //send to update

void setDataToUpdate(DataTypedef dt,void * val)
{
	//u16 data
	union 
	{
		uint8_t arr[2];
		uint16_t data;
	}datau16;
	
	//float32 data
	union 
	{
		uint8_t arr[4];
		float	data;
	}dataflt32;
	
	switch(dt)
	{
		case DISTANCE:	//²â¾à				4 	u8
		{
			g_SendDataTemp[4] = *(uint8_t *)val;
			break;
		}
		case PRESSURE:	//Ñ¹Á¦				5 	u16
		{ 
			datau16.data = *((uint16_t *)val);
			memcpy(&g_SendDataTemp[5],datau16.arr,2);
			break;
		}
		case HRTIMES:		//ÐÄÂÊ				7 	u8
		{
			g_SendDataTemp[7] = *(uint8_t *)val;
			break;
		}
		case SPO2PCT:		//ÑªÑõ±¥ºÍ¶È	8		f32
		{
			dataflt32.data = *((float *)val);
			memcpy(&g_SendDataTemp[8],dataflt32.arr,4);
			break;
		}
		case HBDATA1:		//ÑªÑõ²¨ÐÎ1		12	f32
		{
			dataflt32.data = *((float *)val);
			memcpy(&g_SendDataTemp[12],dataflt32.arr,4);
			break;
		}
		case HBDATA2:		//ÑªÑõ²¨ÐÎ2		16	f32
		{
			dataflt32.data = *((float *)val);
			memcpy(&g_SendDataTemp[16],dataflt32.arr,4);
			break;
		}
		case HMTEMP:		//ÌåÎÂ				20	f32	
		{
			dataflt32.data = *((float *)val);
			memcpy(&g_SendDataTemp[20],dataflt32.arr,4);
			break;
		}
		case SMOKING:		//ÑÌÎí				24  u8
		{
			g_SendDataTemp[24] = *(uint8_t *)val;
			break;
		}
		case ENVTEMP:		//ÎÂ¶È				25	u8
		{
			g_SendDataTemp[25] = *(uint8_t *)val;
			break;
		}
		case ENVHUMT:		//Êª¶È				26	u8
		{
			g_SendDataTemp[26] = *(uint8_t *)val;
			break;
		}
		case LIGHTPW:		//¹âÕÕ				27	u16
		{
			datau16.data = *((uint16_t *)val);
			memcpy(&g_SendDataTemp[27],datau16.arr,2);
			break;
		}
		default:break;
	}
}

void sendDataToUpdate(void)
{
	memcpy(g_SendDataRead,g_SendDataTemp,29);
	g_SendDataRead[0] = 0x5A;
	g_SendDataRead[1] = 0xA5;
	g_SendDataRead[2] = 0x01;
	g_SendDataRead[3] = 24;
	
	uint16_t crc16 = CalCRC16(g_SendDataRead,29);
	g_SendDataRead[29] = (crc16 >> 8) &0xff;
	g_SendDataRead[30] = crc16 &0xff;
	
	HAL_UART_Transmit_DMA(&huart1,g_SendDataRead,31);
	HAL_UART_Transmit_DMA(&huart2,g_SendDataRead,31);
}

//50Hz Task Long Time

void Task06Main(void)
{
	
	//wait for hardware configure
	while(getHardwareState());
	
	
	for(;;)
	{		
		sendDataToUpdate();
		
		osDelay(20);
	}
}



