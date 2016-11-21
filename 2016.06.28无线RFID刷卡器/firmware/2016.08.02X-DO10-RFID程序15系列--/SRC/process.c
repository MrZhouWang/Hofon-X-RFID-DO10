#define PRO_GLOBAL
#include "include.h"
UINT8 SetAddrFlag=0;
UINT8 ID_ASCII[35]={0};
/*******************************************************************************
	 					�Զ��ֽڶ�������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Read(void)
{
	UINT8 tIndex = 0,ID_ASCIINum=0;
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
				FramToSendbuff((UINT8 *)g_ucTempbuf,ID_ASCII,ID_Num);	//ת��ΪASCII��
				ID_ASCIINum=ID_Num*2;           //ID_ASCIINum ������ȫ�ֱ���Ҫ��Ȼÿ�ζ��� ���������Խ��
				uart.wBuffer[2]	= ID_ASCIINum;
				for(tIndex = 0; tIndex < ID_ASCIINum; tIndex++)
					uart.wBuffer[3+tIndex] = ID_ASCII[tIndex];
				uart.wLen = 3+ID_ASCIINum ;
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
		retCRC = CRC16   	(uart.rBuffer, uart.rIndex-2);		//CRC
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H
		//usCRC=retCRC; 
		if (retCRC == usCRC) 						       //crcУ��ɹ� 
		{
			if(uart.rBuffer[3] == 0X00)					 //��ַ����  00 06 ?? 00 ?? ?? CRCH CRCL
			{
				ResetAddr();
			}
			else if(uart.rBuffer[3] == 0X01)	   //���յ�ַ������ʾ������ȴ�����ȷ��  00 06 ?? 01 ?? ## CRCH CRCL
			{
				if(beSettingAddr != 0X02)	         //ֻ������һ�ε�ַ
				{
					beSettingAddr = 1;        			 //���ø�����Ϣ����ʾ��ַ�趨��ʾ
					toSettedAddr[0] = (UINT8)(uart.rBuffer[5]/100);
					toSettedAddr[1] = (UINT8)(((UINT8)(uart.rBuffer[5]%100))/10);
					toSettedAddr[2] = uart.rBuffer[5]%10;
	        SetAddrFlag=180;                 //ָʾ����˸������������
				}
			}
			else if(uart.rBuffer[3] == 0X11)		//�㲥���   00 06 ?? 11 ?? ?? CRCH CRCL
			{
				ClearAll();
				beSettingAddr = 0;
			}
			else if(uart.rBuffer[3] == 0X20)	  //�㲥���ò���1(��ǩ��Ӧ���������ɺ�����ģʽ)  00 06 ?? 20 ?? ## CRCH CRCL
			{
				led_flash_NO = (UINT8)(uart.rBuffer[5]);  //��ʾ����ֵ
				led_flash_mode = LED_FLASH_MODE_ALWAYS;
				FinishedLED = led_flash_NO;
				if(FinishedLED > 3)
				   FinishedLED = 0;
				SaveParaToBlock();
			}
			else if(uart.rBuffer[3] == 0X21)				    //�㲥���ò���2(һ�Զ�����ʱ���������ȴ�ʱ��)  00 06 ?? 21 ?? ## CRCH CRCL
			{
				WtTime = (UINT8)(uart.rBuffer[5]);			  //��ʾ������ʱ��ֵ
				if(WtTime > 5)
				   WtTime = 5;
				SaveParaToBlock();
			}
			else if(uart.rBuffer[3] == 0X22)				    //�㲥���ò���3(���ܼ��趨)  00 06 ?? 22 ?? ## CRCH CRCL
			{
				FnDefine = (UINT8)(uart.rBuffer[5]);	    //��ʾ���ܼ�����ֵ
				if(FnDefine > 1)
				   FnDefine = 0;
				SaveParaToBlock();
			}
			else if(uart.rBuffer[3] == 0X23)				    //�㲥���ò���4(��ʾģʽ�趨)			00 06 ?? 23 ?? ## CRCH CRCL
			{
			   
				DispMode = (UINT8)(uart.rBuffer[5]);      //��ʾ��ʾģʽ����ֵ
				if(DispMode > 1)
				   DispMode = 0;
				SaveParaToBlock();
			}
		}
	}
/**********��ַһ��һͨ������**********************************************************/
	else if (uart.rBuffer[0] == addr)					//��ַУ��
	{
		retCRC = CRC16(uart.rBuffer, uart.rIndex-2);
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H
    //usCRC=retCRC;
		if (retCRC == usCRC) 						      //crcУ��ɹ� 
		{
			switch (uart.rBuffer[1])
			{
			case READ_MULTI_BYTE:	              //�ն��ϴ����ݣ���ȡɨ��ǹ
			{
					if((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x01))			//��ַȷ�� Ҳ����������Э��
					{
						ACK_Multi_ReadTestAddr();			//��ַ�趨�Ƿ�ɹ�����
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x03))	//�Ĵ�������03��ʾ��ȡ�Ƿ�OK��������
					{        				        
						ACK_Multi_Read();							//Ӧ��Modbus 0x03�Ųɼ���ť����
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x05))	//�Ĵ�������05��ʾ��ȡɨ�赽��������Ϣ
					{        				        
						ACK_Multi_Read();							//Ӧ��Modbus 0x03�Ųɼ���ť����
					}
					break;
      }
			case WRITE_SINGLE_BYTE:	            //�������´�����,0X06��modbus�����������������������Ϣ����
			{
					if(uart.rIndex == 0X08)				  //����06�������8�ֽڳ���
					{
						ACK_Single_Write();		        //�ѽ��յ����������ȥ				
						if(uart.rBuffer[3] == 0X00)     
						{
							ClearAll();                 //���ȫ��(�����ʾ��Ϣ���ơ�������)�����OK��Ϣ+ɨ��ǹ��Ϣ+������Ϣ
						}
						
						else if(uart.rBuffer[3] == 0X01 || uart.rBuffer[3] == 0X02)
						{
							Display_Clear();            //�����ʾ����(����ơ�������)�������������OK��Ϣ+ɨ��ǹ��Ϣ+������Ϣ
						}
						
						else if(uart.rBuffer[3] == 0x05)	
						{
				         RC522_rFlag  = OFF;   //���RFID��Ϣ	
						}
						
						else if(uart.rBuffer[3] == 0X10)   		
						{
							IsOkPressed = FALSE;		    //���OK��ť��Ϣ					
						}
						else if(uart.rBuffer[3] >= 0x11 && uart.rBuffer[3] <= 0x1A)	//��������
						{
							lightTowerIndex = uart.rBuffer[3]-0X11;
							tempValue = (uart.rBuffer[5] >> 5)&0X07;
							
							LightTowerCtl[lightTowerIndex].led_status = tempValue != 0 ? ON :OFF;  //���ơ���ȿ������ݻ�ȡ
							
							switch((uart.rBuffer[5] >> 3)&0X3)   //�������ڿ������ݻ�ȡ
							{
							case 0:		LightTowerCtl[lightTowerIndex].led_flash_period = 100;		break;
							case 1:		LightTowerCtl[lightTowerIndex].led_flash_period = 200;		break;
							case 2:		LightTowerCtl[lightTowerIndex].led_flash_period = 500;		break;
							case 3:		LightTowerCtl[lightTowerIndex].led_flash_period = 1000;		break;
							}
							
						  LightTowerCtl[lightTowerIndex].led_switch_ratio = (uart.rBuffer[5] >> 0)&0X07;//��������������ݻ�ȡ
							
							LightTowerCtl[lightTowerIndex].led_status_cur = OFF;  //��ʼ��״̬����������
							LightTowerCtl[lightTowerIndex].led_flash_count = 0;
						}						
						else if(uart.rBuffer[3] == 0x20)		//��������		
						{
							if(uart.rBuffer[5] == 0x00)	//����
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
						ACK_Multi_Write();//��Ӧ�������͵���Ϣ
						
					  led_flash_NO	 = (uart.rBuffer[6] >> 5)&0X07;    //��˸�����
						
						switch((uart.rBuffer[6] >> 3)&0X3)               //��˸ʱ����
						{
						  case 0:		led_flash_period = 80;		break;
						  case 1:		led_flash_period = 160;		break;
						  case 2:		led_flash_period = 320;		break;
						  case 3:		led_flash_period = 640;		break;
						  default:	led_flash_period = 640;		break;
						}					
					  led_flash_mode   = (uart.rBuffer[6] >> 0)&0X07; //��˸ģʽ					
						led_flash_status = 0;                           //��˸��������λ
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
	}
	for(i=0;i<50;i++)       //���������յ�����Ϣ
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

	if (uart.rIndex < ONE_CMD_MIN_LENGTH)      //���ݰ����Ȳ���
		 return;   		
  //�ȿ���ַ��CRCУ�顢���ݳ��ȵ��ж�
	//�ٸ��ݵڶ����ֽڵķ�Χ�ж��ǲ�����Э��������Ǿ�Э�����
	//��ַ��CRC����У��,�´�ָ��洢,MODBUSӦ��,CRC_Check�����а�����ַ�趨����
	DiscodeProtocalV1();
}
/*******************************************************************************
								END					
*******************************************************************************/

