#include "include.h"
					
								 

UINT8 keyStatus = CMD_KEY_NULL;		//���ΰ��µİ���
UINT8 keyLastDown = CMD_KEY_NULL;	//�ϴΰ��µİ���
UINT8 IsOkPressed  = OFF;			//������ʾ��ֵ״̬�£���ʾ�Ƿ������޸Ľ���λ�������֣��޸Ľ���λ�£���λ0��Ҫ��ʾ��������Ҫ��ʾ��

/*******************************************************************************
	 					��ֹ��������
	input:   none     
	output:  none					
*******************************************************************************/
void Delay(void)
{
    UINT8 i;
    
    for (i=0; i<5; i++)
    {
        Delay1Ms();
    }
}
/*******************************************************************************
	 					ɨ������ӳ���
	input:   none     
	output:  none					
*******************************************************************************/
void Scan_Keyboard(void)
{
	keyStatus = CMD_KEY_NULL;
	//�ϴ��а������£�����ȴ��ϴΰ����ɿ��󣬲Ų�����һ�ΰ���
	if(keyLastDown == CMD_KEY_OK)		
	{
		if(!(KEY_OK == 0))
		{
			keyLastDown = CMD_KEY_NULL;
		}
	}
	//ȷ���ϴΰ��������ɿ�������½�����������
	if(keyLastDown == CMD_KEY_NULL)
	{
		if (KEY_OK == 0)						//ȷ��
		{
			Delay();
			if (KEY_OK == 0)
			{
				keyStatus = CMD_KEY_OK;
				keyLastDown = keyStatus;
			}
		}
	}
}
/*******************************************************************************
	 					OK�����������
	input:   none     
	output:  none					
*******************************************************************************/
void KeyProcessOK(void)
{
	IsOkPressed = ON;
}
/*******************************************************************************
	 					�����������
	input:   none     
	output:  none					
*******************************************************************************/
void Process_Keyboard(void)
{
	UINT16 updata_num = 0;
	UINT8 i = 0;

	if(SystemLocked)
	{  
		keyStatus = CMD_KEY_NULL;
		return;
	}

	if(beSettingAddr == 1)
	{ 
		//������Ŷ����µ�ַ
		address[0] = toSettedAddr[0];		address[1] = toSettedAddr[1];		address[2] = toSettedAddr[2];
		SaveParaToBlock();
		beSettingAddr  = 2;
		SetAddrFlag = 0;                //���õ�ַ��ָʾ����
		led_flash_NO = LED_NONE;				//����ǰ����һ��ƣ����պ����ڶ����
	}
	else
	{
		switch (keyStatus)
		{
		case CMD_KEY_OK:					//OK
			KeyProcessOK();
			break;
		default:
			break;
		}
	}
}
/*******************************************************************************
								END					
*******************************************************************************/

