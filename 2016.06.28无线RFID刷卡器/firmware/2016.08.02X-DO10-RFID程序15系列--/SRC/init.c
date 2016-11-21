#include "include.h"

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 端口初始化
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void IO_Init(void)
{
  P0M1 |= 0x0;
	P0M0 |= 0x0E;	 //P0.1 p0.2 P0.3推挽输出
  
  P1M1 &= 0x00;
	P1M0 |= 0xff;	 //p1口 all推挽输出	
	/***485接收**************/
	M485_CTRL_OFF;//接收状态
	/***熄灭灯塔**************/
	CNTRL1_OFF;			CNTRL2_OFF;			CNTRL3_OFF;
	CNTRL4_OFF;			CNTRL5_OFF;			CNTRL6_OFF;
	CNTRL7_OFF;			CNTRL8_OFF;			CNTRL9_OFF;
	CNTRL10_OFF;

	led_off();
	
	BEEP_OFF;
	RC522_rFlag  = OFF;   //清除RFID信息	
	sys.led_scan_flag = OFF;
	AUXR &= ~0X20;        //0：允许使用内部扩展的1024字节扩展RAM；
	CLK_DIV=0;            //外部晶体时钟或内部R/C振荡时钟
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 开机自检测程序 显示版本号，各段码
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void Start_Init(void)
{

	//系统数据复位
	RC522_rFlag   = OFF;

	LED_RXD485_ON;		LED_TXD485_ON;

	//点亮指示灯
	led_light_ctrl(LED_BLUE);		// 仅亮红灯显示
 	DelayMs(500);

	led_light_ctrl(LED_GREEN);	// 仅亮绿灯显示
 	DelayMs(500);
	led_light_ctrl(LED_RED);		// 仅亮蓝灯显示
 	DelayMs(500);

	led_light_ctrl(LED_RG);			// 仅亮红绿灯显示
 	DelayMs(500);
	led_light_ctrl(LED_RB);			// 仅亮红蓝灯显示
 	DelayMs(500);
	led_light_ctrl(LED_GB);			// 仅亮绿蓝灯显示
 	DelayMs(500);
	led_light_ctrl(LED_NONE);		// 灭所有显示
	LED_RXD485_OFF;		LED_TXD485_OFF; BEEP_OFF;
	M485_CTRL_OFF;              //接收模式 	 
	ClearAll();
}