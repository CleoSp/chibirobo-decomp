#include "game/xchr.h"
#include "game/cgame.h"
#include "game/cparticle.h"
#include "game/cgs.h"
#include "game/texturecopy.h"

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
		(&gGame.mParticle)->Kill(object->mGenerator);
		object->mGenerator = NULL;
	}
}

extern "C" {

/**
 * @note Address: 0x800D7D3C
 * @note Size: 0x7C
 */
void ClearWireEffect(XObjWireEff* object)
{
	ClearAnim(object);
	KillEffect(object);
}

/**
 * @note Address: 0x800D7DB8
 * @note Size: 0x16C
 */
void ConfigurePlugWireEffect(XObjWireEff* object, s32 anim, s32 effect, f32 rate, u32 color)
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
		object->mGenerator = (&gGame.mParticle)->Create(0x10052, 0, object->mJObj, FALSE);
	} else {
		KillEffect(object);
	}
	object->vf12C(0, color >> 24, color >> 16, color >> 8, 0.0f);
}

// XObjWireEff::DrawWithView
/**
 * @note Address: 0x800D7F24
 * @note Size: 0x60
 */
void DrawWithView__11XObjWireEffFPA4_f(XObjWireEff* object, Mtx view)
{
	if ((object->mStatus.mFlags & 1) && (object->mStatus.mFlags & 0x10000)) {
		DrawWithView__4XObjFPA4_f(object, view);
	}
}

// XObjWireEff::UpdateObject
/**
 * @note Address: 0x800D7F84
 * @note Size: 0x60
 */
void UpdateObject__11XObjWireEffFUl(XObjWireEff* object, u32 frames)
{
	if ((object->mStatus.mFlags & 1) && (object->mAnim >= 0 || object->mEffect >= 0)) {
		UpdateObject__4XObjFUl(object, frames);
	}
}

// XObjWireEff::Release
/**
 * @note Address: 0x800D7FE4
 * @note Size: 0x84
 */
void Release__11XObjWireEffFv(XObjWireEff* object)
{
	ClearAnim(object);
	KillEffect(object);
	Release__4XObjFv(object);
}

// XObjWireEff::ResetObjectState
/**
 * @note Address: 0x800D8068
 * @note Size: 0x60
 */
void ResetObjectState__11XObjWireEffFv(XObjWireEff* object)
{
	ResetObjectState__4XObjFv(object);
	ClearAnim(object);
	object->mGenerator = NULL;
}

/**
 * @note Address: 0x800D80C8
 * @note Size: 0xC
 */
void DisablePlugSparkEffect(XObjPlug* object)
{
	object->mWire.mSparkMode = -1;
}

/**
 * @note Address: 0x800D80D4
 * @note Size: 0x10
 */
void SetPlugSparkMode(XObjPlug* object, s32 value)
{
	object->mWire.mSparkMode  = value;
	object->mWire.mSparkTimer = -1;
}

// XObjPlug::vfDC
/**
 * @note Address: 0x800D80E4
 * @note Size: 0x54
 */
void vfDC__8XObjPlugFiUc(XObjPlug* object, BOOL enabled, u8 value)
{
	vfDC__4XObjFiUc(object, enabled, value);
	SetPlugCordAlphaMode(&object->mCord, enabled, value);
}

// XObjPlug::DrawObjectEffects
/**
 * @note Address: 0x800D8138
 * @note Size: 0x4C
 */
void DrawObjectEffects__8XObjPlugFv(XObjPlug* object)
{
	if (object->mJObj != NULL) {
		DrawPlugCordNodes(&object->mCord, gGame.mGs.mCamera->mViewMtx, 1.0f, 1.0f, 1.0f);
	}
}

// XObjPlug::DrawGroundShadow, the cord's shadow
/**
 * @note Address: 0x800D8184
 * @note Size: 0x1A8
 */
void DrawGroundShadow__8XObjPlugFv(XObjPlug* object)
{
	object->mCord.mColor = 0x14141440;
	object->mCord._60    = 0x14141440;
	object->mCord.mWidth = 0.15f;
	if (object->mJObj != NULL) {
		CGs* gs = &gGame.mGs;
		FlushCaptureGraphics(gs);
		DrawPlugCordSegments(&object->mCord, NULL, 1.0f, 1.0f, 1.0f);
		InvalidateCaptureGraphics(gs);
	}
	object->mCord.mColor = 0x141414FF;
	object->mCord._60    = 0x141414FF;
	object->mCord.mWidth = 0.2f;
}

// XObjPlug::DrawReflection
/**
 * @note Address: 0x800D832C
 * @note Size: 0x2C
 */
void DrawReflection__8XObjPlugFPA4_f(XObjPlug* object, Mtx view)
{
	object->DrawWithView(view);
}

// XObjPlug::DrawWithView
/**
 * @note Address: 0x800D8358
 * @note Size: 0x74
 */
void DrawWithView__8XObjPlugFPA4_f(XObjPlug* object, Mtx view)
{
	DrawWithView__4XObjFPA4_f(object, view);
	object->mWire.DrawWithView(view);
	if (object->mJObj != NULL) {
		DrawPlugCordSegments(&object->mCord, view, 1.0f, 1.0f, 1.0f);
	}
}

} // extern "C"
