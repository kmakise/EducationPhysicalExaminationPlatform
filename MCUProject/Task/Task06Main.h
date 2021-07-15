#ifndef __TASK06MAIN_H
#define __TASK06MAIN_H

typedef enum
{
	DISTANCE,	//+���					4 	u8
	PRESSURE,	//+ѹ��					5 	u16
	HRTIMES,	//+����					7 	u8
	SPO2PCT,	//+Ѫ�����Ͷ�		8		f32
	HBDATA1,	//+Ѫ������1		12	f32
	HBDATA2,	//+Ѫ������2		16	f32
	HMTEMP,		//+����					20	f32	
	SMOKING,	//+����					24  u8
	ENVTEMP,  //+�¶�					25	u8
	ENVHUMT,	//+ʪ��					26	u8
	LIGHTPW,	//+����					27	u16

}DataTypedef;


void setDataToUpdate(DataTypedef dt,void * data);

void Task06Main(void);

#endif /*__TASK06MAIN_H*/


