#ifndef _PROCESS_h_ 
#define _PROCESS_h_

#ifdef  PRO_GLOBAL
#define PRO_EXT
#else
#define PRO_EXT extern
#endif


PRO_EXT UINT8 SetAddrFlag; //设置地址指示灯标志 闪烁及蜂鸣器响

PRO_EXT UINT8 ID_ASCII[35];


#define TO_RUN_COMMAND		3
#define INIT_PARAM_COMMAND	0x20
#define NETWORK_IO				5
#define NETWORK_IO_STATUS		6
#define PARAM_READ_RESP		0x8e
#define PARAM_WRITE_RESP		0x90

#define ONE_CMD_MIN_LENGTH           6

#define READ_MULTI_BYTE			0x03			  //0X03号命令
#define WRITE_SINGLE_BYTE		0x06
#define WRITE_MULTI_BYTE     	0x10			//0X10号命令
#define MODBUS_D_DATA_LEGTH 	40        //终端下传MODBUS 数据长度


void PrepareRequestIdentity(void);
void ConnectSetup(void);
void ChangeMode(void);
void PrepareIOrequest(void);
void DN1022_Init(void);	
void Cmd_Process(void);
void ACK_Multi_Write_Error1(void);

#endif













