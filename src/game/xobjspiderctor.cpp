#include "game/xobjspider.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern XObjSpider lbl_804A06B8[16];
XObjSpider* __dt__10XObjSpiderFv(XObjSpider* object, s32 flags);
}

/**
 * @note Address: 0x80133E84
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80133E84()
{
	__destroy_arr(lbl_804A06B8, (ConstructorDestructor*)__dt__10XObjSpiderFv, sizeof(XObjSpider), ARRAY_SIZE(lbl_804A06B8));
}

/**
 * @note Address: 0x80133EBC
 * @note Size: 0x84
 */
XObjSpider::XObjSpider()
{
}
