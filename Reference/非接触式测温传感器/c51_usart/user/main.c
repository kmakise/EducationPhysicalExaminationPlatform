#include <reg52.h>
#include "usart.h"
#include "iic.h"  
/*
Keil:uVision2
MCU:STC90C516RD+
Ӳ���ӷ���
GY-MCU90615---STM32
1��GY-MCU90615_RX---STM32_TX,STM32��λ������A5 45 EA��ģ��
2��STM32_TX---FT232,STM32�������ϴ�����λ��
3��GY-MCU90615_TX---STM32_RX������ģ���¶�����
���˵��:
�ó�����ô��ڷ�ʽ��ȡģ��Ƕ����ݣ�������9600
���Ե�����λ���Ƚ�ģ�鴮�����ó�9600��Ȼ���ٽ������ϲ�����
ָ��:A5 AE 53,ģ���踴λ��Ч

ע���жϺ���λ��stc_it.c
��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
void send_com(u8 datas)
{
	u8 bytes[3]={0};
	bytes[0]=0xa5;
	bytes[1]=datas;//�����ֽ�
	USART_Send(bytes,3);//����֡ͷ�������ֽڡ�У���
}
int main(void)
{
	float TO=0,TA=0;
    u8 sum=0,i=0;
	int datas[2]={0};
	Usart_Int(9600);
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
				datas[0]=(TEMP_data[4]<<8)|TEMP_data[5];//�Ŵ�100���������¶�
				TA=(float)((TEMP_data[6]<<8)|TEMP_data[7])/100;//�õ���ʵ�¶�
				datas[1]=(TEMP_data[6]<<8)|TEMP_data[7];//�Ŵ�100���Ļ����¶�
				send_out(datas,2,0x45);//�ϴ�����λ��
			}
			Receive_ok=0;//����������ϱ�־
		}
	}
}
