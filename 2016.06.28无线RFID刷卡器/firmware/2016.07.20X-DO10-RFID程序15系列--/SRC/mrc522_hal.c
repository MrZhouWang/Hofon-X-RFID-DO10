#include "include.h"
////////////////////////////////////////////////////////////////////////////////
// �������� : дRC522�Ĵ���
// ������� : 
// ���ز��� : 
// ˵��     : Address[IN]:�Ĵ�����ַ   value[IN]:д���ֵ          
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
// �������� : ��RC522�Ĵ���
// ������� : 
// ���ز��� : ������ֵ
// ˵��     : Address[IN]:�Ĵ�����ַ         
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
// �������� : ��RC522�Ĵ���λ
// ������� : 
// ���ز��� : 
// ˵��     : reg[IN]:�Ĵ�����ַ        mask[IN]:��λֵ       
////////////////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}
////////////////////////////////////////////////////////////////////////////////
// �������� : ��RC522�Ĵ���λ
// ������� : 
// ���ز��� : 
// ˵��     : reg[IN]:�Ĵ�����ַ     mask[IN]:��λֵ       
////////////////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
}