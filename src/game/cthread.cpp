#include "Dolphin/OS/OSAlloc.h"
#include "game/cthread.h"
#include "game/arcache.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__7CThread[];
extern void* __vt__10ARPreCache[];
}

/**
 * @note Address: 0x80019480
 * @note Size: 0x8C
 */
extern "C" BOOL fn_80019480(CThread* thread, OSThreadStartFunction func, void* arg, u32 stackSize, OSPriority prio, u16 attr)
{
	thread->mStack     = (u8*)OSAllocFromHeap(__OSCurrHeap, stackSize);
	thread->mStackSize = stackSize;
	if (OSCreateThread(&thread->mThread, func, arg, (u8*)thread->mStack + thread->mStackSize, thread->mStackSize, prio, attr)) {
		thread->mStackSize = stackSize;
		return TRUE;
	}
	return FALSE;
}

// todo: weak copy
/**
 * @note Address: 0x8001950C
 * @note Size: 0x48
 */
extern "C" CThread* fn_8001950C(CThread* thread, s32 flags)
{
	if (thread != NULL) {
		*(void***)((u8*)thread + 0x320) = __vt__7CThread; // _320 vtable
		if ((s16)flags > 0) {
			::operator delete(thread);
		}
	}
	return thread;
}

// todo: weak copy
/**
 * @note Address: 0x80019554
 * @note Size: 0x74
 */
extern "C" ARPreCache* fn_80019554(ARPreCache* cache, s32 flags)
{
	if (cache != NULL) {
		*(void***)((u8*)cache + 0x1E4) = __vt__10ARPreCache; // _1E4 vtable
		cache->mFile[1].CFile::~CFile();
		cache->mFile[0].CFile::~CFile();
		if ((s16)flags > 0) {
			::operator delete(cache);
		}
	}
	return cache;
}
