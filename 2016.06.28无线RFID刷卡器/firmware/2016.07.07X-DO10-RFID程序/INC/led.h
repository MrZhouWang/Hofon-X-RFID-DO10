#ifndef _LED_H_
#define _LED_H_

///////////////////////////////---���IO����START---/////////////////////////////////////
//595��������
sbit p_oe       = P2^7;
sbit p_sh_clk   = P2^5;
sbit p_st_clk   = P2^6;
sbit p_sdata    = P4^4;

#define OE_SET	(p_oe = 1)
#define OE_CLR	(p_oe = 0)

#define SH_CLK_SET	(p_sh_clk = 1)
#define SH_CLK_CLR	(p_sh_clk = 0)

#define ST_CLK_SET	(p_st_clk = 1)
#define ST_CLK_CLR	(p_st_clk = 0)

#define SDATA_SET	(p_sdata = 1)
#define SDATA_CLR	(p_sdata = 0)

//�������ʾ�����ҷֱ�Ϊ��1λ-->��6λ
#define DIGITAL_LED1	0 //��1λ�����
#define DIGITAL_LED2	1 //��2λ�����
#define DIGITAL_LED3	2 //��3λ�����

#define DIGITAL_LED_PWR_ON	0 //����ܵ�Դ����״̬
#define DIGITAL_LED_PWR_OFF	1 //����ܵ�Դ����״̬

sbit digital_led1_port	=P1^6 ;//��1λ����ܵĵ�ԴIO
sbit digital_led2_port	=P3^4 ;//��2λ����ܵĵ�ԴIO
sbit digital_led3_port	=P1^4 ;//��3λ����ܵĵ�ԴIO

///////////////////////////////----���IO����END----/////////////////////////////////////
//�����������6·
sbit CNTRL1       = P0^2;		//��һ·���		 red led
sbit CNTRL2       = P0^0;		//�ڶ�·���		  yellow led
sbit CNTRL3       = P0^1;		//����·���		  green led
sbit CNTRL4       = P0^3;		//����·���		  blue led
sbit CNTRL5       = P1^0;		//����·���		  whilte led
sbit CNTRL6       = P1^1;		//����·���		  δ����
//����ָʾ�źŵ�(3mmLED)
sbit LED_CNTRL1 =  P2^2;
sbit LED_CNTRL2 =  P3^6;
sbit LED_CNTRL3 =  P2^3;
sbit LED_CNTRL4 =  P3^7;
sbit LED_CNTRL5 =  P2^1;
sbit LED_CNTRL6 =  P1^7;
//��ɫ����ָʾ��
sbit p_led_red	=  P0^4;
sbit p_led_green = P0^5;
sbit p_led_blue	=  P0^6;
//485�շ��źŵ�(3mm)
sbit LED_RXD485    = P4^1; // 485
sbit LED_TXD485    = P4^6; // 485
//485����
sbit M485_CTRL  = P2^4; // 485

#define CNTRL1_ON	(CNTRL1 = 1)
#define CNTRL1_OFF	(CNTRL1 = 0)
#define CNTRL2_ON	(CNTRL2 = 1)
#define CNTRL2_OFF	(CNTRL2 = 0)
#define CNTRL3_ON	(CNTRL3 = 1)
#define CNTRL3_OFF	(CNTRL3 = 0)
#define CNTRL4_ON	(CNTRL4 = 1)
#define CNTRL4_OFF	(CNTRL4 = 0)
#define CNTRL5_ON	(CNTRL5 = 1)
#define CNTRL5_OFF	(CNTRL5 = 0)
#define CNTRL6_ON	(CNTRL6 = 1)
#define CNTRL6_OFF	(CNTRL6 = 0)

#define LED_CNTRL1_ON	(LED_CNTRL1 = 1)
#define LED_CNTRL1_OFF	(LED_CNTRL1 = 0)
#define LED_CNTRL2_ON	(LED_CNTRL2 = 1)
#define LED_CNTRL2_OFF	(LED_CNTRL2 = 0)
#define LED_CNTRL3_ON	(LED_CNTRL3 = 1)
#define LED_CNTRL3_OFF	(LED_CNTRL3 = 0)
#define LED_CNTRL4_ON	(LED_CNTRL4 = 1)
#define LED_CNTRL4_OFF	(LED_CNTRL4 = 0)
#define LED_CNTRL5_ON	(LED_CNTRL5 = 1)
#define LED_CNTRL5_OFF	(LED_CNTRL5 = 0)
#define LED_CNTRL6_ON	(LED_CNTRL6 = 1)
#define LED_CNTRL6_OFF	(LED_CNTRL6 = 0)

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

#define DIGITAL_LED_LOAD_DELAY	3000

#define COUNT_FLICKER_ON	        500
#define COUNT_FLICKER_OFF	200

#define LED_LOAD_CNT_PER		2000

#define LED_NUMBER		3
//#define LED_TBL_MAX     37
#define LED_TBL_MAX     128

typedef struct 
{
	UINT8 	  num;
	UINT8     show;   //������Ƿ���ʾ��"ON"==>��ʾ,"OFF"�ر�
}OS_NUM;
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
//led ����˸����ģʽ
#define LED_FLASH_MODE_ALWAYS		0X00
#define LED_FLASH_MODE_1V1			0X01
#define LED_FLASH_MODE_1V2			0X02
#define LED_FLASH_MODE_1V5			0X03
#define LED_FLASH_MODE_1V10			0X04
#define LED_FLASH_MODE_2V1			0X05
#define LED_FLASH_MODE_5V1			0X06
#define LED_FLASH_MODE_10V1			0X07

typedef struct 
{
	UINT8	led_status;				//��	����	��
	UINT16	led_flash_period; 		//���ڳ���
	UINT8	led_switch_ratio;		//���������0����
	UINT8	led_status_cur;			//��ǰ������״̬�����Կ�����˸
	UINT16 	led_flash_count;		//��˸����ʱ��ͳ��
}LIGHTTOWER_CTL;
extern LIGHTTOWER_CTL LightTowerCtl[6];

extern UINT8 address[3];
extern UINT8 baudrate;
extern UINT8 ucBaud;
extern UINT8 FinishedLED;					   //��ɺ��LED�������(0,1,2,3-->N,R,G,B)
extern UINT8 WtTime;						   //�������ȴ�ʱ��(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
extern UINT8 FnDefine;						   //���ܼ�����(0,1->���,�������)
extern UINT8 DispMode;						   //��ʾģʽ����(0,1->����ģʽ,ָʾģʽ)����ͳһ600U,900U����ͳһͨ�����κ���ʾ����ķ�ʽ
                 
extern UINT8 ucShift;
extern volatile OS_NUM idata  led[LED_NUMBER];
extern volatile OS_NUM idata bled[LED_NUMBER];

extern UINT8  xdata  LED_TBL[LED_TBL_MAX] ; 	        //�������ʾ��Ӧ��

void digital_led_cntrl_Init(void);
void led_SupplyPower(void);
void led_Show(UINT8 ucValue);
void led_Scan(void);

//void led_light_ctrl(UINT8 ucValue, UINT8 ucStatus);
void led_light_ctrl(UINT8 ucValue);
void CMD_led_flash(void);
void CMD_LigheTower_flash(void);
void Display_Clear(void);

void digital_led_disp(void);
void digital_led_dispaddr();

#endif

