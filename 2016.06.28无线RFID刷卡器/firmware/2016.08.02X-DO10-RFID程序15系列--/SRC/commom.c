#define COMMON_GLOBAL
#include "include.h"

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 清除命令(地址清除时、广播清除时、显示清除时)
// 输入参数 : 
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void ClearAll(void)
{
	led_flash_NO = LED_NONE;
	//蜂鸣器清除
	BEEP_OFF; 
	//OK信息清除
 	IsOkPressed = FALSE;
	//扫描信息清除
	RC522_rFlag   = OFF;
		//灯塔清除
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
// 功能描述 : 将需要的数据包转化为ASCII码 与扫描枪数据保持一致
// 输入参数 : source： 要转化的数组  
//					  des   ： 转化后数据存放的数组 
//						cnt   ： 要转化的数组大小  
// 返回参数 : 
// 说明     :  
////////////////////////////////////////////////////////////////////////////////
void FramToSendbuff(INT8U *source,INT8U *des,INT8U cnt)
{
   INT8U i,temp;
   {
        for(i=0;i<cnt;i++)
        {
            temp = (source[i]>>4);  //高四位
            if(temp>9)
            {
                des[2*(i+1)-2] =  temp+65-0x0A;
            }
            else
            {
                des[2*(i+1)-2] =  temp+0x30;
            }
            temp = (source[i]&0x0F);  //低四位
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