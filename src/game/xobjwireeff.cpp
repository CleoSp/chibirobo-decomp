#include "game/xobjattachment.h"
#include "game/cgame.h"
#include "game/cparticle.h"

// todo: cleanup
static inline CParticle* Particles()
{
	return (CParticle*)lbl_8025C400.mParticle;
}

// todo: cleanup
static inline void ClearAnimationState(XObjWireEff* object)
{
	object->mStatus.mFlags &= ~0x0000000000010000ULL;
	object->mAnim   = -1;
	object->mEffect = -1;
}

// todo: cleanup
static inline void KillEffect(XObjWireEff* object)
{
	HSD_Generator* effect = object->mGenerator;
	if (effect != NULL) {
		Particles()->Kill(effect);
		object->mGenerator = NULL;
	}
}

/**
 * @note Address: 0x800D7D3C
 * @note Size: 0x7C
 */
extern "C" void fn_800D7D3C(XObjWireEff* object)
{
	ClearAnimationState(object);
	KillEffect(object);
}

/**
 * @note Address: 0x800D7DB8
 * @note Size: 0x16C
 */
extern "C" void fn_800D7DB8(XObjWireEff* object, s32 animation, s32 effect, u32 color, f32 rate)
{
	object->mAnim   = animation;
	object->mEffect = effect;
	object->mRate   = rate;
	if (object->mAnim >= 0) {
		object->mStatus.mFlags |= 0x0000000000010000ULL;
		object->SetAnim(object->mAnim, FALSE, NULL, NULL, NULL, 0.0f, object->mRate, 0.0f, 10.0f);
	} else {
		object->mStatus.mFlags &= ~0x0000000000010000ULL;
	}
	if (object->mEffect >= 0) {
		KillEffect(object);
		object->mGenerator = Particles()->Create(0x10052, 0, object->mJObj, FALSE);
	} else {
		KillEffect(object);
	}
	object->vf12C(0, color >> 24, (color >> 16) & 0xFF, (color >> 8) & 0xFF, 0.0f);
}

// XObjWireEff::vfB0
/**
 * @note Address: 0x800D7F24
 * @note Size: 0x60
 */
extern "C" void fn_800D7F24(XObjWireEff* object, Mtx view)
{
	u64 flags = object->mStatus.mFlags;
	if ((flags & 1) != 0 && (flags & 0x10000) != 0)
		fn_800908C0(object, view);
}

// XObjWireEff::vfA4
/**
 * @note Address: 0x800D7F84
 * @note Size: 0x60
 */
extern "C" void fn_800D7F84(XObjWireEff* object, u32 frames)
{
	if ((object->mStatus.mFlags & 1) != 0 && (object->mAnim >= 0 || object->mEffect >= 0)) {
		fn_80090ED4(object, frames);
	}
}

// XObjWireEff::Release
/**
 * @note Address: 0x800D7FE4
 * @note Size: 0x84
 */
extern "C" void fn_800D7FE4(XObjWireEff* object)
{
	ClearAnimationState(object);
	KillEffect(object);
	fn_800911C0(object);
}

// XObjWireEff::vf98
/**
 * @note Address: 0x800D8068
 * @note Size: 0x60
 */
extern "C" void fn_800D8068(XObjWireEff* object)
{
	fn_800928E8(object);
	ClearAnimationState(object);
	object->mGenerator = NULL;
}
