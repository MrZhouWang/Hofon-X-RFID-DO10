
#include "include.h"


typedef union union_temp16
{
    UINT16 un_temp16 ;
    UINT8  un_temp8[2];
}UNION_16;


UINT8 Byte_Read(UINT8 * add);              //读一字节，调用前需打开IAP 功能
void Byte_Program(UINT8 * add, UINT8 ch);  //字节编程，调用前需打开IAP 功能
void Sector_Erase(UINT16 add);            //擦除扇区
void IAP_Disable();                       //关闭IAP 功能


//读一字节，调用前需打开IAP 功能，入口:DPTR = 字节地址，返回:A = 读出字节
UINT8 Byte_Read(UINT8 * add)
{
    UNION_16 my_unTemp16;
    
    IAP_DATA = 0x00; //清空数据寄存器
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM 字节读命令

    my_unTemp16.un_temp16 = (UINT16)add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = CMD_TRIG_L;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    return (IAP_DATA);
}

//字节编程，调用前需打开IAP 功能，入口:DPTR = 字节地址, A= 须编程字节的数据
void Byte_Program(UINT8 * add, UINT8 ch)
{
    UNION_16 my_unTemp16;
    
    IAP_CONTR = ENABLE_ISP;         //打开 IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM 字节编程命令

    my_unTemp16.un_temp16 = (UINT16)add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    IAP_DATA = ch;                  //要编程的数据先送进IAP_DATA 寄存器
    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = CMD_TRIG_L;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

//擦除扇区, 入口:DPTR = 扇区地址
void Sector_Erase(UINT16 add)
{
    UNION_16 my_unTemp16;
    
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM 扇区擦除命令

    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = CMD_TRIG_L;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

void IAP_Disable()
{
    //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    IAP_CONTR = 0;      //关闭IAP 功能
    IAP_CMD   = 0;      //清命令寄存器,使命令寄存器无命令,此句可不用
    IAP_TRIG  = 0;      //清命令触发寄存器,使命令触发寄存器无触发,此句可不用
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}
 

/*************************************************************************
	                    Read len bytes from eeprom
**************************************************************************/

void eeprom_read_block(UINT8 * Buffer, UINT8 * beginAddr, UINT8 len)
{
    UINT8 i;

    for (i = 0; i < len; i++)
    {
        Buffer[i] = Byte_Read(beginAddr);
        beginAddr++;
    }
    
    return;
}

/*************************************************************************
	                    write len bytes to eeprom
**************************************************************************/
void eeprom_write_block(UINT8 * Buffer, UINT8 * beginAddr, UINT8 len)
{
    UINT8 i;

    for (i = 0; i < len; i++)
    {
        Byte_Program(beginAddr, Buffer[i]);
        beginAddr++;
    }

    return;
}

/*************************************************************************
	                    erase a sector
**************************************************************************/

//data flash: all 4K 
//512Byte/sector, 8 sectors
//Writing data flash must erase the sector, and then write every byte;
//Each sector address as follows:
UINT16 code SECTOR_PAGE[] = {	   //STC12C5A60S2   2 SECTOR?
    0x0000, 0x0010, 0x0020, 0x0600, 
    0x0800, 0x0A00, 0x0C00, 0x0E00
};

void eeprom_erase_sector(UINT16 pageAddr)
{
    Sector_Erase(pageAddr);
    
    return;
}

/*************************************
	Read Parameter from inn EEPROM
	with verify
*************************************/
#define PARA_LEN    5
UINT8 ReadParaInEeprom(UINT8 offset)
{
	UINT8 i = offset;
	UINT8 para_page_0[PARA_LEN];
	UINT8 para_page_1[PARA_LEN];
	UINT8 para_page_2[PARA_LEN];

	if (offset >= PARA_LEN) return 0xff;
	
    eeprom_read_block(para_page_0, (UINT8 *)SECTOR_PAGE[0], PARA_LEN);
    eeprom_read_block(para_page_1, (UINT8 *)SECTOR_PAGE[1], PARA_LEN);
    eeprom_read_block(para_page_2, (UINT8 *)SECTOR_PAGE[2], PARA_LEN);
   
	if (para_page_0[i] == para_page_1[i])
	{
//		if (para_page_1[i] != para_page_2[i])           //sector 2 error
//		{
		    para_page_2[i] = para_page_0[i];            //Modify error byte
//            eeprom_erase_sector(SECTOR_PAGE[2]);        //Erase sector 2
//                                                        //Write sector 2
//	    	eeprom_write_block(para_page_2, (UINT8 *)SECTOR_PAGE[2] , PARA_LEN);
//		}
		return para_page_0[i];	
	}
	else if (para_page_0[i] == para_page_2[i])          //sector 1 error
	{
        para_page_1[i] = para_page_0[i];                //Modify error byte
//    	eeprom_erase_sector(SECTOR_PAGE[1]);            //Erase sector 1
//    	                                                //Write sector 1
//	    eeprom_write_block(para_page_1, (UINT8 *)SECTOR_PAGE[1] , PARA_LEN);
    	return para_page_0[i];	
	}
	else                                                //Sector 0 error
	{
	    para_page_0[i] = para_page_1[i];                //Modify error byte
//	    eeprom_erase_sector(SECTOR_PAGE[0]);            //Erase sector 0
//	    eeprom_write_block(para_page_0, (UINT8 *)SECTOR_PAGE[0] , PARA_LEN);
    	return para_page_1[i];				
	}
}
/*******************************************************************************
	 		    		save Parameter to EEPROM
	input:   none
	output:  none					
*******************************************************************************/
void SaveParaToBlock()
{
	UINT8 i;
	UINT8 buffer[5];
	buffer[0] = address[0] * 100 + address[1]*10 + address[2];
	buffer[1] = FinishedLED;
	buffer[2] = WtTime;
	buffer[3] = FnDefine;
	buffer[4] = DispMode;
	eeprom_erase_sector(SECTOR_PAGE[0]); 
	for (i=0; i<3; i++)
	{   
		eeprom_write_block (buffer, (UINT8 *)SECTOR_PAGE[i], 5);
		led_blink();
	}
}
/*******************************************************************************
	 		    		Load Parameter from EEPROM
	input:   none		   
	output:  none					
*******************************************************************************/
void LoadParaInEeprom(void)
{
	UINT8 buffer[5];

	buffer[0] = ReadParaInEeprom(EE_ADDRESS);
	buffer[1] = ReadParaInEeprom(EE_FINISHEDLED);
	buffer[2] = ReadParaInEeprom(EE_WTTIME);
	buffer[3] = ReadParaInEeprom(EE_FNDEFINE);
	buffer[4] = ReadParaInEeprom(EE_DISPMODE);

    if (buffer[0] > MAX_ADDR)   
    {
        buffer[0] = 1;         	//默认加载参数
    }
    if (buffer[1] > 3) 			//Finished LED		0,1,2,3
    {
        buffer[1] = 0;         
    }
    if (buffer[2] > 5)			//WtTime			0:80ms;	1:160ms;	2:240ms;	3:320ms;	4:400;	5:480
    {
        buffer[2] = 5;         
    }
    if (buffer[3] > 1)			//FnDefine			0:ZeroPick;		1:FullContainer Pick
    {
        buffer[3] = 0;         
    }
    if (buffer[4] > 1)			//DispMode			0:TaskDisp;		1:Text Disp
    {
        buffer[4] = 0;         
    }

	address[0] = (UINT8)(buffer[0]/100);
	address[1] = (UINT8)(((UINT8)(buffer[0]%100))/10);
	address[2] = buffer[0]%10;

	FinishedLED = buffer[1];
	WtTime = buffer[2];
	FnDefine = buffer[3];
	DispMode = buffer[4];
}
/*******************************************************************************
								END					
*******************************************************************************/




