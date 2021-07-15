/**
  ******************************************************************************
  * File Name          : DHT11.c
  * Description        : Code for DHT11 Template and Hum Sensor Driver
  ******************************************************************************
  * @author  kmakise-zdd171
  * @version V1.0.0
  * @date    2021-1-18
  * @brief   DHT11 Driver
	******************************************************************************
  * @attention
  *
  * All rights reserved.
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "dht11.h"


void delay_us(uint16_t time)
{    
   uint16_t i=0;  
   while(time--)
   {
      i=41;  //�Լ�����
      while(i--) ;    
   }
}

//��λDHT11
void DHT11_Rst(void)	   
{                 
  DHT11_OUT_L ; 	//����DQ
  HAL_Delay(20);    	//��������18ms
  DHT11_OUT_H; 	//DQ=1 
	delay_us(30);     	//��������20~40us
}
//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
  while (DHT11_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
//��DHT11��ȡһ��λ
//����ֵ��1/0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHT11_IN)return 1;
	else return 0;		   
}
//��DHT11��ȡһ���ֽ�
//����ֵ������������
uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
uint16_t dht11_read_data(uint8_t *temp,uint8_t *humi)
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}