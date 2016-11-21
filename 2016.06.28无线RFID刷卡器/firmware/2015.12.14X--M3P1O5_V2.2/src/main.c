/*=========================================================
// �������˵��
//  2014��8��6�ո���  �汾v2.0
//	 1.��ʾ�����汾����ʾLED���η�����LED�û���
//	   �ֿⱣ��ԭ���Ļ����ϰѿ�����128���ֿ⣬�����Ժ���ġ�
//   2.����X_M3��F1��F4����ʾ����
//   3.��ȡDispMod����ȷ

//  2014��9��22�ո���  �汾v2.1
//	 1.���ӵ�ģʽ0=��1=�죬2=�̣�3=����4=���̣�5=������6=������7=������
//     ��led_light_ctrl	��������Ӧ��������7��ģʽ
	 2015��12��14�ո���  �汾v2.2
//	 1.main.h�ļ���MAX_ADDR   99��Ϊ     256  20151214 
=========================================================*/


#include "main.h"
#include "uart.h"
#include "led.h"
#include "key.h"
#include "process.h"
#include "eeprom.h"

volatile OS_SYS sys;

///////////////////////////////ȫ�ֱ���/////////////////////////////////////
volatile UINT8   digital_led_loading = OFF;
volatile UINT8   digital_led_refresh = OFF;
volatile UINT16  digital_led_loading_static = 0;
volatile UINT8   led_static_warn = OFF;//������ǩ���°�ť�����Ϣ10���Ӻ�
                                                                //�����ݶ���û���ɼ���ȥ������ɫ��˸����
volatile UINT16   led_static_warn_cnt = 0;
volatile UINT8 led_flash_switch =OFF;

volatile UINT8 led_flash_NO =LED_RED;					//�������
volatile UINT16 led_flash_period=1000;					//��˸���ڿ���
volatile UINT8 led_flash_mode =LED_FLASH_MODE_1V1;		//����ʱ���������:0����
volatile UINT8 led_flash_status = 0;					//��˸���Ƶ�ʱ���Ƶ�ǰ��״̬,0������1������
volatile UINT8 led_flash_NO_BK =LED_RED;				//������Ʊ�������(����������ʹ��)

//δ��������
volatile UINT8 beSettingAddr = 0;						//0:������վַ		1:����վַ		2��վַ�趨�Ѿ�����OK��
volatile UINT8 toSettedAddr[3];							//���趨��ֵַ
volatile UINT8 task_unupdate_num = 0;		        	//����δ�´���ҵ��������
volatile UINT8 task_unfinish_num =0;					//δ�����������= 0,1...MAX_TASK_NUM
volatile UINT8 task_unfinish_NO[MAX_TASK_NUM] ={0};		//��λ��
volatile UINT8 task_un_goods_numH[MAX_TASK_NUM] ={0};	//��Ʒ���ݸ�λ
volatile UINT8 task_un_goods_numL[MAX_TASK_NUM] ={0};	//��Ʒ���ݵ�λ

///����������
volatile UINT8 task_finished_num =0;		  			//�������������= 0,1...MAX_TASK_NUM
volatile UINT8 task_finished_NO[MAX_TASK_NUM] ={0};	  	//��λ��
volatile UINT8 task_ed_goods_numH[MAX_TASK_NUM] ={0};	//��Ʒ���ݸ�λ
volatile UINT8 task_ed_goods_numL[MAX_TASK_NUM] ={0};	//��Ʒ���ݵ�λ

volatile UINT8 SystemLocked = OFF;						//����ϵͳ������ϵͳ

volatile DisplayInfo DispInfoCur;						//��ǰ��ʾ��Ϣ		 
///////////////////////////////ȫ�ֱ���-END-////////////////////////////////
// ������ʾ�汾��
UINT8	const  code  Version[LED_NUMBER]={'V',0x3C,'2'};	   //3A=0.��3B=1.��3C=2.��3D=3.��3E=4.��3F=5.��
/*******************************************************************************
	 					1�����ӳٺ���
*******************************************************************************/
void Delay1Ms() 
{
    UINT16 i = 852;
    while (i != 0) {i--;}
}

/*******************************************************************************
	 					�����ӳٺ���
    input   usDelay     �ӳ�ʱ��ֵ
	output  none
*******************************************************************************/

void DelayMs(UINT16 usDelay)
{
    for(; usDelay>0; usDelay--)
    {
        Delay1Ms();
    }   
}
/*******************************************************************************
	 					  �˿ڳ�ʼ��
*******************************************************************************/
void IO_Init(void)
{
//	P0M1 = 0x00;
//	P0M0 = 0x0f;
//
	P1M1 = 0x00;
	P1M0 = 0x80;

	P2M1 = 0x00;
	P2M0 = 0x0e;

	P3M1 = 0x00;
	P3M0 = 0xc0;

	P4SW = 0x70;

//	//��Ϩ��ָʾ��
	CNTRL1_OFF;			CNTRL2_OFF;			CNTRL3_OFF;
	CNTRL4_OFF;			CNTRL5_OFF;			CNTRL6_OFF;

	LED_CNTRL1_OFF;		LED_CNTRL2_OFF;		LED_CNTRL3_OFF;
	LED_CNTRL4_OFF;		LED_CNTRL5_OFF;		LED_CNTRL6_OFF;
	sys.led_scan_flag = OFF;
}




/***************************************************
		T0��ʱ����ʼ��
***************************************************/
#define TH0_INIT        0xf8
#define TL0_INIT        0xcc

void InitT0(void)
{
    //��ʱ1ms//Ĭ��Ϊ12Tģʽ=>T0_CLK=SYS_CLK/12=11 059 200 /12 =921600	    
    TMOD = (TMOD & 0xf0) | 0x01;    //mode 1: 16 bit counter
    TH0  = TH0_INIT;
    TL0  = TL0_INIT;  

    ET0  = 1;                       //allow interrupt
    TR0  = 1;                       //open timer0

    return;
}

/*******************************************************************************
	��ʱ��T0�жϷ������(ms�ж�)																	   
	Input :		�ж�����
	Output :	sys.flag ��Ϊ��Ч
*******************************************************************************/
void timer0_int (void) interrupt 1
{ 
	int index = 0;

    TH0  = TH0_INIT;
    TL0  = TL0_INIT;
	  
    sys.led_scan_flag  = ON;
    //����1-485������������
    if (uart.rIndex != 0)	 			
    {
        if (uart.rTimer-- == 0) 
        {
			uart.rFlag  = ON;
		}
    }

    //����2-232������������
    if (uart232.rIndex != 0)	 			
    {
		if (uart232.rTimer-- == 0) 
		{
			uart232.rFlag  = ON;
			digital_led_refresh = ON;
		}
    }    
    //�ݼ����������Լ�1
    if (sys.led_flash_count != 0)
        sys.led_flash_count--;
	for(index = 0; index < 6; index++)
	{
		if(LightTowerCtl[index].led_flash_count != 0)
			LightTowerCtl[index].led_flash_count--;
	}
    //����ɨ�������
    led_Scan();
}
/*******************************************************************************
					�������ʾ����
	input:   
	output:  none					
*******************************************************************************/
void digital_led_disp(void)
{
	SINT16 i=0,j=0;
	//������ʾ
	for(i = 0; i < LED_NUMBER; i++)
	{
		led[i].show = OFF;
	}
	//���232����ɨ������,����ʾɨ�����������
	if(uart232.rFlag == ON)
	{
		j = LED_NUMBER-1;
		for(i = uart232.rIndex-1; i>= 0 && j>=0; i--)
		{
			if(uart232.rBuffer[i] >= '0' && uart232.rBuffer[i] <= '9')
			{
				led[j].num = uart232.rBuffer[i]-'0';
				led[j].show = ON;
			}
			else if(uart232.rBuffer[i] >= 'A' && uart232.rBuffer[i] <= 'Z')
			{
				led[j].num = uart232.rBuffer[i]-'A'+10;
				led[j].show = ON;
			}
			else if(uart232.rBuffer[i] >= 'a' && uart232.rBuffer[i] <= 'z')
			{
				led[j].num = uart232.rBuffer[i]-'a'+10;
				led[j].show = ON;
			}
			else if(uart232.rBuffer[i] == '-')
			{
				led[j].num = LED_TBL_MAX-1;
				led[j].show = ON;
			}
			if(uart232.rBuffer[i] != '\r' && uart232.rBuffer[i] !='\n')
				j--;
		}
	}
	//����,�������ʾ��Ϣ,����ʾ��Ϣ
	else if(DispInfoCur.SRCodeLen > 0)
	{
		j = LED_NUMBER-1;
		for(i = DispInfoCur.SRCodeLen-1; i>=0 && j>=0; i--)
		{
			if(DispInfoCur.SRCode[i] >= '0' && DispInfoCur.SRCode[i] <= '9')
			{
				led[j].num = DispInfoCur.SRCode[i]-'0';
				led[j].show = ON;
			}
			else if(DispInfoCur.SRCode[i] >= 'A' && DispInfoCur.SRCode[i] <= 'Z')
			{
				led[j].num = DispInfoCur.SRCode[i]-'A'+10;
				led[j].show = ON;
			}
			else if(DispInfoCur.SRCode[i] >= 'a' && DispInfoCur.SRCode[i] <= 'z')
			{
				led[j].num = DispInfoCur.SRCode[i]-'a'+10;
				led[j].show = ON;
			}
			else if(DispInfoCur.SRCode[i] == '-')
			{
				led[j].num = LED_TBL_MAX-1;
				led[j].show = ON;
			}
			if(uart232.rBuffer[i] != '\r' && uart232.rBuffer[i] !='\n')
				j--;
		}
	}  
}
/*******************************************************************************
	 					�����Լ�����
    input:   none     
    output:  none
*******************************************************************************/ 
void digital_led_dispaddr()
{
    //���ص�ַ
    led[0].num = toSettedAddr[0];		       
    led[1].num = toSettedAddr[1];
    led[2].num = toSettedAddr[2];

    led[0].show = ON;
    led[1].show = ON;
    led[2].show = ON;					
}

/*******************************************************************************
	 					�����Լ����� ��ʾ�汾�ţ������룬
    input:   none     
    output:  none
*******************************************************************************/
INT8U  const  code   LEDSecBit[7] = {37,38,39,40,41,42,36};		//�����Ӧ��LED���
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
/*******************************************************************************
	 						main program				
*******************************************************************************/   

int main(void) 
{   
	DelayMs(50);    //�ȴ���Դ�ȶ�        

	EA = 0;
	digital_led_cntrl_Init();
	IO_Init();
 	InitT0();

	LoadParaInEeprom();
	baudrate   = BD_9600;
	UART232_Init();
	UART485_Init();	//�̶�ʹ��38400������


	EA = 1;//ʹ���ж�             
	Start_Init();
	M485_CTRL_OFF;
	   					
	while(1)
	{	
		P0M1 = 0x00;	  //����ǿ�������
		P0M0 = 0x0f;

		P1M1 = 0x00;
		P1M0 = 0x83;
		
		//��ַ�趨ʱ��ʾ��ַ��������
		if(beSettingAddr == 1 ||  beSettingAddr == 2)
		{
			if(digital_led_refresh == ON)
			{
				digital_led_refresh = OFF;
				digital_led_dispaddr();
			}
		}
		//1���źŵ���3��ledָʾ����˸����
		CMD_led_flash();
		//������Դ����
		CMD_LigheTower_flash();
		//ˢ��һ���������ֵ��������ʾ��ǰ��ҵ���Լ����ݻ��ԣ�
		if( digital_led_refresh == ON) 
		{
			digital_led_refresh = OFF;
			digital_led_disp();
		}
		//��������(�а���ʱ)
		Scan_Keyboard();
		if (keyStatus != CMD_KEY_NULL) 
		{
			Process_Keyboard();
		}
		//����485ͨѶ
		if (uart.rFlag == ON)
		{
//			Send485SerialByte((UINT8 *)uart.rBuffer, uart.rIndex); 
			Cmd_Process();
			uart.rFlag   = OFF;
			uart.rIndex  = 0;
		}
	}                
}
/*******************************************************************************
								END					
*******************************************************************************/

