#include "game/xobjflags.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjCoin lbl_80405710[0x60];
XObjCoin* fn_800E2460(XObjCoin* object, s32 flags);
}

/**
 * @note Address: 0x80134400
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80134400()
{
	__destroy_arr(lbl_80405710, (ConstructorDestructor*)fn_800E2460, sizeof(XObjCoin), ARRAY_SIZE(lbl_80405710));
}

/**
 * @note Address: 0x80134438
 * @note Size: 0x48
 */
XObjCoin::XObjCoin()
{
	mFlags = &lbl_8037DFCC;
}
