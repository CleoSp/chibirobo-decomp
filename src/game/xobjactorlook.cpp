#include "game/xobjactor.h"
#include "game/gmath.h"

extern "C" {
// a zero vec in the tu's rodata
extern Vec lbl_8020B2F0;
}

/**
 * @note Address: 0x8010753C
 * @note Size: 0xC
 */
extern "C" bool fn_8010753C(XObjActor* actor)
{
	return (actor->_998.mFlags & 2) != 0;
}

/**
 * @note Address: 0x80107548
 * @note Size: 0xA0
 */
// vf138, knocked from pos
extern "C" void fn_80107548(XObjActor* actor, const Vec* pos, f32 strength)
{
	fn_8008A6C0(actor, strength);
	actor->_998.mFlags |= 2;
	f32 x               = pos->x;
	f32 y               = pos->y;
	f32 z               = pos->z;
	Vec hit             = lbl_8020B2F0;
	hit.x               = x;
	hit.y               = y;
	hit.z               = z;
	actor->mHitPos      = hit;
	actor->mHitStrength = strength;
}

/**
 * @note Address: 0x801075E8
 * @note Size: 0x14
 */
extern "C" s32 fn_801075E8(XObjActor* actor, s32 unused, s32 value)
{
	actor->_998.mFlags |= 1;
	return value;
}

/**
 * @note Address: 0x801075FC
 * @note Size: 0x4
 */
extern "C" void fn_801075FC(XObjActor* actor)
{
}

/**
 * @note Address: 0x80107600
 * @note Size: 0x20
 */
extern "C" void fn_80107600(XObjActor* actor)
{
	fn_8009047C(actor);
}

/**
 * @note Address: 0x80107620
 * @note Size: 0x70
 */
extern "C" void fn_80107620(XObjActor* actor)
{
	if (actor->_998.mFlags & 0x80) {
		actor->Push(actor->vf168(), 0x80, actor->_AA0);
	}
	actor->vfB0(NULL);
}

/**
 * @note Address: 0x80107690
 * @note Size: 0x8
 */
// the head's turn limits
extern "C" f32 fn_80107690(XObjActor* actor)
{
	return -20.0f * DEG2RAD;
}

/**
 * @note Address: 0x80107698
 * @note Size: 0x8
 */
extern "C" f32 fn_80107698(XObjActor* actor)
{
	return 20.0f * DEG2RAD;
}

/**
 * @note Address: 0x801076A0
 * @note Size: 0x8
 */
extern "C" f32 fn_801076A0(XObjActor* actor)
{
	return -80.0f * DEG2RAD;
}

/**
 * @note Address: 0x801076A8
 * @note Size: 0x8
 */
extern "C" f32 fn_801076A8(XObjActor* actor)
{
	return 80.0f * DEG2RAD;
}

/**
 * @note Address: 0x801076B0
 * @note Size: 0x2C
 */
// look at a Vec*
extern "C" void fn_801076B0(CXObj* object, s32 target, f32 rate)
{
	XObjActor* actor = (XObjActor*)object;
	actor->_998.mFlags |= 0x80;
	actor->mLookMode   = 1;
	actor->mLookRate   = rate;
	actor->mLookTarget = target;
	actor->_998.mFlags |= 0x400;
}

/**
 * @note Address: 0x801076DC
 * @note Size: 0x20
 */
extern "C" void fn_801076DC(CXObj* object, f32 angle, f32 rate)
{
	XObjActor* actor = (XObjActor*)object;
	actor->_998.mFlags |= 0x80;
	actor->mLookMode  = 3;
	actor->mLookAngle = angle;
	actor->mLookRate  = rate;
}

/**
 * @note Address: 0x801076FC
 * @note Size: 0x20
 */
extern "C" void fn_801076FC(CXObj* object, s32 target, f32 rate)
{
	XObjActor* actor = (XObjActor*)object;
	actor->_998.mFlags |= 0x80;
	actor->mLookMode   = 1;
	actor->mLookRate   = rate;
	actor->mLookTarget = target;
}

/**
 * @note Address: 0x8010771C
 * @note Size: 0x34
 */
// look ahead again
extern "C" void fn_8010771C(CXObj* object, f32 rate)
{
	XObjActor* actor = (XObjActor*)object;
	actor->_998.mFlags |= 0x80;
	actor->mLookMode  = 0;
	actor->mLookRate  = rate;
	actor->mLookAngle = 0.0f;
	actor->_998.mFlags &= ~0x400;
	actor->_AB0 = 0.0f;
}
