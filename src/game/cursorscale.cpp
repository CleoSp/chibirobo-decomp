#include "game/title.h"
#include "game/cjobjset.h"

// y is negated for screen space

/**
 * @note Address: 0x80025150
 * @note Size: 0xD8
 */
// Cursor's SetScaleY
extern "C" void fn_80025150(Cursor* cursor, f32 y, s32 force)
{
	y                            = -y;
	cursor->mMoveScale.y         = y;
	cursor->mMoveScale.mTarget.y = y;
	if (cursor->mJObj != NULL) {
		cursor->CJObj::SetScaleY(y, force);
	}
}

/**
 * @note Address: 0x80025228
 * @note Size: 0x190
 */
// Cursor's SetScale(scale)
extern "C" void fn_80025228(Cursor* cursor, f32 scale, s32 force)
{
	f32 y                        = -scale;
	cursor->mMoveScale.x         = scale;
	cursor->mMoveScale.y         = y;
	cursor->mMoveScale.z         = scale;
	cursor->mMoveScale.mTarget.x = scale;
	cursor->mMoveScale.mTarget.y = y;
	cursor->mMoveScale.mTarget.z = scale;
	if (cursor->mJObj != NULL) {
		cursor->CJObj::SetScale(scale, y, scale, force);
	}
}

/**
 * @note Address: 0x800253B8
 * @note Size: 0x190
 */
// Cursor's SetScale(x, y, z)
extern "C" void fn_800253B8(Cursor* cursor, f32 x, f32 y, f32 z, s32 force)
{
	y                            = -y;
	cursor->mMoveScale.x         = x;
	cursor->mMoveScale.y         = y;
	cursor->mMoveScale.z         = z;
	cursor->mMoveScale.mTarget.x = x;
	cursor->mMoveScale.mTarget.y = y;
	cursor->mMoveScale.mTarget.z = z;
	if (cursor->mJObj != NULL) {
		cursor->CJObj::SetScale(x, y, z, force);
	}
}

/**
 * @note Address: 0x80025548
 * @note Size: 0x19C
 */
// Cursor's SetScale(v)
extern "C" void fn_80025548(Cursor* cursor, const Vec& scale, s32 force)
{
	f32 x                        = scale.x;
	f32 y                        = -scale.y;
	f32 z                        = scale.z;
	cursor->mMoveScale.x         = x;
	cursor->mMoveScale.y         = y;
	cursor->mMoveScale.z         = z;
	cursor->mMoveScale.mTarget.x = x;
	cursor->mMoveScale.mTarget.y = y;
	cursor->mMoveScale.mTarget.z = z;
	if (cursor->mJObj != NULL) {
		cursor->CJObj::SetScale(x, y, z, force);
	}
}
