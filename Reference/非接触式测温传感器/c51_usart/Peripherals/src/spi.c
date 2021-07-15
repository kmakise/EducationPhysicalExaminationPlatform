#include "spi.h"
void Spi_Int(void)
{
	CS=1;
	SCK=1;
	MOSI=1;
	MISO=1;	
}
/*
1��
Ҫ��spi�ٶ�С��400Khz���ó�����õľ���Ϊ11.0592M�����������Ϊ
12/11.0592us��Spi_RW����һbit���ݺͽ���һbit���ݹ���5�����
����һ�����һ���������ڣ�5�������ҪԼ5.4us����1000000/5.4ԼΪ185K
С��400Khz������Ҫ��
2��
spi��CPOL=1����ʱ�ӿ���״̬Ϊ�ߵ�ƽ
3��
spi��CPHA=1����ʱ����λΪ1���ڶ���ʱ�������ز���
4:
��ΪCPIO=1��CPHA=1�����Ե�һ�����������½��أ��ڶ���������Ϊ�����أ�
���ڵ�һ���½���ʱ�������������MOSI��ʼ������ӻ���ʵҲ���ڵ�һ����
����׼����MOSI�������������½��غ�һ��ʱ��MISO���Բ�������������ǰ������
*/

uint8_t Spi_RW(uint8_t Data)
{
	unsigned char i;
	SCK = 1;
   	for(i=0;i<8;i++) 
   	{ 
	 MOSI=(Data & 0x80);/*д 1 bit���Ӹ�����д��ʱ��ǰ�����*/ 
	 Data = (Data << 1); //����������һλ����������
	 SCK = 0;  //ʱ�����ͣ���һ�������أ���ʱ���ݻ�δ���ӻ�����  
     Data |= MISO; //��MISO��ȡ1 bit,�����Data���λ ��ʱ�Ӻ��ز��� 
	 SCK = 1;/*CPIO=1���ڶ���������*/      		  	
	}
    return(Data); 
}
void Spi_write_buf(uint8_t add, uint8_t *pBuf, uint8_t num)
{
	uint8_t i=0;
	CS=0;
	Spi_RW(add);
	while(i<num)
	Spi_RW(pBuf[i++]);
	CS=1;
}

void Spi_read_buf(uint8_t add, uint8_t *pBuf, uint8_t num)
{
	uint8_t i=0;
	CS=0;
	Spi_RW(add);
	while(i<num)
    {
	pBuf[i++]=Spi_RW(0);
    }
	CS=1;
}
