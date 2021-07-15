#include <reg52.h>
#include "usart.h"
#include "iic.h"  
/*
Keil:uVision2
MCU:STC90C516RD+
硬件接法：
GY-MCU90615---STM32
1、GY-MCU90615_RX---STM32_TX,STM32复位将发送A5 45 EA给模块
2、STM32_TX---FT232,STM32将数据上传给上位机
3、GY-MCU90615_TX---STM32_RX，接收模块温度数据
软件说明:
该程序采用串口方式获取模块角度数据，波特率9600
所以得用上位机先将模块串口设置成9600，然后再进行以上操作：
指令:A5 AE 53,模块需复位生效

注：中断函数位于stc_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
void send_com(u8 datas)
{
	u8 bytes[3]={0};
	bytes[0]=0xa5;
	bytes[1]=datas;//功能字节
	USART_Send(bytes,3);//发送帧头、功能字节、校验和
}
int main(void)
{
	float TO=0,TA=0;
    u8 sum=0,i=0;
	int datas[2]={0};
	Usart_Int(9600);
	send_com(0x45);//发送读方位角指令
 	while(1)
	{
		if(Receive_ok)//串口接收完毕
		{
			for(sum=0,i=0;i<(TEMP_data[3]+4);i++)//TEMP_data[3]=4
			sum+=TEMP_data[i];
			if(sum==TEMP_data[i])//校验和判断
			{
				TO=(float)((TEMP_data[4]<<8)|TEMP_data[5])/100;//得到真实温度
				datas[0]=(TEMP_data[4]<<8)|TEMP_data[5];//放大100倍的物体温度
				TA=(float)((TEMP_data[6]<<8)|TEMP_data[7])/100;//得到真实温度
				datas[1]=(TEMP_data[6]<<8)|TEMP_data[7];//放大100倍的环境温度
				send_out(datas,2,0x45);//上传给上位机
			}
			Receive_ok=0;//处理数据完毕标志
		}
	}
}
