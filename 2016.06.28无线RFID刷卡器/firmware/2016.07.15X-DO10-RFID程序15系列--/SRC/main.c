/*=========================================================
// 程序更改说明

=========================================================*/

#define  MAIN_GLOBAL
#include "include.h"
volatile unsigned short timerc;
volatile OS_SYS sys;

///////////////////////////////全局变量/////////////////////////////////////

unsigned char code rc_data[16] ={0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 

//unsigned char g_ucTempbuf[20];
void led_blink();
void delay_ms(unsigned int z);
void led_off();
void led_on();  






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
//	DelayMs(200);    //等待电源稳定        
//
//	EA = 0;
//	digital_led_cntrl_Init();
//	IO_Init();
// 	InitT0();
//
//	LoadParaInEeprom();
//	baudrate   = BD_9600;
//	UART232_Init();
//	UART485_Init();	//固定使用38400波特率
//
//
//	           
//	Start_Init();
//	M485_CTRL_OFF;
//	EA = 1;//使能中断     					
//	while(1)
//	{	
//		P0M1 = 0x00;	  //灯塔强推挽输出
//		P0M0 = 0x0f;
//
//		P1M1 = 0x00;
//		P1M0 = 0x83;
//		
//		//地址设定时显示地址，并亮灯
//		if(beSettingAddr == 1 ||  beSettingAddr == 2)
//		{
//			if(digital_led_refresh == ON)
//			{
//				digital_led_refresh = OFF;
//				digital_led_dispaddr();
//			}
//		}
//		//1个信号灯与3组led指示灯闪烁控制
//		CMD_led_flash();
//		//灯塔电源控制
//		CMD_LigheTower_flash();
//		//刷新一次数码管数值（包括显示当前作业的以及数据回显）
//		if( digital_led_refresh == ON) 
//		{
//			digital_led_refresh = OFF;
//			digital_led_disp();
//		}
//		//按键处理(有按键时)
//		Scan_Keyboard();
//		if (keyStatus != CMD_KEY_NULL) 
//		{
//			Process_Keyboard();
//		}
//		//处理485通讯
//		if (uart.rFlag == ON)
//		{
//			Send485SerialByte((UINT8 *)uart.rBuffer, uart.rIndex); 	//串口打印接收到的数据
//			Cmd_Process();
//			uart.rFlag   = OFF;
//			uart.rIndex  = 0;
//		}
//	}             

	 UART485_Init();	//固定使用38400波特率
	 InitT0();	 //初始化定时器1 
     PcdReset();
     PcdAntennaOff(); 
     PcdAntennaOn(); 
     while ( 1 )
     {   
	 M1_Read(PICC_AUTHENT1A ,3,16,DefaultKey);
//        status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
//         if (status != MI_OK)
//         {    
//              InitializeSystem( );
//              PcdReset();
//              PcdAntennaOff(); 
//              PcdAntennaOn(); 
//			  led_on();
//			  continue;
//         }	     
////		 printf("卡的类型:");
////         for(i=0;i<2;i++)
////		 {
////		     temp=g_ucTempbuf[i];
////		     printf("%X",temp);	
////		 }
//			
//         status = PcdAnticoll(g_ucTempbuf);//防冲撞
//         if (status != MI_OK)
//         { 
//		     continue;    
//	     }        
//				
////		 printf("卡序列号：");	//超级终端显示,
////		 for(i=0;i<4;i++)
////		 {
////			 temp=g_ucTempbuf[i];
////			 printf("%X",temp);			
////		 }
//
//         status = PcdSelect(g_ucTempbuf);//选定卡片
//         if (status != MI_OK)
//         { 
//		     continue;    
//		 }
//         status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//验证卡片密码
//         if (status != MI_OK)
//         {   
//		 	 continue;    
//		 }
//         
//         status = PcdWrite(1, data1);//写块
//         if (status != MI_OK)
//         {  
//		    continue;
//	     }
//		 //Send485SerialByte("nihao",3);
//		 
//		 while(1)
//		 {
//		    
//		 	M1_Read(PICC_AUTHENT1A ,3,16,DefaultKey);
//   		 }
	}
}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 定时器中断入口
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
void timer0_int (void) interrupt 1
{ 
	TH0  = TH0_INIT;
    TL0  = TL0_INIT;
//    rc_cont++;
//	if(rc_cont>RC_CON)
//	{
//	    rc_cont=0;
//	    M1_Read(PICC_AUTHENT1A ,3,16,DefaultKey);	
//	}
    timerc++;
	if(timerc>1000)
	{
	   LED4  =1;
	  
	}
	else
	   LED4 =0;
	if(timerc>2000)
	   timerc =0;
//	int index = 0;
	if(rc_blink_read>0)	  //读卡或者写卡成功后指示灯亮
	{
		rc_blink_read--;
		led_on();
	}
	else
	{
		led_off();
	}

//	  
//    sys.led_scan_flag  = ON;
//    //串口1-485缓冲区有数据
//    if (uart.rIndex != 0)	 			
//    {
//        if (uart.rTimer-- == 0) 
//        {
//			uart.rFlag  = ON;
//		}
//    }
//
//    //串口2-232缓冲区有数据
//    if (uart232.rIndex != 0)	 			
//    {
//		if (uart232.rTimer-- == 0) 
//		{
//			uart232.rFlag  = ON;
//			digital_led_refresh = ON;
//		}
//    }    
//    //递减计算器，自减1
//    if (sys.led_flash_count != 0)
//        sys.led_flash_count--;
//	for(index = 0; index < 6; index++)
//	{
//		if(LightTowerCtl[index].led_flash_count > 0)
//			LightTowerCtl[index].led_flash_count--;
//	}
//    //挨个扫描数码管
//    led_Scan();

}
/*******************************************************************************
								END					
*******************************************************************************/

