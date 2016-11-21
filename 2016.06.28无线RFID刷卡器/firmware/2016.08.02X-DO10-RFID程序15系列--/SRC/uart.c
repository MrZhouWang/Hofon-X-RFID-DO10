#include "include.h"

volatile OS_UART xdata uart;

#define RCV_OVER_TIMER	        4
extern void Send485SerialByte(UINT8 *Ram, UINT16 len);

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 485初始化
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void InitT485(void)
{
	TMOD = (TMOD & 0x0f) | 0x20;	 //8位自动重载定时器
	SCON = 0xd0;			      	     //串行口工作模式3选择
	IP   = 0x10;				           //PS set the high
	PCON  &= 0x7f;                 //SOMD =0

	//波特率 38400
	AUXR |= 0x40;        			     //定时器1时钟为Fosc,即1T
	AUXR &= 0xfe;       			     //串口1选择定时器1为波特率发生器
	TL1 = 0xF7;            			   //设定定时初值
	TH1 = 0xF7;            			   //设定定时器重装值


	ET1  = 0;                      //close timer1 interrupt
	TR1  = 1;	                     //open timer1
	ES   = 1;				               //Enabled serial port interrrup 
	
	
	
  RE_USART_IO();                  //串口重映射
  

  return;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 串口重映射
// 输入参数 : 
// 返回参数 : 
// 说明     : 串口重映射到  P3.6 P3.7 上面
////////////////////////////////////////////////////////////////////////////////
void RE_USART_IO(void)
{
	 ACC = P_SW1;                   
   ACC &= ~(S1_S0 | S1_S1);      //S1_S0=1 S1_S1=0
   ACC |= S1_S0;                 //(P3.6/RxD_2, P3.7/TxD_2)
	 P_SW1 = ACC;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 串口1初始化
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void UART485_Init(void)
{
	InitT485();
	
	uart.rIndex = 0;
	uart.rFlag  = 0;
	uart.rTimer = 0;	
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 串口1中断入口函数
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void SCON485_int (void) interrupt 4 
{
	if (RI)
    {
		   RI = 0;
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
		
	  //while(TI);
	 	SBUF = byte;           
		while(!TI);
		TI = 0;             
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 485发送一个字节数据
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
//void Send485SendByte(UINT8 byte)
//{
//	  M485_CTRL_ON;
//	  LED_TXD485_ON;//发射状态
//	  DelayMs(5);
//	  Usart1_SendByte(byte);
//	  DelayMs(5);
//	  M485_CTRL_OFF;//接收状态
//	  LED_TXD485_OFF;
//}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 485发送字符串程序
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void Send485SeriaDate(UINT8 *Ram, UINT16 len)
{
  UINT8 ite;
	
	M485_CTRL_ON;
	LED_TXD485_ON;//发射状态
	DelayMs(1);
	for(ite=0;ite<len;ite++)
	{
		 ACC = Ram[ite];
		 if(P == 0)
		 {
			 TB8 = 0;
		 }
		 else
		 {
			 TB8 =1;
		 }
		 Usart1_SendByte(Ram[ite]);
	}
	DelayMs(1);
	M485_CTRL_OFF;//接收状态
	LED_TXD485_OFF;
}
/*******************************************************************************
								END					
*******************************************************************************/
