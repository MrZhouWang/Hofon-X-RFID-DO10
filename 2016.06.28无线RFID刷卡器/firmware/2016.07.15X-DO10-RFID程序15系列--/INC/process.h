#ifndef _dnet_h_ 
#define _dnet_h_

//#include "STC_NEW_8051.H"

#define DISPLAY_MAX_NUM         9999
//COMMAND
#define INIT_ID_COMMAND 		1
#define INIT_NETWORK_COMMAND 	2
#define TO_RUN_COMMAND		3
#define INIT_PARAM_COMMAND	0x20
#define NETWORK_IO				5
#define NETWORK_IO_STATUS		6
#define PARAM_READ_RESP		0x8e
#define PARAM_WRITE_RESP		0x90

#define ONE_CMD_MIN_LENGTH           6

#define READ_MULTI_BYTE			0x03			//0X03号命令
#define WRITE_SINGLE_BYTE		0x06
#define WRITE_MULTI_BYTE     	0x10			//0X10号命令
#define MODBUS_D_DATA_LEGTH 	40//终端下传MODBUS 数据长度

//协议如下
//  0      1    2   3   4   5   6   7   8     
//命令字  长度 【2 - 7字节协议如上述】  和校验 
//        6+1     
//长度：从第一个数据字节起到最后一个字节，包括校验码。
//校验：校验码的产生方式—累加和。从命令号开始计算，到数据结束。
//
void PrepareRequestIdentity(void);
void ConnectSetup(void);
void ChangeMode(void);
void PrepareIOrequest(void);
void DN1022_Init(void);	
void Cmd_Process(void);
void ACK_Multi_Write_Error1(void);

#endif













