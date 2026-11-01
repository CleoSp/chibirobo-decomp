#include "game/xobjleticker.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjLeticker lbl_80467330[12];
XObjLeticker* __dt__12XObjLetickerFv(XObjLeticker* object, s32 flags);
}

/**
 * @note Address: 0x80134300
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80134300()
{
	__destroy_arr(lbl_80467330, (ConstructorDestructor*)__dt__12XObjLetickerFv, sizeof(XObjLeticker), ARRAY_SIZE(lbl_80467330));
}

/**
 * @note Address: 0x80134338
 * @note Size: 0x48
 */
XObjLeticker::XObjLeticker()
{
}
