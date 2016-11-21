#include "include.h"
UINT8 address[3];
UINT8 baudrate;
UINT8 ucAddr[3];                        //���ݵ�ַ
UINT8 ucBaud;                           //���ݲ�����

UINT8 FinishedLED = LED_NONE;			//�����ɺ�����ǰ��ָʾ��������ɫ
UINT8 WtTime = 5;						//һ�Զ�Ӧ��ʱ���������ȴ�ʱ��
UINT8 FnDefine = 0;						//Fn���ܼ�����	0-����	1-����
UINT8 DispMode = 0;						//��ʾģʽ	0-����ģʽ	1-������Ϣģʽ


///////////////////////////////////////////////////////////////////////////////


//��ȡ����ʱ�䣬��λms
UINT16 GetLedOnTime()
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case 0:						//����ģʽ��������ô˺���
		break;			
	case 1:
		unRet = led_flash_period/2;
		break;
	case 2:
		unRet = led_flash_period/3;
		break;
	case 3:
		unRet = led_flash_period/6;
		break;
	case 4:
		unRet = led_flash_period/11;
		break;
	case 5:
		unRet = led_flash_period/3*2;
		break;
	case 6:
		unRet = led_flash_period/6*5;
		break;
	case 7:
		unRet = led_flash_period/11*10;
		break;
	default:
		break;
	}
	return unRet;
}
//��ȡ����ʱ�䣬��λms
UINT16 GetLedOffTime()
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case 0:					   //����ģʽ��������ô˺���
		break;				
	case 1:
		unRet = led_flash_period/2;
		break;
	case 5:
		unRet = led_flash_period/3;
		break;
	case 6:
		unRet = led_flash_period/6;
		break;
	case 7:
		unRet = led_flash_period/11;
		break;
	case 2:
		unRet = led_flash_period/3*2;
		break;
	case 3:
		unRet = led_flash_period/6*5;
		break;
	case 4:
		unRet = led_flash_period/11*10;
		break;
	default:
		break;
	}
	return unRet;
}
//**********************RGB��������˸����***************************
void CMD_led_flash(void)
{
	if(RC522_rFlag == ON)
	{
		led_light_ctrl(LED_GREEN);			// ��������ɫ
		return;
	}
	if(led_flash_NO != LED_NONE)
    {


		if(led_flash_mode == LED_FLASH_MODE_ALWAYS)	//������ʾ
		{
			led_light_ctrl(led_flash_NO);			// ��������Ӧ��ɫ
		}
		else//��˸��ʾ
		{
			//��ǰ��Ϊ��״̬
			if(led_flash_status == 0)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(sys.led_flash_count == 0)
				{
					led_light_ctrl(led_flash_NO);			// ��������Ӧ��ɫ
					led_flash_status = 1;	//�л�������״̬
					sys.led_flash_count = GetLedOnTime();
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(sys.led_flash_count == 0)
				{
					led_light_ctrl(LED_NONE);
					led_flash_status = 0;	//�л�������״̬
					sys.led_flash_count = GetLedOffTime();
				}
			}
		}
	}
	else//��ǰ����ʾ
	{
		led_light_ctrl(LED_NONE);	
	}
}
/*******************************************************************************
	                    ����ǰ��ʾled����            
    Input :	 UINT8 ucValue   //LED_RED, LED_GREEN, LED_BLUE 
    Output:  None	
*******************************************************************************/
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


//�������(��ַ���ʱ���㲥���ʱ����ʾ���ʱ)
void Display_Clear(void)
{
	//ָʾ�����
	led_flash_NO = LED_NONE;
	//���������
	BEEP_OFF;
}
/*******************************************************************************
	 					�����Լ�����
    input:   none     
    output:  none
*******************************************************************************/ 
void led_on()
{
//	LED_R_ON;
//	LED_G_ON;
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
////////////////////////////////////////////////////////////////////////////////
// �������� : LED��˸ 
// ������� : ��
// ���ز��� : ��
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
void led_blink()
{
	led_on();
	DelayMs(30);
	led_off();
	DelayMs(35);
	led_on();
	DelayMs(30);
	led_off();
	DelayMs(25);
	led_on();
	DelayMs(30);
	led_off();
	DelayMs(25);
}
/*******************************************************************************
								END					
*******************************************************************************/

