#include "include.h"
					
								 

UINT8 keyStatus = CMD_KEY_NULL;		//本次按下的按键
UINT8 keyLastDown = CMD_KEY_NULL;	//上次按下的按键
UINT8 IsOkPressed  = OFF;			//工作显示数值状态下，表示是否正在修改焦点位，以区分（修改焦点位下，高位0需要显示，否则不需要显示）

/*******************************************************************************
	 					防止按键拉动
	input:   none     
	output:  none					
*******************************************************************************/
void Delay(void)
{
    UINT8 i;
    
    for (i=0; i<5; i++)
    {
        Delay1Ms();
    }
}
/*******************************************************************************
	 					扫描键盘子程序
	input:   none     
	output:  none					
*******************************************************************************/
void Scan_Keyboard(void)
{
	keyStatus = CMD_KEY_NULL;
	//上次有按键按下，则需等待上次按键松开后，才测试下一次按键
	if(keyLastDown == CMD_KEY_OK)		
	{
		if(!(KEY_OK == 0))
		{
			keyLastDown = CMD_KEY_NULL;
		}
	}
	//确定上次按键都已松开的情况下进入下述函数
	if(keyLastDown == CMD_KEY_NULL)
	{
		if (KEY_OK == 0)						//确认
		{
			Delay();
			if (KEY_OK == 0)
			{
				keyStatus = CMD_KEY_OK;
				keyLastDown = keyStatus;
			}
		}
	}
}
/*******************************************************************************
	 					OK按键处理程序
	input:   none     
	output:  none					
*******************************************************************************/
void KeyProcessOK(void)
{
	IsOkPressed = ON;
}
/*******************************************************************************
	 					按键处理程序
	input:   none     
	output:  none					
*******************************************************************************/
void Process_Keyboard(void)
{
	UINT16 updata_num = 0;
	UINT8 i = 0;

	if(SystemLocked)
	{  
		keyStatus = CMD_KEY_NULL;
		return;
	}

	if(beSettingAddr == 1)
	{ 
		//任意键排都记下地址
		address[0] = toSettedAddr[0];		address[1] = toSettedAddr[1];		address[2] = toSettedAddr[2];
		SaveParaToBlock();
		beSettingAddr  = 2;
		SetAddrFlag = 0;                //设置地址灯指示清零
		led_flash_NO = LED_NONE;				//接收前亮第一组灯，接收后亮第二组灯
	}
	else
	{
		switch (keyStatus)
		{
		case CMD_KEY_OK:					//OK
			KeyProcessOK();
			break;
		default:
			break;
		}
	}
}
/*******************************************************************************
								END					
*******************************************************************************/

