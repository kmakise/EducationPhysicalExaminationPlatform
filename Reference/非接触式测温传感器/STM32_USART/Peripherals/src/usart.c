#include "usart.h"
#include "string.h"
u8 TEMP_data[20]={0},Receive_ok=0;
void Usart_Int(uint32_t BaudRatePrescaler)
{
	GPIO_InitTypeDef GPIO_usartx;
	USART_InitTypeDef Usart_X;
	/////////////////////////////////
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	  //USART1_TX   PA.9
  GPIO_usartx.GPIO_Pin = GPIO_Pin_9;
  GPIO_usartx.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_usartx.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
  GPIO_Init(GPIOA, &GPIO_usartx); 
  //USART1_RX	  PA.10
  GPIO_usartx.GPIO_Pin = GPIO_Pin_10;
  GPIO_usartx.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_usartx); 
	
	Usart_X.USART_BaudRate=BaudRatePrescaler;
	Usart_X.USART_WordLength=USART_WordLength_8b;//8λ���ݸ�ʽ
	Usart_X.USART_StopBits=USART_StopBits_1;//1λֹͣλ
	Usart_X.USART_Parity=USART_Parity_No;
	Usart_X.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	Usart_X.USART_Mode= USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &Usart_X);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�
  USART_Cmd(USART1, ENABLE);
	/////////////////////////////////
}
//����һ���ֽ�����
//input:byte,�����͵�����
void USART1_send_byte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//�ȴ��������
	USART1->DR=byte;	
}
//���Ͷ��ֽ�����
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		USART1_send_byte(Buffer[i++]);
	}
}
//���Ͷ��ֽ�����+У���
void USART_Send(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		if(i<(Length-1))
		Buffer[Length-1]+=Buffer[i];//�ۼ�Length-1ǰ������
		USART1_send_byte(Buffer[i++]);
	}
}
//����һ֡����
void send_out(int16_t *data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[30],i=0,k=0;
	memset(TX_DATA,0,(2*length+5));//���㻺��TX_DATA
	TX_DATA[i++]=0X5A;//֡ͷ
	TX_DATA[i++]=0X5A;//֡ͷ
	TX_DATA[i++]=send;//�����ֽ�
	TX_DATA[i++]=2*length;//���ݸ���
	for(k=0;k<length;k++)//�������ݵ�����TX_DATA
	{
		TX_DATA[i++]=(uint16_t)data[k]>>8;
		TX_DATA[i++]=(uint16_t)data[k];
	}
	USART_Send(TX_DATA,2*length+5);	
}
	void display(int16_t *num,u8 send,u8 count)
{
	  u8 i=0;
		USART1_send_byte(0X0d);
		USART1_send_byte(0X0a);
		USART1_send_byte(send);
	  USART1_send_byte(':');
		for(i=0;i<count;i++)
		{
			if(num[i]<0)
			{
				num[i]=-num[i];
				USART1_send_byte('-');
			}
			else
				USART1_send_byte('+');
		
			USART1_send_byte(0x30|(num[i]/10000));	
			USART1_send_byte(0x30|(num[i]%10000/1000));
			USART1_send_byte(0x30|(num[i]%1000/100));
			//USART_send_byte(0x2e);
			USART1_send_byte(0x30|(num[i]%100/10));
			USART1_send_byte(0x30|(num[i]%10));
			USART1_send_byte(',');	
	}
}
void CHeck(uint8_t *re_data)
{
	
}
