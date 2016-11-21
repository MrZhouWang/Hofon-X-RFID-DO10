
#ifndef _KEY_H_
#define _KEY_H_

sbit KEY_OK  = P3^5;



extern UINT8 keyStatus;
extern UINT8 IsOkPressed;

void Scan_Keyboard(void);
void Process_Keyboard(void);

#endif
