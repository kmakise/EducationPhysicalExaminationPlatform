#include "delay.h"
static u8  Times_us=0;//1us��Ҫ�ƵĴ���
static u16 Times_ms=0;//1ms��Ҫ�ƵĴ���
//SYSCLK=72M,SYSCLK/8=9M
void delay_init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	Times_us=SYSCLK/8;	//Times_us=9��	    
	Times_ms=(u16)Times_us*1000;//Times_ms=9000��
}
//SYSCLK=72M,SYSCLK/8=9M,����һ����Ϊ9��֮һ ΢�룬1us��Ҫ����9��
//����SysTick->LOAD=nus*SYSCLK/8=nus*9=Count
//Count<2^24��Max_Count=16777215
//���nus=Max_Count/9=16777215/9=1864135us
//�����nusΪ1864135
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=(nus*Times_us); //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 	
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//SYSCLK=72M,SYSCLK/8=9M,1΢�������9�Σ���1���������9000��
//����SysTick->LOAD=nms*9000=Count
//Count<2^24��Max_Count=16777215
//���nms=Max_Count/9000=16777215/9000=1864.135ms��
//�����nms=1864
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(nms*Times_ms);//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           
	SysTick->CTRL=0x01 ;   
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));  
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;       	    
}




