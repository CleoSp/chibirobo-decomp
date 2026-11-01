#include "game/xobjflags.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjDust lbl_804B9850[0x40];
XObjDust* fn_800E2240(XObjDust* object, s32 flags);
}

/**
 * @note Address: 0x80133CB8
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80133CB8()
{
	__destroy_arr(lbl_804B9850, (ConstructorDestructor*)fn_800E2240, sizeof(XObjDust), ARRAY_SIZE(lbl_804B9850));
}

/**
 * @note Address: 0x80133CF0
 * @note Size: 0x48
 */
XObjDust::XObjDust()
{
	mFlags = &lbl_8037E1DC;
}

/**
 * @note Address: 0x80133D38
 * @note Size: 0x44
 */
XObjFlags::XObjFlags()
{
	mFlags = NULL;
}
