#include "exti.h"
#include <math.h>
//B�˿��ⲿ�������ж�����
//input��GPIO_Pin_x Ҫ���õ��ж�����
void Exti_Int(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,uint8_t GPIO_PortSource)
{
	GPIO_InitTypeDef GPIO_Exti;
	EXTI_InitTypeDef EXTI_X;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	/*�ж�����I/O����*/
	GPIO_Exti.GPIO_Pin=GPIO_Pin_x;
	GPIO_Exti.GPIO_Mode=GPIO_Mode_IPD;//��������
	GPIO_Init(GPIOx, &GPIO_Exti);
	
	/*���˿�B���ж���ӳ������*/
	GPIO_EXTILineConfig(GPIO_PortSource, sqrt(GPIO_Pin_x));
	/*�����ж���*/
	EXTI_X.EXTI_Line=(uint32_t)GPIO_Pin_x;
	EXTI_X.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_X.EXTI_Trigger=EXTI_Trigger_Rising;//�������ж�
	EXTI_X.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_X);
}
