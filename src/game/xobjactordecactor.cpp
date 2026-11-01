#include "game/xobjactor.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjActorDeca lbl_80490B88[2];
XObjActorDeca* fn_8012F4A4(XObjActorDeca* object, s32 flags);
}

/**
 * @note Address: 0x80134014
 * @note Size: 0x38
 */
// todo: weak copy, fn_8012F4A4 is ~XObjActorDeca, old name kept for the function pointer
extern "C" void fn_80134014()
{
	__destroy_arr(lbl_80490B88, (ConstructorDestructor*)fn_8012F4A4, sizeof(XObjActorDeca), ARRAY_SIZE(lbl_80490B88));
}

/**
 * @note Address: 0x8013404C
 * @note Size: 0x58
 */
XObjActorDeca::XObjActorDeca()
{
}

/**
 * @note Address: 0x801340A4
 * @note Size: 0x5C
 */
XObjActor::XObjActor()
{
}
