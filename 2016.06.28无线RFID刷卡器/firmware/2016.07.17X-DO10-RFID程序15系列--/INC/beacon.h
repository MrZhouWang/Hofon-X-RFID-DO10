#ifndef _BEACON_H_
#define _BEACON_H_
#include "include.h"

//�����������6·
sbit CNTRL1       = P0^2;		//��һ·�������		 
sbit CNTRL2       = P0^3;		//�ڶ�·�������		  
sbit CNTRL3       = P1^0;		//����·�������		  
sbit CNTRL4       = P1^1;		//����·�������		  
sbit CNTRL5       = P1^2;		//����·�������		  
sbit CNTRL6       = P1^3;		//����·�������		  
sbit CNTRL7       = P1^4;		//����·�������		 
sbit CNTRL8       = P1^5;		//�ڰ�·�������		 
sbit CNTRL9       = P1^6;		//�ھ�·�������		  
sbit CNTRL10      = P1^7;		//��ʮ·�������		 

#define CNTRL1_ON	(CNTRL1 = 1)
#define CNTRL1_OFF	(CNTRL1 = 0)
#define CNTRL2_ON	(CNTRL2 = 1)
#define CNTRL2_OFF	(CNTRL2 = 0)
#define CNTRL3_ON	(CNTRL3 = 1)
#define CNTRL3_OFF	(CNTRL3 = 0)
#define CNTRL4_ON	(CNTRL4 = 1)
#define CNTRL4_OFF	(CNTRL4 = 0)
#define CNTRL5_ON	(CNTRL5 = 1)
#define CNTRL5_OFF	(CNTRL5 = 0)
#define CNTRL6_ON	(CNTRL6 = 1)
#define CNTRL6_OFF	(CNTRL6 = 0)
#define CNTRL7_ON	(CNTRL7 = 1)
#define CNTRL7_OFF	(CNTRL7 = 0)
#define CNTRL8_ON	(CNTRL8 = 1)
#define CNTRL8_OFF	(CNTRL8 = 0)
#define CNTRL9_ON	(CNTRL9 = 1)
#define CNTRL9_OFF	(CNTRL9 = 0)
#define CNTRL10_ON	(CNTRL10 = 1)
#define CNTRL10_OFF	(CNTRL10 = 0)

typedef struct 
{
	volatile UINT8	led_status;				//��	����	��
	volatile UINT16	led_flash_period; 		//���ڳ���
	volatile UINT8	led_switch_ratio;		//���������0����
	volatile UINT8	led_status_cur;			//��ǰ������״̬�����Կ�����˸
	volatile UINT16 	led_flash_count;		//��˸����ʱ��ͳ��
}LIGHTTOWER_CTL;
extern LIGHTTOWER_CTL LightTowerCtl[10];







extern UINT16 GetLightTowerOnTime(int LightTowIndex);
extern UINT16 GetLightTowerOffTime(int LightTowIndex);
extern void CMD_LigheTower_flash(void);
#endif
