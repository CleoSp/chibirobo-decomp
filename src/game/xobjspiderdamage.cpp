#include "game/xobjspider.h"
#include "game/cgame.h"
#include "game/xchr.h"

extern "C" Vec lbl_8020AC5C;

/**
 * @note Address: 0x800F2350
 * @note Size: 0x14
 */
extern "C" s32 fn_800F2350(XObjSpider* spider, s32 unused, s32 value)
{
	spider->mFlags.mFlags |= 1;
	return value;
}

/**
 * @note Address: 0x800F2364
 * @note Size: 0xF0
 */
extern "C" void fn_800F2364(XObjSpider* spider, const Vec* from, f32 strength)
{
	u32 flags = spider->mFlags.mFlags;
	if (flags & 2)
		return;
	if (flags & 0x10)
		return;
	if (flags & 0x8000)
		return;
	XObjChr* player = lbl_8025C400.mPlayer;
	if (fn_8009C1BC(&player->mAttachments, 2) < 2)
		return;
	fn_8008A6C0(spider, strength);
	spider->mFlags.mFlags |= 2;
	f32 x = from->x;
	f32 y = from->y;
	f32 z = from->z;
	// retail copies the zero vector in first
	Vec pos              = lbl_8020AC5C;
	pos.x                = x;
	pos.y                = y;
	pos.z                = z;
	spider->mHitPos      = pos;
	spider->mHitStrength = strength;
}
