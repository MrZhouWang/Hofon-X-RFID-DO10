#ifndef _TIMER_H
#define _TIMER_H


#ifdef TIMER_GLOBAL
	#define TIMER_EXT
#else
	#define TIMER_EXT extern
#endif

#define TH0_INIT        0xfc
#define TL0_INIT        0x17


void InitT0(void);
	 










#endif

