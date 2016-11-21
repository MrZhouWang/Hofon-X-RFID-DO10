/*=========================================================
// 程序更改说明

=========================================================*/

#define  MAIN_GLOBAL
#include "include.h"
unsigned short timerc;
volatile OS_SYS sys;

///////////////////////////////全局变量/////////////////////////////////////
volatile UINT8   digital_led_loading = OFF;
volatile UINT8   digital_led_refresh = OFF;
volatile UINT16  digital_led_loading_static = 0;
volatile UINT8   led_static_warn = OFF;//当本标签按下按钮后的信息10秒钟后，
                                                                //其数据都还没被采集上去，则蓝色闪烁）；
//volatile UINT16   led_static_warn_cnt = 0;
//volatile UINT8 led_flash_switch =OFF;

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
 
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : main program
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
int main(void) 
{   
	DelayMs(200);    //等待电源稳定        

	EA = 0;
	digital_led_cntrl_Init();
	IO_Init();
 	InitT0();

	LoadParaInEeprom();
	baudrate   = BD_9600;
	UART232_Init();
	UART485_Init();	//固定使用38400波特率


	           
	Start_Init();
	M485_CTRL_OFF;
	EA = 1;//使能中断     					
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
			Send485SerialByte((UINT8 *)uart.rBuffer, uart.rIndex); 	//串口打印接收到的数据
			Cmd_Process();
			uart.rFlag   = OFF;
			uart.rIndex  = 0;
		}
	}                
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 定时器中断入口
// 输入参数 : 
// 返回参数 : 
// 说明     : 1ms中断
////////////////////////////////////////////////////////////////////////////////
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
		if(LightTowerCtl[index].led_flash_count > 0)
			LightTowerCtl[index].led_flash_count--;
	}
    //挨个扫描数码管
    led_Scan();
	timerc++;
	if(timerc>1000)
	{
	   p_led_red =1;
	  
	}
	else
	   p_led_red =0;
	if(timerc>2000)
	   timerc=0;
}
/*******************************************************************************
								END					
*******************************************************************************/

