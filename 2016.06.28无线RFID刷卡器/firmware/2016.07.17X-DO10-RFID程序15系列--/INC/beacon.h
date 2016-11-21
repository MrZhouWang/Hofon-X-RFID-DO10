#ifndef _BEACON_H_
#define _BEACON_H_
#include "include.h"

//灯塔控制输出6路
sbit CNTRL1       = P0^2;		//第一路灯塔输出		 
sbit CNTRL2       = P0^3;		//第二路灯塔输出		  
sbit CNTRL3       = P1^0;		//第三路灯塔输出		  
sbit CNTRL4       = P1^1;		//第四路灯塔输出		  
sbit CNTRL5       = P1^2;		//第五路灯塔输出		  
sbit CNTRL6       = P1^3;		//第六路灯塔输出		  
sbit CNTRL7       = P1^4;		//第七路灯塔输出		 
sbit CNTRL8       = P1^5;		//第八路灯塔输出		 
sbit CNTRL9       = P1^6;		//第九路灯塔输出		  
sbit CNTRL10      = P1^7;		//第十路灯塔输出		 

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
	volatile UINT8	led_status;				//亮	或者	灭
	volatile UINT16	led_flash_period; 		//周期长度
	volatile UINT8	led_switch_ratio;		//亮灭比例，0常亮
	volatile UINT8	led_status_cur;			//当前的亮灭状态，用以控制闪烁
	volatile UINT16 	led_flash_count;		//闪烁控制时间统计
}LIGHTTOWER_CTL;
extern LIGHTTOWER_CTL LightTowerCtl[10];







extern UINT16 GetLightTowerOnTime(int LightTowIndex);
extern UINT16 GetLightTowerOffTime(int LightTowIndex);
extern void CMD_LigheTower_flash(void);
#endif
