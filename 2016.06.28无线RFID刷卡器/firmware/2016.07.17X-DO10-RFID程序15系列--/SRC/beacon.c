#include "include.h"
LIGHTTOWER_CTL LightTowerCtl[10];
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 获取灯塔周期内点亮时间，与灯塔熄灭时间形成一周期
// 输入参数 : LightTowIndex：  第几个灯塔
// 返回参数 : 灯塔周期内点亮时间
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
UINT16 GetLightTowerOnTime(int LightTowIndex)
{
	UINT16 unRet = LightTowerCtl[LightTowIndex].led_flash_period;
	switch(LightTowerCtl[LightTowIndex].led_switch_ratio)
	{
	case 0X00:	
		break;				//常亮模式，不会调用此函数
	case 0X01: 
	    unRet = LightTowerCtl[LightTowIndex].led_flash_period/2;
	    break;
	case 0X02:	
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/3;
	    break;
	case 0X03:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/6;	
	    break;
	case 0X04:	
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/11;
		break;
	case 0X05:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/3*2;
		break;
	case 0X06:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/6*5;
		break;
	case 0X07:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/11*10;
		break;
	default:
		break;
	}
	return unRet;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 获取灯塔周期内熄灭时间，与灯塔点亮时间形成一周期
// 输入参数 : LightTowIndex：  第几个灯塔
// 返回参数 : 灯塔周期内熄灭时间
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
UINT16 GetLightTowerOffTime(int LightTowIndex)
{
	UINT16 unRet = LightTowerCtl[LightTowIndex].led_flash_period;
	switch(LightTowerCtl[LightTowIndex].led_switch_ratio)
	{
	case 0X00:										//常亮模式，不会调用此函数
		break;			
	case 0X01:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/2;
		break;
	case 0X05:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/3;
		break;
	case 0X06:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/6;
		break;
	case 0X07:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/11;
		break;
	case 0X02:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/3*2;
		break;
	case 0X03:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/6*5;
		break;
	case 0X04:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/11*10;
		break;
	default:
		break;
	}
	return unRet;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 灯塔亮灭周期等控制，
// 输入参数 : 无
// 返回参数 : 无
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void CMD_LigheTower_flash(void)
{ 
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED1
	if(LightTowerCtl[0].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[0].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL1_ON;
		}
		//闪烁
		else
		{	
			//当前灯为灭状态
			if(LightTowerCtl[0].led_status_cur == OFF)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(LightTowerCtl[0].led_flash_count == 0)
				{
			        CNTRL1_ON;

					LightTowerCtl[0].led_status_cur = ON;
					LightTowerCtl[0].led_flash_count = GetLightTowerOnTime(0);
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(LightTowerCtl[0].led_flash_count == 0)
				{
			        CNTRL1_OFF;

					LightTowerCtl[0].led_status_cur = OFF;
					LightTowerCtl[0].led_flash_count = GetLightTowerOffTime(0);
				}
			}
		}
	}
	else
	{
        CNTRL1_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED2
	if(LightTowerCtl[1].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[1].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL2_ON;
		}
		//闪烁
		else
		{	
			//当前灯为灭状态
			if(LightTowerCtl[1].led_status_cur == OFF)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(LightTowerCtl[1].led_flash_count == 0)
				{
			        CNTRL2_ON;
					LightTowerCtl[1].led_status_cur = ON;
					LightTowerCtl[1].led_flash_count = GetLightTowerOnTime(1);
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(LightTowerCtl[1].led_flash_count == 0)
				{
			        CNTRL2_OFF;

					LightTowerCtl[1].led_status_cur = OFF;
					LightTowerCtl[1].led_flash_count = GetLightTowerOffTime(1);
				}
			}
		}
	}
	else
	{
        CNTRL2_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED3
	if(LightTowerCtl[2].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[2].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL3_ON;
		}
		//闪烁
		else
		{	
			//当前灯为灭状态
			if(LightTowerCtl[2].led_status_cur == OFF)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(LightTowerCtl[2].led_flash_count == 0)
				{
			        CNTRL3_ON;

					LightTowerCtl[2].led_status_cur = ON;
					LightTowerCtl[2].led_flash_count = GetLightTowerOnTime(2);
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(LightTowerCtl[2].led_flash_count == 0)
				{
			        CNTRL3_OFF;

					LightTowerCtl[2].led_status_cur = OFF;
					LightTowerCtl[2].led_flash_count = GetLightTowerOffTime(2);
				}
			}
		}
	}
	else
	{
        CNTRL3_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED4
	if(LightTowerCtl[3].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[3].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL4_ON;
		}
		//闪烁
		else
		{	
			//当前灯为灭状态
			if(LightTowerCtl[3].led_status_cur == OFF)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(LightTowerCtl[3].led_flash_count == 0)
				{
			        CNTRL4_ON;

					LightTowerCtl[3].led_status_cur = ON;
					LightTowerCtl[3].led_flash_count = GetLightTowerOnTime(3);
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(LightTowerCtl[3].led_flash_count == 0)
				{
			        CNTRL4_OFF;

					LightTowerCtl[3].led_status_cur = OFF;
					LightTowerCtl[3].led_flash_count = GetLightTowerOffTime(3);
				}
			}
		}
	}
	else
	{
        CNTRL4_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED5
	if(LightTowerCtl[4].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[4].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL5_ON;
		}
		//闪烁
		else
		{	
			//当前灯为灭状态
			if(LightTowerCtl[4].led_status_cur == OFF)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(LightTowerCtl[4].led_flash_count == 0)
				{
			        CNTRL5_ON;

					LightTowerCtl[4].led_status_cur = ON;
					LightTowerCtl[4].led_flash_count = GetLightTowerOnTime(4);
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(LightTowerCtl[4].led_flash_count == 0)
				{
			        CNTRL5_OFF;

					LightTowerCtl[4].led_status_cur = OFF;
					LightTowerCtl[4].led_flash_count = GetLightTowerOffTime(4);
				}
			}
		}
	}
	else
	{
        CNTRL5_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED6
	if(LightTowerCtl[5].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[5].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL6_ON;
		}
		//闪烁
		else
		{	
			//当前灯为灭状态
			if(LightTowerCtl[5].led_status_cur == OFF)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(LightTowerCtl[5].led_flash_count == 0)
				{
			        CNTRL6_ON;

					LightTowerCtl[5].led_status_cur = ON;
					LightTowerCtl[5].led_flash_count = GetLightTowerOnTime(5);
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(LightTowerCtl[5].led_flash_count == 0)
				{
			        CNTRL6_OFF;

					LightTowerCtl[5].led_status_cur = OFF;
					LightTowerCtl[5].led_flash_count = GetLightTowerOffTime(5);
				}
			}
		}
	}
	else
	{
        CNTRL6_OFF;
	}
/*********************************LED7********************************************/
	if(LightTowerCtl[6].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[6].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL7_ON;
		}
		//闪烁
		else
		{	
			//当前灯为灭状态
			if(LightTowerCtl[6].led_status_cur == OFF)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(LightTowerCtl[6].led_flash_count == 0)
				{
			        CNTRL7_ON;

					LightTowerCtl[6].led_status_cur = ON;
					LightTowerCtl[6].led_flash_count = GetLightTowerOnTime(6);
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(LightTowerCtl[6].led_flash_count == 0)
				{
			        CNTRL7_OFF;

					LightTowerCtl[6].led_status_cur = OFF;
					LightTowerCtl[6].led_flash_count = GetLightTowerOffTime(6);
				}
			}
		}
	}
	else
	{
        CNTRL7_OFF;
	}
/*********************************LED8********************************************/
	if(LightTowerCtl[7].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[7].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL8_ON;
		}
		//闪烁
		else
		{	
			//当前灯为灭状态
			if(LightTowerCtl[7].led_status_cur == OFF)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(LightTowerCtl[7].led_flash_count == 0)
				{
			        CNTRL8_ON;

					LightTowerCtl[7].led_status_cur = ON;
					LightTowerCtl[7].led_flash_count = GetLightTowerOnTime(7);
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(LightTowerCtl[7].led_flash_count == 0)
				{
			        CNTRL8_OFF;

					LightTowerCtl[7].led_status_cur = OFF;
					LightTowerCtl[7].led_flash_count = GetLightTowerOffTime(7);
				}
			}
		}
	}
	else
	{
        CNTRL8_OFF;
	}
/*********************************LED9********************************************/
	if(LightTowerCtl[8].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[8].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL9_ON;
		}
		//闪烁
		else
		{	
			//当前灯为灭状态
			if(LightTowerCtl[8].led_status_cur == OFF)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(LightTowerCtl[8].led_flash_count == 0)
				{
			        CNTRL9_ON;

					LightTowerCtl[8].led_status_cur = ON;
					LightTowerCtl[8].led_flash_count = GetLightTowerOnTime(8);
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(LightTowerCtl[8].led_flash_count == 0)
				{
			        CNTRL9_OFF;

					LightTowerCtl[8].led_status_cur = OFF;
					LightTowerCtl[8].led_flash_count = GetLightTowerOffTime(8);
				}
			}
		}
	}
	else
	{
        CNTRL9_OFF;
	}
/*********************************LED10********************************************/
	if(LightTowerCtl[9].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[9].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL10_ON;
		}
		//闪烁
		else
		{	
			//当前灯为灭状态
			if(LightTowerCtl[9].led_status_cur == OFF)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(LightTowerCtl[9].led_flash_count == 0)
				{
			        CNTRL10_ON;

					LightTowerCtl[9].led_status_cur = ON;
					LightTowerCtl[9].led_flash_count = GetLightTowerOnTime(9);
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(LightTowerCtl[9].led_flash_count == 0)
				{
			        CNTRL10_OFF;

					LightTowerCtl[9].led_status_cur = OFF;
					LightTowerCtl[9].led_flash_count = GetLightTowerOffTime(9);
				}
			}
		}
	}
	else
	{
        CNTRL10_OFF;
	}
}