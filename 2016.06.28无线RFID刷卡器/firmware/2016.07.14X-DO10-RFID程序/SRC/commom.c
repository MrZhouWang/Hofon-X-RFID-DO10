#define COMMON_GLOBAL
#include "include.h"

////////////////////////////////////////////////////////////////////////////////
// �������� : �������(��ַ���ʱ���㲥���ʱ����ʾ���ʱ)
// ������� : 
// ���ز��� : 
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
void ClearAll(void)
{
	//��ʾ��Ϣ���
	UINT8 i;
	DispInfoCur.SRCodeLen = 0;
	for (i=0; i<LED_NUMBER; i++)
	{
		led[i].num = 0;
		led[i].show = OFF;
	}
	//ָʾ�����
	led_flash_NO = LED_NONE;
	//���������
	 
	//OK��Ϣ���
 	IsOkPressed = FALSE;
	//ɨ����Ϣ���
	uart232.rFlag   = OFF;
	uart232.rIndex  = 0;
	//�������
	LightTowerCtl[0].led_status = OFF;
	LightTowerCtl[1].led_status = OFF;
	LightTowerCtl[2].led_status = OFF;
	LightTowerCtl[3].led_status = OFF;
	LightTowerCtl[4].led_status = OFF;
	LightTowerCtl[5].led_status = OFF;
}
