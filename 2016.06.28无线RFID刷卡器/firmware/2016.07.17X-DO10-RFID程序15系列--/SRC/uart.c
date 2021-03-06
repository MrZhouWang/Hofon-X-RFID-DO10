#include "include.h"

volatile OS_UART xdata uart;
volatile UINT8 InTemp;
bit busy;

#define RCV_OVER_TIMER	        2
#define RCV_OVER_TIMER232		2
extern void Serial232PutChar(UINT8 dat);
extern void Send485SerialByte(UINT8 *Ram, UINT16 len);


/***************************************************
		UART1初始化
  											7     6     5     4     3     2     1     0     Reset Value
sfr SCON = 0x98; //Serial Control         SM0/FE SM1   SM2   REN   TB8   RB8    TI    RI    0000,0000
SM0 SM1:
 0   0 	       同步移位寄存器		fosc/12
 0   1		   10位异步收发         由定时器控制
 1   0         11位异步收发  		fosc/32或fosc/64
 1   1         11位异步收发         由定时器控制
P = PSW.0      P=1奇   P=0偶
***************************************************/
void InitT485(void)
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE8;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1


//	TMOD = (TMOD & 0x0f) | 0x20;	//8位自动重载定时器
//	SCON = 0xd0;			      	//串行口工作模式3选择
//	IP   = 0x10;				    //PS set the high
//	PCON  &= 0x7f;                  //SOMD =0
//
//	//波特率 38400
//	AUXR |= 0x40;        			//定时器1时钟为Fosc,即1T
//	AUXR &= 0xfe;       			//串口1选择定时器1为波特率发生器
//	TL1 = 0xF7;            			//设定定时初值
//	TH1 = 0xF7;            			//设定定时器重装值
//
//
//	ET1  = 0;                       //close timer1 interrupt
//	TR1  = 1;	                    //open timer1
	ES   = 1;				        //Enabled serial port interrrup 
	EA=1;

}
/*******************************************************************************
	 						串口1初始化
	input :     none
	default:
	    data bits:  8
	    start bits: 1
	    stop bits:  1
	    parity:     NONE					
*******************************************************************************/
void UART485_Init(void)
{
	InitT485();
	uart.rIndex = 0;
	uart.rFlag  = 0;
	uart.rTimer = 0;	
}

/*******************************************************************************
	 					 串口1中断函数
*******************************************************************************/
void SCON485_int (void) interrupt 4 using 1
{
	if (RI)
    {
		RI = 0;
        //SBUF=SBUF; 
		if ( uart.rIndex >= UART_BUF_LEN ) return;
		if(uart.rFlag == ON)
			return;
		uart.rBuffer[uart.rIndex++] = SBUF;	
		uart.rTimer = RCV_OVER_TIMER;
	}

}


////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 串口1发送一个字节数据
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void Usart1_SendByte(UINT8 byte)
{
		while(TI);
	 	SBUF = byte;           
		while(!TI);
		TI=0;             
}
/***********************************************
        串口发送程序
input :     *Ram    
            len     
output :    NONE
***********************************************/
void Send485SeriaDate(UINT8 *Ram, UINT16 len)
{
    UINT8 ite;
	
	M485_CTRL_ON;
	LED_TXD485_ON;
	_nop_();
	_nop_();
	for(ite=0;ite<len;ite++)
	{
		Usart1_SendByte(Ram[ite]);
	}
	_nop_();
	_nop_();
	M485_CTRL_OFF;
	LED_TXD485_OFF;
    _nop_();
    _nop_();

}
///////////////////////////////////////////////--UART232--配置--////////////////////////////////

/***************************************************
		UART2初始化
***************************************************/
//void InitT232(void)
//{
//	//扫描枪串口配置:8BIT数据位、无奇偶校验、1位停止位
//	#if (PARITYBIT232 == NONE_PARITY)
//        S2CON = 0x5A;               //8位可变波特率 (无校验位)
//	#elif (PARITYBIT232 == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
//        S2CON = 0xda;               //9位可变波特率,校验位初始为1
//	#elif (PARITYBIT232 == SPACE_PARITY)
//        S2CON = 0xd5;               //9位可变波特率,校验位初始为0
//	#endif
//    
//	switch (baudrate)
//	{
//		case BD_4800:
//			BRT = -(FOSC/32/BD232_4800);      //设置独立波特率发生器的重载初值
//			break;
//		case BD_9600:
//			BRT = -(FOSC/32/BD232_9600);      //设置独立波特率发生器的重载初值
//			break;
//		case BD_19200:
//			BRT = -(FOSC/32/BD232_19200);      //设置独立波特率发生器的重载初值
//			PCON |= 0x80;
//			break;
//		case BD_38400:
//			BRT = -(FOSC/32/BD232_38400);      //设置独立波特率发生器的重载初值
//			break;
//		default:
//			BRT = -(FOSC/32/BD232_9600);      //设置独立波特率发生器的重载初值
//			break;
//        }
//        AUXR = 0x14;                //独立波特率发生器工作在1T模式
//        IE2 = 0x01;                 //使能串口2中断	
//    return;
//}
/*******************************************************************************
	 						串口初始化
	input :     none
	default:
	    data bits:  8
	    start bits: 1
	    stop bits:  1
	    parity:     NONE					
*******************************************************************************/
//void UART232_Init(void)
//{
//	InitT232();
//	uart232.rIndex = 0;
//	uart232.rFlag  = 0;
//	uart232.rTimer = 0;	
//}

/*******************************************************************************
	 					 扫描枪串口中断函数
				
*******************************************************************************/
//void SCON232_int (void) interrupt 8
//{    
//	if(uart232.rFlag == ON)
//	{
//		uart232.rFlag   = OFF;
//		uart232.rIndex  = 0;
//		InTemp = 0;
//	}
//	// 接收中断
//	if(S2CON & S2RI) 
//	{
//		S2CON &= ~S2RI;         //清除接收完成标志                 
//		if ( uart232.rIndex >= UART232_BUF_LEN )
//			return;
//		uart232.rBuffer[uart232.rIndex++] = S2BUF;	
//		uart232.rTimer = RCV_OVER_TIMER232;        
//	}
//    //发送完成中断
//	if (S2CON & S2TI)
//	{
//		S2CON &= ~S2TI;         //清除发送完成标志
//		busy = 0;
//	}
//}

/*******************************************************************************
								END					
*******************************************************************************/
