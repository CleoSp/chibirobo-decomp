#include "game/xobjactor.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjActorToy lbl_8046EF00[15];
XObjActorToy* fn_801155B4(XObjActorToy* object, s32 flags);
}

/**
 * @note Address: 0x80134248
 * @note Size: 0x38
 */
// todo: weak copy, fn_801155B4 is ~XObjActorToy, old name kept for the function pointer
extern "C" void fn_80134248()
{
	__destroy_arr(lbl_8046EF00, (ConstructorDestructor*)fn_801155B4, sizeof(XObjActorToy), ARRAY_SIZE(lbl_8046EF00));
}

/**
 * @note Address: 0x80134280
 * @note Size: 0x80
 */
XObjActorToy::XObjActorToy()
{
}
