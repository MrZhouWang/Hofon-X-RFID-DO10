#ifndef _MYTYPE_H_
#define _MYTYPE_H_

#define UINT8	unsigned char
#define UINT16	unsigned short
#define UINT32	unsigned long

#define SINT8	char
#define SINT16	short int
#define SINT32	long int

#define INT16U	unsigned int
#define INT8U	unsigned char

#define FAIL 		0
#define OK   		1

#define FALSE       0
#define TRUE        1


#define NO          0
#define YES         1

#define ON			1
#define OFF			0

#define HIGH(x) ((x &0xff00)>>8)
#define LOW(x)  (x &0xff)
#define _MIN(a,b)  (a)<(b) ? a :b
#define _MAX(a,b)  (a)>(b) ? a :b

#define NOP	    (_nop_())



#endif
