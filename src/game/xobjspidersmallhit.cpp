#include "game/xobjspidersmall.h"

extern "C" {
// the tu's { 0, 0, 0 } literal, its rodata isn't split out with this unit
extern Vec lbl_8020AE68;
}

/**
 * @note Address: 0x800FD9EC
 * @note Size: 0x14
 */
extern "C" s32 fn_800FD9EC(XObjSpiderSmall* spider, s32 unused, s32 value)
{
	spider->mSpiderStatus.mFlags |= 1;
	return value;
}

/**
 * @note Address: 0x800FDA00
 * @note Size: 0xBC
 */
// hit from pos
extern "C" void fn_800FDA00(XObjSpiderSmall* spider, const Vec* pos, f32 strength)
{
	u32 flags = spider->mSpiderStatus.mFlags;
	if (flags & 2) {
		return;
	}
	if (flags & 0x10) {
		return;
	}
	if (flags & 0x8000) {
		return;
	}
	fn_8008A6C0(spider, strength);
	spider->mSpiderStatus.mFlags |= 2;
	f32 x                = pos->x;
	f32 y                = pos->y;
	f32 z                = pos->z;
	Vec hit              = lbl_8020AE68;
	hit.x                = x;
	hit.y                = y;
	hit.z                = z;
	spider->mHitPos      = hit;
	spider->mHitStrength = strength;
}

/**
 * @note Address: 0x800FDABC
 * @note Size: 0xF4
 */
extern "C" s32 fn_800FDABC(XObjSpiderSmall* spider, s32 count, const s32* args)
{
	for (s32 i = 0; i < count; ++i) { } // retail does this
	if (count < 1) {
		return 0;
	}
	spider->mEvtArgs[0] = args[0];
	if (count < 2) {
		spider->mEvtArgs[1] = 0;
	} else {
		spider->mEvtArgs[1] = args[1];
	}
	if (count < 3) {
		spider->mEvtArgs[2] = 0;
	} else {
		spider->mEvtArgs[2] = args[2];
	}
	if (count < 4) {
		spider->mEvtArgs[3] = 0;
	} else {
		spider->mEvtArgs[3] = args[3];
	}
	if (count < 5) {
		spider->mEvtArgs[4] = 0;
	} else {
		spider->mEvtArgs[4] = args[4];
	}
	if (count < 6) {
		spider->mEvtArgs[5] = 0;
	} else {
		spider->mEvtArgs[5] = args[5];
	}
	return 0;
}

/**
 * @note Address: 0x800FDBB0
 * @note Size: 0x4
 */
extern "C" void fn_800FDBB0(XObjSpiderSmall* spider)
{
}

/**
 * @note Address: 0x800FDBB4
 * @note Size: 0x4
 */
extern "C" void fn_800FDBB4(XObjSpiderSmall* spider)
{
}

/**
 * @note Address: 0x800FDBB8
 * @note Size: 0x4
 */
extern "C" void fn_800FDBB8(XObjSpiderSmall* spider)
{
}
