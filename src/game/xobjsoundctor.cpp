#include "game/xobjsound.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjSound lbl_80400A00[8];
XObjSound* fn_800DD32C(XObjSound* object, s32 flags);
}

/**
 * @note Address: 0x80134480
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80134480()
{
	__destroy_arr(lbl_80400A00, (ConstructorDestructor*)fn_800DD32C, sizeof(XObjSound), ARRAY_SIZE(lbl_80400A00));
}

/**
 * @note Address: 0x801344B8
 * @note Size: 0x48
 */
XObjSound::XObjSound()
{
}
