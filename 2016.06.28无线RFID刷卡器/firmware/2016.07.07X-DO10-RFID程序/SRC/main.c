/*=========================================================
// �������˵��

=========================================================*/

#define  MAIN_GLOBAL
#include "include.h"
unsigned short timerc;
volatile OS_SYS sys;

///////////////////////////////ȫ�ֱ���/////////////////////////////////////
volatile UINT8   digital_led_loading = OFF;
volatile UINT8   digital_led_refresh = OFF;
volatile UINT16  digital_led_loading_static = 0;
volatile UINT8   led_static_warn = OFF;//������ǩ���°�ť�����Ϣ10���Ӻ�
                                                                //�����ݶ���û���ɼ���ȥ������ɫ��˸����
//volatile UINT16   led_static_warn_cnt = 0;
//volatile UINT8 led_flash_switch =OFF;

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
 
////////////////////////////////////////////////////////////////////////////////
// �������� : main program
// ������� : 
// ���ز��� : 
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
int main(void) 
{   
	DelayMs(200);    //�ȴ���Դ�ȶ�        

	EA = 0;
	digital_led_cntrl_Init();
	IO_Init();
 	InitT0();

	LoadParaInEeprom();
	baudrate   = BD_9600;
	UART232_Init();
	UART485_Init();	//�̶�ʹ��38400������


	           
	Start_Init();
	M485_CTRL_OFF;
	EA = 1;//ʹ���ж�     					
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
			Send485SerialByte((UINT8 *)uart.rBuffer, uart.rIndex); 	//���ڴ�ӡ���յ�������
			Cmd_Process();
			uart.rFlag   = OFF;
			uart.rIndex  = 0;
		}
	}                
}
////////////////////////////////////////////////////////////////////////////////
// �������� : ��ʱ���ж����
// ������� : 
// ���ز��� : 
// ˵��     : 1ms�ж�
////////////////////////////////////////////////////////////////////////////////
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
		if(LightTowerCtl[index].led_flash_count > 0)
			LightTowerCtl[index].led_flash_count--;
	}
    //����ɨ�������
    led_Scan();
	timerc++;
	if(timerc>1000)
	{
	   p_led_red =1;
	  
	}
	else
	   p_led_red =0;
	if(timerc>2000)
	   timerc=0;
}
/*******************************************************************************
								END					
*******************************************************************************/

