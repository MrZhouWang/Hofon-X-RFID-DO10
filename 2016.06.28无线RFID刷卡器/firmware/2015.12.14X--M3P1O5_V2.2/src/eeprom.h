#ifndef _EEPROM_H_
#define _EEPROM_H_

//#include "STC_NEW_8051.H"
#include <intrins.H>
#include "mytype.h"

#define CMD_TRIG_H	0x5a//0x46
#define CMD_TRIG_L      0xa5//0xB9 jhtemp

/*
sfr IAP_DATA    = 0xC2;
sfr IAP_ADDRH   = 0xC3;
sfr IAP_ADDRL   = 0xC4;
sfr IAP_CMD     = 0xC5;
sfr IAP_TRIG    = 0xC6;
sfr IAP_CONTR   = 0xC7;
*/
//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0x80 //系统工作时钟<30MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x82 //系统工作时钟<20MHz 时，对IAP_CONTR 寄存器设置此值
#define ENABLE_ISP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x84 //系统工作时钟<6MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x85 //系统工作时钟<3MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x86 //系统工作时钟<2MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x87 //系统工作时钟<1MHz 时，对IAP_CONTR 寄存器设置此值

#define SECTOR_0_ADDR       0
#define SECTOR_1_ADDR       1 
#define SECTOR_2_ADDR       2
#define SECTOR_3_ADDR       3
#define SECTOR_4_ADDR       4
#define SECTOR_5_ADDR       5
#define SECTOR_6_ADDR       6
#define SECTOR_7_ADDR       7
#define EEPROM_BLOCK_SIZE   0x0200

extern UINT16 code SECTOR_PAGE[];

UINT8 eeprom_read_byte(const UINT8 * addr);
void  eeprom_write_byte(const UINT8 * addr, UINT8 temp);
void  eeprom_read_block(UINT8 * Buffer, UINT8 * beginAddr, UINT8 len);
void  eeprom_write_block(UINT8 * Buffer, UINT8 * beginAddr, UINT8 len);
void eeprom_erase_sector(UINT16 pageAddr);

void eeprom_test(void);

#define EE_ADDRESS		0
#define EE_FINISHEDLED 	1
#define EE_WTTIME		2
#define EE_FNDEFINE		3
#define EE_DISPMODE		4


void SaveParaToBlock(void);
void LoadParaInEeprom(void);




#endif









