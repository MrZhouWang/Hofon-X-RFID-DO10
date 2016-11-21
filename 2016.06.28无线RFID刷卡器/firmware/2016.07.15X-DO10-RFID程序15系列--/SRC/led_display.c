#include "include.h"
//
///*尽量按ASCII码来作表 ，LED是共阳极的，A=b0,B=b1,C=b2,D=b3,E=b4,F=b5,G=b6,H=b7,-*/
//UINT8  xdata   LED_TBL[LED_TBL_MAX] = {
///*---------------------------------------------------------------------*/
////数码管显示0-9的七段码字节
////0-15，0--9,A-f
////0	  1    2    3    4     5    6    7    8    9	a	 b    c    d    e    f   
//0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
////16-31，0.--9.,A.-f.
////0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   a.   b.   c.   d.   e.   f.
////0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e,  
////g    H    i   j    k     L   m    n  //o   P    q     r    s    t    U    v     
//0XFF,0x8B,0XFF,0xF1,0XFF,0xc7,0XFF,0xab,0xa3,0x8c,0XFF,0XFF,0x92,0x87,0xc1,0xe3,
////32-47                                                   +		    -
////0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x8f,0xff,0xbf,0xff,0xff,
////w   x    y    z //36 G亮  f亮  a亮 b亮  c亮  d亮  e亮    +   G亮	-	 h亮
//0XFF,0x89,0XFF,0XFF,  0xBF,0xdf,0xfe,0xfd,0xfb,0xf7,0xef,0x8f,0xbf,0xbf,0x7f,0xff,
////48--63
////0	  1    2    3    4     5    6    7    8    9  //0.   1.   2.   3.   4.   5.//:    ;    <    =    >    ?  
//0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x40,0x79,0x24,0x30,0x19,0x12,//0xff,0xff,0xff,0xff,0xff,0xff,
////64
////@
//0xff, 
////65--90  
////A	  B    C    D    E     F    G    H    I    J    K    L    M    N
//0x88,0x83,0xc6,0xa1,0x86,0x8e,0xc2,0x89,0xf9,0xe1,0xbf,0xc7,0x8f,0xab,
////O	  P    Q    R    S     T    U    V    W    X    Y    Z	
//0xa3,0x8c,0x98,0xaf,0x92,0x87,0xc1,0xe3,0x9c,0xff,0x91,0xa4,
////91--96  '['==turn on; ']'==turn off;
////[	  \    ]    ^    _     .
////0x00,0xff,0xff,0xff,0xfd,0xff,
////           6.   7.   8.   9.
//0x00,0xff,  0x02,0x78,0x00,0x10,
////97-122
////a	  b    c    d    e     f    g    h    i    j    k    l    m    n
//0x88,0x83,0xa7,0xa1,0x86,0x8e,0xc2,0x89,0xfb,0xe1,0xbf,0xc7,0x8f,0xab,
////o	  p    q    r    s     t    u    v    w    x    y    z
//0xa3,0x8c,0x98,0xaf,0x92,0x87,0xc1,0xe3,0x9c,0xff,0x91,0xa4,
////123-127
//0xBF,0xBF,0xBF,0xBF,0xBF,
//};
//
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : LED数码管SPI控制IO初始化
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :   
//////////////////////////////////////////////////////////////////////////////////
//void digital_led_cntrl_Init(void)
//{	
//	OE_SET;			//DISABLE
//	SH_CLK_CLR;
//	ST_CLK_CLR;
//	OE_CLR;			//ENALBE
//	
//	ucShift = 0;
//}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : LED数码管 根据位置开通对应数码管  
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :   
//////////////////////////////////////////////////////////////////////////////////
//void led_SupplyPower(void)
//{
//	UINT8 led_pwr_no;              
//
//	led_pwr_no = ucShift;
//
//	if(led_pwr_no == DIGITAL_LED1)//百位数码管电源
//	{
////		LED_PORT_OUT =  0x1f;                               //LED 1-LED5 ==> OFF
//		digital_led1_port = DIGITAL_LED_PWR_ON;        
//		digital_led2_port = DIGITAL_LED_PWR_OFF; 
//		digital_led3_port = DIGITAL_LED_PWR_OFF; 
//	}
//	else if(led_pwr_no == DIGITAL_LED2)//十位数码管电源
//	{
////		LED_PORT_OUT =  0x1f;                               //LED 1-LED5 ==> OFF
//		digital_led1_port = DIGITAL_LED_PWR_OFF;
//		digital_led2_port = DIGITAL_LED_PWR_ON; 
//		digital_led3_port = DIGITAL_LED_PWR_OFF; 
//	}
//	else if(led_pwr_no == DIGITAL_LED3)//个位数码管电源
//	{
////		LED_PORT_OUT =  0x1f;                               //LED 1-LED5 ==> OFF
//		digital_led1_port = DIGITAL_LED_PWR_OFF;
//		digital_led2_port = DIGITAL_LED_PWR_OFF; 
//		digital_led3_port = DIGITAL_LED_PWR_ON; 
//	}
//}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 显示当前扫描的led内容   
//// 输入参数 : 当前要显示的数值
//// 返回参数 : 
//// 说明     :   
//////////////////////////////////////////////////////////////////////////////////
//void led_Show(UINT8 ucValue)
//{
//	UINT8 i;
//
//	ST_CLK_CLR;
//	for (i=0; i<8; i++)
//	{
//		if (ucValue & 0x80)
//			SDATA_SET;
//		else
//			SDATA_CLR;
//		SH_CLK_CLR;
//		NOP;
//		SH_CLK_SET;
//		ucValue = ucValue << 1; 
//	}
//	
//	ST_CLK_SET;
//	led_SupplyPower();						      //供电
//}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 显示一个digital led  
//// 输入参数 : 
//// 返回参数 : 
//// 说明     : 显示一个数码管数字，扫描时间: 72us 
//////////////////////////////////////////////////////////////////////////////////
//void led_Scan(void)
//{
//    UINT8 ucValue;
//
//    if (sys.led_scan_flag == OFF) return;
//    sys.led_scan_flag = OFF;
//
//	//非配置状态(显示数值状态)下，并且当前不是修改数值下，则先控制当前刷新的位数，数值位，高位0不显示
//    if (led[ucShift].show == ON)
//    {
//        if(led[ucShift].num >= LED_TBL_MAX )
//        {
//            led[ucShift].num = LED_TBL_MAX -1;
//        }
//        ucValue = LED_TBL[led[ucShift].num];     //查表 	......
//    }
//    else
//    {
//        ucValue = DISPLAY_OFF;                                      //关闭显示
//    }
//    led_Show(ucValue);						            //显示
//
//    ucShift++;
//    if (ucShift >= LED_NUMBER) 
//    {
//    	ucShift = 0; 
//    }
//}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 清除命令(地址清除时、广播清除时、显示清除时)
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
//void Display_Clear(void)
//{
//	//显示信息清除
//	UINT8 i;
//	DispInfoCur.SRCodeLen = 0;
//	for (i=0; i<LED_NUMBER; i++)
//	{
//		led[i].num = 0;
//		led[i].show = OFF;
//	}
//	//指示灯清除
//	led_flash_NO = LED_NONE;
//	//蜂鸣器清除
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 数码管显示控制
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
//void digital_led_disp(void)
//{
//	SINT16 i=0,j=0;
//	//控制显示
//	for(i = 0; i < LED_NUMBER; i++)
//	{
//		led[i].show = OFF;
//	}
//	//如果232口有扫描数据,则显示扫描进来的数据
//	if(uart232.rFlag == ON)
//	{
//		j = LED_NUMBER-1;
//		for(i = uart232.rIndex-1; i>= 0 && j>=0; i--)
//		{
//			if(uart232.rBuffer[i] >= '0' && uart232.rBuffer[i] <= '9')
//			{
//				led[j].num = uart232.rBuffer[i]-'0';
//				led[j].show = ON;
//			}
//			else if(uart232.rBuffer[i] >= 'A' && uart232.rBuffer[i] <= 'Z')
//			{
//				led[j].num = uart232.rBuffer[i]-'A'+10;
//				led[j].show = ON;
//			}
//			else if(uart232.rBuffer[i] >= 'a' && uart232.rBuffer[i] <= 'z')
//			{
//				led[j].num = uart232.rBuffer[i]-'a'+10;
//				led[j].show = ON;
//			}
//			else if(uart232.rBuffer[i] == '-')
//			{
//				led[j].num = LED_TBL_MAX-1;
//				led[j].show = ON;
//			}
//			if(uart232.rBuffer[i] != '\r' && uart232.rBuffer[i] !='\n')
//				j--;
//		}
//	}
//	//否则,如果有显示信息,则显示信息
//	else if(DispInfoCur.SRCodeLen > 0)
//	{
//		j = LED_NUMBER-1;
//		for(i = DispInfoCur.SRCodeLen-1; i>=0 && j>=0; i--)
//		{
//			if(DispInfoCur.SRCode[i] >= '0' && DispInfoCur.SRCode[i] <= '9')
//			{
//				led[j].num = DispInfoCur.SRCode[i]-'0';
//				led[j].show = ON;
//			}
//			else if(DispInfoCur.SRCode[i] >= 'A' && DispInfoCur.SRCode[i] <= 'Z')
//			{
//				led[j].num = DispInfoCur.SRCode[i]-'A'+10;
//				led[j].show = ON;
//			}
//			else if(DispInfoCur.SRCode[i] >= 'a' && DispInfoCur.SRCode[i] <= 'z')
//			{
//				led[j].num = DispInfoCur.SRCode[i]-'a'+10;
//				led[j].show = ON;
//			}
//			else if(DispInfoCur.SRCode[i] == '-')
//			{
//				led[j].num = LED_TBL_MAX-1;
//				led[j].show = ON;
//			}
//			if(uart232.rBuffer[i] != '\r' && uart232.rBuffer[i] !='\n')
//				j--;
//		}
//	}  
//}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 开机自检测程序   显示地址
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
//void digital_led_dispaddr()
//{
//    //加载地址
//    led[0].num = toSettedAddr[0];		       
//    led[1].num = toSettedAddr[1];
//    led[2].num = toSettedAddr[2];
//
//    led[0].show = ON;
//    led[1].show = ON;
//    led[2].show = ON;					
//}
//
