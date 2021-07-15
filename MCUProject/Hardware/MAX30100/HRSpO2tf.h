#ifndef __HRSPO2TF_H
#define __HRSPO2TF_H

#include "stm32f4xx_hal.h"

typedef enum
{
	BLD_NORMAL,		//正常
	BLD_ERROR,		//侦测错误
	
}BloodState;//血液状态

typedef struct
{
	uint16_t 		heart;		//心率数据
	float 			SpO2;			//血氧数据
	float 			hb;				//血红蛋白
	float 			hbo2;			//氧合血红蛋白
	
	BloodState	state;		//状态
	uint8_t   	update;		//信息更新标志位
	uint8_t   	display;	//数据更新标志位
}BloodData;

extern BloodData g_blooddata;

//血液检测信息更新
void blood_data_update(void);
//血液信息转换
void blood_data_translate(void);

#endif /* __HRSPO2TF_H*/
