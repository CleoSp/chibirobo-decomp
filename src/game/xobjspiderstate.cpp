#include "game/xobjspider.h"

/**
 * @note Address: 0x800F4818
 * @note Size: 0x50
 */
extern "C" void vf94__10XObjSpiderFv(XObjSpider* spider)
{
	fn_800F4350(spider);
	spider->SetState(0);
	spider->mFlags.mFlags |= 0x04000000;
}

/**
 * @note Address: 0x800F4868
 * @note Size: 0x58
 */
extern "C" void Init__10XObjSpiderFUl(XObjSpider* spider, u32 id)
{
	fn_800929C4(spider, id);
	fn_800F4350(spider);
	spider->SetState(0);
	spider->mFlags.mFlags |= 0x04000000;
}

// the state handlers, one per state
struct XObjSpiderFuncs {
	XObjSpider::Func mFuncs[10]; // _00
};
STATIC_ASSERT(sizeof(XObjSpiderFuncs) == 0x78);

// this tu's .data, the handler template is at _1C8 and the table at _240
extern "C" u8 lbl_8022AA10[];

/**
 * @note Address: 0x800F48C0
 * @note Size: 0x130
 */
// todo: hand rolled __sinit, really the table's initializer, needs the handlers as methods
extern "C" void __sinit_xobjspider_cpp()
{
	*(XObjSpiderFuncs*)(lbl_8022AA10 + 0x240) = *(XObjSpiderFuncs*)(lbl_8022AA10 + 0x1C8);
}

__declspec(section ".ctors") extern void (*const sSpiderFuncsInit)() = __sinit_xobjspider_cpp;
