#include "types.h"
#include "Version.h"
#include "dolphin/mtx.h"

extern "C" u8 lbl_8025C400[];

extern "C" s32 fn_8001E6C8(void* obj, s32 id);
extern "C" void* fn_8001DF00(void* obj, Vec* pos, s32 arg);
extern "C" void fn_8001E468(void* obj, s32 id, void* arg, s32 arg2);

// rel/ana.cpp
extern "C" s32 CmdDropHeldObj(s32 cmd, void* arg, void* arg2);

extern "C" void UpdateStage(void* arg);

// past the end of CGame's symbol size
#if defined(VERSION_JP)
// 0xCE8 earlier in japan, its CGame is shorter
#define SOUND() ((void*)(lbl_8025C400 + 0xE713C))
#else
#define SOUND() ((void*)(lbl_8025C400 + 0xE7E24))
#endif

typedef void (*RelSlotFunc)(void* arg);
typedef s32 (*RelCmdFunc)(s32 cmd, void* arg, void* arg2);

extern "C" {
RelSlotFunc _prolog_slots[7] = { 0, UpdateStage };
RelCmdFunc _prolog_cmds[]    = { CmdDropHeldObj };
}

/**
 * @note Address: stage18 .text 0x0
 * @note Size: 0x98
 */
extern "C" void UpdateStage(void* arg)
{
	Vec pos;

	pos.x = -278.67f;
	pos.y = 0.0f;
	pos.z = 265.43f;
	if (fn_8001E6C8(SOUND(), 0x9001F)) {
		fn_8001E468(SOUND(), 0x9001F, fn_8001DF00(SOUND(), &pos, 0), 0);
	}
}
