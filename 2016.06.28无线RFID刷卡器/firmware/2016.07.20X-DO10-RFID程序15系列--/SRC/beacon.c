#include "include.h"
LIGHTTOWER_CTL LightTowerCtl[10];
////////////////////////////////////////////////////////////////////////////////
// �������� : ��ȡ���������ڵ���ʱ�䣬�����Ϩ��ʱ���γ�һ����
// ������� : LightTowIndex��  �ڼ�������
// ���ز��� : ���������ڵ���ʱ��
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
UINT16 GetLightTowerOnTime(int LightTowIndex)
{
	UINT16 unRet = LightTowerCtl[LightTowIndex].led_flash_period;
	switch(LightTowerCtl[LightTowIndex].led_switch_ratio)
	{
	case 0X00:	
		break;				//����ģʽ��������ô˺���
	case 0X01: 
	    unRet = LightTowerCtl[LightTowIndex].led_flash_period/2;
	    break;
	case 0X02:	
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/3;
	    break;
	case 0X03:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/6;	
	    break;
	case 0X04:	
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/11;
		break;
	case 0X05:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/3*2;
		break;
	case 0X06:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/6*5;
		break;
	case 0X07:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/11*10;
		break;
	default:
		break;
	}
	return unRet;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : ��ȡ����������Ϩ��ʱ�䣬���������ʱ���γ�һ����
// ������� : LightTowIndex��  �ڼ�������
// ���ز��� : ����������Ϩ��ʱ��
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
UINT16 GetLightTowerOffTime(int LightTowIndex)
{
	UINT16 unRet = LightTowerCtl[LightTowIndex].led_flash_period;
	switch(LightTowerCtl[LightTowIndex].led_switch_ratio)
	{
	case 0X00:										//����ģʽ��������ô˺���
		break;			
	case 0X01:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/2;
		break;
	case 0X05:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/3;
		break;
	case 0X06:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/6;
		break;
	case 0X07:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/11;
		break;
	case 0X02:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/3*2;
		break;
	case 0X03:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/6*5;
		break;
	case 0X04:
		unRet = LightTowerCtl[LightTowIndex].led_flash_period/11*10;
		break;
	default:
		break;
	}
	return unRet;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : �����������ڵȿ��ƣ�
// ������� : ��
// ���ز��� : ��
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
void CMD_LigheTower_flash(void)
{ 
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED1
	if(LightTowerCtl[0].led_status == ON)
	{
		//����
		if(LightTowerCtl[0].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL1_ON;
		}
		//��˸
		else
		{	
			//��ǰ��Ϊ��״̬
			if(LightTowerCtl[0].led_status_cur == OFF)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[0].led_flash_count == 0)
				{
			        CNTRL1_ON;

					LightTowerCtl[0].led_status_cur = ON;
					LightTowerCtl[0].led_flash_count = GetLightTowerOnTime(0);
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[0].led_flash_count == 0)
				{
			        CNTRL1_OFF;

					LightTowerCtl[0].led_status_cur = OFF;
					LightTowerCtl[0].led_flash_count = GetLightTowerOffTime(0);
				}
			}
		}
	}
	else
	{
        CNTRL1_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED2
	if(LightTowerCtl[1].led_status == ON)
	{
		//����
		if(LightTowerCtl[1].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL2_ON;
		}
		//��˸
		else
		{	
			//��ǰ��Ϊ��״̬
			if(LightTowerCtl[1].led_status_cur == OFF)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[1].led_flash_count == 0)
				{
			        CNTRL2_ON;
					LightTowerCtl[1].led_status_cur = ON;
					LightTowerCtl[1].led_flash_count = GetLightTowerOnTime(1);
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[1].led_flash_count == 0)
				{
			        CNTRL2_OFF;

					LightTowerCtl[1].led_status_cur = OFF;
					LightTowerCtl[1].led_flash_count = GetLightTowerOffTime(1);
				}
			}
		}
	}
	else
	{
        CNTRL2_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED3
	if(LightTowerCtl[2].led_status == ON)
	{
		//����
		if(LightTowerCtl[2].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL3_ON;
		}
		//��˸
		else
		{	
			//��ǰ��Ϊ��״̬
			if(LightTowerCtl[2].led_status_cur == OFF)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[2].led_flash_count == 0)
				{
			        CNTRL3_ON;

					LightTowerCtl[2].led_status_cur = ON;
					LightTowerCtl[2].led_flash_count = GetLightTowerOnTime(2);
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[2].led_flash_count == 0)
				{
			        CNTRL3_OFF;

					LightTowerCtl[2].led_status_cur = OFF;
					LightTowerCtl[2].led_flash_count = GetLightTowerOffTime(2);
				}
			}
		}
	}
	else
	{
        CNTRL3_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED4
	if(LightTowerCtl[3].led_status == ON)
	{
		//����
		if(LightTowerCtl[3].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL4_ON;
		}
		//��˸
		else
		{	
			//��ǰ��Ϊ��״̬
			if(LightTowerCtl[3].led_status_cur == OFF)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[3].led_flash_count == 0)
				{
			        CNTRL4_ON;

					LightTowerCtl[3].led_status_cur = ON;
					LightTowerCtl[3].led_flash_count = GetLightTowerOnTime(3);
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[3].led_flash_count == 0)
				{
			        CNTRL4_OFF;

					LightTowerCtl[3].led_status_cur = OFF;
					LightTowerCtl[3].led_flash_count = GetLightTowerOffTime(3);
				}
			}
		}
	}
	else
	{
        CNTRL4_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED5
	if(LightTowerCtl[4].led_status == ON)
	{
		//����
		if(LightTowerCtl[4].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL5_ON;
		}
		//��˸
		else
		{	
			//��ǰ��Ϊ��״̬
			if(LightTowerCtl[4].led_status_cur == OFF)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[4].led_flash_count == 0)
				{
			        CNTRL5_ON;

					LightTowerCtl[4].led_status_cur = ON;
					LightTowerCtl[4].led_flash_count = GetLightTowerOnTime(4);
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[4].led_flash_count == 0)
				{
			        CNTRL5_OFF;

					LightTowerCtl[4].led_status_cur = OFF;
					LightTowerCtl[4].led_flash_count = GetLightTowerOffTime(4);
				}
			}
		}
	}
	else
	{
        CNTRL5_OFF;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//LED6
	if(LightTowerCtl[5].led_status == ON)
	{
		//����
		if(LightTowerCtl[5].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL6_ON;
		}
		//��˸
		else
		{	
			//��ǰ��Ϊ��״̬
			if(LightTowerCtl[5].led_status_cur == OFF)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[5].led_flash_count == 0)
				{
			        CNTRL6_ON;

					LightTowerCtl[5].led_status_cur = ON;
					LightTowerCtl[5].led_flash_count = GetLightTowerOnTime(5);
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[5].led_flash_count == 0)
				{
			        CNTRL6_OFF;

					LightTowerCtl[5].led_status_cur = OFF;
					LightTowerCtl[5].led_flash_count = GetLightTowerOffTime(5);
				}
			}
		}
	}
	else
	{
        CNTRL6_OFF;
	}
/*********************************LED7********************************************/
	if(LightTowerCtl[6].led_status == ON)
	{
		//����
		if(LightTowerCtl[6].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL7_ON;
		}
		//��˸
		else
		{	
			//��ǰ��Ϊ��״̬
			if(LightTowerCtl[6].led_status_cur == OFF)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[6].led_flash_count == 0)
				{
			        CNTRL7_ON;

					LightTowerCtl[6].led_status_cur = ON;
					LightTowerCtl[6].led_flash_count = GetLightTowerOnTime(6);
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[6].led_flash_count == 0)
				{
			        CNTRL7_OFF;

					LightTowerCtl[6].led_status_cur = OFF;
					LightTowerCtl[6].led_flash_count = GetLightTowerOffTime(6);
				}
			}
		}
	}
	else
	{
        CNTRL7_OFF;
	}
/*********************************LED8********************************************/
	if(LightTowerCtl[7].led_status == ON)
	{
		//����
		if(LightTowerCtl[7].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL8_ON;
		}
		//��˸
		else
		{	
			//��ǰ��Ϊ��״̬
			if(LightTowerCtl[7].led_status_cur == OFF)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[7].led_flash_count == 0)
				{
			        CNTRL8_ON;

					LightTowerCtl[7].led_status_cur = ON;
					LightTowerCtl[7].led_flash_count = GetLightTowerOnTime(7);
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[7].led_flash_count == 0)
				{
			        CNTRL8_OFF;

					LightTowerCtl[7].led_status_cur = OFF;
					LightTowerCtl[7].led_flash_count = GetLightTowerOffTime(7);
				}
			}
		}
	}
	else
	{
        CNTRL8_OFF;
	}
/*********************************LED9********************************************/
	if(LightTowerCtl[8].led_status == ON)
	{
		//����
		if(LightTowerCtl[8].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL9_ON;
		}
		//��˸
		else
		{	
			//��ǰ��Ϊ��״̬
			if(LightTowerCtl[8].led_status_cur == OFF)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[8].led_flash_count == 0)
				{
			        CNTRL9_ON;

					LightTowerCtl[8].led_status_cur = ON;
					LightTowerCtl[8].led_flash_count = GetLightTowerOnTime(8);
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[8].led_flash_count == 0)
				{
			        CNTRL9_OFF;

					LightTowerCtl[8].led_status_cur = OFF;
					LightTowerCtl[8].led_flash_count = GetLightTowerOffTime(8);
				}
			}
		}
	}
	else
	{
        CNTRL9_OFF;
	}
/*********************************LED10********************************************/
	if(LightTowerCtl[9].led_status == ON)
	{
		//����
		if(LightTowerCtl[9].led_switch_ratio == LED_FLASH_MODE_ALWAYS)
		{
	        CNTRL10_ON;
		}
		//��˸
		else
		{	
			//��ǰ��Ϊ��״̬
			if(LightTowerCtl[9].led_status_cur == OFF)
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[9].led_flash_count == 0)
				{
			        CNTRL10_ON;

					LightTowerCtl[9].led_status_cur = ON;
					LightTowerCtl[9].led_flash_count = GetLightTowerOnTime(9);
				}
			}
			//��ǰΪ����
			else
			{
				//��������������л�������״̬���������øı�������ڲ���
				if(LightTowerCtl[9].led_flash_count == 0)
				{
			        CNTRL10_OFF;

					LightTowerCtl[9].led_status_cur = OFF;
					LightTowerCtl[9].led_flash_count = GetLightTowerOffTime(9);
				}
			}
		}
	}
	else
	{
        CNTRL10_OFF;
	}
}