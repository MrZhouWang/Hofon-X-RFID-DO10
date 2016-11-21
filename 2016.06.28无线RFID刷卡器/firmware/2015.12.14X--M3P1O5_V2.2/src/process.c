#include "main.h"
#include "uart.h"
#include "process.h"
#include "led.h"
#include "key.h"
#include "eeprom.h"

#define DATA_LEN     		        7
#define DEVICE_NAME_LEN		23

/*******************************************************************************
	 					对多字节读命令响应
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
	//看子命令
	switch(uart.rBuffer[5])
	{
		case 0X03:		//读取按钮按下信息
		{
			uart.wBuffer[2] = 0x01;                         //1 byte
			if(IsOkPressed)
				uart.wBuffer[3] = 0x01;                     //Pressed
			else
				uart.wBuffer[3] = 0x00;                     //UnPressed
			uart.wLen       = 4; 
			break;
		}
		case 0X05:		//读取扫描枪
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
	 					对多字节写命令正常响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Single_Write(void)
{
	Send485SerialByte((UINT8 *)uart.rBuffer, uart.rIndex); 
}
/*******************************************************************************
	 					对多字节写命令正常响应
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
	//显示清除,数据复位
	ClearAll();
	beSettingAddr = 0;
	//地址清零
	address[0] = 0;		address[1] = 0;		address[2] = 0;
	SaveParaToBlock();
}
/*******************************************************************************
	 					数据包校验,并将MODBUS的数据存入cmdBuffer[]中
	input:   none     
	output:  none	
	
rBuffer[0]: ==> ADDR
rBuffer[1]: ==> MODBUS功能码
rBuffer[2]: ==> 起始地址H
rBuffer[3]: ==> 起始地址L
rBuffer[4]: ==> 寄存器数量H
rBuffer[5]: ==> 寄存器数量L

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

	//0号广播命令地址，不需回复
	if(uart.rBuffer[0] == 0x00 && uart.rBuffer[1] == 0X06 && uart.rIndex == 8)	
	{
		LED_RXD485_ON;
		LED_TXD485_ON;
			
		retCRC = CRC16   	(uart.rBuffer, uart.rIndex-2);		//CRC
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H
		if (retCRC == usCRC) 						//crc校验成功 
		{
			//00 06 ?? 00 ?? ?? CRCH CRCL
			if(uart.rBuffer[3] == 0X00)					//地址清零
			{
				ResetAddr();
			}
			//00 06 ?? 01 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X01)				//接收地址，并显示，进入等待按键确认
			{
				if(beSettingAddr != 0X02)
				{
					beSettingAddr = 1;
					toSettedAddr[0] = (UINT8)(uart.rBuffer[5]/100);
					toSettedAddr[1] = (UINT8)(((UINT8)(uart.rBuffer[5]%100))/10);
					toSettedAddr[2] = uart.rBuffer[5]%10;
					//设置更新信息，显示地址设定提示
					led_flash_mode = LED_FLASH_MODE_ALWAYS;	//常亮
					led_flash_NO = LED_GREEN;				//接收前亮第一组灯，接收后亮第二组灯
					digital_led_refresh = ON;
				}
			}
			//0X10段	广播显示
			//00 06 ?? 10 ## ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X10)				//广播显示命令,如果## ## == 0X00 0X00，则显示地址
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
				//显示灯塔
				for(lightTowerIndex = 0; lightTowerIndex < 6; lightTowerIndex++)
				{
					LightTowerCtl[lightTowerIndex].led_status = ON;
					//亮灭周期控制数据获取
					LightTowerCtl[lightTowerIndex].led_flash_period = 80;
					//亮灭比例控制数据获取
				    LightTowerCtl[lightTowerIndex].led_switch_ratio = 0X03;//(1:on	5:off)
					//初始化状态、计数数据
					LightTowerCtl[lightTowerIndex].led_status_cur = OFF;
					LightTowerCtl[lightTowerIndex].led_flash_count = 0;
				}
			}
			//00 06 ?? 11 ?? ?? CRCH CRCL
			else if(uart.rBuffer[3] == 0X11)				//广播清除
			{
				ClearAll();
				beSettingAddr = 0;
			}
			//0X20段	广播存储段
			//00 06 ?? 20 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X20)				//广播设置参数1(标签对应拣货任务完成后亮灯模式)
			{
			    //显示亮灯值
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
			else if(uart.rBuffer[3] == 0X21)				//广播设置参数2(一对多任务时，进度条等待时间)
			{
			    //显示进度条时间值
			    led[0].num = 0X0F;	    led[1].num = 0X02;	    led[2].num = (UINT8)(uart.rBuffer[5]);
			    led[0].show = ON;	    led[1].show = ON;	    led[2].show = ON;

				WtTime = (UINT8)(uart.rBuffer[5]);
				if(WtTime > 5)
				   WtTime = 5;
				SaveParaToBlock();
			}
			//00 06 ?? 22 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X22)				//广播设置参数3(功能键设定)
			{
			    //显示功能键定义值
			    led[0].num = 0X0F;	    led[1].num = 0X03;	    led[2].num = (UINT8)(uart.rBuffer[5]);
			    led[0].show = ON;	    led[1].show = ON;	    led[2].show = ON;

				FnDefine = (UINT8)(uart.rBuffer[5]);
				if(FnDefine > 1)
				   FnDefine = 0;
				SaveParaToBlock();
			}
			//00 06 ?? 23 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X23)				//广播设置参数4(显示模式设定)
			{
			    //显示显示模式定义值
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
	//其它通信命令
	else if (uart.rBuffer[0] == addr)					//地址校验
	{
		LED_RXD485_ON;
		LED_TXD485_ON;

		retCRC = CRC16(uart.rBuffer, uart.rIndex-2);
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H

		if (retCRC == usCRC) 						//crc校验成功 
		{
			switch (uart.rBuffer[1])
			{
			case READ_MULTI_BYTE:	//终端上传数据，读取扫描枪
				{
					if((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x01))			//寄存器数量01表示尝试地址是否已经设定的命令
					{
						ACK_Multi_ReadTestAddr();										//地址设定是否成功测试
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x03))	//寄存器数量03表示读取是否按OK键的命令
					{        				        
						ACK_Multi_Read();												//应答Modbus 0x03号采集按钮命令
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x05))	//寄存器数量05表示读取扫描到的条码信息
					{        				        
						ACK_Multi_Read();												//应答Modbus 0x03号采集按钮命令
					}
					break;
        		}
			case WRITE_SINGLE_BYTE:	//控制器下传数据,0X06号modbus命令：清屏命令；清除按键按下信息命令
				{
					if(uart.rIndex == 0X08)						//所有06号命令都是8字节长度
					{
						ACK_Single_Write();
						//清除全部(清除显示信息、灯、蜂鸣器)，清除OK信息+扫描枪信息+灯塔信息
						if(uart.rBuffer[3] == 0X00)
						{
							ClearAll();
						}
						//清除显示部分(清除显示信息、灯、蜂鸣器)，不包括，清除OK信息+扫描枪信息+灯塔信息
						else if(uart.rBuffer[3] == 0X01 || uart.rBuffer[3] == 0X02)
						{
							Display_Clear();
						}
						//清除扫描枪信息
						else if(uart.rBuffer[3] == 0x05)		
						{
				            uart232.rFlag   = OFF;
				            uart232.rIndex  = 0;
							digital_led_refresh = ON;
						}
						//清除OK按钮信息
						else if(uart.rBuffer[3] == 0X10)   		
						{
							IsOkPressed = FALSE;							
						}
						//锁定解锁
						if(uart.rBuffer[3] == 0x20)				
						{
							if(uart.rBuffer[5] == 0x00)			//锁定
							{
								SystemLocked = OFF;
							}
							else if(uart.rBuffer[5] == 0x01)	//解锁
							{
								SystemLocked = ON;
							}
						}
						else if(uart.rBuffer[3] >= 0x11 && uart.rBuffer[3] <= 0x16)	//灯塔控制
						{
							lightTowerIndex = uart.rBuffer[3]-0X11;
							tempValue = (uart.rBuffer[5] >> 5)&0X07;
							//亮灯、灭等控制数据获取
							LightTowerCtl[lightTowerIndex].led_status = tempValue != 0 ? ON :OFF;
							//亮灭周期控制数据获取
							switch((uart.rBuffer[5] >> 3)&0X3)
							{
							case 0:		LightTowerCtl[lightTowerIndex].led_flash_period = 80;		break;
							case 1:		LightTowerCtl[lightTowerIndex].led_flash_period = 160;		break;
							case 2:		LightTowerCtl[lightTowerIndex].led_flash_period = 320;		break;
							case 3:		LightTowerCtl[lightTowerIndex].led_flash_period = 640;		break;
							}
							//亮灭比例控制数据获取
						    LightTowerCtl[lightTowerIndex].led_switch_ratio = (uart.rBuffer[5] >> 0)&0X07;
							//初始化状态、计数数据
							LightTowerCtl[lightTowerIndex].led_status_cur = OFF;
							LightTowerCtl[lightTowerIndex].led_flash_count = 0;
						}
					}
					break;
				}
			case WRITE_MULTI_BYTE:														//终端下传数据
				{
					if (uart.rBuffer[8] == uart.rIndex-11 && uart.rBuffer[8] <= MAX_SRCODE_NUM)	
					{
						ACK_Multi_Write();
						//开始处理命令，包括数据暂存接收、各种响应等
						//此处升级版本中应获取亮灯模式 uart.rBuffer[6]，蜂鸣器模式 uart.rBuffer[7]
						//uart.rBuffer[6]==>LED指示灯的显示方式处理
						//闪烁灯序号
					    led_flash_NO	 = (uart.rBuffer[6] >> 5)&0X07;
						//闪烁时间间隔
						switch((uart.rBuffer[6] >> 3)&0X3)
						{
						case 0:		led_flash_period = 80;		break;
						case 1:		led_flash_period = 160;		break;
						case 2:		led_flash_period = 320;		break;
						case 3:		led_flash_period = 640;		break;
						default:	led_flash_period = 640;		break;
						}
						//闪烁模式
					    led_flash_mode   = (uart.rBuffer[6] >> 0)&0X07;
						//闪烁用数据置位
						led_flash_status = 0;
						sys.led_flash_count = 0;
						//显示字符信息
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
	 					处理MODBUS总站下发的命令
0X03命令:  功能码             =>  1 BYTE  (0X03)
                        起始地址      =>   2 BYTE (0X0000~0XFFFF)
                        寄存器数量=>   2 BYTE (1~125)                       

0X10命令:  功能码             =>   1 BYTE (0X10)
                        起始地址      =>   2 BYTE (0X0000~0XFFFF)
                        寄存器数量=>   2 BYTE (1~120)
                        字节数            =>   1 BYTE (2 )
                        寄存器数1    =>   1 BYTE  ==> 是否请条形码缓冲区,0不清,1清除
                        寄存器数2    =>   1 BYTE  ==> 输出5个LED等控制状态
                                                                                   //xxxabcdef => 0~4bit 代表灯的开关状态
*******************************************************************************/
void Cmd_Process(void)
{
    //数据包长度不够
	UINT8 lightTowerIndex = 0;
	UINT8 tempDiscodeValue = 0X00;
	if (uart.rIndex < ONE_CMD_MIN_LENGTH)
		return;   		
    //先看地址、CRC校验、数据长度等判断
	//再根据第二个字节的范围判断是采用新协议解析还是旧协议解析
	//地址与CRC数据校验,下传指令存储,MODBUS应答,CRC_Check函数中包含地址设定命令
	DiscodeProtocalV1();
}
/*******************************************************************************
								END					
*******************************************************************************/

