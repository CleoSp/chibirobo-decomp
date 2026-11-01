#include "game/xobjfree.h"
#include "game/xobjhbox.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjFree lbl_804F41F0[0x120];
extern XObjHBox lbl_804E1060[0x10];
XObjFree* fn_800DD5A0(XObjFree* object, s32 flags);
XObjHBox* fn_800E239C(XObjHBox* object, s32 flags);
}

/**
 * @note Address: 0x80133B98
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80133B98()
{
	__destroy_arr(lbl_804F41F0, (ConstructorDestructor*)fn_800DD5A0, sizeof(XObjFree), ARRAY_SIZE(lbl_804F41F0));
}

/**
 * @note Address: 0x80133BD0
 * @note Size: 0x3C
 */
XObjFree::XObjFree()
{
}

/**
 * @note Address: 0x80133C0C
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80133C0C()
{
	__destroy_arr(lbl_804E1060, (ConstructorDestructor*)fn_800E239C, sizeof(XObjHBox), ARRAY_SIZE(lbl_804E1060));
}

/**
 * @note Address: 0x80133C44
 * @note Size: 0x44
 */
XObjHBox::XObjHBox()
{
}

/**
 * @note Address: 0x80133C88
 * @note Size: 0x30
 */
XObjHBoxPart::XObjHBoxPart()
{
}
