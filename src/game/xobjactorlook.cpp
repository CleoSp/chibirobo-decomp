#include "game/xobjactor.h"

extern "C" {
// a zero vec in the tu's rodata
extern Vec lbl_8020B2F0;
extern const f32 lbl_8065E120;
extern const f32 lbl_8065E154;
extern const f32 lbl_8065E158;
extern const f32 lbl_8065E15C;
extern const f32 lbl_8065E160;
}

/**
 * @note Address: 0x8010753C
 * @note Size: 0xC
 */
extern "C" bool vf160__9XObjActorFv(XObjActor* actor)
{
	return (actor->_998.mFlags & 2) != 0;
}

/**
 * @note Address: 0x80107548
 * @note Size: 0xA0
 */
// vf138, knocked from pos
extern "C" void vf138__9XObjActorFRC3Vecf(XObjActor* actor, const Vec* pos, f32 strength)
{
	vf138__4XObjFRC3Vecf(actor, pos, strength);
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
extern "C" s32 vf100__9XObjActorFll(XObjActor* actor, s32 unused, s32 value)
{
	actor->_998.mFlags |= 1;
	return value;
}

/**
 * @note Address: 0x801075FC
 * @note Size: 0x4
 */
extern "C" void vfCC__9XObjActorFv(XObjActor* actor)
{
}

/**
 * @note Address: 0x80107600
 * @note Size: 0x20
 */
extern "C" void vfC8__9XObjActorFv(XObjActor* actor)
{
	vfC8__4XObjFv(actor);
}

/**
 * @note Address: 0x80107620
 * @note Size: 0x70
 */
extern "C" void vfAC__9XObjActorFv(XObjActor* actor)
{
	if (actor->_998.mFlags & 0x80) {
		actor->Push(actor->vf168(), 0x80, actor->mHeadYaw);
	}
	actor->vfB0(NULL);
}

/**
 * @note Address: 0x80107690
 * @note Size: 0x8
 */
// the head's turn limits
extern "C" f32 vf17C__9XObjActorFv(XObjActor* actor)
{
	return lbl_8065E154;
}

/**
 * @note Address: 0x80107698
 * @note Size: 0x8
 */
extern "C" f32 vf178__9XObjActorFv(XObjActor* actor)
{
	return lbl_8065E158;
}

/**
 * @note Address: 0x801076A0
 * @note Size: 0x8
 */
extern "C" f32 vf174__9XObjActorFv(XObjActor* actor)
{
	return lbl_8065E15C;
}

/**
 * @note Address: 0x801076A8
 * @note Size: 0x8
 */
extern "C" f32 vf170__9XObjActorFv(XObjActor* actor)
{
	return lbl_8065E160;
}

/**
 * @note Address: 0x801076B0
 * @note Size: 0x2C
 */
// look at a Vec*
extern "C" void SetActorLookAtPosition(CXObj* object, const Vec* target, f32 rate)
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
extern "C" void SetActorLookAngle(CXObj* object, f32 angle, f32 rate)
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
extern "C" void SetActorLookTarget(CXObj* object, const Vec* target, f32 rate)
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
extern "C" void ResetActorLook(CXObj* object, f32 rate)
{
	XObjActor* actor = (XObjActor*)object;
	actor->_998.mFlags |= 0x80;
	actor->mLookMode  = 0;
	actor->mLookRate  = rate;
	actor->mLookAngle = lbl_8065E120;
	actor->_998.mFlags &= ~0x400;
	actor->mHeadPitch = lbl_8065E120;
}
