#define DEFER_THREAD_DESTRUCTOR
#include "Dolphin/OS/OSAlloc.h"
#include "game/cthread.h"
#include "game/arcache.h"

#pragma section RX ".text.thread"

__declspec(section ".text.thread") BOOL CThread::Create(OSThreadStartFunction func, void* arg, u32 stackSize, OSPriority prio, u16 attr)
{
	mStack     = (u8*)OSAllocFromHeap(__OSCurrHeap, stackSize);
	mStackSize = stackSize;
	if (OSCreateThread(&mThread, func, arg, mStack + mStackSize, mStackSize, prio, attr)) {
		mStackSize = stackSize;
		return TRUE;
	}
	return FALSE;
}

__declspec(section ".text.thread") CThread::~CThread()
{
}
__declspec(section ".text.thread") ARPreCache::~ARPreCache()
{
}
