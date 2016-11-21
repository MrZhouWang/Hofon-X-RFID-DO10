/**********************************************
*  CPU:STC12W1K16S
*  ����:11.0592MHZ
------------------------------------------------*/

#define  MAIN_GLOBAL
#include "include.h"
volatile unsigned short timerc;
volatile OS_SYS sys;
///////////////////////////////ȫ�ֱ���/////////////////////////////////////

unsigned char code rc_data[16] ={0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};//M1����ĳһ��дΪ���¸�ʽ����ÿ�ΪǮ�����ɽ��տۿ�ͳ�ֵ����
                                                                                                                  //4�ֽڽ����ֽ���ǰ����4�ֽڽ��ȡ����4�ֽڽ�1�ֽڿ��ַ��1�ֽڿ��ַȡ����1�ֽڿ��ַ��1�ֽڿ��ַȡ�� 

unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  //ԭʼ����
unsigned char code DefaultKeyA[6] = {7,7,7,7,7,7};    //����A
unsigned char code DefaultKeyB[6] = {8,8,8,8,8,8};    //����B

                                                      //�����ݶ���û���ɼ���ȥ������ɫ��˸����
volatile UINT8 led_flash_NO =LED_RED;					        //�������
volatile UINT16 led_flash_period=1000;					      //��˸���ڿ���
volatile UINT8 led_flash_mode =LED_FLASH_MODE_1V1;		//����ʱ���������:0����
volatile UINT8 led_flash_status = 0;					        //��˸���Ƶ�ʱ���Ƶ�ǰ��״̬,0������1������
volatile UINT8 led_flash_NO_BK =LED_RED;				      //������Ʊ�������(����������ʹ��)


volatile UINT8 beSettingAddr = 0;						          //0:������վַ		1:����վַ		2��վַ�趨�Ѿ�����OK��
volatile UINT8 toSettedAddr[3];							          //���趨��ֵַ

volatile UINT8 SystemLocked = OFF;					          //����ϵͳ������ϵͳ
	 
///////////////////////////////ȫ�ֱ���-END-////////////////////////////////







////////////////////////////////////////////////////////////////////////////////
// �������� : main program
// ������� : 
// ���ز��� : 
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
int main(void) 
{   
	DelayMs(50);                //�ȴ���Դ�ȶ�        
  
	EA = 0;
	IO_Init();
 	InitT0();

	LoadParaInEeprom();

	UART485_Init();	            //�̶�ʹ��38400������
	
	PcdReset();
  PcdAntennaOff(); 
  PcdAntennaOn(); 
	
	Start_Init();
	
	RC_WorkeMode=RC_ReadMode; 	//����  ����/д��/������

	

	BEEP_OFF;
	WDT_Start();                //�������Ź�	
	EA = 1;                     //ʹ���ж� 	
	while(1)
	{	
    WDT_Reset();              //��λ���Ź�
	  if(RC_WorkeMode==RC_WriteMode)                          //дģʽ	/**************RFID����д����***********************/
		   M1_WriteDate(PICC_AUTHENT1A,8,rc_data,DefaultKey);   //дIC��
		else if(RC_WorkeMode==RC_ReadMode)                      //��ģʽ
		   M1_Read(PICC_AUTHENT1A ,0,16,DefaultKey);            //ɨ���IC��
		else if(RC_WorkeMode==RC_ChangeMode)                    //������
		   M1_ChangeKey(7);

		CMD_led_flash();	                 //RGB����

		CMD_LigheTower_flash();	           //��������

		Scan_Keyboard();	                 //��������(�а���ʱ)
		if (keyStatus != CMD_KEY_NULL) 
		{
			Process_Keyboard();
		}

		if (uart.rFlag == ON)              //����485ͨѶ
		{
			//Send485SeriaDate((UINT8 *)uart.rBuffer, uart.rIndex); 	//���ڴ�ӡ���յ�������
			Cmd_Process();
			uart.rFlag   = OFF;
			uart.rIndex  = 0;
		}
		WDT_Reset();                      //��λ���Ź�
	}                 
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��ʱ���ж����
//// ������� : 
//// ���ز��� : 
//// ˵��     :  2ms
//////////////////////////////////////////////////////////////////////////////////
void timer0_int (void) interrupt 1
{ 
	  char index = 0;
	  TH0  = TH0_INIT;
    TL0  = TL0_INIT;

		if(rc_blink_read>1)	           //��������д���ɹ���ָʾ����
		{
			rc_blink_read--;
			led_on();
			BEEP_ON;
		}
		else if(rc_blink_read==1)
		{
			rc_blink_read=0;
			led_off();
			BEEP_OFF;
		}

    
    if (uart.rIndex != 0)	 		     //����1-485������������	
    {
        if (uart.rTimer-- == 0) 
        {
			      uart.rFlag  = ON;
	     	}
    }
		
		
		if(SetAddrFlag>1)              //���õ�ַʱָʾ
		{
			 SetAddrFlag--;  
		}
		if(SetAddrFlag>0)
			 LED_G_ON;
		
		if(SetAddrFlag>2)
			 BEEP_ON;
		if(SetAddrFlag==2)
			 BEEP_OFF;
    if(SetAddrFlag>150)     
			 led_on();
		else if((SetAddrFlag<=150)&&(SetAddrFlag>115))
		   led_off();
		else if((SetAddrFlag<=115)&&(SetAddrFlag>85))
		   led_on();
		else if((SetAddrFlag<=85)&&(SetAddrFlag>50))
		   led_off();
		else if((SetAddrFlag<=50)&&(SetAddrFlag>20))
		   led_on();
		else if((SetAddrFlag<=20)&&(SetAddrFlag>3))
		   led_off();
		
		
		
    
    if (sys.led_flash_count != 0)     //�ݼ����������Լ�1 ����RGB��˸����
        sys.led_flash_count--;
		
		for(index = 0; index < 10; index++)
	  {
		   if(LightTowerCtl[index].led_flash_count > 0)
			    LightTowerCtl[index].led_flash_count--;
	  }

}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ���ÿ��Ź�
//// ������� : 
//// ���ز��� : 
//// ˵��     : ������ֵ 
//////////////////////////////////////////////////////////////////////////////////
void WDT_Start()
{
	  WDT_CONTR = 0x07;       //���ÿ��Ź�Ϊ������ 11.0952Mhzʱ Լ9��
    WDT_CONTR |= 0x20;      //�������Ź�
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��λ���Ź�
//// ������� : 
//// ���ز��� : 
//// ˵��     : ����ֵ����
//////////////////////////////////////////////////////////////////////////////////
void WDT_Reset()
{
    WDT_CONTR |= 0x10;      //��λ���Ź�
}
/*******************************************************************************
								END					
*******************************************************************************/

