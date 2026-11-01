#include "game/menu.h"
#include "game/xobjs.h"

extern "C" {
extern Vec lbl_80362048;
extern Vec lbl_8036203C;
void fn_80026B9C(XObj* object, u32 frames);
}

// the placement entry's scale, in percent there
static inline Vec EntryScale(XObj* object)
{
	UnkStageObj* entry = object->mCoord._00;
	Vec scale          = lbl_80362048;
	scale.x            = entry->mScale[0] / 100.0f;
	scale.y            = entry->mScale[1] / 100.0f;
	scale.z            = entry->mScale[2] / 100.0f;
	return scale;
}

/**
 * @note Address: 0x800621E0
 * @note Size: 0xD8
 */
extern "C" CVec fn_800621E0(XObj* object)
{
	return EntryScale(object);
}

// and its position, in centimeters
static inline Vec EntryPos(XObj* object)
{
	UnkStageObj* entry = object->mCoord._00;
	Vec pos            = lbl_8036203C;
	pos.x              = entry->mPos[0] / 100.0f;
	pos.y              = entry->mPos[1] / 100.0f;
	pos.z              = entry->mPos[2] / 100.0f;
	return pos;
}

/**
 * @note Address: 0x800622B8
 * @note Size: 0xD8
 */
extern "C" CVec fn_800622B8(XObj* object)
{
	return EntryPos(object);
}

/**
 * @note Address: 0x80062390
 * @note Size: 0x48
 */
extern "C" void fn_80062390(MenuCursor* cursor, u32 frames)
{
	if (cursor->mStatus.mFlags & 1ULL)
		fn_80026B9C(cursor, frames);
}

/**
 * @note Address: 0x800623D8
 * @note Size: 0x20
 */
extern "C" void fn_800623D8(MenuCursor* cursor)
{
	fn_800928E8(cursor);
}
