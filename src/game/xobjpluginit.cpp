#include "game/xobjattachment.h"

// XObjPlug::vf94
/**
 * @note Address: 0x800D9A54
 * @note Size: 0x28
 */
extern "C" void fn_800D9A54(XObjPlug* object)
{
	object->mStatus.mFlags &= ~0x0400000000000000ULL;
}

/**
 * @note Address: 0x800D9A7C
 * @note Size: 0x8
 */
extern "C" void fn_800D9A7C(XObjPlug* object, f32 speed)
{
	object->mCord.mSpeed = speed;
}

// XObjPlug::vf90
/**
 * @note Address: 0x800D9A84
 * @note Size: 0x8C
 */
extern "C" void fn_800D9A84(XObjPlug* object, u32 id)
{
	fn_800929C4(object, id);
	fn_800DBA7C(&object->mCord, 20, 0.0f, 0.0f, 0.0f, 0.5f);
	object->mCord.mColor = 0x141414FF;
	object->mCord._60    = 0x141414FF;
	object->mCord.mWidth = 0.2f;
	object->mLoadState   = 0;
	object->mWire.vf90(id);
}
