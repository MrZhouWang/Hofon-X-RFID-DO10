/**********************RC522实验开发板例程************************
*  CPU:STC89C52RC
*  晶振:11.0592MHZ
------------------------------------------------*/
#define MAIN_GLOBAL
#include "include.h"
unsigned char con_flag;

unsigned char code data1[16] ={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0x08,0x77,0X8F,0x69,0x77,0x77,0x77,0x77,0x77,0x77};//
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char code data2[4]  = {0,0,0,0x01};
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
unsigned char code NewKey[6] = {0x77,0x77,0x77,0x77,0x77,0x77}; 

unsigned char g_ucTempbuf[20];
void led_blink();
void delay_ms(unsigned int z);
void led_off();
void led_on();  
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 主函数  
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void main( )
{    
     unsigned char status,i,m;
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
			  led_on();
			  continue;
         }	     
		 printf("卡的类型:");
         for(i=0;i<2;i++)
		 {
		     temp=g_ucTempbuf[i];
		     printf("%X",temp);	
		 }
			
         status = PcdAnticoll(g_ucTempbuf);//防冲撞
         if (status != MI_OK)
         { 
		     continue;    
	     }        
				
		 printf("卡序列号：");	//超级终端显示,
		 for(i=0;i<4;i++)
		 {
			 temp=g_ucTempbuf[i];
			 printf("%X",temp);			
		 }

         status = PcdSelect(g_ucTempbuf);//选定卡片
         if (status != MI_OK)
         { 
		     
		     continue;    
		 }
		 
         status = PcdAuthState(PICC_AUTHENT1B, 8, DefaultKey, g_ucTempbuf);//验证卡片密码
         if (status != MI_OK)
         {   
		     
		 	 continue;    
		 }

         status = PcdWrite(8, data1);//写块
         if (status != MI_OK)
         {  
		    continue;
	     }
		 printf("\n");
         printf("ok");
         while(1)
		 {
         	status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
         	if (status != MI_OK)
         	{   
			      m++;
				  if(m>5)
				  {
				     m=21;
				     con_flag=0;
				  }
				
 		          InitializeSystem( );
                  PcdReset();
                  PcdAntennaOff(); 
                  PcdAntennaOn(); 
			      continue;
            }
			else
			   m=0;
		   if(con_flag==0)
		   {
			   status = PcdAnticoll(g_ucTempbuf);//防冲撞
	           if (status != MI_OK)
	           {    
			   		continue;  
			   }
			   status = PcdSelect(g_ucTempbuf);//选定卡片
	           if (status != MI_OK)
	           {  
			        continue;   
			   }
	         
	           status = PcdAuthState(PICC_AUTHENT1B, 8, DefaultKey, g_ucTempbuf);//验证卡片密码
	           if (status != MI_OK)
	           {  
			        continue;    
		       }
	
	
//	           status = PcdValue(PICC_DECREMENT,1,data2);//扣款
//	           if (status != MI_OK)
//	           {   
//			        continue;   
//			   }
//			 
//	           status = PcdBakValue(1, 2);//块备份
//	           if (status != MI_OK)
//	           {   
//			   	    continue;    
//			   }
	         
	           status = PcdRead(8, g_ucTempbuf);//读块
	           if (status != MI_OK)
	           {  
			        continue;   
			   }
	           printf("卡读块：");	//超级终端显示,
	           for(i=0;i<16;i++)
			   {
					temp=g_ucTempbuf[i];
					printf("%X",temp);
							
			   }
	
			   printf("\n");
	           led_blink();				 		         
			   PcdHalt();
			   con_flag=1;
			}
		}
    }
}


////////////////////////////////////////////////////////////////////////////////
// 功能描述 ：系统初始化  
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 各种初始化
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 延时1ms
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void delay_ms(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
	for(y=125;y>0;y--);	
} 
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : LED灭 
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void led_on()
{
	LED4=0;
	LED6=0;
	LED5=0 ;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : LED亮 
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void led_off()
{
	LED4=1;
	LED6=1;
	LED5=1 ;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : LED闪烁 
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void led_blink()
{
	led_on();
	delay_ms(300);
	led_off();
	delay_ms(350);
	led_on();
	delay_ms(300);
	led_off();
	delay_ms(250);
	led_on();
	delay_ms(300);
	led_off();
	delay_ms(250);
}


   
  

