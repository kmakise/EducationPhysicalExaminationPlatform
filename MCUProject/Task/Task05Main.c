/**
  ******************************************************************************
  * File Name          : Task05Main.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @author  kmakise-zdd171
  * @version V1.0.0
  * @date    2021-1-29
  * @brief   task5
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
#include "Task05Main.h"

#include "Task01Main.h"
#include "Task06Main.h"
#include "usart.h"
#include "stdlib.h"

uint8_t g_TEMP_data[20] = {0};
uint8_t g_Receive_ok = 0;
float		g_IRtempTO = 0;

uint8_t len=0,rebuf[20]={0};

void IRtempUARTRec(void)
{
	if(UART4->SR&(1<<5))
	{	 
		rebuf[len++] = (UART4->DR&(uint8_t)0x00FF);
		//HAL_UART_Receive(&huart4,&rebuf[len++],1,1);
		
		if (rebuf[0]!=0x5a)//帧头不对
			len = 0;	
		if ((len == 2)&&(rebuf[1] != 0x5a))//帧头不对
			len = 0;
		if((len == 4)&&(rebuf[3] != 4))
			len = 0;
		if(len > 15)
			len = 0;

		if(len>3)//i等于4时，已经接收到数据量字节rebuf[3]
		{
			if(len!=(rebuf[3]+5))//判断是否接收一帧数据完毕
				return;	
			switch(rebuf[2])//接收完毕后处理
			{
				case 0x45:
					if(!g_Receive_ok)//当数据处理完成后才接收新的数据
					{
						memcpy(g_TEMP_data,rebuf,9);//拷贝接收到的数据
						for(int i = 0;i < 20;i++)
						{
							rebuf[i] = 0;
						}
						g_Receive_ok=1;//接收完成标志
					}
					break;
				case 0x15:break;
				case 0x35:break;
			}
			len = 0;//缓存清0
		}
	}
}

void IRtempRead(void)
{
	float TO=0,TA=0;
  uint8_t sum=0,i=0;
	int16_t data[2]={0};
	
	if(g_Receive_ok)//串口接收完毕
	{
		for(i = 0;i < (g_TEMP_data[3]+4);i++)
		{
			sum += g_TEMP_data[i];
		}
		if(sum==g_TEMP_data[i])//校验和判断
		{
			TO = (float)((g_TEMP_data[4]<<8)|g_TEMP_data[5])/100;//得到真实温度
			TA = (float)((g_TEMP_data[6]<<8)|g_TEMP_data[7])/100;//得到真实温度
			g_IRtempTO = TO + 1.68;
			setDataToUpdate(HMTEMP,&g_IRtempTO);
		}
		g_Receive_ok = 0;//处理数据完毕标志
	}
}




void Task05Main(void)
{
	//wait for hardware configure
	while(getHardwareState());
	__HAL_UART_ENABLE_IT(&huart4,UART_IT_RXNE);
	for(;;)
	{
		IRtempRead();
		osDelay(100);
	}
}



