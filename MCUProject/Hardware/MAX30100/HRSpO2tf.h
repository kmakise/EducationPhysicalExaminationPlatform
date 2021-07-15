#ifndef __HRSPO2TF_H
#define __HRSPO2TF_H

#include "stm32f4xx_hal.h"

typedef enum
{
	BLD_NORMAL,		//����
	BLD_ERROR,		//������
	
}BloodState;//ѪҺ״̬

typedef struct
{
	uint16_t 		heart;		//��������
	float 			SpO2;			//Ѫ������
	float 			hb;				//Ѫ�쵰��
	float 			hbo2;			//����Ѫ�쵰��
	
	BloodState	state;		//״̬
	uint8_t   	update;		//��Ϣ���±�־λ
	uint8_t   	display;	//���ݸ��±�־λ
}BloodData;

extern BloodData g_blooddata;

//ѪҺ�����Ϣ����
void blood_data_update(void);
//ѪҺ��Ϣת��
void blood_data_translate(void);

#endif /* __HRSPO2TF_H*/
