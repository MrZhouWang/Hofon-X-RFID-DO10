/**********************RC522实验开发板例程************************
*  CPU:STC89C52RC
*  晶振:11.0592MHZ
------------------------------------------------*/
#include <STC15F2K60S2.H>

#include  <STDIO.H>
#include <string.h>
#include "main.h"
#include "mfrc522.h"

unsigned char code data1[16] = {0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char code data2[4]  = {0,0,0,0x01};
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 

unsigned char g_ucTempbuf[20];                        
void delay1(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
	for(y=110;y>0;y--);	
}  
  void led_on()
{
	LED4=0;
	LED6=0;
	LED5=0 ;
}
void led_off()
{
	LED4=1;
	LED6=1;
	LED5=1 ;
}
void led_blink()
{
	led_on();
	delay1(12000);
	led_off();
	delay1(12000);
}

void main( )
{    
     unsigned char status,i,j;
	  unsigned int temp;
     InitializeSystem( );
     PcdReset();
     PcdAntennaOff(); 
     PcdAntennaOn(); 
     while ( 1 )
     {   


         status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
         if (status != MI_OK)
         {    
              InitializeSystem( );
              PcdReset();
              PcdAntennaOff(); 
              PcdAntennaOn(); 
			  led_blink();
			  continue;
         }
		 else
		 {
		      

		 }	     
			printf("卡的类型:");
            for(i=0;i<2;i++)
					{
						temp=g_ucTempbuf[i];
						printf("%X",temp);	
					}
			
         status = PcdAnticoll(g_ucTempbuf);//防冲撞
         if (status != MI_OK)
         {    continue;    }

         
					////////以下为超级终端打印出的内容////////////////////////
				
					printf("卡序列号：");	//超级终端显示,
					for(i=0;i<4;i++)
					{
						temp=g_ucTempbuf[i];
						printf("%X",temp);
						
					}

					///////////////////////////////////////////////////////////

         status = PcdSelect(g_ucTempbuf);//选定卡片
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//验证卡片密码
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdWrite(1, data1);//写块
         if (status != MI_OK)
         {    continue;    }
while(1)
		{
         status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
         if (status != MI_OK)
         {   
 		 InitializeSystem( );
     PcdReset();
     PcdAntennaOff(); 
     PcdAntennaOn(); 
			  continue;
         }
		 status = PcdAnticoll(g_ucTempbuf);//防冲撞
         if (status != MI_OK)
         {    continue;    }
		status = PcdSelect(g_ucTempbuf);//选定卡片
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//验证卡片密码
         if (status != MI_OK)
         {    continue;    }


         status = PcdValue(PICC_DECREMENT,1,data2);//扣款
         if (status != MI_OK)
         {    continue;    }
		 
         status = PcdBakValue(1, 2);//块备份
         if (status != MI_OK)
         {    continue;    }
         
         status = PcdRead(2, g_ucTempbuf);//读块
         if (status != MI_OK)
         {    continue;    }
        	printf("卡读块：");	//超级终端显示,
         for(i=0;i<16;i++)
					{
						temp=g_ucTempbuf[i];
						printf("%X",temp);
						
					}

		 printf("\n");

	    for(i=0;i<250;i++)//先输出一种频率的声音
        {
         for(j=0;j<80;j++);

        }
		delay1(100);
		delay1(100);

		delay1(200);			 		         
		 PcdHalt();
		}
    }
}


/////////////////////////////////////////////////////////////////////
//系统初始化
/////////////////////////////////////////////////////////////////////
void InitializeSystem()
{
/*	P0M1 = 0x0; P0M2 = 0x0;
	P1M1 = 0x0;	P1M2 = 0x0;                      
	P3M1 = 0x0; P3M2 = 0xFF; 
*/	P0 = 0xFF; P1 = 0xFF; P3 = 0xFF;P2 = 0xFF;

 	TMOD=0x21;		   //设T0为方式1，GATE=1；
	SCON=0x50;
	TH1=0xFD;          //波特率为2400bps
	TL1=0xFD;
	TH0=0;
	TL0=0; 
	TR0=1;  
	ET0=1;             //允许T0中断
	TR1=1;			   //开启定时器
	TI=1;
	EA=1;			   	//开启总中断
	
	ES = 1;
	RI = 1;

 
}



   
  

