#ifndef _LED_H_
#define _LED_H_

#ifdef LED_GLOBAL
#define EXT_LED
#else
#define EXT_LED extern
#endif


///////////////////////////////---���IO����START---/////////////////////////////////////
//��ɫ����ָʾ��
sbit p_led_red	=  P3^3;
sbit p_led_green = P5^4;
sbit p_led_blue	=  P5^5;
//������
sbit beeper = P3^4;
//485�շ��źŵ�(3mm)
sbit LED_RXD485    = P2^0; // 485
sbit LED_TXD485    = P2^0; // 485
//485����
sbit M485_CTRL  = P0^1; // 485




#define BEEP_ON (beeper=0)
#define BEEP_OFF (beeper=1)

#define LED_R_ON	(p_led_red = 0)
#define LED_R_OFF	(p_led_red = 1)
#define LED_G_ON	(p_led_green = 0)
#define LED_G_OFF	(p_led_green = 1)
#define LED_B_ON	(p_led_blue = 0)
#define LED_B_OFF	(p_led_blue = 1)


#define LED_RXD485_OFF	(LED_RXD485 = 1)// 485
#define LED_RXD485_ON	(LED_RXD485 = 0)// 485
#define LED_TXD485_OFF	(LED_TXD485 = 1)// 485
#define LED_TXD485_ON	(LED_TXD485 = 0)// 485

#define M485_CTRL_ON	  (M485_CTRL = 1)
#define M485_CTRL_OFF (M485_CTRL = 0)




/////////////////////////////////////////////////////////////////////////////////////////////////
//led light ��������
#define LED_NONE	0
#define LED_RED		1
#define LED_GREEN	2
#define LED_BLUE	3

#define LED_RG		4
#define LED_RB		5
#define LED_GB		6
#define LED_RGB		7
//RGB ����˸����ģʽ
#define LED_FLASH_MODE_ALWAYS		0X00
#define LED_FLASH_MODE_1V1			0X01
#define LED_FLASH_MODE_1V2			0X02
#define LED_FLASH_MODE_1V5			0X03
#define LED_FLASH_MODE_1V10			0X04
#define LED_FLASH_MODE_2V1			0X05
#define LED_FLASH_MODE_5V1			0X06
#define LED_FLASH_MODE_10V1			0X07






EXT_LED UINT8 address[3];
EXT_LED UINT8 baudrate;
EXT_LED UINT8 ucBaud;
EXT_LED UINT8 FinishedLED;					   //��ɺ��LED�������(0,1,2,3-->N,R,G,B)
EXT_LED UINT8 WtTime;						       //�������ȴ�ʱ��(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
EXT_LED UINT8 FnDefine;						     //���ܼ�����(0,1->���,�������)
EXT_LED UINT8 DispMode;						     //��ʾģʽ����(0,1->����ģʽ,ָʾģʽ)����ͳһ600U,900U����ͳһͨ�����κ���ʾ����ķ�ʽ
                 
EXT_LED UINT8 ucShift;
EXT_LED UINT16 RFID_FlagON;            //��ˢ����־λ

void led_light_ctrl(UINT8 ucValue);
void CMD_led_flash(void);
void Display_Clear(void);

void led_off();
void led_on(); 

#endif

