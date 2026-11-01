#include "game/xobjspiderboss.h"
#include "game/xobjspidersmall.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjSpiderSmall lbl_804ADE48[16];
}

/**
 * @note Address: 0x80133D7C
 * @note Size: 0x5C
 */
XObjSpiderBoss::XObjSpiderBoss()
{
}

/**
 * @note Address: 0x80133DD8
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80133DD8()
{
	__destroy_arr(lbl_804ADE48, (ConstructorDestructor*)__dt__15XObjSpiderSmallFv, sizeof(XObjSpiderSmall), ARRAY_SIZE(lbl_804ADE48));
}

/**
 * @note Address: 0x80133E10
 * @note Size: 0x74
 */
XObjSpiderSmall::XObjSpiderSmall()
{
}
