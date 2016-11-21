#ifndef _MRC522_HAL_H_
#define _MRC522_HAL_H_
#include "include.h"


//¶Ë¿Ú¶¨Òå

sbit     MF522_SDA  = P2^4;		     //SDA
sbit     MF522_SCK  = P2^6;        //SCK
sbit     MF522_MOSI = P2^7;        //MOSI
sbit     MF522_MISO = P0^0;        //MISO
sbit     MF522_RST  = P2^5;        //RST



extern void WriteRawRC(unsigned char Address, unsigned char value);
extern unsigned char ReadRawRC(unsigned char Address);
extern void SetBitMask(unsigned char reg,unsigned char mask) ;
extern void ClearBitMask(unsigned char reg,unsigned char mask) ;

#endif