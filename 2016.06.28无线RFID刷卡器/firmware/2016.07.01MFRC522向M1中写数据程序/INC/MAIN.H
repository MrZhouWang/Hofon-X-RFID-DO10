#ifndef __MAIN_H_
#define __MAIN_H

#ifdef MAIN_GLOBAL
#define MAIN_EXT 
#else
#define MAIN_EXT extern
#endif

//////////////////////////////////
//端口定义
//
//sbit     MF522_SDA  = P2^2;		//SDA
//sbit     MF522_SCK  = P2^1;        //SCK
//sbit     MF522_MOSI = P3^7;         //MOSI
//sbit     MF522_MISO = P2^3;        //MISO
//sbit     MF522_RST  = P2^4;         //RST
////指示灯
//sbit     LED_GREEN  = P2^5;  
////蜂鸣器引脚定义
//sbit     LED4    = P0^4;
//sbit     LED5    = P0^5;
//sbit     LED6    = P0^6;
//
//sbit Speaker  =  P2^6;






sbit     MF522_SDA  = P2^4;		//SDA
sbit     MF522_SCK  = P2^6;        //SCK
sbit     MF522_MOSI = P2^7;         //MOSI
sbit     MF522_MISO = P0^0;        //MISO
sbit     MF522_RST  = P2^5;         //RST
//蜂鸣器引脚定义
sbit     LED4    = P3^3;
sbit     LED5    = P5^5;
sbit     LED6    = P5^7;
/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
MAIN_EXT void InitializeSystem();                                   
MAIN_EXT void delay_ms(unsigned int z);
MAIN_EXT void delay1(unsigned int z);
MAIN_EXT void led_on();
MAIN_EXT void led_off();
MAIN_EXT void led_blink();                                   

#endif






