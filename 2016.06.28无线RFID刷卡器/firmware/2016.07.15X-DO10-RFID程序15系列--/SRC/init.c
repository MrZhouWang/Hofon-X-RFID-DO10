#include "include.h"

INT8U  const  code   LEDSecBit[7] = {37,38,39,40,41,42,36};		//�����Ӧ��LED���
UINT8	const  code  Version[LED_NUMBER]={'V',0x3C,'2'};	   //������ʾ�汾��
////////////////////////////////////////////////////////////////////////////////
// �������� : �˿ڳ�ʼ��
// ������� : 
// ���ز��� : 
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
void IO_Init(void)
{
//	P0M1 = 0x00;
//	P0M0 = 0x0f;
//
	P1M1 = 0x00;
	P1M0 = 0x80;	 //p1.7�������

	P2M1 = 0x00;
	P2M0 = 0x0e;	//P2.3 P2.2 P2.1�������

	P3M1 = 0x00;
	P3M0 = 0xc0;  //p3.7 p3.6�������

	P4SW = 0x70;	 //	   P4.5 P4.6, P4.4 ����Ϊ��ͨIO��

//	//��Ϩ��ָʾ��
	CNTRL1_OFF;			CNTRL2_OFF;			CNTRL3_OFF;
	CNTRL4_OFF;			CNTRL5_OFF;			CNTRL6_OFF;

	LED_CNTRL1_OFF;		LED_CNTRL2_OFF;		LED_CNTRL3_OFF;
	LED_CNTRL4_OFF;		LED_CNTRL5_OFF;		LED_CNTRL6_OFF;
	sys.led_scan_flag = OFF;
	AUXR &= ~0X20;//0������ʹ���ڲ���չ��1024�ֽ���չRAM��
	CLK_DIV=0;//�ⲿ����ʱ�ӻ��ڲ�R/C��ʱ��
}
////////////////////////////////////////////////////////////////////////////////
// �������� : �����Լ����� ��ʾ�汾�ţ�������
// ������� : 
// ���ز��� : 
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
void Start_Init(void)
{
//	UINT8 i = 0;
	UINT8 i,j;
	//��ʾ�汾
 	for (i=0; i<LED_NUMBER; i++)
	{
		led[i].num  = Version[i];
//		led[i].delay = 0;
		led[i].show  = ON;
	}
	DelayMs(800);
    //��ʾ������
	for(i = 0; i < 7; i++)
	{
		for(j = 0; j < LED_NUMBER; j++)
		{  led[j].num = LEDSecBit[i];  } 		    //������

		DelayMs(300);    //��ʱ��ʾ
	}
	//��ʾȫ8
	for (i=0; i<LED_NUMBER; i++)
	{
		led[i].num  = 0X08;
//		led[i].delay = 0;
		led[i].show  = ON;
	}
//	//ϵͳ���ݸ�λ
	DispInfoCur.SRCodeLen = 0;
	uart232.rFlag   = OFF;
	uart232.rIndex  = 0;

	LED_RXD485_ON;		LED_TXD485_ON;

	//����LED
//	for (i=0; i<LED_NUMBER; i++)
//	{
//		led[i].num  = 0X08;
//		led[i].show  = ON;
//	}
//	sys.led_scan_flag = ON;
// 	DelayMs(1000);
	//��ʾ��ַ
	led[0].num = address[0];		       
    led[1].num = address[1];
    led[2].num = address[2];
	//����ָʾ��
//	led_light_ctrl(LED_RED, ON);
	led_light_ctrl(LED_RED);			// ���������ʾ
 	DelayMs(500);
//	led_light_ctrl(LED_RED, OFF);
//	led_light_ctrl(LED_GREEN, ON);
	led_light_ctrl(LED_GREEN);			// �����̵���ʾ
 	DelayMs(500);
//	led_light_ctrl(LED_GREEN, OFF);
//	led_light_ctrl(LED_BLUE, ON);
	led_light_ctrl(LED_BLUE);			// ����������ʾ
 	DelayMs(500);
//	led_light_ctrl(LED_BLUE, OFF);

	for (i=0; i<LED_NUMBER; i++)
	{ led[i].num  = ' '; }

	led_light_ctrl(LED_RG);			// �������̵���ʾ
 	DelayMs(500);
	led_light_ctrl(LED_RB);			// ������������ʾ
 	DelayMs(500);
	led_light_ctrl(LED_GB);			// ������������ʾ
 	DelayMs(500);
	led_light_ctrl(LED_NONE);		// ��������ʾ
	//�����źŵ�
	CNTRL1_ON;		LED_CNTRL1_ON;		CNTRL4_ON;		LED_CNTRL4_ON;		
	led[0].num = 'F'-'A'+10;			led[1].num = 0X01;		led[2].num = FinishedLED;		       
	DelayMs(1000);		
	CNTRL1_OFF;		LED_CNTRL1_OFF;		CNTRL4_OFF;		LED_CNTRL4_OFF;
	CNTRL2_ON;		LED_CNTRL2_ON;		CNTRL5_ON;		LED_CNTRL5_ON;		
	led[0].num = 'F'-'A'+10;			led[1].num = 0X02;		led[2].num = WtTime;		       
	DelayMs(1000);
	CNTRL2_OFF;		LED_CNTRL2_OFF;		CNTRL5_OFF;		LED_CNTRL5_OFF;
	CNTRL3_ON;		LED_CNTRL3_ON;		CNTRL6_ON;		LED_CNTRL6_ON;
	led[0].num = 'F'-'A'+10;			led[1].num = 0X03;		led[2].num = FnDefine;		       
	DelayMs(1000);
	CNTRL3_OFF;		LED_CNTRL3_OFF;		CNTRL6_OFF;		LED_CNTRL6_OFF;
	led[0].num = 'F'-'A'+10;			led[1].num = 0X04;		led[2].num = DispMode;		       
	DelayMs(1000);
	LED_RXD485_OFF;		LED_TXD485_OFF;
	//�����ʾ
	ClearAll();
}