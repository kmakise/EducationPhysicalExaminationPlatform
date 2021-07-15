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
		
		if (rebuf[0]!=0x5a)//֡ͷ����
			len = 0;	
		if ((len == 2)&&(rebuf[1] != 0x5a))//֡ͷ����
			len = 0;
		if((len == 4)&&(rebuf[3] != 4))
			len = 0;
		if(len > 15)
			len = 0;

		if(len>3)//i����4ʱ���Ѿ����յ��������ֽ�rebuf[3]
		{
			if(len!=(rebuf[3]+5))//�ж��Ƿ����һ֡�������
				return;	
			switch(rebuf[2])//������Ϻ���
			{
				case 0x45:
					if(!g_Receive_ok)//�����ݴ�����ɺ�Ž����µ�����
					{
						memcpy(g_TEMP_data,rebuf,9);//�������յ�������
						for(int i = 0;i < 20;i++)
						{
							rebuf[i] = 0;
						}
						g_Receive_ok=1;//������ɱ�־
					}
					break;
				case 0x15:break;
				case 0x35:break;
			}
			len = 0;//������0
		}
	}
}

void IRtempRead(void)
{
	float TO=0,TA=0;
  uint8_t sum=0,i=0;
	int16_t data[2]={0};
	
	if(g_Receive_ok)//���ڽ������
	{
		for(i = 0;i < (g_TEMP_data[3]+4);i++)
		{
			sum += g_TEMP_data[i];
		}
		if(sum==g_TEMP_data[i])//У����ж�
		{
			TO = (float)((g_TEMP_data[4]<<8)|g_TEMP_data[5])/100;//�õ���ʵ�¶�
			TA = (float)((g_TEMP_data[6]<<8)|g_TEMP_data[7])/100;//�õ���ʵ�¶�
			g_IRtempTO = TO + 1.68;
			setDataToUpdate(HMTEMP,&g_IRtempTO);
		}
		g_Receive_ok = 0;//����������ϱ�־
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



