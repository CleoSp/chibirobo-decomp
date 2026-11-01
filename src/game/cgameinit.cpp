#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/arcache.h"
#include "game/fbmirror.h"
#include "game/cthread.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"
#include "PowerPC_EABI_Support/Runtime/global_destructor_chain.h"

extern "C" {
// the file's .bss, the globals below and their destructor chain nodes all sit in it
extern u8 lbl_8025C200[];
extern void* __vt__11ARCacheInfo[];
extern void* __vt__10ARPreCache[];
extern void* __vt__5CFile[];
extern void* __vt__7CThread[];
CGame* __ct__5CGameFv(CGame* game);
CGame* __dt__5CGameFv(CGame* game, s32 flags);
ARCacheInfo* __dt__11ARCacheInfoFv(ARCacheInfo* info, s32 flags);
FbMirrorEx* fn_80019700(FbMirrorEx* mirror);
FbMirrorEx* __dt__10FbMirrorExFv(FbMirrorEx* mirror, s32 flags);
void fn_800195C8(void);
ARPreCache* __dt__10ARPreCacheFv(ARPreCache* cache, s32 flags);
CThread* __dt__7CThreadFv(CThread* thread, s32 flags);
}

// past the game object, ARCacheInfo then FbMirrorEx[5], ARPreCache and CThread, each after
// its 0xC byte chain node
#define GLOBALS (lbl_8025C200 + 0x200 + sizeof(CGame))

// todo: hand rolled __sinit, the real tu makes this from its globals
/**
 * @note Address: 0x8001933C
 * @note Size: 0x144
 */
extern "C" void fn_8001933C(void)
{
	u8* storage = lbl_8025C200;
	CGame* game = __ct__5CGameFv(&lbl_8025C400);
#if defined(VERSION_JP)
	__register_global_object(game, (void*)__dt__5CGameFv, storage + 0x134);
#else
	__register_global_object(game, (void*)__dt__5CGameFv, storage + 0x140);
#endif

	ARCacheInfo* info              = (ARCacheInfo*)(GLOBALS + 0x19C);
	info->_00                      = 0;
	*(void***)((u8*)info + 0x4008) = __vt__11ARCacheInfo;
	info->_04                      = 0;
	__register_global_object(info, (void*)__dt__11ARCacheInfoFv, (u8*)info - 0xC);

	__construct_array(GLOBALS + 0x41B4, (ConstructorDestructor)fn_80019700, (ConstructorDestructor)__dt__10FbMirrorExFv, sizeof(FbMirrorEx),
	                  5);
	__register_global_object(NULL, (void*)fn_800195C8, GLOBALS + 0x41A8);

	ARPreCache* cache                        = (ARPreCache*)(GLOBALS + 0x5084);
	*(void***)((u8*)cache + 0x1E4)           = __vt__10ARPreCache;
	*(void***)((u8*)&cache->mFile[0] + 0xE8) = __vt__5CFile;
	cache->mFile[0].mArchive                 = NULL;
	cache->mFile[0].mEntrynum                = -1;
	cache->mFile[0].mBuffer                  = NULL;
	cache->mFile[0].mResult                  = 0;
	cache->mFile[0].mState                   = 1;
	*(void***)((u8*)&cache->mFile[1] + 0xE8) = __vt__5CFile;
	cache->mFile[1].mArchive                 = NULL;
	cache->mFile[1].mEntrynum                = -1;
	cache->mFile[1].mBuffer                  = NULL;
	cache->mFile[1].mResult                  = 0;
	cache->mFile[1].mState                   = 1;
	cache->_00                               = 0;
	__register_global_object(cache, (void*)__dt__10ARPreCacheFv, (u8*)cache - 0xC);

	CThread* thread                 = (CThread*)(GLOBALS + 0x5278);
	*(void***)((u8*)thread + 0x320) = __vt__7CThread;
	thread->mStack                  = NULL;
	__register_global_object(thread, (void*)__dt__7CThreadFv, (u8*)thread - 0xC);
}
