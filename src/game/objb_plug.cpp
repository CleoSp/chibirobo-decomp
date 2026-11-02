#include "game/xchr.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/texturecopy.h"
#include "game/ccamera.h"
#include "game/cparticle.h"
#include "game/stagedata.h"

// XObjWireEff, the sparks along the cord, and XObjPlug, the plug chibi robo pulls around

static inline void ClearAnim(XObjWireEff* object)
{
	object->mStatus.mFlags &= ~0x10000ULL;
	object->mAnim   = -1;
	object->mEffect = -1;
}

static inline void KillEffect(XObjWireEff* object)
{
	if (object->mGenerator != NULL) {
		(&lbl_8025C400.mParticle)->Kill(object->mGenerator);
		object->mGenerator = NULL;
	}
}

extern "C" {

/**
 * @note Address: 0x800D7D3C
 * @note Size: 0x7C
 */
void fn_800D7D3C(XObjWireEff* object)
{
	ClearAnim(object);
	KillEffect(object);
}

/**
 * @note Address: 0x800D7DB8
 * @note Size: 0x16C
 */
void fn_800D7DB8(XObjWireEff* object, s32 anim, s32 effect, f32 rate, u32 color)
{
	object->mAnim   = anim;
	object->mEffect = effect;
	object->mRate   = rate;
	if (object->mAnim >= 0) {
		object->mStatus.mFlags |= 0x10000ULL;
		object->SetAnim(object->mAnim, FALSE, NULL, NULL, NULL, 0.0f, object->mRate, 0.0f, 10.0f);
	} else {
		object->mStatus.mFlags &= ~0x10000ULL;
	}
	if (object->mEffect >= 0) {
		KillEffect(object);
		object->mGenerator = (&lbl_8025C400.mParticle)->Create(0x10052, 0, object->mJObj, FALSE);
	} else {
		KillEffect(object);
	}
	object->vf12C(0, color >> 24, color >> 16, color >> 8, 0.0f);
}

// XObjWireEff::vfB0
/**
 * @note Address: 0x800D7F24
 * @note Size: 0x60
 */
void fn_800D7F24(XObjWireEff* object, Mtx view)
{
	if ((object->mStatus.mFlags & 1) && (object->mStatus.mFlags & 0x10000)) {
		fn_800908C0(object, view);
	}
}

// XObjWireEff::vfA4
/**
 * @note Address: 0x800D7F84
 * @note Size: 0x60
 */
void fn_800D7F84(XObjWireEff* object, u32 frames)
{
	if ((object->mStatus.mFlags & 1) && (object->mAnim >= 0 || object->mEffect >= 0)) {
		fn_80090ED4(object, frames);
	}
}

// XObjWireEff::Release
/**
 * @note Address: 0x800D7FE4
 * @note Size: 0x84
 */
void fn_800D7FE4(XObjWireEff* object)
{
	ClearAnim(object);
	KillEffect(object);
	fn_800911C0(object);
}

// XObjWireEff::vf98
/**
 * @note Address: 0x800D8068
 * @note Size: 0x60
 */
void fn_800D8068(XObjWireEff* object)
{
	fn_800928E8(object);
	ClearAnim(object);
	object->mGenerator = NULL;
}

/**
 * @note Address: 0x800D80C8
 * @note Size: 0xC
 */
void fn_800D80C8(XObjPlug* object)
{
	object->mWire._990 = -1;
}

/**
 * @note Address: 0x800D80D4
 * @note Size: 0x10
 */
void fn_800D80D4(XObjPlug* object, s32 value)
{
	object->mWire._990 = value;
	object->mWire._994 = -1;
}

// XObjPlug::vfDC
/**
 * @note Address: 0x800D80E4
 * @note Size: 0x54
 */
void fn_800D80E4(XObjPlug* object, BOOL enabled, u8 value)
{
	fn_80091834(object, enabled, value);
	fn_800DB7A8(&object->mCord, enabled, value);
}

// XObjPlug::vfC0
/**
 * @note Address: 0x800D8138
 * @note Size: 0x4C
 */
void fn_800D8138(XObjPlug* object)
{
	if (object->mJObj != NULL) {
		fn_800DA4D0(&object->mCord, lbl_8025C400.mGs.mCamera->mViewMtx, 1.0f, 1.0f, 1.0f);
	}
}

// XObjPlug::vfB8, the cord's shadow
/**
 * @note Address: 0x800D8184
 * @note Size: 0x1A8
 */
void fn_800D8184(XObjPlug* object)
{
	object->mCord.mColor = 0x14141440;
	object->mCord._60    = 0x14141440;
	object->mCord.mWidth = 0.15f;
	if (object->mJObj != NULL) {
		CGs* gs = &lbl_8025C400.mGs;
		FlushCaptureGraphics(gs);
		fn_800DA68C(&object->mCord, NULL, 1.0f, 1.0f, 1.0f);
		InvalidateCaptureGraphics(gs);
	}
	object->mCord.mColor = 0x141414FF;
	object->mCord._60    = 0x141414FF;
	object->mCord.mWidth = 0.2f;
}

// XObjPlug::vfB4
/**
 * @note Address: 0x800D832C
 * @note Size: 0x2C
 */
void fn_800D832C(XObjPlug* object, Mtx view)
{
	object->vfB0(view);
}

// XObjPlug::vfB0
/**
 * @note Address: 0x800D8358
 * @note Size: 0x74
 */
void fn_800D8358(XObjPlug* object, Mtx view)
{
	fn_800908C0(object, view);
	object->mWire.vfB0(view);
	if (object->mJObj != NULL) {
		fn_800DA68C(&object->mCord, view, 1.0f, 1.0f, 1.0f);
	}
}

/**
 * @note Address: 0x800D9068
 * @note Size: 0x34
 */
CVec fn_800D9068(const XObjPlugCord* cord, s32 index)
{
	return cord->mPoints[index].mPos;
}

} // extern "C"
