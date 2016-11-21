/*=========================================================
// 程序更改说明
//  2014年8月6日更改  版本v2.0
//	 1.显示开机版本，显示LED各段方便检测LED好坏。
//	   字库保持原来的基础上把库扩成128的字库，方便以后更改。
//   2.修正X_M3的F1，F4均显示错误
//   3.读取DispMod不正确

//  2014年9月22日更改  版本v2.1
//	 1.增加灯模式0=灭，1=红，2=绿，3=蓝，4=红绿，5=红蓝，6=绿蓝，7=红绿蓝
//     改led_light_ctrl	函数，对应设置上面7种模式
	 2015年12月14日更改  版本v2.2
//	 1.main.h文件中MAX_ADDR   99改为     256  20151214 
=========================================================*/


#include "main.h"
#include "uart.h"
#include "led.h"
#include "key.h"
#include "process.h"
#include "eeprom.h"

volatile OS_SYS sys;

///////////////////////////////全局变量/////////////////////////////////////
volatile UINT8   digital_led_loading = OFF;
volatile UINT8   digital_led_refresh = OFF;
volatile UINT16  digital_led_loading_static = 0;
volatile UINT8   led_static_warn = OFF;//当本标签按下按钮后的信息10秒钟后，
                                                                //其数据都还没被采集上去，则蓝色闪烁）；
volatile UINT16   led_static_warn_cnt = 0;
volatile UINT8 led_flash_switch =OFF;

volatile UINT8 led_flash_NO =LED_RED;					//灯组控制
volatile UINT16 led_flash_period=1000;					//闪烁周期控制
volatile UINT8 led_flash_mode =LED_FLASH_MODE_1V1;		//亮灭时间比例控制:0常亮
volatile UINT8 led_flash_status = 0;					//闪烁控制灯时，灯当前的状态,0——灭；1——亮
volatile UINT8 led_flash_NO_BK =LED_RED;				//灯组控制备份数据(拉进度条后使用)

//未完成任务块
volatile UINT8 beSettingAddr = 0;						//0:非设置站址		1:设置站址		2：站址设定已经按下OK键
volatile UINT8 toSettedAddr[3];							//待设定地址值
volatile UINT8 task_unupdate_num = 0;		        	//主机未下传作业任务总数
volatile UINT8 task_unfinish_num =0;					//未完成任务总数= 0,1...MAX_TASK_NUM
volatile UINT8 task_unfinish_NO[MAX_TASK_NUM] ={0};		//货位号
volatile UINT8 task_un_goods_numH[MAX_TASK_NUM] ={0};	//商品数据高位
volatile UINT8 task_un_goods_numL[MAX_TASK_NUM] ={0};	//商品数据低位

///已完成任务块
volatile UINT8 task_finished_num =0;		  			//已完成任务总数= 0,1...MAX_TASK_NUM
volatile UINT8 task_finished_NO[MAX_TASK_NUM] ={0};	  	//货位号
volatile UINT8 task_ed_goods_numH[MAX_TASK_NUM] ={0};	//商品数据高位
volatile UINT8 task_ed_goods_numL[MAX_TASK_NUM] ={0};	//商品数据低位

volatile UINT8 SystemLocked = OFF;						//锁定系统、解锁系统

volatile DisplayInfo DispInfoCur;						//当前显示信息		 
///////////////////////////////全局变量-END-////////////////////////////////
// 开机显示版本号
UINT8	const  code  Version[LED_NUMBER]={'V',0x3C,'2'};	   //3A=0.；3B=1.；3C=2.；3D=3.；3E=4.；3F=5.；
/*******************************************************************************
	 					1毫秒延迟函数
*******************************************************************************/
void Delay1Ms() 
{
    UINT16 i = 852;
    while (i != 0) {i--;}
}

/*******************************************************************************
	 					毫秒延迟函数
    input   usDelay     延迟时间值
	output  none
*******************************************************************************/

void DelayMs(UINT16 usDelay)
{
    for(; usDelay>0; usDelay--)
    {
        Delay1Ms();
    }   
}
/*******************************************************************************
	 					  端口初始化
*******************************************************************************/
void IO_Init(void)
{
//	P0M1 = 0x00;
//	P0M0 = 0x0f;
//
	P1M1 = 0x00;
	P1M0 = 0x80;

	P2M1 = 0x00;
	P2M0 = 0x0e;

	P3M1 = 0x00;
	P3M0 = 0xc0;

	P4SW = 0x70;

//	//先熄灭指示灯
	CNTRL1_OFF;			CNTRL2_OFF;			CNTRL3_OFF;
	CNTRL4_OFF;			CNTRL5_OFF;			CNTRL6_OFF;

	LED_CNTRL1_OFF;		LED_CNTRL2_OFF;		LED_CNTRL3_OFF;
	LED_CNTRL4_OFF;		LED_CNTRL5_OFF;		LED_CNTRL6_OFF;
	sys.led_scan_flag = OFF;
}




/***************************************************
		T0定时器初始化
***************************************************/
#define TH0_INIT        0xf8
#define TL0_INIT        0xcc

void InitT0(void)
{
    //定时1ms//默认为12T模式=>T0_CLK=SYS_CLK/12=11 059 200 /12 =921600	    
    TMOD = (TMOD & 0xf0) | 0x01;    //mode 1: 16 bit counter
    TH0  = TH0_INIT;
    TL0  = TL0_INIT;  

    ET0  = 1;                       //allow interrupt
    TR0  = 1;                       //open timer0

    return;
}

/*******************************************************************************
	定时器T0中断服务程序(ms中断)																	   
	Input :		中断向量
	Output :	sys.flag 置为有效
*******************************************************************************/
void timer0_int (void) interrupt 1
{ 
	int index = 0;

    TH0  = TH0_INIT;
    TL0  = TL0_INIT;
	  
    sys.led_scan_flag  = ON;
    //串口1-485缓冲区有数据
    if (uart.rIndex != 0)	 			
    {
        if (uart.rTimer-- == 0) 
        {
			uart.rFlag  = ON;
		}
    }

    //串口2-232缓冲区有数据
    if (uart232.rIndex != 0)	 			
    {
		if (uart232.rTimer-- == 0) 
		{
			uart232.rFlag  = ON;
			digital_led_refresh = ON;
		}
    }    
    //递减计算器，自减1
    if (sys.led_flash_count != 0)
        sys.led_flash_count--;
	for(index = 0; index < 6; index++)
	{
		if(LightTowerCtl[index].led_flash_count != 0)
			LightTowerCtl[index].led_flash_count--;
	}
    //挨个扫描数码管
    led_Scan();
}
/*******************************************************************************
					数码管显示控制
	input:   
	output:  none					
*******************************************************************************/
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
	 					开机自检测程序 显示版本号，各段码，
    input:   none     
    output:  none
*******************************************************************************/
INT8U  const  code   LEDSecBit[7] = {37,38,39,40,41,42,36};		//段码对应的LED库表
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
/*******************************************************************************
	 						main program				
*******************************************************************************/   

int main(void) 
{   
	DelayMs(50);    //等待电源稳定        

	EA = 0;
	digital_led_cntrl_Init();
	IO_Init();
 	InitT0();

	LoadParaInEeprom();
	baudrate   = BD_9600;
	UART232_Init();
	UART485_Init();	//固定使用38400波特率


	EA = 1;//使能中断             
	Start_Init();
	M485_CTRL_OFF;
	   					
	while(1)
	{	
		P0M1 = 0x00;	  //灯塔强推挽输出
		P0M0 = 0x0f;

		P1M1 = 0x00;
		P1M0 = 0x83;
		
		//地址设定时显示地址，并亮灯
		if(beSettingAddr == 1 ||  beSettingAddr == 2)
		{
			if(digital_led_refresh == ON)
			{
				digital_led_refresh = OFF;
				digital_led_dispaddr();
			}
		}
		//1个信号灯与3组led指示灯闪烁控制
		CMD_led_flash();
		//灯塔电源控制
		CMD_LigheTower_flash();
		//刷新一次数码管数值（包括显示当前作业的以及数据回显）
		if( digital_led_refresh == ON) 
		{
			digital_led_refresh = OFF;
			digital_led_disp();
		}
		//按键处理(有按键时)
		Scan_Keyboard();
		if (keyStatus != CMD_KEY_NULL) 
		{
			Process_Keyboard();
		}
		//处理485通讯
		if (uart.rFlag == ON)
		{
//			Send485SerialByte((UINT8 *)uart.rBuffer, uart.rIndex); 
			Cmd_Process();
			uart.rFlag   = OFF;
			uart.rIndex  = 0;
		}
	}                
}
/*******************************************************************************
								END					
*******************************************************************************/

