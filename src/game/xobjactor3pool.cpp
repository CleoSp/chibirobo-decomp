#include "game/xobjactor.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjActor3 lbl_8039CDF0[48];
XObjActor3* fn_80108424(XObjActor3* object, s32 flags);
}

/**
 * @note Address: 0x80134500
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80134500()
{
	__destroy_arr(lbl_8039CDF0, (ConstructorDestructor*)fn_80108424, sizeof(XObjActor3), ARRAY_SIZE(lbl_8039CDF0));
}
