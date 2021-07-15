#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
 #include "LED.h"
 /*
Keil: MDK5.10.0.2
MCU:stm32f103c8
硬件接法：
GY-MCU90615---STM32
1、GY-MCU90615_RX---STM32_TX,STM32复位将发送A5 45 EA给模块
2、STM32_TX---FT232,STM32将数据上传给上位机
3、GY-MCU90615_TX---STM32_RX，接收模块温度数据
软件说明:
该程序采用串口方式获取模块角度数据，波特率115200

联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1

注：中断函数位于stm32f10x_it.c
*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_X;
  
  /* 4个抢占优先级，4个响应优先级 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /*抢占优先级可打断中断级别低的中断*/
	/*响应优先级按等级执行*/
	NVIC_X.NVIC_IRQChannel = USART1_IRQn;//中断向量
  NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
  NVIC_X.NVIC_IRQChannelSubPriority = 0;//响应优先级
  NVIC_X.NVIC_IRQChannelCmd = ENABLE;//使能中断响应
  NVIC_Init(&NVIC_X);
}
void send_com(u8 data)
{
	u8 bytes[3]={0};
	bytes[0]=0xa5;
	bytes[1]=data;//功能字节
	USART_Send(bytes,3);//发送帧头、功能字节、校验和
}
int main(void)
{
	float TO=0,TA=0;
  u8 sum=0,i=0;
	int16_t data[2]={0};
	delay_init(72);
	NVIC_Configuration();
	Usart_Int(115200);
	delay_ms(1);//等待模块初始化完成
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
				data[0]=(TEMP_data[4]<<8)|TEMP_data[5];//放大100倍的物体温度
				TA=(float)((TEMP_data[6]<<8)|TEMP_data[7])/100;//得到真实温度
				data[1]=(TEMP_data[6]<<8)|TEMP_data[7];//放大100倍的环境温度
				send_out(data,2,0x45);//上传给上位机
			}
			Receive_ok=0;//处理数据完毕标志
		}
	}
}
