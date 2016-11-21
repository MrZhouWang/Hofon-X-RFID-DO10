#define PRO_GLOBAL
#include "include.h"
UINT8 SetAddrFlag=0;
UINT8 ID_ASCII[35]={0};
/*******************************************************************************
	 					对多字节读命令响应
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
			if(RC522_rFlag == ON)
			{
				FramToSendbuff((UINT8 *)g_ucTempbuf,ID_ASCII,ID_Num);	//转化为ASCII码
				ID_ASCIINum=ID_Num*2;           //ID_ASCIINum 不能是全局变量要不然每次二倍 会造成数组越界
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
	 					对多字节写命令正常响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Single_Write(void)
{
	Send485SeriaDate((UINT8 *)uart.rBuffer, uart.rIndex); 
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
	Send485SeriaDate((UINT8 *)uart.wBuffer, uart.wLen); 
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
	UINT8 index = 0,i=0;
	UINT16 usValue = 0; 
	//0号广播命令地址，不需回复
	if(uart.rBuffer[0] == 0x00 && uart.rBuffer[1] == 0X06 && uart.rIndex == 8)	
	{
		retCRC = CRC16   	(uart.rBuffer, uart.rIndex-2);		//CRC
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H
		//usCRC=retCRC; 
		if (retCRC == usCRC) 						       //crc校验成功 
		{
			if(uart.rBuffer[3] == 0X00)					 //地址清零  00 06 ?? 00 ?? ?? CRCH CRCL
			{
				ResetAddr();
			}
			else if(uart.rBuffer[3] == 0X01)	   //接收地址，并显示，进入等待按键确认  00 06 ?? 01 ?? ## CRCH CRCL
			{
				if(beSettingAddr != 0X02)	         //只能设置一次地址
				{
					beSettingAddr = 1;        			 //设置更新信息，显示地址设定提示
					toSettedAddr[0] = (UINT8)(uart.rBuffer[5]/100);
					toSettedAddr[1] = (UINT8)(((UINT8)(uart.rBuffer[5]%100))/10);
					toSettedAddr[2] = uart.rBuffer[5]%10;
	        SetAddrFlag=180;                 //指示灯闪烁及蜂鸣器响声
				}
			}
			else if(uart.rBuffer[3] == 0X11)		//广播清除   00 06 ?? 11 ?? ?? CRCH CRCL
			{
				ClearAll();
				beSettingAddr = 0;
			}
			else if(uart.rBuffer[3] == 0X20)	  //广播设置参数1(标签对应拣货任务完成后亮灯模式)  00 06 ?? 20 ?? ## CRCH CRCL
			{
				led_flash_NO = (UINT8)(uart.rBuffer[5]);  //显示亮灯值
				led_flash_mode = LED_FLASH_MODE_ALWAYS;
				FinishedLED = led_flash_NO;
				if(FinishedLED > 3)
				   FinishedLED = 0;
				SaveParaToBlock();
			}
			else if(uart.rBuffer[3] == 0X21)				    //广播设置参数2(一对多任务时，进度条等待时间)  00 06 ?? 21 ?? ## CRCH CRCL
			{
				WtTime = (UINT8)(uart.rBuffer[5]);			  //显示进度条时间值
				if(WtTime > 5)
				   WtTime = 5;
				SaveParaToBlock();
			}
			else if(uart.rBuffer[3] == 0X22)				    //广播设置参数3(功能键设定)  00 06 ?? 22 ?? ## CRCH CRCL
			{
				FnDefine = (UINT8)(uart.rBuffer[5]);	    //显示功能键定义值
				if(FnDefine > 1)
				   FnDefine = 0;
				SaveParaToBlock();
			}
			else if(uart.rBuffer[3] == 0X23)				    //广播设置参数4(显示模式设定)			00 06 ?? 23 ?? ## CRCH CRCL
			{
			   
				DispMode = (UINT8)(uart.rBuffer[5]);      //显示显示模式定义值
				if(DispMode > 1)
				   DispMode = 0;
				SaveParaToBlock();
			}
		}
	}
/**********地址一对一通信命令**********************************************************/
	else if (uart.rBuffer[0] == addr)					//地址校验
	{
		retCRC = CRC16(uart.rBuffer, uart.rIndex-2);
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H
    //usCRC=retCRC;
		if (retCRC == usCRC) 						      //crc校验成功 
		{
			switch (uart.rBuffer[1])
			{
			case READ_MULTI_BYTE:	              //终端上传数据，读取扫描枪
			{
					if((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x01))			//地址确认 也被当作心跳协议
					{
						ACK_Multi_ReadTestAddr();			//地址设定是否成功测试
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x03))	//寄存器数量03表示读取是否按OK键的命令
					{        				        
						ACK_Multi_Read();							//应答Modbus 0x03号采集按钮命令
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x05))	//寄存器数量05表示读取扫描到的条码信息
					{        				        
						ACK_Multi_Read();							//应答Modbus 0x03号采集按钮命令
					}
					break;
      }
			case WRITE_SINGLE_BYTE:	            //控制器下传数据,0X06号modbus命令：清屏命令；清除按键按下信息命令
			{
					if(uart.rIndex == 0X08)				  //所有06号命令都是8字节长度
					{
						ACK_Single_Write();		        //把接收到的命令反馈回去				
						if(uart.rBuffer[3] == 0X00)     
						{
							ClearAll();                 //清除全部(清除显示信息、灯、蜂鸣器)，清除OK信息+扫描枪信息+灯塔信息
						}
						
						else if(uart.rBuffer[3] == 0X01 || uart.rBuffer[3] == 0X02)
						{
							Display_Clear();            //清除显示部分(清除灯、蜂鸣器)，不包括，清除OK信息+扫描枪信息+灯塔信息
						}
						
						else if(uart.rBuffer[3] == 0x05)	
						{
				         RC522_rFlag  = OFF;   //清除RFID信息	
						}
						
						else if(uart.rBuffer[3] == 0X10)   		
						{
							IsOkPressed = FALSE;		    //清除OK按钮信息					
						}
						else if(uart.rBuffer[3] >= 0x11 && uart.rBuffer[3] <= 0x1A)	//灯塔控制
						{
							lightTowerIndex = uart.rBuffer[3]-0X11;
							tempValue = (uart.rBuffer[5] >> 5)&0X07;
							
							LightTowerCtl[lightTowerIndex].led_status = tempValue != 0 ? ON :OFF;  //亮灯、灭等控制数据获取
							
							switch((uart.rBuffer[5] >> 3)&0X3)   //亮灭周期控制数据获取
							{
							case 0:		LightTowerCtl[lightTowerIndex].led_flash_period = 100;		break;
							case 1:		LightTowerCtl[lightTowerIndex].led_flash_period = 200;		break;
							case 2:		LightTowerCtl[lightTowerIndex].led_flash_period = 500;		break;
							case 3:		LightTowerCtl[lightTowerIndex].led_flash_period = 1000;		break;
							}
							
						  LightTowerCtl[lightTowerIndex].led_switch_ratio = (uart.rBuffer[5] >> 0)&0X07;//亮灭比例控制数据获取
							
							LightTowerCtl[lightTowerIndex].led_status_cur = OFF;  //初始化状态、计数数据
							LightTowerCtl[lightTowerIndex].led_flash_count = 0;
						}						
						else if(uart.rBuffer[3] == 0x20)		//锁定解锁		
						{
							if(uart.rBuffer[5] == 0x00)	//锁定
							{
								SystemLocked = OFF;
							}
							else if(uart.rBuffer[5] == 0x01)	//解锁
							{
								SystemLocked = ON;
							}
						}
					}
					break;
			}
			case WRITE_MULTI_BYTE:		//终端下传数据  	清除已经按下的OK信息
			{
					if (uart.rBuffer[8] == uart.rIndex-11 && uart.rBuffer[8] <= MAX_SRCODE_NUM)	
					{
						ACK_Multi_Write();//回应主机发送的消息
						
					  led_flash_NO	 = (uart.rBuffer[6] >> 5)&0X07;    //闪烁灯序号
						
						switch((uart.rBuffer[6] >> 3)&0X3)               //闪烁时间间隔
						{
						  case 0:		led_flash_period = 80;		break;
						  case 1:		led_flash_period = 160;		break;
						  case 2:		led_flash_period = 320;		break;
						  case 3:		led_flash_period = 640;		break;
						  default:	led_flash_period = 640;		break;
						}					
					  led_flash_mode   = (uart.rBuffer[6] >> 0)&0X07; //闪烁模式					
						led_flash_status = 0;                           //闪烁用数据置位
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
	for(i=0;i<50;i++)       //最后清除接收到的信息
	{
		uart.rBuffer[i]=0;
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

	if (uart.rIndex < ONE_CMD_MIN_LENGTH)      //数据包长度不够
		 return;   		
  //先看地址、CRC校验、数据长度等判断
	//再根据第二个字节的范围判断是采用新协议解析还是旧协议解析
	//地址与CRC数据校验,下传指令存储,MODBUS应答,CRC_Check函数中包含地址设定命令
	DiscodeProtocalV1();
}
/*******************************************************************************
								END					
*******************************************************************************/

