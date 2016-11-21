#include "include.h"

LIGHTTOWER_CTL LightTowerCtl[6];

UINT8 address[3];
UINT8 baudrate;
UINT8 ucAddr[3];                        //备份地址
UINT8 ucBaud;                           //备份波特率

UINT8 FinishedLED = LED_NONE;			//拣货完成后，清零前，指示灯亮的颜色
UINT8 WtTime = 5;						//一对多应用时，进度条等待时间
UINT8 FnDefine = 0;						//Fn功能键定义	0-零拣货	1-满箱
UINT8 DispMode = 0;						//显示模式	0-任务模式	1-区域信息模式


///////////////////////////////////////////////////////////////////////////////
UINT8 ucShift;					        //LED数码管当前显示位置"1==>6"
volatile OS_NUM idata  led[LED_NUMBER];	//运行时的数据
volatile OS_NUM idata bled[LED_NUMBER];		

#define DISPLAY_OFF     0xff
//UINT8 LED_TBL[LED_TBL_MAX] = 	        //数码管显示对应表
//{			
//    //0      1    2      3    4     5     6     7      
//    0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
//    //8      9    A      b    C     d     E     F
//    0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e,
//    //g      H    i      j    k     L     m     n      
//    0XFF, 0x8B, 0XFF, 0xF1, 0XFF, 0xc7, 0XFF, 0xab,
//    //o      P    q      r    s     t     U     v      
//    0xa3, 0x8c, 0XFF, 0XFF, 0x92, 0x87, 0xc1, 0xe3,
//    //w      x    y      z    -        
//    0XFF, 0x89, 0XFF, 0XFF, 0xBF
//
//};

//尽量按ASCII码来作表 ，LED是共阳极的，A=b0,B=b1,C=b2,D=b3,E=b4,F=b5,G=b6,H=b7,
UINT8    LED_TBL[LED_TBL_MAX] = {
/*---------------------------------------------------------------------*/
//数码管显示0-9的七段码字节
//0-15，0--9,A-f
//0	  1    2    3    4     5    6    7    8    9	a	 b    c    d    e    f   
0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
//16-31，0.--9.,A.-f.
//0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   a.   b.   c.   d.   e.   f.
//0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e,  
//g    H    i   j    k     L   m    n  //o   P    q     r    s    t    U    v     
0XFF,0x8B,0XFF,0xF1,0XFF,0xc7,0XFF,0xab,0xa3,0x8c,0XFF,0XFF,0x92,0x87,0xc1,0xe3,
//32-47                                                   +		    -
//0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x8f,0xff,0xbf,0xff,0xff,
//w   x    y    z //36 G亮  f亮  a亮 b亮  c亮  d亮  e亮    +   G亮	-	 h亮
0XFF,0x89,0XFF,0XFF,  0xBF,0xdf,0xfe,0xfd,0xfb,0xf7,0xef,0x8f,0xbf,0xbf,0x7f,0xff,
//48--63
//0	  1    2    3    4     5    6    7    8    9  //0.   1.   2.   3.   4.   5.//:    ;    <    =    >    ?  
0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x40,0x79,0x24,0x30,0x19,0x12,//0xff,0xff,0xff,0xff,0xff,0xff,
//64
//@
0xff, 
//65--90  
//A	  B    C    D    E     F    G    H    I    J    K    L    M    N
0x88,0x83,0xc6,0xa1,0x86,0x8e,0xc2,0x89,0xf9,0xe1,0xbf,0xc7,0x8f,0xab,
//O	  P    Q    R    S     T    U    V    W    X    Y    Z	
0xa3,0x8c,0x98,0xaf,0x92,0x87,0xc1,0xe3,0x9c,0xff,0x91,0xa4,
//91--96  '['==turn on; ']'==turn off;
//[	  \    ]    ^    _     .
//0x00,0xff,0xff,0xff,0xfd,0xff,
//           6.   7.   8.   9.
0x00,0xff,  0x02,0x78,0x00,0x10,
//97-122
//a	  b    c    d    e     f    g    h    i    j    k    l    m    n
0x88,0x83,0xa7,0xa1,0x86,0x8e,0xc2,0x89,0xfb,0xe1,0xbf,0xc7,0x8f,0xab,
//o	  p    q    r    s     t    u    v    w    x    y    z
0xa3,0x8c,0x98,0xaf,0x92,0x87,0xc1,0xe3,0x9c,0xff,0x91,0xa4,
//123-127
0xBF,0xBF,0xBF,0xBF,0xBF,
};

/*******************************************************************************
	                    LED数码管SPI控制IO初始化
    Input :	 None
    Output:  None	
*******************************************************************************/
void digital_led_cntrl_Init(void)
{	
	OE_SET;			//DISABLE
	SH_CLK_CLR;
	ST_CLK_CLR;
	OE_CLR;			//ENALBE
	
	ucShift = 0;
}
/*******************************************************************************
	                    LED数码管 根据位置开通对应数码管
    Input :	 None
    Output:  None	
*******************************************************************************/
void led_SupplyPower(void)
{
	UINT8 led_pwr_no;              

	led_pwr_no = ucShift;

	if(led_pwr_no == DIGITAL_LED1)//百位数码管电源
	{
//		LED_PORT_OUT =  0x1f;                               //LED 1-LED5 ==> OFF
		digital_led1_port = DIGITAL_LED_PWR_ON;        
		digital_led2_port = DIGITAL_LED_PWR_OFF; 
		digital_led3_port = DIGITAL_LED_PWR_OFF; 
	}
	else if(led_pwr_no == DIGITAL_LED2)//十位数码管电源
	{
//		LED_PORT_OUT =  0x1f;                               //LED 1-LED5 ==> OFF
		digital_led1_port = DIGITAL_LED_PWR_OFF;
		digital_led2_port = DIGITAL_LED_PWR_ON; 
		digital_led3_port = DIGITAL_LED_PWR_OFF; 
	}
	else if(led_pwr_no == DIGITAL_LED3)//个位数码管电源
	{
//		LED_PORT_OUT =  0x1f;                               //LED 1-LED5 ==> OFF
		digital_led1_port = DIGITAL_LED_PWR_OFF;
		digital_led2_port = DIGITAL_LED_PWR_OFF; 
		digital_led3_port = DIGITAL_LED_PWR_ON; 
	}
}

/*******************************************************************************
	                    显示当前扫描的led内容          
    Input :	 UINT8 ucValue   //当前要显示的数值
    Output:  None	
*******************************************************************************/
void led_Show(UINT8 ucValue)
{
	UINT8 i;

	ST_CLK_CLR;
	for (i=0; i<8; i++)
	{
		if (ucValue & 0x80)
			SDATA_SET;
		else
			SDATA_CLR;
		SH_CLK_CLR;
		NOP;
		SH_CLK_SET;
		ucValue = ucValue << 1; 
	}
	
	ST_CLK_SET;
	led_SupplyPower();						      //供电
}

/*******************************************************************************
	                    显示一个digital led         
    Input :	          None
    Output:         None
    Description:  显示一个数码管数字，扫描时间: 72us         
*******************************************************************************/

void led_Scan(void)
{
    UINT8 ucValue;

    if (sys.led_scan_flag == OFF) return;
    sys.led_scan_flag = OFF;

	//非配置状态(显示数值状态)下，并且当前不是修改数值下，则先控制当前刷新的位数，数值位，高位0不显示
    if (led[ucShift].show == ON)
    {
        if(led[ucShift].num >= LED_TBL_MAX )
        {
            led[ucShift].num = LED_TBL_MAX -1;
        }
        ucValue = LED_TBL[led[ucShift].num];     //查表 	......
    }
    else
    {
        ucValue = DISPLAY_OFF;                                      //关闭显示
    }
    led_Show(ucValue);						            //显示

    ucShift++;
    if (ucShift >= LED_NUMBER) 
    {
    	ucShift = 0; 
    }
}

//获取灯亮时间，单位ms
UINT16 GetLedOnTime()
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case 0:						//常亮模式，不会调用此函数
		break;			
	case 1:
		unRet = led_flash_period/2;
		break;
	case 2:
		unRet = led_flash_period/3;
		break;
	case 3:
		unRet = led_flash_period/6;
		break;
	case 4:
		unRet = led_flash_period/11;
		break;
	case 5:
		unRet = led_flash_period/3*2;
		break;
	case 6:
		unRet = led_flash_period/6*5;
		break;
	case 7:
		unRet = led_flash_period/11*10;
		break;
	default:
		break;
	}
	return unRet;
}
//获取灯灭时间，单位ms
UINT16 GetLedOffTime()
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case 0:					   //常亮模式，不会调用此函数
		break;				
	case 1:
		unRet = led_flash_period/2;
		break;
	case 5:
		unRet = led_flash_period/3;
		break;
	case 6:
		unRet = led_flash_period/6;
		break;
	case 7:
		unRet = led_flash_period/11;
		break;
	case 2:
		unRet = led_flash_period/3*2;
		break;
	case 3:
		unRet = led_flash_period/6*5;
		break;
	case 4:
		unRet = led_flash_period/11*10;
		break;
	default:
		break;
	}
	return unRet;
}
//获取灯亮时间，单位ms
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
//获取灯灭时间，单位ms
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
void CMD_led_flash(void)
{
	if(uart232.rFlag == ON)
	{
//		led_light_ctrl(LED_RED, OFF);
//		led_light_ctrl(LED_GREEN, ON);
//		led_light_ctrl(LED_BLUE, OFF);
		led_light_ctrl(LED_GREEN);			// 仅点亮绿色
		return;
	}
    //LED指示灯控制
//    if(led_flash_NO == LED_RED || led_flash_NO == LED_GREEN || led_flash_NO == LED_BLUE)//当前为亮
	if(led_flash_NO != LED_NONE)
    {
//		switch(led_flash_NO)
//		{
//		case LED_RED:		led_light_ctrl(LED_GREEN, OFF);		led_light_ctrl(LED_BLUE, OFF);		break;
//		case LED_GREEN:		led_light_ctrl(LED_RED, OFF);		led_light_ctrl(LED_BLUE, OFF);		break;
//		case LED_BLUE:		led_light_ctrl(LED_GREEN, OFF);		led_light_ctrl(LED_RED, OFF);		break;
//		}
		if(led_flash_mode == LED_FLASH_MODE_ALWAYS)	//常亮显示
		{
//			led_light_ctrl(led_flash_NO, ON);
			led_light_ctrl(led_flash_NO);			// 仅点亮对应颜色
		}
		else//闪烁显示
		{
			//当前灯为灭状态
			if(led_flash_status == 0)
			{
				//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				if(sys.led_flash_count == 0)
				{
//					led_light_ctrl(led_flash_NO, ON);
					led_light_ctrl(led_flash_NO);			// 仅点亮对应颜色
					led_flash_status = 1;	//切换到灯亮状态
					sys.led_flash_count = GetLedOnTime();
				}
			}
			//当前为亮灯
			else
			{
				//灯灭计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				if(sys.led_flash_count == 0)
				{
//					led_light_ctrl(led_flash_NO, OFF);
					led_light_ctrl(LED_NONE);
					led_flash_status = 0;	//切换到灯亮状态
					sys.led_flash_count = GetLedOffTime();
				}
			}
		}
	}
	else//当前不显示
	{
//        led_light_ctrl(LED_RED, OFF);
//        led_light_ctrl(LED_GREEN, OFF);
//        led_light_ctrl(LED_BLUE, OFF);
		led_light_ctrl(LED_NONE);	
	}
}

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
			LED_CNTRL1_ON;
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
					LED_CNTRL1_ON;

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
					LED_CNTRL1_OFF;

					LightTowerCtl[0].led_status_cur = OFF;
					LightTowerCtl[0].led_flash_count = GetLightTowerOffTime(0);
				}
			}
		}
	}
	else
	{
        CNTRL1_OFF;
		LED_CNTRL1_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED2
	if(LightTowerCtl[1].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[1].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL2_ON;
			LED_CNTRL2_ON;
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
					LED_CNTRL2_ON;

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
					LED_CNTRL2_OFF;

					LightTowerCtl[1].led_status_cur = OFF;
					LightTowerCtl[1].led_flash_count = GetLightTowerOffTime(1);
				}
			}
		}
	}
	else
	{
        CNTRL2_OFF;
		LED_CNTRL2_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED3
	if(LightTowerCtl[2].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[2].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL3_ON;
			LED_CNTRL3_ON;
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
					LED_CNTRL3_ON;

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
					LED_CNTRL3_OFF;

					LightTowerCtl[2].led_status_cur = OFF;
					LightTowerCtl[2].led_flash_count = GetLightTowerOffTime(2);
				}
			}
		}
	}
	else
	{
        CNTRL3_OFF;
		LED_CNTRL3_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED4
	if(LightTowerCtl[3].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[3].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL4_ON;
			LED_CNTRL4_ON;
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
					LED_CNTRL4_ON;

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
					LED_CNTRL4_OFF;

					LightTowerCtl[3].led_status_cur = OFF;
					LightTowerCtl[3].led_flash_count = GetLightTowerOffTime(3);
				}
			}
		}
	}
	else
	{
        CNTRL4_OFF;
		LED_CNTRL4_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED5
	if(LightTowerCtl[4].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[4].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL5_ON;
			LED_CNTRL5_ON;
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
					LED_CNTRL5_ON;

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
					LED_CNTRL5_OFF;

					LightTowerCtl[4].led_status_cur = OFF;
					LightTowerCtl[4].led_flash_count = GetLightTowerOffTime(4);
				}
			}
		}
	}
	else
	{
        CNTRL5_OFF;
		LED_CNTRL5_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED6
	if(LightTowerCtl[5].led_status == ON)
	{
		//常亮
		if(LightTowerCtl[5].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL6_ON;
			LED_CNTRL6_ON;
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
					LED_CNTRL6_ON;

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
					LED_CNTRL6_OFF;

					LightTowerCtl[5].led_status_cur = OFF;
					LightTowerCtl[5].led_flash_count = GetLightTowerOffTime(5);
				}
			}
		}
	}
	else
	{
        CNTRL6_OFF;
		LED_CNTRL6_OFF;
	}
}
/*******************************************************************************
	                    给当前显示led供电            
    Input :	 UINT8 ucValue   //LED_RED, LED_GREEN, LED_BLUE 
             UINT8 ucStatus  //ON or OFF
    Output:  None	
*******************************************************************************/
//void led_light_ctrl(UINT8 ucValue, UINT8 ucStatus)
//{
//	switch (ucValue)
//	{
//		case LED_RED:
//			if (ucStatus == ON)
//				LED_R_ON;
//			else
//				LED_R_OFF;
//			break;
//		
//		case LED_GREEN:
//			if (ucStatus == ON)
//				LED_G_ON;
//			else
//				LED_G_OFF;
//			break;
//		
//		case LED_BLUE:
//			if (ucStatus == ON)
//				LED_B_ON;
//			else
//				LED_B_OFF;
//			break;
//			
//		default:
//		 	break;
//	}
//}
/*******************************************************************************
	                    给当前显示led供电            
    Input :	 UINT8 ucValue   //LED_RED, LED_GREEN, LED_BLUE 
    Output:  None	
*******************************************************************************/
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


//清除命令(地址清除时、广播清除时、显示清除时)
void Display_Clear(void)
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
}


////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 数码管显示控制
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void digital_led_disp(void)
{
	SINT16 i=0,j=0;
	//控制显示
	for(i = 0; i < LED_NUMBER; i++)
	{
		led[i].show = OFF;
	}
	//如果232口有扫描数据,则显示扫描进来的数据
	if(uart232.rFlag == ON)
	{
		j = LED_NUMBER-1;
		for(i = uart232.rIndex-1; i>= 0 && j>=0; i--)
		{
			if(uart232.rBuffer[i] >= '0' && uart232.rBuffer[i] <= '9')
			{
				led[j].num = uart232.rBuffer[i]-'0';
				led[j].show = ON;
			}
			else if(uart232.rBuffer[i] >= 'A' && uart232.rBuffer[i] <= 'Z')
			{
				led[j].num = uart232.rBuffer[i]-'A'+10;
				led[j].show = ON;
			}
			else if(uart232.rBuffer[i] >= 'a' && uart232.rBuffer[i] <= 'z')
			{
				led[j].num = uart232.rBuffer[i]-'a'+10;
				led[j].show = ON;
			}
			else if(uart232.rBuffer[i] == '-')
			{
				led[j].num = LED_TBL_MAX-1;
				led[j].show = ON;
			}
			if(uart232.rBuffer[i] != '\r' && uart232.rBuffer[i] !='\n')
				j--;
		}
	}
	//否则,如果有显示信息,则显示信息
	else if(DispInfoCur.SRCodeLen > 0)
	{
		j = LED_NUMBER-1;
		for(i = DispInfoCur.SRCodeLen-1; i>=0 && j>=0; i--)
		{
			if(DispInfoCur.SRCode[i] >= '0' && DispInfoCur.SRCode[i] <= '9')
			{
				led[j].num = DispInfoCur.SRCode[i]-'0';
				led[j].show = ON;
			}
			else if(DispInfoCur.SRCode[i] >= 'A' && DispInfoCur.SRCode[i] <= 'Z')
			{
				led[j].num = DispInfoCur.SRCode[i]-'A'+10;
				led[j].show = ON;
			}
			else if(DispInfoCur.SRCode[i] >= 'a' && DispInfoCur.SRCode[i] <= 'z')
			{
				led[j].num = DispInfoCur.SRCode[i]-'a'+10;
				led[j].show = ON;
			}
			else if(DispInfoCur.SRCode[i] == '-')
			{
				led[j].num = LED_TBL_MAX-1;
				led[j].show = ON;
			}
			if(uart232.rBuffer[i] != '\r' && uart232.rBuffer[i] !='\n')
				j--;
		}
	}  
}
/*******************************************************************************
	 					开机自检测程序
    input:   none     
    output:  none
*******************************************************************************/ 
void digital_led_dispaddr()
{
    //加载地址
    led[0].num = toSettedAddr[0];		       
    led[1].num = toSettedAddr[1];
    led[2].num = toSettedAddr[2];

    led[0].show = ON;
    led[1].show = ON;
    led[2].show = ON;					
}


/*******************************************************************************
								END					
*******************************************************************************/

