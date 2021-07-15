#include "spi.h"
/*
input:BaudRatePrescaler
#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0000)
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018)
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028)
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030)
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038)
*/
void Spi1_Int(uint16_t BaudRatePrescaler)
{
	GPIO_InitTypeDef GPIO_spix;
	SPI_InitTypeDef  SPI_X;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
	/**GPIO_Pin_5-SCK******GPIO_Pin_6-MISO*****GPIO_Pin_7-MOSI******/
	GPIO_spix.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_spix.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_spix.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_spix);
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	/*A2-NSS*/
	GPIO_spix.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_spix.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_spix.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_spix);
	GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3);
	
	SPI_X.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//˫��˫��ȫ˫��
	SPI_X.SPI_Mode=SPI_Mode_Master;//��ģʽ
	SPI_X.SPI_DataSize=SPI_DataSize_8b;//����λ��С8λ
	SPI_X.SPI_CPOL=SPI_CPOL_High;//ʱ�Ӽ��ԣ�����ʱʱ��Ϊ�ߵ�ƽ
	SPI_X.SPI_CPHA=SPI_CPHA_2Edge;//ʱ����λ����ʱ�ӵĵ�2�������زɼ�����
	SPI_X.SPI_NSS=SPI_NSS_Soft;//NSS�ź���������� 
	SPI_X.SPI_BaudRatePrescaler=BaudRatePrescaler;//256��Ƶ�����ʣ�RCC_APB2=72M��72M/256=281.25KHz
	SPI_X.SPI_FirstBit=SPI_FirstBit_MSB;//���ݸ�λ��ǰ
	SPI_X.SPI_CRCPolynomial=7;//CRC��λֵ
	SPI_Init(SPI1, &SPI_X);
	SPI_Cmd(SPI1, ENABLE);
}
//spi��д�ײ�����
//spiд���̣����ͻ���Ĵ�������--��λ�Ĵ���
//spi�����̣���λ�Ĵ�������--���ջ���Ĵ���
//TxData�����������
 u8 Spi_RW(u8 TxData)
{		
	
  /*�����ͻ���Ĵ���Ϊ��ʱ��1������ȴ�*/	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				  
	SPI_I2S_SendData(SPI1, TxData); //����һ�����ݵ����ͻ���Ĵ���

 /*�����ջ���Ĵ����ǿ�ʱ��1������ȴ�*/	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	 						    
	return SPI_I2S_ReceiveData(SPI1); //�ӽ��ջ���Ĵ�����ȡһ������				    
}
//д����
/*
add:�Ĵ�����ַ
*pBuf��д������ݻ���
num��д������ݸ���
*/
void Spi_write_buf(uint8_t add, uint8_t *pBuf, uint8_t num,uint16_t GPIO_Pin_X)
{
	uint8_t i=0;
	GPIO_ResetBits(GPIOA,GPIO_Pin_X);//NSS����ѡ������
	Spi_RW(add);
	while(i<num)
	Spi_RW(pBuf[i++]);
	GPIO_SetBits(GPIOA,GPIO_Pin_X);//NSS�����ͷ�����
}

//������
/*
add:�Ĵ�����ַ
*pBuf����ȡ����
num����ȡ�����ݸ���
*/
void Spi_read_buf(uint8_t add, uint8_t *pBuf, uint8_t num,uint16_t GPIO_Pin_X)
{
	uint8_t i=0;
	GPIO_ResetBits(GPIOA,GPIO_Pin_X);//NSS����ѡ������
	Spi_RW(add);
	while(i<num)
	pBuf[i++]=Spi_RW(0);
	GPIO_SetBits(GPIOA,GPIO_Pin_X);//NSS�����ͷ�����
}
