#define TIMER_GLOBAL
#include "include.h"
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 定时器初始化
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void InitT0(void)
{
    //定时1ms//默认为12T模式=>T0_CLK=SYS_CLK/12=11 059 200 /12 =921600	    
    TMOD = (TMOD & 0xf0) | 0x01;    //mode 1: 16 bit counter	//
    TH0  = TH0_INIT;
    TL0  = TL0_INIT;  

    ET0  = 1;                       //allow interrupt
    TR0  = 1;                       //open timer0
}
