#define LED_GLOBAL
#include "include.h"
UINT8 address[3];
UINT8 baudrate;
UINT8 ucAddr[3];                        //���ݵ�ַ
UINT8 ucBaud;                           //���ݲ�����

UINT8 FinishedLED = LED_NONE;			      //�����ɺ�����ǰ��ָʾ��������ɫ
UINT8 WtTime = 5;						            //һ�Զ�Ӧ��ʱ���������ȴ�ʱ��
UINT8 FnDefine = 0;						          //Fn���ܼ�����	0-����	1-����
UINT8 DispMode = 0;						          //��ʾģʽ	0-����ģʽ	1-������Ϣģʽ


/////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// �������� : ��ȡ����ʱ�䣬��λms
// ������� : ��
// ���ز��� : ��
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
UINT16 GetLedOnTime()
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case LED_FLASH_MODE_ALWAYS	:						                   //����ģʽ��������ô˺���
		break;			
	case LED_FLASH_MODE_1V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V2:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V5:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V10:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_2V1:
		unRet = led_flash_period*2;
		break;
	case LED_FLASH_MODE_5V1:
		unRet = led_flash_period*5;
		break;
	case LED_FLASH_MODE_10V1:
		unRet = led_flash_period*10;
		break;
	default:
		break;
	}
	return unRet;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : ��ȡ����ʱ�䣬��λms
// ������� : ��
// ���ز��� : ��
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
UINT16 GetLedOffTime()                        
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case LED_FLASH_MODE_ALWAYS:					                            //����ģʽ��������ô˺���
		break;				
	case LED_FLASH_MODE_1V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_2V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_5V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_10V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V2:
		unRet = led_flash_period*2;
		break;
	case LED_FLASH_MODE_1V5:
		unRet = led_flash_period*5;
		break;
	case LED_FLASH_MODE_1V10:
		unRet = led_flash_period*10;
		break;
	default:
		break;
	}
	return unRet;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RGB��������˸���� 
// ������� : ��
// ���ز��� : ��
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
void CMD_led_flash(void)
{
	if(RC522_rFlag == ON)
	{
  		led_light_ctrl(LED_GREEN);			          // ��������ɫ
  		return;
	}
	if(led_flash_NO != LED_NONE)
    {


		if(led_flash_mode == LED_FLASH_MODE_ALWAYS)	//������ʾ
		{
			led_light_ctrl(led_flash_NO);			        // ��������Ӧ��ɫ
		}
		else                                        //��˸��ʾ
		{
			
			if(led_flash_status == 0)                 //��ǰ��Ϊ��״̬
			{
				
				if(sys.led_flash_count == 0)            //��������������л�������״̬���������øı�������ڲ���
				{
					led_light_ctrl(led_flash_NO);			    // ��������Ӧ��ɫ
					led_flash_status = 1;	                //�л�������״̬
					sys.led_flash_count = GetLedOnTime();
				}
			}
			
			else                                      //��ǰΪ����
			{
				
				if(sys.led_flash_count == 0)            //��������������л�������״̬���������øı�������ڲ���
				{
					led_light_ctrl(LED_NONE);
					led_flash_status = 0;	                //�л�������״̬
					sys.led_flash_count = GetLedOffTime();
				}
			}
		}
	}
	else                                          //��ǰ����ʾ
	{
		led_light_ctrl(LED_NONE);	
	}
}
////////////////////////////////////////////////////////////////////////////////
// �������� : ����ǰ��ʾled���� 
// ������� : ��
// ���ز��� : ��
// ˵��     :
////////////////////////////////////////////////////////////////////////////////

void led_light_ctrl(UINT8 ucValue)
{
	switch (ucValue)
	{
		case LED_RED:  		LED_G_OFF; LED_B_OFF;  LED_R_ON;  break;		
		case LED_GREEN:		LED_R_OFF; LED_B_OFF;  LED_G_ON;  break;
		case LED_BLUE:		LED_R_OFF; LED_G_OFF;  LED_B_ON;  break;
		
		case LED_RG:  		LED_B_OFF; LED_R_ON;   LED_G_ON;  break;		
		case LED_RB:		LED_G_OFF; LED_R_ON;   LED_B_ON;  break;
		case LED_GB:		LED_R_OFF; LED_G_ON;   LED_B_ON;  break;
		case LED_RGB:		LED_R_ON;  LED_G_ON;   LED_B_ON;  break;			
		default:			LED_R_OFF; LED_G_OFF; LED_B_OFF;break;
	}
}



void Display_Clear(void)                //�������(��ַ���ʱ���㲥���ʱ����ʾ���ʱ)
{
	
	led_flash_NO = LED_NONE;              //ָʾ�����
	
	BEEP_OFF;                             //���������
}
/*******************************************************************************
	 					�����Լ�����
    input:   none     
    output:  none
*******************************************************************************/ 
void led_on()
{
	  LED_R_ON;
  	LED_G_ON;
    LED_B_ON;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : LED�� 
// ������� : ��
// ���ز��� : ��
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
void led_off()
{
	 LED_R_OFF;
	 LED_G_OFF;
   LED_B_OFF;
}

/*******************************************************************************
								END					
*******************************************************************************/

