#include "include.h"

////////////////////////////////////////////////////////////////////////////////
// �������� : �˿ڳ�ʼ��
// ������� : 
// ���ز��� : 
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
void IO_Init(void)
{
  P0M1 |= 0x0;
	P0M0 |= 0x0E;	 //P0.1 p0.2 P0.3�������
  
  P1M1 &= 0x00;
	P1M0 |= 0xff;	 //p1�� all�������	
	/***485����**************/
	M485_CTRL_OFF;//����״̬
	/***Ϩ�����**************/
	CNTRL1_OFF;			CNTRL2_OFF;			CNTRL3_OFF;
	CNTRL4_OFF;			CNTRL5_OFF;			CNTRL6_OFF;
	CNTRL7_OFF;			CNTRL8_OFF;			CNTRL9_OFF;
	CNTRL10_OFF;

	led_off();
	
	BEEP_OFF;
	RC522_rFlag  = OFF;   //���RFID��Ϣ	
	sys.led_scan_flag = OFF;
	AUXR &= ~0X20;        //0������ʹ���ڲ���չ��1024�ֽ���չRAM��
	CLK_DIV=0;            //�ⲿ����ʱ�ӻ��ڲ�R/C��ʱ��
}
////////////////////////////////////////////////////////////////////////////////
// �������� : �����Լ����� ��ʾ�汾�ţ�������
// ������� : 
// ���ز��� : 
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
void Start_Init(void)
{

	//ϵͳ���ݸ�λ
	RC522_rFlag   = OFF;

	LED_RXD485_ON;		LED_TXD485_ON;

	//����ָʾ��
	led_light_ctrl(LED_BLUE);		// ���������ʾ
 	DelayMs(500);

	led_light_ctrl(LED_GREEN);	// �����̵���ʾ
 	DelayMs(500);
	led_light_ctrl(LED_RED);		// ����������ʾ
 	DelayMs(500);

	led_light_ctrl(LED_RG);			// �������̵���ʾ
 	DelayMs(500);
	led_light_ctrl(LED_RB);			// ������������ʾ
 	DelayMs(500);
	led_light_ctrl(LED_GB);			// ������������ʾ
 	DelayMs(500);
	led_light_ctrl(LED_NONE);		// ��������ʾ
	LED_RXD485_OFF;		LED_TXD485_OFF; BEEP_OFF;
	M485_CTRL_OFF;              //����ģʽ 	 
	ClearAll();
}