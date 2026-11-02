#include "types.h"

extern "C" s32 CmdNop(s32 cmd, void* arg, void* arg2);

typedef void (*RelSlotFunc)(void* arg);
typedef s32 (*RelCmdFunc)(s32 cmd, void* arg, void* arg2);

extern "C" {
RelSlotFunc _prolog_slots[7] = { 0 };
RelCmdFunc _prolog_cmds[]    = { CmdNop };
}

/**
 * @note Address: stage08 .text 0x0
 * @note Size: 0x8
 */
extern "C" s32 CmdNop(s32 cmd, void* arg, void* arg2)
{
	return 0;
}
