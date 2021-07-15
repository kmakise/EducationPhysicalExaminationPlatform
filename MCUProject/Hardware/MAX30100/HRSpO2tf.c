/**
  ******************************************************************************
  * File Name          : HRSpO2tf.c
  * Description        : Code for IR R data to HR and SpO2 Calculate
  ******************************************************************************
  * @author  kmakise-zdd171
  * @version V1.0.0
  * @date    2021-1-18
  * @brief   Ѫ��Ũ�ȼ���
	******************************************************************************
  * @attention
  *
  * All rights reserved.
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "HRSpO2tf.h"
#include "MAX30100.h"
#include "algorithm.h"
#include "math.h"

#include "usart.h"
#include "stdio.h"

/*Global data space ----------------------------------------------------------*/

#define CORRECTED_VALUE			12   			//�궨ѪҺ��������

struct compx FFTBUF1[FFT_N+16];       //FFT������������S[1]��ʼ��ţ����ݴ�С�Լ�����
struct compx FFTBUF2[FFT_N+16];       //FFT������������S[1]��ʼ��ţ����ݴ�С�Լ�����
uint16_t g_fft_index = 0;         	 	//fft��������±�
BloodData g_blooddata = {0};					//ѪҺ���ݴ洢


void test(float data1,float data2)
{
		static uint8_t str[50];
		sprintf((char *)str,"%f,%f\r\n",data1,data2);
		HAL_UART_Transmit_DMA(&huart1,str,sizeof(str));
}

//ѪҺ�����Ϣ����
void blood_data_update(void)
{	
	static DC_FilterData dc1 = {.w = 0,.init = 0,.a = 0.8};
	static DC_FilterData dc2 = {.w = 0,.init = 0,.a = 0.8};
	
	static float data1buf[20];
	static uint8_t data1cur = 0;
	static float data2buf[20];
	static uint8_t data2cur = 0;
	
	uint16_t temp_num=0;
	uint16_t fifo_word_buff[1][2];

	temp_num = max30100_Bus_Read(INTERRUPT_REG);
	
	//��־λ��ʹ��ʱ ��ȡFIFO
	if (INTERRUPT_REG_A_FULL&temp_num)
	{
		//��ȡFIFO
		max30100_FIFO_Read(0x05,fifo_word_buff,1); //read the hr and spo2 data form fifo in reg=0x05
		
		float data1 = dc_filter(fifo_word_buff[0][0],&dc1)+100.0;
		float data2 = dc_filter(fifo_word_buff[0][1],&dc2)+100.0;
		
		data1buf[data1cur] = data1;
		data2buf[data2cur] = data2;
		
		data1 = 0;
		data2 = 0;
		
		for(int i = 0;i < 20;i++)
		{
			data1 += data1buf[i];
			data2 += data2buf[i];
		}

		data1 /= 20;
		data2 /= 20;
		
		data1cur = (data1cur < 19) ? data1cur + 1 : 0;
		data2cur = (data2cur < 19) ? data2cur + 1 : 0;
		
//		fifo_word_buff[0][0] = data1;
//		fifo_word_buff[0][1] = data2;
		
//		test(data1,data2);
		
		g_blooddata.hb = data1 + 50;
		g_blooddata.hbo2 = data2 + 50;
		
		//������д��fft���벢������
		for(int i = 0;i < 1;i++)
		{
			if(g_fft_index < FFT_N)
			{
				FFTBUF1[g_fft_index].real = fifo_word_buff[i][0];
				FFTBUF1[g_fft_index].imag= 0;
				FFTBUF2[g_fft_index].real = fifo_word_buff[i][1];
				FFTBUF2[g_fft_index].imag= 0;
				g_fft_index++;
			}
		}
		
		//��Ϣ���±�־λ
		g_blooddata.update++;
		
	}
}


//ѪҺ��Ϣת��
void blood_data_translate(void)
{	
	//������д�����
	if(g_fft_index>=FFT_N)
	{
		//���ٸ���Ҷ�任
		FFT(FFTBUF1);
		FFT(FFTBUF2);
		
		//��ƽ��
		for(int i = 0;i < FFT_N;i++) 
		{
			FFTBUF1[i].real = sqrtf(FFTBUF1[i].real * FFTBUF1[i].real + FFTBUF1[i].imag * FFTBUF1[i].imag);
			FFTBUF2[i].real = sqrtf(FFTBUF2[i].real * FFTBUF2[i].real + FFTBUF2[i].imag * FFTBUF2[i].imag);
		}
		
		//��ȡ��ֵ�� 10-100��ͨ Ƶ�ʷ�Χ30-292��/����
		uint16_t s1_max_index = find_max_num_index(FFTBUF1, 100);
		uint16_t s2_max_index = find_max_num_index(FFTBUF2, 100);
		
		//���HbO2��Hb�ı仯Ƶ���Ƿ�һ��
		if(s1_max_index == s2_max_index)
		{
			//���ʼ���
			uint16_t Heart_Rate = 60 * SAMPLES_PER_SECOND * 
														s2_max_index / FFT_N;
			
			g_blooddata.heart = Heart_Rate;
			
			//Ѫ����������
			float sp02_num = (FFTBUF1[s1_max_index].real * FFTBUF1[0].real)
											/(FFTBUF2[s1_max_index].real * FFTBUF2[0].real);
			
			sp02_num = sp02_num * SAMPLES_PER_SECOND + CORRECTED_VALUE;
			
			g_blooddata.SpO2 = sp02_num;
			
			//״̬����
			g_blooddata.state = BLD_NORMAL;
			

//			for(int i = 0;i < FFT_N;i++)
//			{
//				static uint8_t str[50];
//				sprintf((char *)str,"%f,%f\r\n\0",FFTBUF1[i].real,FFTBUF2[i].real);
//				HAL_UART_Transmit(&huart1,str,sizeof(str),20);
//			}
//			static uint8_t str[50];
//			sprintf((char *)str,"H:%d,S:%f\r\n",g_blooddata.heart,g_blooddata.SpO2 );
//			HAL_UART_Transmit(&huart1,str,sizeof(str),20);
			
		}
		else //���ݷ����쳣
		{
			g_blooddata.heart = 0;
			g_blooddata.SpO2 	= 0;
			g_blooddata.state = BLD_ERROR;
		}
		
		g_fft_index = 0;
	}
}
