#ifndef _MAIN_H_
#define _MAIN_H_

#include <STC15F2K60S2.H>
#include "mytype.h"

#ifdef MAIN_GLOBAL
#define MAIN_EXT
#else
#define MAIN_EXT extern
#endif
//����rc522����ģʽ�Ƕ�����д************************
#define RC_WriteMode  1
#define RC_ReadMode  0
#define RC_ChangeMode  2
MAIN_EXT unsigned char RC_WorkeMode; 


#define ID_Num 16 //���幤�˹��ŵ��ֽ���
#define RC_DateMAX 16//һ�����ܴ�����ֽ���
MAIN_EXT unsigned char  g_ucTempbuf[RC_DateMAX]; //mrc522����buffer
MAIN_EXT volatile unsigned char code DefaultKey[6];
//MAIN_EXT unsigned char code af_change_data[16];//�޸�����������
//MAIN_EXT unsigned char code change_data[16];

MAIN_EXT unsigned char code rc_data[16] ;

#define DEV_CODE 0XA3
#define PRT_CNT_DELAY	20000

//�´���������
#define CMD_MODE00	0X00	
#define CMD_MODE01	0X01	
#define CMD_MODE02	0X02
#define CMD_MODE03	0X03

#define SMQ_DATA_CLR	0
#define SMQ_DATA_STORE	1

#define CMD_KEY_NULL     0
#define CMD_KEY_OK		1
#define CMD_KEY_ADD	2
#define CMD_KEY_SHT	3
#define CMD_KEY_CFG_L	4
#define CMD_KEY_CFG_S	5

#define MAX_ADDR        256//99      //1 - 99		 �����趨��ַ�޸�Ϊ 256		 20151214
#define MAX_BAUDRATE    3

#define TIMER_DATA_CHANGE	50      //100ms
#define TIMER_CONFIG        5000    //30s  �޸�Ϊ 10s
#define TIMER_MODIFY        5000    //10S

#define DELAY_100MS     100
#define DELAY_200MS     200
#define DELAY_300MS     300
#define DELAY_400MS     400
#define DELAY_1000MS     1000
#define DELAY_10000MS     10000

#define MAX_TASK_NUM 	2						//������ҵ��������
#define MAX_SRCODE_NUM 	20						//�����յ���ʾ��Ϣ

#define DIGITAL_LED_LOADING_DELAY  40000000    	// 1=100mS
#define DIGITAL_LED_REFRESH_DELAY  10    		// 1=100mS
//LEDָʾ�ƿ���
extern UINT16 led_flash_period ;
extern UINT8 led_flash_NO_BK ;
extern UINT8 led_flash_NO ;
extern UINT8 led_flash_mode ;
extern UINT8 led_flash_status;
//δ��������
extern volatile UINT8 beSettingAddr;				//0:������վַ		1:����վַ		2��վַ�趨�Ѿ�����OK��
extern volatile UINT8 toSettedAddr[3];				//���趨��ֵַ
extern volatile UINT8 task_unupdate_num ;			//����δ�´���ҵ��������
extern volatile UINT8 task_unfinish_num ;			//δ�����������= 0,1...MAX_TASK_NUM
extern volatile UINT8 task_unfinish_NO[MAX_TASK_NUM] ;		//��λ��
extern volatile UINT8 task_un_goods_numH[MAX_TASK_NUM];	//��Ʒ���ݸ�λ
extern volatile UINT8 task_un_goods_numL[MAX_TASK_NUM] ;	//��Ʒ���ݵ�λ

///����������
extern volatile UINT8 task_finished_num ;		  	//�������������= 0,1...MAX_TASK_NUM
extern volatile UINT8 task_finished_NO[MAX_TASK_NUM] ;	  	//��λ��
extern volatile UINT8 task_ed_goods_numH[MAX_TASK_NUM] ;	//��Ʒ���ݸ�λ
extern volatile UINT8 task_ed_goods_numL[MAX_TASK_NUM];	//��Ʒ���ݵ�λ

extern volatile UINT8 digital_led_refresh ;

extern volatile UINT8 SystemLocked;												//����ϵͳ������ϵͳ

//typedef struct 
//{
//	UINT8 SRCodeLen;															//���кų���
//	UINT8 SRCode[MAX_SRCODE_NUM];												//���к�
//}DisplayInfo;																	
//extern volatile DisplayInfo DispInfoCur;										//��ǰ��ʾ��Ϣ					 

typedef struct
{
	UINT16 led_flash_count;			//�ݼ���������ϵͳ�ڶ�ʱ����1MS�Լ�1
	UINT8  led_scan_flag;    		//led_Scan()==>LEDɨ������
}OS_SYS;

extern volatile OS_SYS sys;

////////////////////////////////////////////////////////////////////////////////
// �������� ��ϵͳ��ʼ��  
// ������� : ��
// ���ز��� : ��
// ˵��     : ���ֳ�ʼ��
////////////////////////////////////////////////////////////////////////////////
void InitializeSystem();
#endif
