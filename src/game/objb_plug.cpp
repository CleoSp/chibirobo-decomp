#include "game/xchr.h"
#include "game/cgame.h"
#include "game/gmath.h"
#include "game/csound.h"
#include "game/cgs.h"
#include "game/texturecopy.h"
#include "game/ccamera.h"
#include "game/cparticle.h"
#include "game/stagedata.h"
#include "game/cjobjinlineattach.h"
extern "C" {
#include <sysdolphin/mtx.h>
#include <sysdolphin/random.h>
}

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

// XObjWireEff::vfB0
/**
 * @note Address: 0x800D7F24
 * @note Size: 0x60
 */
void vfB0__11XObjWireEffFPA4_f(XObjWireEff* object, Mtx view)
{
	if ((object->mStatus.mFlags & 1) && (object->mStatus.mFlags & 0x10000)) {
		vfB0__4XObjFPA4_f(object, view);
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
void vfDC__8XObjPlugFiUc(XObjPlug* object, BOOL enabled, u8 value)
{
	vfDC__4XObjFiUc(object, enabled, value);
	SetPlugCordAlphaMode(&object->mCord, enabled, value);
}

// XObjPlug::vfC0
/**
 * @note Address: 0x800D8138
 * @note Size: 0x4C
 */
void vfC0__8XObjPlugFv(XObjPlug* object)
{
	if (object->mJObj != NULL) {
		DrawPlugCordNodes(&object->mCord, gGame.mGs.mCamera->mViewMtx, 1.0f, 1.0f, 1.0f);
	}
}

// XObjPlug::vfB8, the cord's shadow
/**
 * @note Address: 0x800D8184
 * @note Size: 0x1A8
 */
void vfB8__8XObjPlugFv(XObjPlug* object)
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

// XObjPlug::vfB4
/**
 * @note Address: 0x800D832C
 * @note Size: 0x2C
 */
void vfB4__8XObjPlugFPA4_f(XObjPlug* object, Mtx view)
{
	object->vfB0(view);
}

// XObjPlug::vfB0
/**
 * @note Address: 0x800D8358
 * @note Size: 0x74
 */
void vfB0__8XObjPlugFPA4_f(XObjPlug* object, Mtx view)
{
	vfB0__4XObjFPA4_f(object, view);
	object->mWire.vfB0(view);
	if (object->mJObj != NULL) {
		DrawPlugCordSegments(&object->mCord, view, 1.0f, 1.0f, 1.0f);
	}
}

static inline void AttachPlugCord(XObjPlug* plug)
{
	if (plug->_D38 == 0) {
		SetPlugCordEndpointSource(&plug->mCord, 1, &plug->mCordJointPos);
		SetPlugCordEndpointSource(&plug->mCord, 2, &plug->mCordJointTip);
		plug->_D38 = 1;
	}
}

static inline void UpdateCordJoint(XObjPlug* plug, CJObj& joint)
{
	Mtx matrix;
	PSMTXCopy(joint.GetMtx(), matrix);
	plug->mCordJointPos = CVec(matrix[0][3], matrix[1][3], matrix[2][3]);
	CVec tip(0, 0, 5);
	PSMTXMultVec(matrix, &tip, &tip);
	plug->mCordJointTip = tip;
}

#pragma push
#pragma pool_data off
void UpdateObject__8XObjPlugFUl(XObjPlug* plug, u32 frames)
{
	UpdateObject__4XObjFUl(plug, frames);
	if (plug->mStatus.mFlags & 1) {
		fn_800D909C(plug, &plug->mCordAnchor, NULL, NULL);
		if (plug->mStatus.mFlags & 0x0400000000000000ULL) {
			AttachPlugCord(plug);
			s32 path[] = { 0, 0 };
			CJObj joint(FindJointByPath(plug->mJObj, path, 2));
			UpdateCordJoint(plug, joint);
		} else if (plug->_D3C != 0) {
			AttachPlugCord(plug);
			s32 path[] = { 0, 0 };
			CJObj joint(FindJointByPath(plug->mJObj, path, 2));
			UpdateCordJoint(plug, joint);
		} else if (plug->_D38 == 1) {
			SetPlugCordEndpointSource(&plug->mCord, 1, NULL);
			SetPlugCordEndpointSource(&plug->mCord, 2, NULL);
			plug->_D38 = 0;
		}
		const Vec& tint    = plug->mColorStep[1].mValue;
		plug->mCord.mColor = plug->mCord._60 = ((u32)tint.x << 24) | ((u32)tint.y << 16) | ((u32)tint.z << 8) | 0xff;
		UpdatePlugCordSimulation(&plug->mCord, frames);
	} else {
		plug->ResetObjectState();
	}
	plug->mWire.UpdateObject(frames);
	if ((plug->mStatus.mFlags & 1) && !(plug->mStatus.mFlags & 0x0400000000000000ULL) && plug->_D3C == 0) {
		Vec pos = GetPlugCordPointPosition(&plug->mCord, 18);
		plug->SetPos(pos, 0);
		CVec tip, base, rot;
		tip    = GetPlugCordPointPosition(&plug->mCord, 19);
		base   = GetPlugCordPointPosition(&plug->mCord, 18);
		rot.y  = atan2(tip.x - base.x, tip.z - base.z);
		f32 dx = tip.x - base.x, dy = tip.y - base.y, dz = tip.z - base.z;
		f32 pitch = atan2(sqrtf(dx * dx + dz * dz), sqrtf(dy * dy));
		rot.x     = dy < 0 ? HALF_PI - pitch : pitch - HALF_PI;
		rot.z     = 0;
		plug->SetRot(rot, 0);
		Mtx matrix;
		PSMTXCopy(HSD_JObjGetMtxPtr(plug->mJObj), matrix);
		Vec offset = { 0, -0.2f, 0 };
		PSMTXMultVec(matrix, &offset, &offset);
		plug->SetPos(offset, 0);
		if (plug->mWire._990 == 0) {
			if (plug->mWire._994 < 0) {
				CVec pos(plug->mMoveTranslate);
				CVec scale(0.5f, 0.5f, 0.5f);
				Vec rotation = { 0, 0, 0 };
				gGame.mParticle.CreateSRT(0x1001a, 1, pos, scale, rotation, FALSE);
				plug->mWire._994 += HSD_Randi(10) + 16;
			}
			XSound* sound = &gGame.mSound;
			if (fn_8001E6C8(sound, 0xfb) != 1 && fn_8001E6C8(sound, 0xfc) != 1) {
				if (plug->mWire._994 % 2)
					fn_8001E904(sound, 0xfb, 255);
				else
					fn_8001E904(sound, 0xfc, 255);
			}
			plug->mWire._994 -= frames;
		}
	}
}
#pragma pop

/**
 * @note Address: 0x800D9068
 * @note Size: 0x34
 */
CVec GetPlugCordPointPosition(const XObjPlugCord* cord, s32 index)
{
	return cord->mPoints[index].mPos;
}

/**
 * @note Address: 0x800D909C
 * @note Size: 0x32C
 */
// NONMATCHING: temporary-vector lifetime and stack layout.
void fn_800D909C(void*, Vec* position, Vec* rotation, Vec* scale)
{
	s32 path[] = { 0, 1, 0, 1, 1 };
	CJObj joint(GetJointByPath(gGame.GetPlayer()->mJObj, path, 5));
	Mtx matrix;
	if (joint.mJObj != NULL) {
		PSMTXCopy(joint.GetMtx(), matrix);
	}

	static CVec offset(0.0f, 0.0f, -0.4f);
	if (position != NULL) {
		PSMTXMultVec(matrix, &offset, position);
	}
	if (rotation != NULL) {
		HSD_MtxGetRotation(matrix, rotation);
	}
	if (scale != NULL) {
		HSD_MtxGetScale(matrix, scale);
	}
}

} // extern "C"
