#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
 #include "LED.h"
 /*
Keil: MDK5.10.0.2
MCU:stm32f103c8
Ӳ���ӷ���
GY-MCU90615---STM32
1��GY-MCU90615_RX---STM32_TX,STM32��λ������A5 45 EA��ģ��
2��STM32_TX---FT232,STM32�������ϴ�����λ��
3��GY-MCU90615_TX---STM32_RX������ģ���¶�����
���˵��:
�ó�����ô��ڷ�ʽ��ȡģ��Ƕ����ݣ�������115200

��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1

ע���жϺ���λ��stm32f10x_it.c
*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_X;
  
  /* 4����ռ���ȼ���4����Ӧ���ȼ� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /*��ռ���ȼ��ɴ���жϼ���͵��ж�*/
	/*��Ӧ���ȼ����ȼ�ִ��*/
	NVIC_X.NVIC_IRQChannel = USART1_IRQn;//�ж�����
  NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
  NVIC_X.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
  NVIC_X.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж���Ӧ
  NVIC_Init(&NVIC_X);
}
void send_com(u8 data)
{
	u8 bytes[3]={0};
	bytes[0]=0xa5;
	bytes[1]=data;//�����ֽ�
	USART_Send(bytes,3);//����֡ͷ�������ֽڡ�У���
}
int main(void)
{
	float TO=0,TA=0;
  u8 sum=0,i=0;
	int16_t data[2]={0};
	delay_init(72);
	NVIC_Configuration();
	Usart_Int(115200);
	delay_ms(1);//�ȴ�ģ���ʼ�����
	send_com(0x45);//���Ͷ���λ��ָ��
	while(1)
	{
		if(Receive_ok)//���ڽ������
		{
			for(sum=0,i=0;i<(TEMP_data[3]+4);i++)//TEMP_data[3]=4
			sum+=TEMP_data[i];
			if(sum==TEMP_data[i])//У����ж�
			{
				TO=(float)((TEMP_data[4]<<8)|TEMP_data[5])/100;//�õ���ʵ�¶�
				data[0]=(TEMP_data[4]<<8)|TEMP_data[5];//�Ŵ�100���������¶�
				TA=(float)((TEMP_data[6]<<8)|TEMP_data[7])/100;//�õ���ʵ�¶�
				data[1]=(TEMP_data[6]<<8)|TEMP_data[7];//�Ŵ�100���Ļ����¶�
				send_out(data,2,0x45);//�ϴ�����λ��
			}
			Receive_ok=0;//����������ϱ�־
		}
	}
}
