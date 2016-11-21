#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef COMMON_GLOBAL
	#define COMMON_EXT
#else
	#define COMMON_EXT extern
#endif







extern void ClearAll(void);
extern void FramToSendbuff(INT8U *source,INT8U *des,INT8U cnt);

#endif


