/**********************************************
*  CPU:STC12W1K16S
*  晶振:11.0592MHZ
------------------------------------------------*/

#define  MAIN_GLOBAL
#include "include.h"
volatile unsigned short timerc;
volatile OS_SYS sys;

///////////////////////////////全局变量/////////////////////////////////////

unsigned char code rc_data[16] ={0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};//
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //原始密码
unsigned char code DefaultKeyA[6] = {7,7,7,7,7,7}; //密码A
unsigned char code DefaultKeyB[6] = {8,8,8,8,8,8}; //密码B
//volatile UINT8   digital_led_refresh = OFF;
                                                                //其数据都还没被采集上去，则蓝色闪烁）；
volatile UINT8 led_flash_NO =LED_RED;					//灯组控制
volatile UINT16 led_flash_period=1000;					//闪烁周期控制
volatile UINT8 led_flash_mode =LED_FLASH_MODE_1V1;		//亮灭时间比例控制:0常亮
volatile UINT8 led_flash_status = 0;					//闪烁控制灯时，灯当前的状态,0——灭；1——亮
volatile UINT8 led_flash_NO_BK =LED_RED;				//灯组控制备份数据(拉进度条后使用)

//未完成任务块
volatile UINT8 beSettingAddr = 0;						//0:非设置站址		1:设置站址		2：站址设定已经按下OK键
volatile UINT8 toSettedAddr[3];							//待设定地址值

volatile UINT8 SystemLocked = OFF;						//锁定系统、解锁系统
	 
///////////////////////////////全局变量-END-////////////////////////////////
 
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : main program
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
int main(void) 
{   
	DelayMs(20);    //等待电源稳定        

	EA = 0;
	IO_Init();
 	InitT0();

	LoadParaInEeprom();
	baudrate   = BD_9600;

	UART485_Init();	//固定使用38400波特率    
	PcdReset();
  PcdAntennaOff(); 
  PcdAntennaOn(); 
	Start_Init();
	EA = 1;//使能中断 	
	RC_WorkeMode=RC_ReadMode;				
	while(1)
	{	

	  if(RC_WorkeMode==RC_WriteMode)//写模式	/**************RFID卡读写控制***********************/
		   M1_WriteDate(PICC_AUTHENT1A,8,rc_data,DefaultKey); //写IC卡
		else if(RC_WorkeMode==RC_ReadMode)//读模式
		   M1_Read(PICC_AUTHENT1A ,7,16,DefaultKey);//扫描读IC卡
		else if(RC_WorkeMode==RC_ChangeMode)//读模式//改密码
		   M1_ChangeKey(7);

		CMD_led_flash();	    /**************RGB控制***********************/

		CMD_LigheTower_flash();	/********************灯塔控制****************/

		Scan_Keyboard();	    /*************按键处理(有按键时)*************/
		if (keyStatus != CMD_KEY_NULL) 
		{
			Process_Keyboard();
		}

		if (uart.rFlag == ON)   /*************处理485通讯********************/
		{
			Send485SeriaDate((UINT8 *)uart.rBuffer, uart.rIndex); 	//串口打印接收到的数据
			Cmd_Process();
			uart.rFlag   = OFF;
			uart.rIndex  = 0;
		}
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
	char index = 0;
	TH0  = TH0_INIT;
  TL0  = TL0_INIT;
	if(con_flag==ON)
	{
		if(rc_blink_read>0)	  //读卡或者写卡成功后指示灯亮
		{
			rc_blink_read--;
			led_on();
			BEEP_ON;
		}
		else
		{
			led_off();
			BEEP_OFF;
		}
	}

    //串口1-485缓冲区有数据
    if (uart.rIndex != 0)	 			
    {
        if (uart.rTimer-- == 0) 
        {
			      uart.rFlag  = ON;
		    }
    }
   
    //递减计算器，自减1 用于RGB闪烁周期
    if (sys.led_flash_count != 0)
        sys.led_flash_count--;
		
		for(index = 0; index < 10; index++)
	  {
		   if(LightTowerCtl[index].led_flash_count > 0)
			    LightTowerCtl[index].led_flash_count--;
	  }

}
/*******************************************************************************
								END					
*******************************************************************************/

