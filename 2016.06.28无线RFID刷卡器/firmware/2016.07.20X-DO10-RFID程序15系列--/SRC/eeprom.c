
#include "include.h"


typedef union union_temp16
{
    UINT16 un_temp16 ;
    UINT8  un_temp8[2];
}UNION_16;


UINT8 Byte_Read(UINT8 * add);              //��һ�ֽڣ�����ǰ���IAP ����
void Byte_Program(UINT8 * add, UINT8 ch);  //�ֽڱ�̣�����ǰ���IAP ����
void Sector_Erase(UINT16 add);            //��������
void IAP_Disable();                       //�ر�IAP ����


//��һ�ֽڣ�����ǰ���IAP ���ܣ����:DPTR = �ֽڵ�ַ������:A = �����ֽ�
UINT8 Byte_Read(UINT8 * add)
{
    UNION_16 my_unTemp16;
    
    IAP_DATA = 0x00; //������ݼĴ���
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM �ֽڶ�����

    my_unTemp16.un_temp16 = (UINT16)add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ

    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = CMD_TRIG_L;   //����A5h ��ISP/IAP ����������������
    _nop_();
    //EA = 1;
    IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    return (IAP_DATA);
}

//�ֽڱ�̣�����ǰ���IAP ���ܣ����:DPTR = �ֽڵ�ַ, A= �����ֽڵ�����
void Byte_Program(UINT8 * add, UINT8 ch)
{
    UNION_16 my_unTemp16;
    
    IAP_CONTR = ENABLE_ISP;         //�� IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM �ֽڱ������

    my_unTemp16.un_temp16 = (UINT16)add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ

    IAP_DATA = ch;                  //Ҫ��̵��������ͽ�IAP_DATA �Ĵ���
    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = CMD_TRIG_L;   //����A5h ��ISP/IAP ����������������
    _nop_();
    //EA = 1;
    IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}

//��������, ���:DPTR = ������ַ
void Sector_Erase(UINT16 add)
{
    UNION_16 my_unTemp16;
    
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM ������������

    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ

    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = CMD_TRIG_L;   //����A5h ��ISP/IAP ����������������
    _nop_();
    //EA = 1;
    IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}

void IAP_Disable()
{
    //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    IAP_CONTR = 0;      //�ر�IAP ����
    IAP_CMD   = 0;      //������Ĵ���,ʹ����Ĵ���������,�˾�ɲ���
    IAP_TRIG  = 0;      //��������Ĵ���,ʹ������Ĵ����޴���,�˾�ɲ���
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
        buffer[0] = 1;         	//Ĭ�ϼ��ز���
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




