
#ifndef _KEY_H_
#define _KEY_H_

sbit KEY_OK  = P2^0;

#define TIMER_COUNT_100mS	100
#define TIMER_COUNT_1S		500
#define TIMER_COUNT_3S		800

extern UINT8 keyStatus;
extern UINT8 IsOkPressed;

void Scan_Keyboard(void);
void Process_Keyboard(void);

#endif
