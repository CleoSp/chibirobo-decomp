#include "game/xobjflags.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjItem lbl_80441120[0x40];
XObjItem* fn_800E24C0(XObjItem* object, s32 flags);
}

/**
 * @note Address: 0x80134380
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80134380()
{
	__destroy_arr(lbl_80441120, (ConstructorDestructor*)fn_800E24C0, sizeof(XObjItem), ARRAY_SIZE(lbl_80441120));
}

/**
 * @note Address: 0x801343B8
 * @note Size: 0x48
 */
XObjItem::XObjItem()
{
	mFlags = &lbl_8037DDBC;
}
