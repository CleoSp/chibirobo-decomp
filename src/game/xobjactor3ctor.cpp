#include "game/xobjactor.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
// two plain objects pool 5 keeps next to its XObjActor3s
extern XObj lbl_8039BAE0[2];
}

/**
 * @note Address: 0x80134538
 * @note Size: 0xF4
 */
XObjActor3::XObjActor3()
{
}

// 0x8013462C, CPtclDust3's ctor, the compiler emits it here

/**
 * @note Address: 0x8013465C
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_8013465C()
{
	__destroy_arr(lbl_8039BAE0, (ConstructorDestructor*)fn_8000877C, sizeof(XObj), ARRAY_SIZE(lbl_8039BAE0));
}
