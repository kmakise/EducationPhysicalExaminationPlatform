#include "usart.h"
#include "string.h"

void Usart_Int(uint32_t BaudRatePrescaler)
{
	 SCON  = 0x50;//1λ��ʼλ��8λ����λ��1λֹͣλ���첽��ʹ�ܽ���
	 TMOD |= 0x20;//��ʱ��1��������ʽ2��8λ�Զ���װ����ֵ
	//��9600��28800/9600=3,TH1=253=FD��
     TH1   =256-(28800/BaudRatePrescaler); 
	 TL1   =256-(28800/BaudRatePrescaler);
     TCON |=0x40;//������ʱ��1
	 IE   |=0x90;//�򿪴����жϣ�MCU���ж�
}
//����һ���ֽ�
uint8_t send_ok=0,Receive_ok=0,TEMP_data[9]={0};
void USART_send_byte(uint8_t Tx_data)
{
	while(send_ok);//�ȴ����ͻ���Ϊ��
 	SBUF = Tx_data;
	send_ok=1;//�����־��1
}
//����Length-1������+1�������ۼӺ�
void USART_Send(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		if(i<(Length-1))
		Buffer[Length-1]+=Buffer[i];
		USART_send_byte(Buffer[i++]);
	}
}
//����һ֡����
void send_out(int16_t *Data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[13],i=0,k=0;
	memset(TX_DATA,0,(2*length+5));//��շ�������
	TX_DATA[i++]=0X5A;//֡ͷ
	TX_DATA[i++]=0X5A;//֡ͷ
	TX_DATA[i++]=send;//�����ֽ�
	TX_DATA[i++]=2*length;//���ݸ���
	for(k=0;k<length;k++)//�������ݵ�����TX_DATA����
	{
		TX_DATA[i++]=(uint16_t)Data[k]>>8;
		TX_DATA[i++]=(uint16_t)Data[k];
	}
	USART_Send(TX_DATA,(2*length+5));//����һ֡����	
}



