#define LED_GLOBAL
#include "include.h"
UINT8 address[3];
UINT8 baudrate;
UINT8 ucAddr[3];                        //备份地址
UINT8 ucBaud;                           //备份波特率

UINT8 FinishedLED = LED_NONE;			      //拣货完成后，清零前，指示灯亮的颜色
UINT8 WtTime = 5;						            //一对多应用时，进度条等待时间
UINT8 FnDefine = 0;						          //Fn功能键定义	0-零拣货	1-满箱
UINT8 DispMode = 0;						          //显示模式	0-任务模式	1-区域信息模式


/////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 获取灯亮时间，单位ms
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
UINT16 GetLedOnTime()
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case LED_FLASH_MODE_ALWAYS	:						                   //常亮模式，不会调用此函数
		break;			
	case LED_FLASH_MODE_1V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V2:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V5:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V10:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_2V1:
		unRet = led_flash_period*2;
		break;
	case LED_FLASH_MODE_5V1:
		unRet = led_flash_period*5;
		break;
	case LED_FLASH_MODE_10V1:
		unRet = led_flash_period*10;
		break;
	default:
		break;
	}
	return unRet;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 获取灯灭时间，单位ms
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
UINT16 GetLedOffTime()                        
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case LED_FLASH_MODE_ALWAYS:					                            //常亮模式，不会调用此函数
		break;				
	case LED_FLASH_MODE_1V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_2V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_5V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_10V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V2:
		unRet = led_flash_period*2;
		break;
	case LED_FLASH_MODE_1V5:
		unRet = led_flash_period*5;
		break;
	case LED_FLASH_MODE_1V10:
		unRet = led_flash_period*10;
		break;
	default:
		break;
	}
	return unRet;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RGB点亮及闪烁控制 
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void CMD_led_flash(void)
{
	if(RC522_rFlag == ON)
	{
  		led_light_ctrl(LED_GREEN);			          // 仅点亮绿色
  		return;
	}
	if(led_flash_NO != LED_NONE)
    {


		if(led_flash_mode == LED_FLASH_MODE_ALWAYS)	//常亮显示
		{
			led_light_ctrl(led_flash_NO);			        // 仅点亮对应颜色
		}
		else                                        //闪烁显示
		{
			
			if(led_flash_status == 0)                 //当前灯为灭状态
			{
				
				if(sys.led_flash_count == 0)            //灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				{
					led_light_ctrl(led_flash_NO);			    // 仅点亮对应颜色
					led_flash_status = 1;	                //切换到灯亮状态
					sys.led_flash_count = GetLedOnTime();
				}
			}
			
			else                                      //当前为亮灯
			{
				
				if(sys.led_flash_count == 0)            //灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				{
					led_light_ctrl(LED_NONE);
					led_flash_status = 0;	                //切换到灯亮状态
					sys.led_flash_count = GetLedOffTime();
				}
			}
		}
	}
	else                                          //当前不显示
	{
		led_light_ctrl(LED_NONE);	
	}
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 给当前显示led供电 
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////

void led_light_ctrl(UINT8 ucValue)
{
	switch (ucValue)
	{
		case LED_RED:  		LED_G_OFF; LED_B_OFF;  LED_R_ON;  break;		
		case LED_GREEN:		LED_R_OFF; LED_B_OFF;  LED_G_ON;  break;
		case LED_BLUE:		LED_R_OFF; LED_G_OFF;  LED_B_ON;  break;
		
		case LED_RG:  		LED_B_OFF; LED_R_ON;   LED_G_ON;  break;		
		case LED_RB:		LED_G_OFF; LED_R_ON;   LED_B_ON;  break;
		case LED_GB:		LED_R_OFF; LED_G_ON;   LED_B_ON;  break;
		case LED_RGB:		LED_R_ON;  LED_G_ON;   LED_B_ON;  break;			
		default:			LED_R_OFF; LED_G_OFF; LED_B_OFF;break;
	}
}



void Display_Clear(void)                //清除命令(地址清除时、广播清除时、显示清除时)
{
	
	led_flash_NO = LED_NONE;              //指示灯清除
	
	BEEP_OFF;                             //蜂鸣器清除
}
/*******************************************************************************
	 					开机自检测程序
    input:   none     
    output:  none
*******************************************************************************/ 
void led_on()
{
	  LED_R_ON;
  	LED_G_ON;
    LED_B_ON;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : LED亮 
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void led_off()
{
	 LED_R_OFF;
	 LED_G_OFF;
   LED_B_OFF;
}

/*******************************************************************************
								END					
*******************************************************************************/

