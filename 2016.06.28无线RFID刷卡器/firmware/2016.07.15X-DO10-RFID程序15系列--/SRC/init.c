#include "include.h"

INT8U  const  code   LEDSecBit[7] = {37,38,39,40,41,42,36};		//段码对应的LED库表
UINT8	const  code  Version[LED_NUMBER]={'V',0x3C,'2'};	   //开机显示版本号
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 端口初始化
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void IO_Init(void)
{
//	P0M1 = 0x00;
//	P0M0 = 0x0f;
//
	P1M1 = 0x00;
	P1M0 = 0x80;	 //p1.7推挽输出

	P2M1 = 0x00;
	P2M0 = 0x0e;	//P2.3 P2.2 P2.1推挽输出

	P3M1 = 0x00;
	P3M0 = 0xc0;  //p3.7 p3.6推挽输出

	P4SW = 0x70;	 //	   P4.5 P4.6, P4.4 设置为普通IO口

//	//先熄灭指示灯
	CNTRL1_OFF;			CNTRL2_OFF;			CNTRL3_OFF;
	CNTRL4_OFF;			CNTRL5_OFF;			CNTRL6_OFF;

	LED_CNTRL1_OFF;		LED_CNTRL2_OFF;		LED_CNTRL3_OFF;
	LED_CNTRL4_OFF;		LED_CNTRL5_OFF;		LED_CNTRL6_OFF;
	sys.led_scan_flag = OFF;
	AUXR &= ~0X20;//0：允许使用内部扩展的1024字节扩展RAM；
	CLK_DIV=0;//外部晶体时钟或内部R/C振荡时钟
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 开机自检测程序 显示版本号，各段码
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void Start_Init(void)
{
//	UINT8 i = 0;
	UINT8 i,j;
	//显示版本
 	for (i=0; i<LED_NUMBER; i++)
	{
		led[i].num  = Version[i];
//		led[i].delay = 0;
		led[i].show  = ON;
	}
	DelayMs(800);
    //显示各段码
	for(i = 0; i < 7; i++)
	{
		for(j = 0; j < LED_NUMBER; j++)
		{  led[j].num = LEDSecBit[i];  } 		    //段码亮

		DelayMs(300);    //延时显示
	}
	//显示全8
	for (i=0; i<LED_NUMBER; i++)
	{
		led[i].num  = 0X08;
//		led[i].delay = 0;
		led[i].show  = ON;
	}
//	//系统数据复位
	DispInfoCur.SRCodeLen = 0;
	uart232.rFlag   = OFF;
	uart232.rIndex  = 0;

	LED_RXD485_ON;		LED_TXD485_ON;

	//点亮LED
//	for (i=0; i<LED_NUMBER; i++)
//	{
//		led[i].num  = 0X08;
//		led[i].show  = ON;
//	}
//	sys.led_scan_flag = ON;
// 	DelayMs(1000);
	//显示地址
	led[0].num = address[0];		       
    led[1].num = address[1];
    led[2].num = address[2];
	//点亮指示灯
//	led_light_ctrl(LED_RED, ON);
	led_light_ctrl(LED_RED);			// 仅亮红灯显示
 	DelayMs(500);
//	led_light_ctrl(LED_RED, OFF);
//	led_light_ctrl(LED_GREEN, ON);
	led_light_ctrl(LED_GREEN);			// 仅亮绿灯显示
 	DelayMs(500);
//	led_light_ctrl(LED_GREEN, OFF);
//	led_light_ctrl(LED_BLUE, ON);
	led_light_ctrl(LED_BLUE);			// 仅亮蓝灯显示
 	DelayMs(500);
//	led_light_ctrl(LED_BLUE, OFF);

	for (i=0; i<LED_NUMBER; i++)
	{ led[i].num  = ' '; }

	led_light_ctrl(LED_RG);			// 仅亮红绿灯显示
 	DelayMs(500);
	led_light_ctrl(LED_RB);			// 仅亮红蓝灯显示
 	DelayMs(500);
	led_light_ctrl(LED_GB);			// 仅亮绿蓝灯显示
 	DelayMs(500);
	led_light_ctrl(LED_NONE);		// 灭所有显示
	//点亮信号灯
	CNTRL1_ON;		LED_CNTRL1_ON;		CNTRL4_ON;		LED_CNTRL4_ON;		
	led[0].num = 'F'-'A'+10;			led[1].num = 0X01;		led[2].num = FinishedLED;		       
	DelayMs(1000);		
	CNTRL1_OFF;		LED_CNTRL1_OFF;		CNTRL4_OFF;		LED_CNTRL4_OFF;
	CNTRL2_ON;		LED_CNTRL2_ON;		CNTRL5_ON;		LED_CNTRL5_ON;		
	led[0].num = 'F'-'A'+10;			led[1].num = 0X02;		led[2].num = WtTime;		       
	DelayMs(1000);
	CNTRL2_OFF;		LED_CNTRL2_OFF;		CNTRL5_OFF;		LED_CNTRL5_OFF;
	CNTRL3_ON;		LED_CNTRL3_ON;		CNTRL6_ON;		LED_CNTRL6_ON;
	led[0].num = 'F'-'A'+10;			led[1].num = 0X03;		led[2].num = FnDefine;		       
	DelayMs(1000);
	CNTRL3_OFF;		LED_CNTRL3_OFF;		CNTRL6_OFF;		LED_CNTRL6_OFF;
	led[0].num = 'F'-'A'+10;			led[1].num = 0X04;		led[2].num = DispMode;		       
	DelayMs(1000);
	LED_RXD485_OFF;		LED_TXD485_OFF;
	//清除显示
	ClearAll();
}