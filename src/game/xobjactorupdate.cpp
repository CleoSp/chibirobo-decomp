#include "game/xobjactor.h"
#include "game/hsd_cobj.h"
#include "game/gmath.h"

// the tu's rodata from lbl_8020B248, the vec literals the resets copy
struct XObjActorLiterals {
	u8 _00[0x48];  // _00
	Vec mReset[4]; // _48
	Vec mInit[4];  // _78
};
STATIC_ASSERT(sizeof(XObjActorLiterals) == 0xA8);

extern "C" {
extern XObjActorLiterals lbl_8020B248;
}

/**
 * @note Address: 0x80107DE0
 * @note Size: 0x138
 */
// vfA4, the update
extern "C" void fn_80107DE0(XObj* object, u32 frames)
{
	XObjActor* actor = (XObjActor*)object;
	actor->vf158();
	actor->vf14C(frames);
	if (actor->_998.mFlags & 4) {
		actor->vf154(frames);
	}
	fn_80090ED4(actor, frames);
	actor->vf148(frames);
	HSD_AObj* aobj = GetAObj(actor->mJObj);
	f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
	actor->vf164(frame);
}

/**
 * @note Address: 0x80107F18
 * @note Size: 0xFC
 */
extern "C" BOOL fn_80107F18(XObjActor* actor, const CoordInfo& info)
{
	if (fn_80091C80(actor, &info)) {
		actor->_980 = 0.0f;
		actor->_984 = 0.0f;
		actor->_988 = 0.0f;
		for (u32 i = 0; i < 20; ++i) {
			actor->mCmd[i] = 0;
		}
		actor->_A4C = -1;
		actor->_A50 = NULL;
		actor->_A84 = NULL;
		actor->_A88 = 0;
		actor->vf150();
		actor->vf15C();
		actor->mLookMode   = 0;
		actor->_AA0        = 0.0f;
		actor->mLookAngle  = 0.0f;
		actor->mLookRate   = 0.0f;
		actor->mLookTarget = 0;
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80108014
 * @note Size: 0x1DC
 */
extern "C" void fn_80108014(XObjActor* actor)
{
	XObjActorLiterals* lit = &lbl_8020B248;
	actor->_980            = 0.0f;
	actor->_984            = 0.0f;
	actor->_988            = 0.0f;
	actor->_98C            = PI;
	actor->_990            = 0;
	actor->_994            = 0;
	actor->_998.mFlags     = 0;
	actor->_9A0            = 0;
	actor->_9A4            = 0;
	actor->_9A8            = 1.0f;
	actor->_9AC            = 1.0f;
	Vec v0                 = lit->mInit[0];
	actor->mHitPos.x       = v0.x;
	actor->mHitPos.y       = v0.y;
	actor->mHitPos.z       = v0.z;
	Vec v1                 = lit->mInit[1];
	actor->_9BC.x          = v1.x;
	actor->_9BC.y          = v1.y;
	actor->_9BC.z          = v1.z;
	Vec v2                 = lit->mInit[2];
	actor->_9C8.x          = v2.x;
	actor->_9C8.y          = v2.y;
	actor->_9C8.z          = v2.z;
	Vec v3                 = lit->mInit[3];
	actor->_A40.x          = v3.x;
	actor->_A40.y          = v3.y;
	actor->_A40.z          = v3.z;
	actor->mHitStrength    = 0.0f;
	actor->_9D8            = 0.0f;
	actor->_9DC            = 0.0f;
	actor->mCmd[0]         = 0;
	actor->mCmd[1]         = 0;
	actor->mCmd[18]        = 0;
	for (u32 i = 0; i < 16; ++i) {
		actor->mCmd[i + 2] = 0;
	}
	actor->_A38 = 0.0f;
	actor->_A3C = 0.0f;
	for (u32 i = 0; i < 8; ++i) {
		actor->mParts[i]._04 = 0;
		actor->mParts[i]._00 = 0;
	}
	actor->mAnimSfx.mEvents = NULL;
	actor->_A4C             = -1;
	actor->_A50             = NULL;
	actor->_A84             = NULL;
	actor->_A88             = 0;
	actor->mLookMode        = 0;
	actor->_AA0             = 0.0f;
	actor->mLookAngle       = 0.0f;
	actor->mLookRate        = 0.0f;
	actor->mLookTarget      = 0;
}

/**
 * @note Address: 0x801081F0
 * @note Size: 0x200
 */
extern "C" void fn_801081F0(XObjActor* actor)
{
	XObjActorLiterals* lit = &lbl_8020B248;
	fn_800928E8(actor);
	actor->_980         = 0.0f;
	actor->_984         = 0.0f;
	actor->_988         = 0.0f;
	actor->_98C         = PI;
	actor->_990         = 0;
	actor->_994         = 0;
	actor->_998.mFlags  = 0;
	actor->_9A0         = 0;
	actor->_9A4         = 0;
	actor->_9A8         = 1.0f;
	actor->_9AC         = 1.0f;
	Vec v0              = lit->mReset[0];
	actor->mHitPos.x    = v0.x;
	actor->mHitPos.y    = v0.y;
	actor->mHitPos.z    = v0.z;
	Vec v1              = lit->mReset[1];
	actor->_9BC.x       = v1.x;
	actor->_9BC.y       = v1.y;
	actor->_9BC.z       = v1.z;
	Vec v2              = lit->mReset[2];
	actor->_9C8.x       = v2.x;
	actor->_9C8.y       = v2.y;
	actor->_9C8.z       = v2.z;
	Vec v3              = lit->mReset[3];
	actor->_A40.x       = v3.x;
	actor->_A40.y       = v3.y;
	actor->_A40.z       = v3.z;
	actor->mHitStrength = 0.0f;
	actor->_9D8         = 0.0f;
	actor->_9DC         = 0.0f;
	actor->mCmd[0]      = 0;
	actor->mCmd[1]      = 0;
	actor->mCmd[18]     = 0;
	for (u32 i = 0; i < 16; ++i) {
		actor->mCmd[i + 2] = 0;
	}
	actor->_A38 = 0.0f;
	actor->_A3C = 0.0f;
	for (u32 i = 0; i < 8; ++i) {
		actor->mParts[i]._04 = 0;
		actor->mParts[i]._00 = 0;
	}
	actor->_A4C        = -1;
	actor->_A50        = NULL;
	actor->_A84        = NULL;
	actor->_A88        = 0;
	actor->mLookMode   = 0;
	actor->_AA0        = 0.0f;
	actor->mLookAngle  = 0.0f;
	actor->mLookRate   = 0.0f;
	actor->mLookTarget = 0;
}

/**
 * @note Address: 0x801083F0
 * @note Size: 0x34
 */
extern "C" void fn_801083F0(XObjActor* actor, u32 id)
{
	fn_800929C4(actor, id);
	actor->mAnimSfx.mEvents = NULL;
}
