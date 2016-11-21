#ifndef _MAIN_H_
#define _MAIN_H_

#include <STC15F2K60S2.H>
#include "mytype.h"

#ifdef MAIN_GLOBAL
#define MAIN_EXT
#else
#define MAIN_EXT extern
#endif
//定义rc522工作模式是读还是写************************
#define RC_WriteMode  1
#define RC_ReadMode  0
#define RC_ChangeMode  2
MAIN_EXT unsigned char RC_WorkeMode; 


#define ID_Num 16 //定义工人工号的字节数
#define RC_DateMAX 16//一个块能存入的字节数
MAIN_EXT unsigned char  g_ucTempbuf[RC_DateMAX]; //mrc522接收buffer
MAIN_EXT volatile unsigned char code DefaultKey[6];
//MAIN_EXT unsigned char code af_change_data[16];//修改密码后的密码
//MAIN_EXT unsigned char code change_data[16];

MAIN_EXT unsigned char code rc_data[16] ;

#define DEV_CODE 0XA3
#define PRT_CNT_DELAY	20000

//下传的命令码
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

#define MAX_ADDR        256//99      //1 - 99		 最大可设定地址修改为 256		 20151214
#define MAX_BAUDRATE    3

#define TIMER_DATA_CHANGE	50      //100ms
#define TIMER_CONFIG        5000    //30s  修改为 10s
#define TIMER_MODIFY        5000    //10S

#define DELAY_100MS     100
#define DELAY_200MS     200
#define DELAY_300MS     300
#define DELAY_400MS     400
#define DELAY_1000MS     1000
#define DELAY_10000MS     10000

#define MAX_TASK_NUM 	2						//最大的作业任务数量
#define MAX_SRCODE_NUM 	20						//最大接收的显示信息

#define DIGITAL_LED_LOADING_DELAY  40000000    	// 1=100mS
#define DIGITAL_LED_REFRESH_DELAY  10    		// 1=100mS
//LED指示灯控制
extern UINT16 led_flash_period ;
extern UINT8 led_flash_NO_BK ;
extern UINT8 led_flash_NO ;
extern UINT8 led_flash_mode ;
extern UINT8 led_flash_status;
//未完成任务块
extern volatile UINT8 beSettingAddr;				//0:非设置站址		1:设置站址		2：站址设定已经按下OK键
extern volatile UINT8 toSettedAddr[3];				//待设定地址值
extern volatile UINT8 task_unupdate_num ;			//主机未下传作业任务总数
extern volatile UINT8 task_unfinish_num ;			//未完成任务总数= 0,1...MAX_TASK_NUM
extern volatile UINT8 task_unfinish_NO[MAX_TASK_NUM] ;		//货位号
extern volatile UINT8 task_un_goods_numH[MAX_TASK_NUM];	//商品数据高位
extern volatile UINT8 task_un_goods_numL[MAX_TASK_NUM] ;	//商品数据低位

///已完成任务块
extern volatile UINT8 task_finished_num ;		  	//已完成任务总数= 0,1...MAX_TASK_NUM
extern volatile UINT8 task_finished_NO[MAX_TASK_NUM] ;	  	//货位号
extern volatile UINT8 task_ed_goods_numH[MAX_TASK_NUM] ;	//商品数据高位
extern volatile UINT8 task_ed_goods_numL[MAX_TASK_NUM];	//商品数据低位

extern volatile UINT8 digital_led_refresh ;

extern volatile UINT8 SystemLocked;												//锁定系统、解锁系统

//typedef struct 
//{
//	UINT8 SRCodeLen;															//序列号长度
//	UINT8 SRCode[MAX_SRCODE_NUM];												//序列号
//}DisplayInfo;																	
//extern volatile DisplayInfo DispInfoCur;										//当前显示信息					 

typedef struct
{
	UINT16 led_flash_count;			//递减计算器，系统在定时器中1MS自减1
	UINT8  led_scan_flag;    		//led_Scan()==>LED扫描启动
}OS_SYS;

extern volatile OS_SYS sys;

////////////////////////////////////////////////////////////////////////////////
// 功能描述 ：系统初始化  
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 各种初始化
////////////////////////////////////////////////////////////////////////////////
void InitializeSystem();
#endif
