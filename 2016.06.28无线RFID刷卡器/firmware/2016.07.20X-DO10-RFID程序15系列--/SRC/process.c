#include "include.h"

/*******************************************************************************
	 					�Զ��ֽڶ�������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Read(void)
{
	UINT8 tIndex = 0;
	UINT8 addr;
	UINT16 usCRC;
        
	addr    = address[0] * 100 + address[1]*10 + address[2];    

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x03;                         //function code
	//��������
	switch(uart.rBuffer[5])
	{
		case 0X03:		//��ȡ��ť������Ϣ
		{
			uart.wBuffer[2] = 0x01;                         //1 byte
			if(IsOkPressed)
				uart.wBuffer[3] = 0x01;                     //Pressed
			else
				uart.wBuffer[3] = 0x00;                     //UnPressed
			uart.wLen       = 4; 
			break;
		}
		case 0X05:		//��ȡɨ��ǹ
		{
			if(RC522_rFlag == ON)
			{
				uart.wBuffer[2]	= ID_Num;
				for(tIndex = 0; tIndex < ID_Num; tIndex++)
					uart.wBuffer[3+tIndex] = g_ucTempbuf[tIndex];
				uart.wLen = 3+ID_Num;
			}
			else		 
			{
				uart.wBuffer[2] = 0;
				uart.wLen = 3;
			}
			break;
		}
		default:
		{
			break;
		}
	}
	usCRC = CRC16(uart.wBuffer, uart.wLen);
	uart.wBuffer[uart.wLen] = (UINT8)usCRC;
	uart.wBuffer[uart.wLen+1] = (UINT8)(usCRC>>8);
	uart.wLen += 2;                            
//    Send485SeriaDate((UINT8 *)g_ucTempbuf, ID_Num);
	Send485SeriaDate((UINT8 *)uart.wBuffer, uart.wLen); 
}
void ACK_Multi_ReadTestAddr(void)
{
	UINT8 addr;
	UINT16 usCRC;
        
	addr    = address[0] * 100 + address[1]*10 + address[2];    

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x03;                         //function code
	uart.wBuffer[2] = 0x00;                     	//Len
	usCRC = CRC16(uart.wBuffer, 3);
	uart.wBuffer[3] = (UINT8)usCRC;
	uart.wBuffer[4] = (UINT8)(usCRC>>8);
	uart.wLen   = 5;                            
	Send485SeriaDate((UINT8 *)uart.wBuffer, uart.wLen); 
}
/*******************************************************************************
	 					�Զ��ֽ�д����������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Single_Write(void)
{
	Send485SeriaDate((UINT8 *)uart.rBuffer, uart.rIndex); 
}
/*******************************************************************************
	 					�Զ��ֽ�д����������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Write(void)
{
	UINT8 addr    = address[0] * 100 + address[1]*10 + address[2];
	UINT16 usCRC;

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x10;                         //function code
	uart.wBuffer[2] = 0x00;                         //start addr
	uart.wBuffer[3] = 0x00;
	uart.wBuffer[4] = 0x00;
	uart.wBuffer[5] = 0x05;

	usCRC = CRC16(uart.wBuffer, 6);

	uart.wBuffer[6] = (UINT8)usCRC;
	uart.wBuffer[7] = (UINT8)(usCRC>>8);

	uart.wLen        = 8; 
	Send485SeriaDate((UINT8 *)uart.wBuffer, uart.wLen); 
}
/*******************************************************************************
	input:   none     
	output:  none					
*******************************************************************************/
void ResetAddr(void)
{
	//��ʾ���,���ݸ�λ
	ClearAll();
	beSettingAddr = 0;
	//��ַ����
	address[0] = 0;		address[1] = 0;		address[2] = 0;
	SaveParaToBlock();
}
/*******************************************************************************
	 					���ݰ�У��,����MODBUS�����ݴ���cmdBuffer[]��
	input:   none     
	output:  none	
	
rBuffer[0]: ==> ADDR
rBuffer[1]: ==> MODBUS������
rBuffer[2]: ==> ��ʼ��ַH
rBuffer[3]: ==> ��ʼ��ַL
rBuffer[4]: ==> �Ĵ�������H
rBuffer[5]: ==> �Ĵ�������L

0X03:
rBuffer[6]: ==> CRCH
rBuffer[7]: ==> CRCL

0X10:
rBuffer[6]: ==> 2
rBuffer[7]: ==> 0/1
rBuffer[8]: ==> XXXABCDE
rBuffer[9]: ==> CRCH
rBuffer[10]: ==> CRCL

*******************************************************************************/
UINT8 DiscodeProtocalV1(void)
{
	UINT8 addr    = address[0] * 100 + address[1]*10 + address[2];
	UINT16 usCRC, retCRC;
	UINT8 lightTowerIndex = 0;
	UINT8 tempValue = 0;
	UINT8 index = 0,i=0;
	UINT16 usValue = 0; 
	//0�Ź㲥�����ַ������ظ�
	if(uart.rBuffer[0] == 0x00 && uart.rBuffer[1] == 0X06 && uart.rIndex == 8)	
	{
		LED_RXD485_ON;
		LED_TXD485_ON;
		//Send485SerialByte((UINT8 *)uart.rBuffer, uart.rIndex); 	
		retCRC = CRC16   	(uart.rBuffer, uart.rIndex-2);		//CRC
		retCRC=0;
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H
		if (retCRC == usCRC) 						//crcУ��ɹ� 
		{
			//00 06 ?? 00 ?? ?? CRCH CRCL
			if(uart.rBuffer[3] == 0X00)					//��ַ����
			{
				ResetAddr();
			}
			//00 06 ?? 01 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X01)				//���յ�ַ������ʾ������ȴ�����ȷ��
			{
				if(beSettingAddr != 0X02)	 //ֻ������һ�ε�ַ
				{
					beSettingAddr = 1;
					toSettedAddr[0] = (UINT8)(uart.rBuffer[5]/100);
					toSettedAddr[1] = (UINT8)(((UINT8)(uart.rBuffer[5]%100))/10);
					toSettedAddr[2] = uart.rBuffer[5]%10;
					//���ø�����Ϣ����ʾ��ַ�趨��ʾ
					led_flash_mode = LED_FLASH_MODE_ALWAYS;	//����
					led_flash_NO = LED_GREEN;				//����ǰ����һ��ƣ����պ����ڶ����
				}
			}
			//00 06 ?? 11 ?? ?? CRCH CRCL
			else if(uart.rBuffer[3] == 0X11)				//�㲥���
			{
				ClearAll();
				beSettingAddr = 0;
			}
		}
		LED_RXD485_OFF;
		LED_TXD485_OFF;
	}
	//����ͨ������
	else if (uart.rBuffer[0] == addr)					//��ַУ��
	{
		LED_RXD485_ON;
		LED_TXD485_ON;

		retCRC = CRC16(uart.rBuffer, uart.rIndex-2);
		retCRC=0;
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H

		if (retCRC == usCRC) 						//crcУ��ɹ� 
		{
			switch (uart.rBuffer[1])
			{
			case READ_MULTI_BYTE:	//�ն��ϴ����ݣ���ȡɨ��ǹ
				{
					if((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x01))			//��ַȷ�� Ҳ����������Э��
					{
						ACK_Multi_ReadTestAddr();										//��ַ�趨�Ƿ�ɹ�����
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x03))	//�Ĵ�������03��ʾ��ȡ�Ƿ�OK��������
					{        				        
						ACK_Multi_Read();												//Ӧ��Modbus 0x03�Ųɼ���ť����
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x05))	//�Ĵ�������05��ʾ��ȡɨ�赽��������Ϣ
					{        				        
						ACK_Multi_Read();												//Ӧ��Modbus 0x03�Ųɼ���ť����
					}
					break;
        		}
			case WRITE_SINGLE_BYTE:	//�������´�����,0X06��modbus�����������������������Ϣ����
				{
					if(uart.rIndex == 0X08)						//����06�������8�ֽڳ���
					{
						ACK_Single_Write();
						//���ȫ��(�����ʾ��Ϣ���ơ�������)�����OK��Ϣ+ɨ��ǹ��Ϣ+������Ϣ
						if(uart.rBuffer[3] == 0X00)
						{
							ClearAll();
						}
						//�����ʾ����(�����ʾ��Ϣ���ơ�������)�������������OK��Ϣ+ɨ��ǹ��Ϣ+������Ϣ
						else if(uart.rBuffer[3] == 0X01 || uart.rBuffer[3] == 0X02)
						{
							Display_Clear();
						}
						
						else if(uart.rBuffer[3] == 0x05)//���ɨ��ǹ��Ϣ		
						{
				            RC522_rFlag  = OFF;
						}
						//���OK��ť��Ϣ
						else if(uart.rBuffer[3] == 0X10)   		
						{
							IsOkPressed = FALSE;							
						}
						//��������
						if(uart.rBuffer[3] == 0x20)				
						{
							if(uart.rBuffer[5] == 0x00)			//����
							{
								SystemLocked = OFF;
							}
							else if(uart.rBuffer[5] == 0x01)	//����
							{
								SystemLocked = ON;
							}
						}
					}
					break;
				}
			case WRITE_MULTI_BYTE:		//�ն��´�����  	����Ѿ����µ�OK��Ϣ
				{
					if (uart.rBuffer[8] == uart.rIndex-11 && uart.rBuffer[8] <= MAX_SRCODE_NUM)	
					{
						ACK_Multi_Write();
						//��ʼ����������������ݴ���ա�������Ӧ��
						//�˴������汾��Ӧ��ȡ����ģʽ uart.rBuffer[6]��������ģʽ uart.rBuffer[7]
						//uart.rBuffer[6]==>LEDָʾ�Ƶ���ʾ��ʽ����
						//��˸�����
					    led_flash_NO	 = (uart.rBuffer[6] >> 5)&0X07;
						//��˸ʱ����
						switch((uart.rBuffer[6] >> 3)&0X3)
						{
						case 0:		led_flash_period = 80;		break;
						case 1:		led_flash_period = 160;		break;
						case 2:		led_flash_period = 320;		break;
						case 3:		led_flash_period = 640;		break;
						default:	led_flash_period = 640;		break;
						}
						//��˸ģʽ
					    led_flash_mode   = (uart.rBuffer[6] >> 0)&0X07;
						//��˸��������λ
						led_flash_status = 0;
						sys.led_flash_count = 0;
					}
					break;
				}
			default:
				{
					break;
				}
			}
		}
		LED_RXD485_OFF;
		LED_TXD485_OFF;
	}
	for(i=0;i<50;i++)
	{
		uart.rBuffer[i]=0;
	}	
	uart.rIndex = 0;
	uart.rFlag  = OFF;

	return OK;
}

/*******************************************************************************
	 					����MODBUS��վ�·�������
0X03����:  ������             =>  1 BYTE  (0X03)
                        ��ʼ��ַ      =>   2 BYTE (0X0000~0XFFFF)
                        �Ĵ�������=>   2 BYTE (1~125)                       

0X10����:  ������             =>   1 BYTE (0X10)
                        ��ʼ��ַ      =>   2 BYTE (0X0000~0XFFFF)
                        �Ĵ�������=>   2 BYTE (1~120)
                        �ֽ���            =>   1 BYTE (2 )
                        �Ĵ�����1    =>   1 BYTE  ==> �Ƿ��������뻺����,0����,1���
                        �Ĵ�����2    =>   1 BYTE  ==> ���5��LED�ȿ���״̬
                                                                                   //xxxabcdef => 0~4bit ����ƵĿ���״̬
*******************************************************************************/
void Cmd_Process(void)
{
    //���ݰ����Ȳ���
	if (uart.rIndex < ONE_CMD_MIN_LENGTH)
		return;   		
    //�ȿ���ַ��CRCУ�顢���ݳ��ȵ��ж�
	//�ٸ��ݵڶ����ֽڵķ�Χ�ж��ǲ�����Э��������Ǿ�Э�����
	//��ַ��CRC����У��,�´�ָ��洢,MODBUSӦ��,CRC_Check�����а�����ַ�趨����
	DiscodeProtocalV1();
}
/*******************************************************************************
								END					
*******************************************************************************/

