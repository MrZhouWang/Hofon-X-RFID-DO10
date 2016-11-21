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
	led_flash_NO = LED_NONE;
	//���������
	BEEP_OFF; 
	//OK��Ϣ���
 	IsOkPressed = FALSE;
	//ɨ����Ϣ���
	RC522_rFlag   = OFF;
		//�������
	LightTowerCtl[0].led_status = OFF;
	LightTowerCtl[1].led_status = OFF;
	LightTowerCtl[2].led_status = OFF;
	LightTowerCtl[3].led_status = OFF;
	LightTowerCtl[4].led_status = OFF;
	LightTowerCtl[5].led_status = OFF;
	LightTowerCtl[6].led_status = OFF;
	LightTowerCtl[7].led_status = OFF;
	LightTowerCtl[8].led_status = OFF;
	LightTowerCtl[9].led_status = OFF;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : ����Ҫ�����ݰ�ת��ΪASCII�� ��ɨ��ǹ���ݱ���һ��
// ������� : source�� Ҫת��������  
//					  des   �� ת�������ݴ�ŵ����� 
//						cnt   �� Ҫת���������С  
// ���ز��� : 
// ˵��     :  
////////////////////////////////////////////////////////////////////////////////
void FramToSendbuff(INT8U *source,INT8U *des,INT8U cnt)
{
   INT8U i,temp;
   {
        for(i=0;i<cnt;i++)
        {
            temp = (source[i]>>4);  //����λ
            if(temp>9)
            {
                des[2*(i+1)-2] =  temp+65-0x0A;
            }
            else
            {
                des[2*(i+1)-2] =  temp+0x30;
            }
            temp = (source[i]&0x0F);  //����λ
            if(temp>9)
            {
                des[2*(i+1)-1] =  temp+65-0x0A;
            }
            else
            {
               des[2*(i+1)-1] =  temp+0x30;
            }
          
        }
     }
}