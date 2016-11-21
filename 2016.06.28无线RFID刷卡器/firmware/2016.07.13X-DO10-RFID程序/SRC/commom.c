#define COMMON_GLOBAL
#include "include.h"

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 清除命令(地址清除时、广播清除时、显示清除时)
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void ClearAll(void)
{
	//显示信息清除
	UINT8 i;
	DispInfoCur.SRCodeLen = 0;
	for (i=0; i<LED_NUMBER; i++)
	{
		led[i].num = 0;
		led[i].show = OFF;
	}
	//指示灯清除
	led_flash_NO = LED_NONE;
	//蜂鸣器清除
	 
	//OK信息清除
 	IsOkPressed = FALSE;
	//扫描信息清除
	uart232.rFlag   = OFF;
	uart232.rIndex  = 0;
	//灯塔清除
	LightTowerCtl[0].led_status = OFF;
	LightTowerCtl[1].led_status = OFF;
	LightTowerCtl[2].led_status = OFF;
	LightTowerCtl[3].led_status = OFF;
	LightTowerCtl[4].led_status = OFF;
	LightTowerCtl[5].led_status = OFF;
}
