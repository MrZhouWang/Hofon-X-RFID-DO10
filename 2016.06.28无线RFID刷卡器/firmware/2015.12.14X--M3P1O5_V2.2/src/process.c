#include "main.h"
#include "uart.h"
#include "process.h"
#include "led.h"
#include "key.h"
#include "eeprom.h"

#define DATA_LEN     		        7
#define DEVICE_NAME_LEN		23

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
			if(uart232.rFlag == ON)
			{
				uart.wBuffer[2]	= uart232.rIndex;
				for(tIndex = 0; tIndex < uart232.rIndex; tIndex++)
					uart.wBuffer[3+tIndex] = uart232.rBuffer[tIndex];
				uart.wLen = 3+uart232.rIndex;
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
    
	Send485SerialByte((UINT8 *)uart.wBuffer, uart.wLen); 
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
	Send485SerialByte((UINT8 *)uart.wBuffer, uart.wLen); 
}
/*******************************************************************************
	 					�Զ��ֽ�д����������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Single_Write(void)
{
	Send485SerialByte((UINT8 *)uart.rBuffer, uart.rIndex); 
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
	Send485SerialByte((UINT8 *)uart.wBuffer, uart.wLen); 
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
	UINT8 index = 0;
	UINT16 usValue = 0;

	//0�Ź㲥�����ַ������ظ�
	if(uart.rBuffer[0] == 0x00 && uart.rBuffer[1] == 0X06 && uart.rIndex == 8)	
	{
		LED_RXD485_ON;
		LED_TXD485_ON;
			
		retCRC = CRC16   	(uart.rBuffer, uart.rIndex-2);		//CRC
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
				if(beSettingAddr != 0X02)
				{
					beSettingAddr = 1;
					toSettedAddr[0] = (UINT8)(uart.rBuffer[5]/100);
					toSettedAddr[1] = (UINT8)(((UINT8)(uart.rBuffer[5]%100))/10);
					toSettedAddr[2] = uart.rBuffer[5]%10;
					//���ø�����Ϣ����ʾ��ַ�趨��ʾ
					led_flash_mode = LED_FLASH_MODE_ALWAYS;	//����
					led_flash_NO = LED_GREEN;				//����ǰ����һ��ƣ����պ����ڶ����
					digital_led_refresh = ON;
				}
			}
			//0X10��	�㲥��ʾ
			//00 06 ?? 10 ## ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X10)				//�㲥��ʾ����,���## ## == 0X00 0X00������ʾ��ַ
			{
				usValue = ((UINT16)(uart.rBuffer[4])) *256 + (UINT16)(uart.rBuffer[5]);
				if(usValue == 0X00)
				{
				    led[0].num = address[0];	    led[1].num = address[1];	    led[2].num = address[2];
				}
				else if(usValue == 10001)
				{
				    led[0].num = 0X0F;	    		led[1].num = 0X01;	    		led[2].num = FinishedLED;
				}
				else if(usValue == 10002)
				{
				    led[0].num = 0X0F;	    		led[1].num = 0X02;	    		led[2].num = WtTime;
				}
				else if(usValue == 10003)
				{
				    led[0].num = 0X0F;	    		led[1].num = 0X03;	    		led[2].num = FnDefine;
				}
				else if(usValue == 10004)
				{
				    led[0].num = 0X0F;	    		led[1].num = 0X04;	    		led[2].num = DispMode;
				}
				else
				{
					led[0].num = (UINT8)((  usValue % 1000)  / 100);
					led[1].num = (UINT8)((  usValue % 100)    / 10);    
					led[2].num = (UINT8)(   usValue  % 10);
				}
			    led[0].show = ON;	    led[1].show = ON;	    led[2].show = ON;

				led_flash_NO = LED_GREEN;
				led_flash_mode = LED_FLASH_MODE_ALWAYS;
				//��ʾ����
				for(lightTowerIndex = 0; lightTowerIndex < 6; lightTowerIndex++)
				{
					LightTowerCtl[lightTowerIndex].led_status = ON;
					//�������ڿ������ݻ�ȡ
					LightTowerCtl[lightTowerIndex].led_flash_period = 80;
					//��������������ݻ�ȡ
				    LightTowerCtl[lightTowerIndex].led_switch_ratio = 0X03;//(1:on	5:off)
					//��ʼ��״̬����������
					LightTowerCtl[lightTowerIndex].led_status_cur = OFF;
					LightTowerCtl[lightTowerIndex].led_flash_count = 0;
				}
			}
			//00 06 ?? 11 ?? ?? CRCH CRCL
			else if(uart.rBuffer[3] == 0X11)				//�㲥���
			{
				ClearAll();
				beSettingAddr = 0;
			}
			//0X20��	�㲥�洢��
			//00 06 ?? 20 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X20)				//�㲥���ò���1(��ǩ��Ӧ���������ɺ�����ģʽ)
			{
			    //��ʾ����ֵ
			    led[0].num = 0X0F;	    led[1].num = 0X01;	    led[2].num = (UINT8)(uart.rBuffer[5]);
			    led[0].show = ON;	    led[1].show = ON;	    led[2].show = ON;

				led_flash_NO = (UINT8)(uart.rBuffer[5]);
				led_flash_mode = LED_FLASH_MODE_ALWAYS;
				FinishedLED = led_flash_NO;
				if(FinishedLED > 3)
				   FinishedLED = 0;
				SaveParaToBlock();
			}
			//00 06 ?? 21 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X21)				//�㲥���ò���2(һ�Զ�����ʱ���������ȴ�ʱ��)
			{
			    //��ʾ������ʱ��ֵ
			    led[0].num = 0X0F;	    led[1].num = 0X02;	    led[2].num = (UINT8)(uart.rBuffer[5]);
			    led[0].show = ON;	    led[1].show = ON;	    led[2].show = ON;

				WtTime = (UINT8)(uart.rBuffer[5]);
				if(WtTime > 5)
				   WtTime = 5;
				SaveParaToBlock();
			}
			//00 06 ?? 22 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X22)				//�㲥���ò���3(���ܼ��趨)
			{
			    //��ʾ���ܼ�����ֵ
			    led[0].num = 0X0F;	    led[1].num = 0X03;	    led[2].num = (UINT8)(uart.rBuffer[5]);
			    led[0].show = ON;	    led[1].show = ON;	    led[2].show = ON;

				FnDefine = (UINT8)(uart.rBuffer[5]);
				if(FnDefine > 1)
				   FnDefine = 0;
				SaveParaToBlock();
			}
			//00 06 ?? 23 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X23)				//�㲥���ò���4(��ʾģʽ�趨)
			{
			    //��ʾ��ʾģʽ����ֵ
			    led[0].num = 0X0F;	    led[1].num = 0X04;	    led[2].num = (UINT8)(uart.rBuffer[5]);
			    led[0].show = ON;	    led[1].show = ON;	    led[2].show = ON;

				DispMode = (UINT8)(uart.rBuffer[5]);
				if(DispMode > 1)
				   DispMode = 0;
				SaveParaToBlock();
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
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H

		if (retCRC == usCRC) 						//crcУ��ɹ� 
		{
			switch (uart.rBuffer[1])
			{
			case READ_MULTI_BYTE:	//�ն��ϴ����ݣ���ȡɨ��ǹ
				{
					if((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x01))			//�Ĵ�������01��ʾ���Ե�ַ�Ƿ��Ѿ��趨������
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
						//���ɨ��ǹ��Ϣ
						else if(uart.rBuffer[3] == 0x05)		
						{
				            uart232.rFlag   = OFF;
				            uart232.rIndex  = 0;
							digital_led_refresh = ON;
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
						else if(uart.rBuffer[3] >= 0x11 && uart.rBuffer[3] <= 0x16)	//��������
						{
							lightTowerIndex = uart.rBuffer[3]-0X11;
							tempValue = (uart.rBuffer[5] >> 5)&0X07;
							//���ơ���ȿ������ݻ�ȡ
							LightTowerCtl[lightTowerIndex].led_status = tempValue != 0 ? ON :OFF;
							//�������ڿ������ݻ�ȡ
							switch((uart.rBuffer[5] >> 3)&0X3)
							{
							case 0:		LightTowerCtl[lightTowerIndex].led_flash_period = 80;		break;
							case 1:		LightTowerCtl[lightTowerIndex].led_flash_period = 160;		break;
							case 2:		LightTowerCtl[lightTowerIndex].led_flash_period = 320;		break;
							case 3:		LightTowerCtl[lightTowerIndex].led_flash_period = 640;		break;
							}
							//��������������ݻ�ȡ
						    LightTowerCtl[lightTowerIndex].led_switch_ratio = (uart.rBuffer[5] >> 0)&0X07;
							//��ʼ��״̬����������
							LightTowerCtl[lightTowerIndex].led_status_cur = OFF;
							LightTowerCtl[lightTowerIndex].led_flash_count = 0;
						}
					}
					break;
				}
			case WRITE_MULTI_BYTE:														//�ն��´�����
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
						//��ʾ�ַ���Ϣ
					 	DispInfoCur.SRCodeLen = uart.rBuffer[8];
						for(index = 0; index < DispInfoCur.SRCodeLen; index++)
							DispInfoCur.SRCode[index] = uart.rBuffer[9+index];
						digital_led_refresh = ON;
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
	UINT8 lightTowerIndex = 0;
	UINT8 tempDiscodeValue = 0X00;
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

