#ifndef _UART_H_
#define _UART_H_


#define BD_4800      0
#define BD_9600      1
#define BD_19200     2
#define BD_38400    3

#define BAUDRATE_MAX  BD_38400

#define FOSC 11059200L
#define BD232_4800      4800
#define BD232_9600      9600
#define BD232_19200    19200
#define BD232_38400    38400

#define NONE_PARITY     0   //无校验位
#define ODD_PARITY      1   //奇校验
#define EVEN_PARITY     2   //偶校验
#define MARK_PARITY     3   //标记校验
#define SPACE_PARITY    4   //空校验

#define PARITYBIT232    NONE_PARITY
#define PARITYBIT       EVEN_PARITY

#define S2RI  0x01
#define S2TI  0x02
#define S2RB8 0x04
#define S2TB8 0x08


#define UART_BUF_LEN	            50	//串口缓冲器大小
#define UART232_BUF_LEN   			50	//串口缓冲器大小

typedef struct
{
	volatile UINT8 wBuffer[UART_BUF_LEN];
	volatile UINT8 wLen;
	volatile UINT8 rIndex;                     	//接收缓冲区数量
    volatile UINT8 rBuffer[UART_BUF_LEN];
	volatile UINT8 rTimer;						// 计时器
	volatile UINT8 rFlag;						// 接收缓冲器是否有数=ON -> 有数, =OFF无数(在定时器中查询缓冲区状态)
	volatile UINT8 rCheck;                   	// 暂时无用
}OS_UART;

extern volatile OS_UART xdata uart;
extern volatile OS_UART xdata uart232;

extern void InitT485(void);
extern void UART485_Init(void);
extern void UART232_Init(void);
extern void Send485SerialByte(UINT8 *Ram, UINT16 len);
extern UINT16 CRC16 ( UINT8 * puchMsg, UINT16 usDataLen ) ;


#endif

