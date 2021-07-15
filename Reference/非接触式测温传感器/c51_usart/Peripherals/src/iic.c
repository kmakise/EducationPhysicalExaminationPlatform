#include "iic.h"
void delay_1us(u8 x)//������ʱ
{
	while(x--);
}
/////////////////////////////
////////IIC��ʼ����//////////
/*
IIC��ʼ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽ��ɵ͵�ƽ����һ���½��أ�Ȼ��SCL����
*/
u8 I2C_Start(void)
{
		SDA=1; 
		delay_1us(1);	//��ʱ��֤ʱ��Ƶ�ʣ��Ա�ӻ�ʶ��
		SCL=1;
		delay_1us(1);//��ʱ��֤ʱ��Ƶ�ʣ��Ա�ӻ�ʶ��
		if(!SDA) return 0;//SDA��Ϊ�͵�ƽ������æ,�˳�
		SDA=0;   //SCL���ڸߵ�ƽ��ʱ��SDA����
		delay_1us(1);
	  if(SDA) return 0;//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
		SCL=0;
	  delay_1us(1);
	  return 1;
}
//**************************************
//IICֹͣ�ź�
/*
IICֹͣ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ����һ��������
*/
//**************************************
void I2C_Stop(void)
{
    SDA=0;
	SCL=0;
	delay_1us(1);
	SCL=1;
	delay_1us(1);
	SDA=1;//��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ             //��ʱ
}
//**************************************
//IIC����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
/*
Ӧ�𣺵��ӻ����յ����ݺ�����������һ���͵�ƽ�ź�
��׼����SDA��ƽ״̬����SCL�ߵ�ƽʱ����������SDA
*/
//**************************************
void I2C_SendACK(u8 i)
{
    if(1==i)
	  SDA=1;	             //׼����SDA��ƽ״̬����Ӧ��
    else 
	  SDA=0;  						//׼����SDA��ƽ״̬��Ӧ�� 	
	  SCL=1;                    //����ʱ����
    delay_1us(1);                 //��ʱ
    SCL=0;                  //����ʱ����
    delay_1us(1);    
} 
///////�ȴ��ӻ�Ӧ��////////
/*
������(����)������һ�����ݺ󣬵ȴ��ӻ�Ӧ��
���ͷ�SDA���ôӻ�ʹ�ã�Ȼ��ɼ�SDA״̬
*/
/////////////////
u8 I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	u8 i=0;
	SDA=1;	        //�ͷ�SDA
	SCL=1;         //SCL���߽��в���
	while(SDA)//�ȴ�SDA����
	{
		i++;      //�ȴ�����
		if(i==2)//��ʱ����ѭ��
		break;
	}
	if(SDA)//�ٴ��ж�SDA�Ƿ�����
	{
		SCL=0; 
		return 0;//�ӻ�Ӧ��ʧ�ܣ�����0
	}
    delay_1us(1);//��ʱ��֤ʱ��Ƶ��
	SCL=0;
	delay_1us(1); //��ʱ��֤ʱ��Ƶ��
	return 1;//�ӻ�Ӧ��ɹ�������1
}
//**************************************
//��IIC���߷���һ���ֽ�����
/*
һ���ֽ�8bit,��SCL�͵�ƽʱ��׼����SDA��SCL�ߵ�ƽʱ���ӻ�����SDA
*/
//**************************************
void I2C_SendByte(u8 dat)
{
  u8 i;
	SCL=0;//SCL���ͣ���SDA׼��
  for (i=0; i<8; i++)         //8λ������
  {
	if(dat&0x80)//SDA׼��
	 SDA=1;  
	else 
	 SDA=0;
    SCL=1;                //����ʱ�ӣ����ӻ�����
    delay_1us(1);        //��ʱ����IICʱ��Ƶ�ʣ�Ҳ�Ǹ��ӻ������г���ʱ��
    SCL=0;                //����ʱ�ӣ���SDA׼��
    delay_1us(1); 		  //��ʱ����IICʱ��Ƶ��
	dat <<= 1;          //�Ƴ����ݵ����λ  
  }					 
}
//**************************************
//��IIC���߽���һ���ֽ�����
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
    SDA=1;//�ͷ�SDA�����ӻ�ʹ��
    delay_1us(1);         //��ʱ���ӻ�׼��SDAʱ��            
    for (i=0; i<8; i++)         //8λ������
    { 
		dat <<= 1;
			
        SCL=1;                //����ʱ���ߣ������ӻ�SDA
     
		  if(SDA) //������    
		   dat |=0x01;      
       delay_1us(1);     //��ʱ����IICʱ��Ƶ��		
       SCL=0;           //����ʱ���ߣ�������յ�������
       delay_1us(1);   //��ʱ���ӻ�׼��SDAʱ��
    } 
    return dat;
}
//**************************************
//��IIC�豸д��һ���ֽ�����
//**************************************
u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 datas)
{
	  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return 0;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
   
		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
   
		I2C_SendByte(datas);       //�ڲ��Ĵ������ݣ�
	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
		
		I2C_Stop();   //����ֹͣ�ź�
		
		return 1;
}
//**************************************
//��IIC�豸��ȡһ���ֽ�����
//**************************************
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
 if(I2C_Start()==0)  //��ʼ�ź�
	{I2C_Stop(); return 0;}          
	 
	I2C_SendByte(Slave_Address);    //�����豸��ַ+д�ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ
 	if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	
	if(I2C_Start()==0)  //��ʼ�ź�
			{I2C_Stop(); return 0;}            

	I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return 0;}
	
	while(length-1)
	{
		*REG_data++=I2C_RecvByte();       //�����Ĵ�������
		I2C_SendACK(0);               //Ӧ��
		length--;
	}
	*REG_data=I2C_RecvByte();  
	I2C_SendACK(1);     //����ֹͣ�����ź�
	I2C_Stop();                    //ֹͣ�ź�
	return 1;
}

