#include "game/xobjarmy.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjArmy lbl_804924E8[21];
XObjArmy* __dt__8XObjArmyFv(XObjArmy* object, s32 flags);
}

/**
 * @note Address: 0x80133F40
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80133F40()
{
	__destroy_arr(lbl_804924E8, (ConstructorDestructor*)__dt__8XObjArmyFv, sizeof(XObjArmy), ARRAY_SIZE(lbl_804924E8));
}

/**
 * @note Address: 0x80133F78
 * @note Size: 0x88
 */
XObjArmy::XObjArmy()
{
}

// 0x80134000, XObjArmyShot's ctor, the compiler emits it here
