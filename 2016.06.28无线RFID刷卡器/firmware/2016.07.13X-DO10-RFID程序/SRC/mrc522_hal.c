#include "include.h"
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 写RC522寄存器
// 输入参数 : 
// 返回参数 : 
// 说明     : Address[IN]:寄存器地址   value[IN]:写入的值          
////////////////////////////////////////////////////////////////////////////////
void WriteRawRC(unsigned char Address, unsigned char value)
{  
    unsigned char i, ucAddr;

    MF522_SCK = 0;
    MF522_SDA = 0;
    ucAddr = ((Address<<1)&0x7E);

    for(i=8;i>0;i--)
    {
        MF522_MOSI = ((ucAddr&0x80)==0x80);
        MF522_SCK = 1;
        ucAddr <<= 1;
        MF522_SCK = 0;
    }

    for(i=8;i>0;i--)
    {
        MF522_MOSI = ((value&0x80)==0x80);
        MF522_SCK = 1;
        value <<= 1;
        MF522_SCK = 0;
    }
    MF522_SDA = 1;
    MF522_SCK = 1;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 读RC522寄存器
// 输入参数 : 
// 返回参数 : 读出的值
// 说明     : Address[IN]:寄存器地址         
////////////////////////////////////////////////////////////////////////////////
unsigned char ReadRawRC(unsigned char Address)
{
     unsigned char i, ucAddr;
     unsigned char ucResult=0;

     MF522_SCK = 0;
     MF522_SDA = 0;
     ucAddr = ((Address<<1)&0x7E)|0x80;

     for(i=8;i>0;i--)
     {
         MF522_MOSI = ((ucAddr&0x80)==0x80);
         MF522_SCK = 1;
         ucAddr <<= 1;
         MF522_SCK = 0;
     }

     for(i=8;i>0;i--)
     {
         MF522_SCK = 1;
         ucResult <<= 1;
         ucResult|=(bit)MF522_MISO;
         MF522_SCK = 0;
     }

     MF522_SDA = 1;
     MF522_SCK = 1;
     return ucResult;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 置RC522寄存器位
// 输入参数 : 
// 返回参数 : 
// 说明     : reg[IN]:寄存器地址        mask[IN]:置位值       
////////////////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 清RC522寄存器位
// 输入参数 : 
// 返回参数 : 
// 说明     : reg[IN]:寄存器地址     mask[IN]:清位值       
////////////////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
}