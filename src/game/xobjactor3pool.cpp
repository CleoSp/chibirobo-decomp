#include "game/xobjactor.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjActor3 sActor3Pool[48];
XObjActor3* __dt__10XObjActor3Fv(XObjActor3* object, s32 flags);
}

/**
 * @note Address: 0x80134500
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80134500()
{
	__destroy_arr(sActor3Pool, (ConstructorDestructor*)__dt__10XObjActor3Fv, sizeof(XObjActor3), ARRAY_SIZE(sActor3Pool));
}
