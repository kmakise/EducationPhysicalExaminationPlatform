#ifndef __TASK06MAIN_H
#define __TASK06MAIN_H

typedef enum
{
	DISTANCE,	//+测距					4 	u8
	PRESSURE,	//+压力					5 	u16
	HRTIMES,	//+心率					7 	u8
	SPO2PCT,	//+血氧饱和度		8		f32
	HBDATA1,	//+血氧波形1		12	f32
	HBDATA2,	//+血氧波形2		16	f32
	HMTEMP,		//+体温					20	f32	
	SMOKING,	//+烟雾					24  u8
	ENVTEMP,  //+温度					25	u8
	ENVHUMT,	//+湿度					26	u8
	LIGHTPW,	//+光照					27	u16

}DataTypedef;


void setDataToUpdate(DataTypedef dt,void * data);

void Task06Main(void);

#endif /*__TASK06MAIN_H*/


