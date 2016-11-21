#include "include.h"

volatile OS_UART xdata uart;
volatile OS_UART xdata uart232;
volatile UINT8 InTemp;
bit busy;

#define RCV_OVER_TIMER	        2
#define RCV_OVER_TIMER232		2
extern void Serial232PutChar(UINT8 dat);
extern void Send485SerialByte(UINT8 *Ram, UINT16 len);


/***************************************************
		UART1��ʼ��
  											7     6     5     4     3     2     1     0     Reset Value
sfr SCON = 0x98; //Serial Control         SM0/FE SM1   SM2   REN   TB8   RB8    TI    RI    0000,0000
SM0 SM1:
 0   0 	       ͬ����λ�Ĵ���		fosc/12
 0   1		   10λ�첽�շ�         �ɶ�ʱ������
 1   0         11λ�첽�շ�  		fosc/32��fosc/64
 1   1         11λ�첽�շ�         �ɶ�ʱ������
P = PSW.0      P=1��   P=0ż
***************************************************/
void InitT485(void)
{
//    SCON  = 0x50;  //���ô���Ϊģʽ1
//    TMOD &= 0x0F;  //����T1�Ŀ���λ
//    TMOD |= 0x20;  //����T1Ϊģʽ2
//    TH1 = 256 - (11059200/12/32)/9600;  //����T1����ֵ
//    TL1 = TH1;     //��ֵ��������ֵ
//    ET1 = 0;       //��ֹT1�ж�
//    ES  = 1;       //ʹ�ܴ����ж�
//    TR1 = 1;       //����T1


	TMOD = (TMOD & 0x0f) | 0x20;	//8λ�Զ����ض�ʱ��
	SCON = 0xd0;			      	//���пڹ���ģʽ3ѡ��
	IP   = 0x10;				    //PS set the high
	PCON  &= 0x7f;                  //SOMD =0

	//������ 38400
	AUXR |= 0x40;        			//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xfe;       			//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TL1 = 0xF7;            			//�趨��ʱ��ֵ
	TH1 = 0xF7;            			//�趨��ʱ����װֵ


	ET1  = 0;                       //close timer1 interrupt
	TR1  = 1;	                    //open timer1
	ES   = 1;				        //Enabled serial port interrrup 


}
/*******************************************************************************
	 						����1��ʼ��
	input :     none
	default:
	    data bits:  8
	    start bits: 1
	    stop bits:  1
	    parity:     NONE					
*******************************************************************************/
void UART485_Init(void)
{
	InitT485();
	uart.rIndex = 0;
	uart.rFlag  = 0;
	uart.rTimer = 0;	
}

/*******************************************************************************
	 					 ����1�жϺ���
*******************************************************************************/
void SCON485_int (void) interrupt 4
{
	if (RI)
    {
		RI = 0;
         
		if ( uart.rIndex >= UART_BUF_LEN ) return;
		if(uart.rFlag == ON)
			return;
		//	SBUF=SBUF;
		uart.rBuffer[uart.rIndex++] = SBUF;	
		uart.rTimer = RCV_OVER_TIMER;
	}
}
/***********************************************
        ���ڷ��ͳ���
input :     *Ram    
            len     
output :    NONE
***********************************************/
void Send485SerialByte(UINT8 *Ram, UINT16 len)
{
	LED_TXD485_ON;
	M485_CTRL_ON;
	for(;len>0;len--)
	{
		ACC = *Ram;
//		if(P == 0)
//		{
//			TB8 = 0;
//		}
//		else
//		{
//			TB8 =1;
//		}

		SBUF = *Ram;           
		while(!TI){;}             
		TI = 0;
		Ram++;
	}
	M485_CTRL_OFF;
	LED_TXD485_OFF;

}
///////////////////////////////////////////////--UART232--����--////////////////////////////////

/***************************************************
		UART2��ʼ��
***************************************************/
void InitT232(void)
{
	//ɨ��ǹ��������:8BIT����λ������żУ�顢1λֹͣλ
	#if (PARITYBIT232 == NONE_PARITY)
        S2CON = 0x5A;               //8λ�ɱ䲨���� (��У��λ)
	#elif (PARITYBIT232 == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
        S2CON = 0xda;               //9λ�ɱ䲨����,У��λ��ʼΪ1
	#elif (PARITYBIT232 == SPACE_PARITY)
        S2CON = 0xd5;               //9λ�ɱ䲨����,У��λ��ʼΪ0
	#endif
    
	switch (baudrate)
	{
		case BD_4800:
			BRT = -(FOSC/32/BD232_4800);      //���ö��������ʷ����������س�ֵ
			break;
		case BD_9600:
			BRT = -(FOSC/32/BD232_9600);      //���ö��������ʷ����������س�ֵ
			break;
		case BD_19200:
			BRT = -(FOSC/32/BD232_19200);      //���ö��������ʷ����������س�ֵ
			PCON |= 0x80;
			break;
		case BD_38400:
			BRT = -(FOSC/32/BD232_38400);      //���ö��������ʷ����������س�ֵ
			break;
		default:
			BRT = -(FOSC/32/BD232_9600);      //���ö��������ʷ����������س�ֵ
			break;
        }
        AUXR = 0x14;                //���������ʷ�����������1Tģʽ
        IE2 = 0x01;                 //ʹ�ܴ���2�ж�	
    return;
}
/*******************************************************************************
	 						���ڳ�ʼ��
	input :     none
	default:
	    data bits:  8
	    start bits: 1
	    stop bits:  1
	    parity:     NONE					
*******************************************************************************/
void UART232_Init(void)
{
	InitT232();
	uart232.rIndex = 0;
	uart232.rFlag  = 0;
	uart232.rTimer = 0;	
}

/*******************************************************************************
	 					 ɨ��ǹ�����жϺ���
				
*******************************************************************************/
void SCON232_int (void) interrupt 8
{    
	if(uart232.rFlag == ON)
	{
		uart232.rFlag   = OFF;
		uart232.rIndex  = 0;
		InTemp = 0;
	}
	// �����ж�
	if(S2CON & S2RI) 
	{
		S2CON &= ~S2RI;         //���������ɱ�־                 
		if ( uart232.rIndex >= UART232_BUF_LEN )
			return;
		uart232.rBuffer[uart232.rIndex++] = S2BUF;	
		uart232.rTimer = RCV_OVER_TIMER232;        
	}
    //��������ж�
	if (S2CON & S2TI)
	{
		S2CON &= ~S2TI;         //���������ɱ�־
		busy = 0;
	}
}

/*******************************************************************************
								END					
*******************************************************************************/
