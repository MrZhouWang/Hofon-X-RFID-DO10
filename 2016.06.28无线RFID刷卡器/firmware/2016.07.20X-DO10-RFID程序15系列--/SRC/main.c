/**********************************************
*  CPU:STC12W1K16S
*  ����:11.0592MHZ
------------------------------------------------*/

#define  MAIN_GLOBAL
#include "include.h"
volatile unsigned short timerc;
volatile OS_SYS sys;

///////////////////////////////ȫ�ֱ���/////////////////////////////////////

unsigned char code rc_data[16] ={0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};//
//M1����ĳһ��дΪ���¸�ʽ����ÿ�ΪǮ�����ɽ��տۿ�ͳ�ֵ����
//4�ֽڽ����ֽ���ǰ����4�ֽڽ��ȡ����4�ֽڽ�1�ֽڿ��ַ��1�ֽڿ��ַȡ����1�ֽڿ��ַ��1�ֽڿ��ַȡ�� 
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //ԭʼ����
unsigned char code DefaultKeyA[6] = {7,7,7,7,7,7}; //����A
unsigned char code DefaultKeyB[6] = {8,8,8,8,8,8}; //����B
//volatile UINT8   digital_led_refresh = OFF;
                                                                //�����ݶ���û���ɼ���ȥ������ɫ��˸����
volatile UINT8 led_flash_NO =LED_RED;					//�������
volatile UINT16 led_flash_period=1000;					//��˸���ڿ���
volatile UINT8 led_flash_mode =LED_FLASH_MODE_1V1;		//����ʱ���������:0����
volatile UINT8 led_flash_status = 0;					//��˸���Ƶ�ʱ���Ƶ�ǰ��״̬,0������1������
volatile UINT8 led_flash_NO_BK =LED_RED;				//������Ʊ�������(����������ʹ��)

//δ��������
volatile UINT8 beSettingAddr = 0;						//0:������վַ		1:����վַ		2��վַ�趨�Ѿ�����OK��
volatile UINT8 toSettedAddr[3];							//���趨��ֵַ

volatile UINT8 SystemLocked = OFF;						//����ϵͳ������ϵͳ
	 
///////////////////////////////ȫ�ֱ���-END-////////////////////////////////
 
////////////////////////////////////////////////////////////////////////////////
// �������� : main program
// ������� : 
// ���ز��� : 
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
int main(void) 
{   
	DelayMs(20);    //�ȴ���Դ�ȶ�        

	EA = 0;
	IO_Init();
 	InitT0();

	LoadParaInEeprom();
	baudrate   = BD_9600;

	UART485_Init();	//�̶�ʹ��38400������    
	PcdReset();
  PcdAntennaOff(); 
  PcdAntennaOn(); 
	Start_Init();
	EA = 1;//ʹ���ж� 	
	RC_WorkeMode=RC_ReadMode;				
	while(1)
	{	

	  if(RC_WorkeMode==RC_WriteMode)//дģʽ	/**************RFID����д����***********************/
		   M1_WriteDate(PICC_AUTHENT1A,8,rc_data,DefaultKey); //дIC��
		else if(RC_WorkeMode==RC_ReadMode)//��ģʽ
		   M1_Read(PICC_AUTHENT1A ,7,16,DefaultKey);//ɨ���IC��
		else if(RC_WorkeMode==RC_ChangeMode)//��ģʽ//������
		   M1_ChangeKey(7);

		CMD_led_flash();	    /**************RGB����***********************/

		CMD_LigheTower_flash();	/********************��������****************/

		Scan_Keyboard();	    /*************��������(�а���ʱ)*************/
		if (keyStatus != CMD_KEY_NULL) 
		{
			Process_Keyboard();
		}

		if (uart.rFlag == ON)   /*************����485ͨѶ********************/
		{
			Send485SeriaDate((UINT8 *)uart.rBuffer, uart.rIndex); 	//���ڴ�ӡ���յ�������
			Cmd_Process();
			uart.rFlag   = OFF;
			uart.rIndex  = 0;
		}
	}                 
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��ʱ���ж����
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
//////////////////////////////////////////////////////////////////////////////////
void timer0_int (void) interrupt 1
{ 
	char index = 0;
	TH0  = TH0_INIT;
  TL0  = TL0_INIT;
	if(con_flag==ON)
	{
		if(rc_blink_read>0)	  //��������д���ɹ���ָʾ����
		{
			rc_blink_read--;
			led_on();
			BEEP_ON;
		}
		else
		{
			led_off();
			BEEP_OFF;
		}
	}

    //����1-485������������
    if (uart.rIndex != 0)	 			
    {
        if (uart.rTimer-- == 0) 
        {
			      uart.rFlag  = ON;
		    }
    }
   
    //�ݼ����������Լ�1 ����RGB��˸����
    if (sys.led_flash_count != 0)
        sys.led_flash_count--;
		
		for(index = 0; index < 10; index++)
	  {
		   if(LightTowerCtl[index].led_flash_count > 0)
			    LightTowerCtl[index].led_flash_count--;
	  }

}
/*******************************************************************************
								END					
*******************************************************************************/

