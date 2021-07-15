#include "exti.h"
uint8_t ACM_BUF[36]={0},BIT=0;
/*
input:n;�ⲿ�ж�Դ���ã�0-�ⲿ�ж�0��1-�ⲿ�ж�1
input:Mode���ⲿ�жϴ�����ʽ��0-�͵�ƽ������1-�½��ش���
*/
void Exti_Int(bit n,uint8_t Mode)
{
	if (n)
	{
	  TCON|=(uint8_t)(Mode<<2);//�ⲿ�ж�1�жϷ�ʽ����
	  IE  |=(uint8_t)(0x80|(1<<2));//�����ⲿ�ж�1�жϣ��������ж�
	}
	else
	{
	  TCON|=(uint8_t)Mode;//�ⲿ�ж�0�жϷ�ʽ����
	  IE  |=(uint8_t)(0x80|1);//�����ⲿ�ж�0�жϣ��������ж�
	}
}
