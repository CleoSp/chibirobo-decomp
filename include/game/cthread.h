#ifndef _GAME_CTHREAD_H
#define _GAME_CTHREAD_H

#include "Dolphin/OS/OSThread.h"

// an OSThread with its stack, name and vtable from RTTI. the methods are in
// cthread.cpp, which still has its own copy of it
class CThread {
public:
	CThread()
	    : mStack(NULL)
	{
	}

	OSThread mThread; // _00
	u8* mStack;       // _318
	u32 mStackSize;   // _31C
	// _320 vtable

	virtual ~CThread() { } // _08 (weak), the DOL has no __dt__7CThreadFv, fn_8001950C is a weak copy

	// todo: cleanup, card jobs and the movie all start threads like this
	inline void Start(OSThreadStartFunction entry, void* arg, u8* stack, u32 size)
	{
		mStack     = stack;
		mStackSize = size;
		OSCreateThread(&mThread, entry, arg, stack + size, size, 16, OS_THREAD_ATTR_DETACH);
		OSResumeThread(&mThread);
	}
};
typedef char CThread_size_check[sizeof(CThread) == 0x328 ? 1 : -1];

extern "C" {
// the memory card jobs run on this one
extern CThread lbl_8036AC10;
extern u8 lbl_8036AF38[0x1000];
}

#endif
