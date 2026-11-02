#include "game/xobjleticker.h"
#include "game/xchr.h"
#include "game/ccameraaction.h"
#include "game/cmotioncontrolcamera.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/cpad.h"
#include "game/cparticle.h"
#include "game/gmath.h"
#include <math.h>
extern "C" {
#include <dolphin/mtx/mtx.h>
#include <dolphin/mtx/vec.h>
#include <dolphin/os.h>
}
#include <sysdolphin/controller.h>

// XObjLeticker, and the camera that turns around it in steps, CCameraActionLeticker

extern "C" {
void PSMTXMultVecArray(const Mtx m, const Vec* src, Vec* dst, u32 count);
BOOL HSD_AudioSFXSetPitchByID(s32 sfx, s16 pitch);
bool fn_800E4790(XObj* obj, HSD_JObj* jobj);
extern XObjFloor* lbl_8022A008[];
extern XObjFloor* lbl_8065A6D0[2];
}

static inline f32 GetAnimFrame(HSD_JObj* jobj)
{
	HSD_AObj* aobj = GetAObj(jobj);
	return aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
}

static inline f32 GetAnimEndFrame(HSD_JObj* jobj)
{
	HSD_AObj* aobj = GetAObj(jobj);
	return aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
}

// sets the object's floor boxes, they take effect at once
static inline void SetFloors(XObj* obj, XObjFloor* floors)
{
	obj->mFloors = floors;
	obj->_250[1] = 0;
	obj->_250[0] = 0;
	fn_8008D220(obj);
	fn_8008C8A4(obj);
}

// puts an anim in the first free slot of the object's queue
static inline void PushAnim(XObj* obj, s32 id, u16 frame, s16 rate)
{
	for (int i = 0; i < 16; i++) {
		s32 j = (obj->mAnmQueue.mNext + i) % 16;
		if (obj->mAnmQueue.mEntry[j].mId == -1) {
			obj->mAnmQueue.mEntry[j].mId    = id;
			obj->mAnmQueue.mEntry[j].mFrame = frame;
			obj->mAnmQueue.mEntry[j].mRate  = rate;
			return;
		}
	}
}

static inline f32 Cos(f32 x)
{
	return cos(x);
}

static inline f32 Sin(f32 x)
{
	return sin(x);
}

// which quarter an angle faces, 0 front, 1 left, 2 back, 3 right
static inline s32 GetSide(f32 angle)
{
	s32 side;
	if (Cos(0.7853982f) <= Cos(angle)) {
		side = 0;
	} else if (Cos(angle) <= Cos(3.9269907f)) {
		side = 2;
	} else if (0.0f < Sin(angle)) {
		side = 1;
	} else {
		side = 3;
	}
	return side;
}

// whether the eye at that angle stays inside the room
static inline bool CanTurn(CCameraActionLeticker* camera, f32 angle)
{
	if (!camera->mBounded) {
		return true;
	}
	Vec eye;
	camera->CalcEye(&eye, angle);
#if !defined(VERSION_PAL)
	OSReport("%4.2f %4.2f\n", eye.x, eye.z);
#endif
	if (eye.x < camera->mMinX || camera->mMaxX < eye.x || eye.z < camera->mMinZ || camera->mMaxZ < eye.z) {
#if !defined(VERSION_PAL)
		OSReport("CCameraActionLeticker::Update %4.2f %4.2f %d\n", eye.x, eye.z, (s32)(57.295776f * camera->mTurnTo));
#endif
		return false;
	}
	return true;
}

static AnimSfxEvent sSfx[] = {
	{ 7, 10.0f, -1.0f, 1, 0x190, 0xFF, 0, 0, 0 },
	{ -1 },
};

extern "C" {

// turns the first linked object to the angle, then puts _A04 at the top of its stack
/**
 * @note Address: 0x800E260C
 * @note Size: 0x3AC
 */
// NONMATCHING: regalloc
BOOL fn_800E260C(XObjLeticker* leticker, f32 angle)
{
	XObj* part = lbl_8025C400.GetObject(leticker->mLinked[0]);
	f32 my     = HSD_JObjGetRotationY(leticker->mJObj);
	f32 c      = cos(HSD_JObjGetRotationY(part->mJObj) - (angle + my));
	if (c < (f32)cos(0.08726646f)) {
		leticker->_9F8 = 0.0f;
		leticker->_A30 = 0;
		if (part->mAnimId == 0) {
			part->SetRotateY(angle + HSD_JObjGetRotationY(leticker->mJObj), 0.1f);
			leticker->mHeading = angle;
		}
	}
	if (GetAnimFrame(part->mJObj) < GetAnimEndFrame(part->mJObj)) {
		return TRUE;
	}
	s32 path[9]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	HSD_JObj* jobj = FindJointByPath(part->mJObj, path, leticker->_99C + 1);
	if (jobj != NULL) {
		MtxPtr mtx = HSD_JObjGetMtxPtr(jobj);
		Vec pos;
		pos.x = pos.y = pos.z = 0.0f;
		PSMTXMultVec(mtx, &pos, &leticker->_A04);
	}
	return TRUE;
}

/**
 * @note Address: 0x800E2EAC
 * @note Size: 0x1A4
 */
void fn_800E2EAC(XObjLeticker* obj, Vec* dir)
{
	XObj* part;
	for (s32 i = 0; i < obj->mLinkedCount; i++) {
		part = lbl_8025C400.GetObject(obj->mLinked[i]);
		part->SetTranslateX(obj->mMoveTranslate.x, 0);
		part->SetTranslateZ(obj->mMoveTranslate.z, 0);
	}
	if (0.0f < PSVECMag(dir)) {
		obj->vf12C(1, 0, 0, 0, 0.5f);
		obj->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	} else {
		obj->vf12C(1, 0xFF, 0, 0, 1.0f);
		obj->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
	if (obj->mPtcl) {
		(&lbl_8025C400.mParticle)->Kill(0xA5);
		obj->mPtcl = FALSE;
	}
}

} // extern "C"

/**
 * @note Address: 0x800E3050
 * @note Size: 0x1DC
 */
void XObjLeticker::vf148()
{
	if (mState != 3) {
		lbl_8025C400.mStatus.mFlags &= ~0x80ULL;
		lbl_8025C400.mMotionControlCamera.PopAction();
	}
	_A2D = FALSE;
	_A30 = 0;
	if (mState == 2) {
		_9A0 = _9A4[GetSide(mHeading)];
		_9FC = 10.0f * _9A0;
		_9F8 = 10.0f * _99C;
	}
	vf12C(1, 0, 0, 0, 0.5f);
	SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800E3AF0
 * @note Size: 0x4
 */
void XObjLeticker::vf138(const Vec& pos, f32 f)
{
}

/**
 * @note Address: 0x800E54E0
 * @note Size: 0x1A8
 */
void XObjLeticker::StateRide()
{
	XObj* player   = lbl_8025C400.GetPlayer();
	s32 path[2]    = { 0, 9 };
	HSD_JObj* jobj = FindJointByPath(mJObj, path, 2);
	switch (mStep) {
	case 0:
		break;
	case 10:
		Vec pos;
		pos.x = pos.y = pos.z = 0.0f;
		pos.y                 = 0.5f;
		PSMTXMultVec(HSD_JObjGetMtxPtr(jobj), &pos, &pos);
		player->SetPos(pos, 0);
		player->SetRotationY(player->mMoveRotate.y - 0.5235988f, 0);
		break;
	}
}

extern "C" {

// whether the spot under the joint is free, nothing near it in the way
/**
 * @note Address: 0x800E4790
 * @note Size: 0x31C
 */
bool fn_800E4790(XObj* obj, HSD_JObj* jobj)
{
	CVec pos;
	if (jobj != NULL) {
		MtxPtr mtx = HSD_JObjGetMtxPtr(jobj);
		pos.x = pos.y = pos.z = 0.0f;
		PSMTXMultVec(mtx, &pos, &pos);
	}
	pos.y -= 6.0f;
	for (s32 i = 0; i < lbl_8025C400.mXObjs.mObjectCount; ++i) {
		XObj* target = lbl_8025C400.GetObject(i);
		if (!(target->mStatus.mFlags & 1ULL) || !(target->mStatus.mFlags & 0x10000ULL)) {
			continue;
		}
		if ((s32)obj->mId == (s32)target->mId) {
			continue;
		}
		if ((s16)target->GetPool() == 2) {
			continue;
		}
		f32 x    = target->mMoveTranslate.x - pos.x;
		f32 z    = target->mMoveTranslate.z - pos.z;
		f32 dist = sqrtf(x * x + z * z);
		if (dist > 100.0f + (5.0f + target->_6F8)) {
			continue;
		}
		CVec hit, normal;
		if (target->vfFC(&pos, &pos, 5.0f, 12.0f, &hit, &normal, 0, 0) & 2) {
			return false;
		}
	}
	return true;
}

/**
 * @note Address: 0x800E4AAC
 * @note Size: 0x3AC
 */
s32 fn_800E4AAC(XObj* obj, const Vec* pos)
{
	f32 angle = WrapAngle((f32)atan2(pos->x - obj->mMoveTranslate.x, pos->z - obj->mMoveTranslate.z) - obj->mMoveRotate.y);
	if (3.0f + obj->mMoveTranslate.y < lbl_8025C400.GetPlayer()->mMoveTranslate.y) {
		return 0;
	}
	if (Cos(0.7853982f) < Cos(angle)) {
		s32 path[2] = { 0, 2 };
		if (fn_800E4790(obj, FindJointByPath(obj->mJObj, path, 2))) {
			return -1;
		}
	} else if (Cos(angle) < Cos(3.9269907f)) {
		s32 path[2] = { 0, 6 };
		if (fn_800E4790(obj, FindJointByPath(obj->mJObj, path, 2))) {
			return -2;
		}
	} else if (0.0f < Sin(angle)) {
		s32 path[2] = { 0, 4 };
		if (fn_800E4790(obj, FindJointByPath(obj->mJObj, path, 2))) {
			return -3;
		}
	} else {
		s32 path[2] = { 0, 8 };
		if (fn_800E4790(obj, FindJointByPath(obj->mJObj, path, 2))) {
			return -4;
		}
	}
	return 0;
}

} // extern "C"

/**
 * @note Address: 0x800E4E58
 * @note Size: 0x364
 */
void XObjLeticker::vfC0()
{
	XObj* part = lbl_8025C400.GetObject(mLinked[0]);
	switch (mState) {
	case 3:
		if (!(0.0f < _9F8) || (!_A2D && part->mAnimId != 1)) {
			break;
		}
		Mtx mtx;
		u32 colors[50];
		Vec pts[50];
		Vec pos = mMoveTranslate;
		PSMTXTrans(mtx, pos.x, pos.y, pos.z);
		f32 twist = 6.0f * (_9F8 / _9FC) * (f32)sin(0.017453292f * (mFuncTime * 5)) + 72.0f;
		u32 i;
		for (i = 0; i < 50; i++) {
			f32 angle = 0.017453292f * mFuncTime + 0.017453292f * (twist * (s32)i);
			f32 y     = 12.0f + 0.2f * (24.0f * (s32)i / 50.0f) * _9F8 / _9FC;
			pts[i].x  = 7.1f * (f32)sin(angle);
			pts[i].y  = y;
			pts[i].z  = 7.1f * (f32)cos(angle);
			colors[i] = ((48 - (s32)i) * 32 / 48 + 32) | 0x40F000;
		}
		pts[49].x = pts[44].x;
		pts[49].y = pts[44].y;
		pts[49].z = pts[44].z;
		PSMTXMultVecArray(mtx, pts, pts, 50);
		fn_80198F1C(&lbl_8025C400.mGs, pts, colors, 50);
		if (!fn_8001E6C8(&lbl_8025C400.mSound, 0xB0)) {
			fn_8001E904(&lbl_8025C400.mSound, 0xB0, 0xFF);
			HSD_AudioSFXSetPitchByID(0xB0, 600.0f * (_A34 - 1.0f));
		} else {
			fn_8001E468(&lbl_8025C400.mSound, 0xB0, fn_8001DF00(&lbl_8025C400.mSound, &part->mMoveTranslate, 0), 0);
		}
		break;
	}
}

/**
 * @note Address: 0x800E51BC
 * @note Size: 0x324
 */
// NONMATCHING: control flow
void XObjLeticker::vfAC()
{
	vfB0(0);
	switch (mState) {
	case 1: {
		XObj* part     = lbl_8025C400.GetObject(mLinked[0]);
		s32 path[8]    = { 0, 0, 0, 0, 0, 0, 0, 0 };
		HSD_JObj* jobj = FindJointByPath(part->mJObj, path, 7);
		HSD_JObjClearFlags(jobj, 0x10);
		f32 y;
		for (s32 i = 0; 0.0f < (y = mHeight - 24.0f * i); i++) {
			HSD_JObjSetTranslateY(jobj, y);
			HSD_JObjDispAll(jobj, NULL, 7, 0);
		}
		HSD_JObjSetFlags(jobj, 0x10);
		HSD_JObjSetTranslateY(jobj, 0.0f);
		break;
	}
	case 2:
		lbl_8025C400.GetObject(mLinked[0])->mStatus.mFlags &= ~0x80000000ULL;
		break;
	default:
		break;
	}
}

/**
 * @note Address: 0x800E5688
 * @note Size: 0x410
 */
void XObjLeticker::StateCount()
{
	XObj* part;
	s32 count = (s32)_9F8 / 10;
	if (_9A0 < count) {
		count = 10;
	}
	part = lbl_8025C400.GetObject(mLinked[0]);
	if (_99C != count) {
		if (GetAnimEndFrame(part->mJObj) <= GetAnimFrame(part->mJObj)) {
			if (_99C < count) {
				_99C++;
				f32 rate = 1.0f;
				if (!_A2D) {
					rate = 8.0f;
				}
				part->SetAnim(_99C, FALSE, NULL, NULL, NULL, 0.0f, rate, 0.0f, 10.0f);
			} else if (_A2D) {
				_99C--;
				part->SetAnim(24 - _99C, FALSE, NULL, NULL, NULL, 0.0f, 2.0f, 0.0f, 10.0f);
				if (_99C == 0) {
					PushAnim(part, 0, 0, 100);
				}
			} else {
				_9F8 = (_99C - 1) * 10;
			}
			if (_A2D) {
				fn_8001E904(&lbl_8025C400.mSound, 0xAC, 0xFF);
				HSD_AudioSFXSetPitchByID(0xAC, 600.0f * (_A34 - 1.0f));
			}
		}
		SetFloors(part, lbl_8022A008[_99C]);
	}
	if (!(mStatus.mFlags & 0x0800000000000000ULL)) {
		vf12C(1, 0, 0, 0, 0.5f);
		SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
}

/**
 * @note Address: 0x800E5A98
 * @note Size: 0x3E0
 */
void XObjLeticker::StateLift()
{
	XObj* base = lbl_8025C400.GetObject(mLinked[0]);
	XObj* top  = lbl_8025C400.GetObject(mLinked[1]);
	if (base == NULL || top == NULL) {
		return;
	}
	f32 height = 4.0f * (s32)(_9F8 / 3.0f) + 0.5f;
	if (mHeight == height) {
		_A24 = TRUE;
	} else if (mHeight < height) {
		mHeight += 0.5f;
		if (height < mHeight) {
			mHeight = height;
		}
	} else if (height < mHeight && 0.0f < mHeight) {
		mHeight -= 0.5f;
		if (mHeight < 0.0f) {
			mHeight = 0.0f;
		}
	}
	if (mHeight <= 2.5f) {
		mHeight = 2.5f;
	}
	s32 step = mHeight / 4.0f;
	if (_99C != step) {
		if (!fn_8001E6C8(&lbl_8025C400.mSound, 0xA8)) {
			fn_8001E904(&lbl_8025C400.mSound, 0xA8, 0xFF);
		}
		HSD_AudioSFXSetPitchByID(0xA8, 600.0f * (_A34 - 1.0f));
		_99C = step;
		_A24 = FALSE;
	}
	top->SetTranslateY(9.0f + (mHeight + HSD_JObjGetTranslationY(mJObj)), 0);
	_A04.x = top->mMoveTranslate.x;
	_A04.y = top->mMoveTranslate.y;
	_A04.z = top->mMoveTranslate.z;
	if (mStatus.mFlags & 0x0800000000000000ULL) {
		f32 h;
		if (mHeight < 10.0f) {
			h = 19.0f;
		} else {
			h = 5.0f + (9.0f + mHeight) - 2.0f;
		}
		lbl_8065A6D0[0]->_14[1] = (s32)h * 100;
		lbl_8065A6D0[0]->_04[1] = (s32)(h / sqrtf(3.0f)) * 100;
		SetFloors(this, lbl_8065A6D0[0]);
	} else {
		s32 k          = (mHeight - 12.5f) / 4.0f;
		mBox[0]._04[1] = k * 0xA9 + 0x654;
		mBox[0]._14[1] = k * 400 + 1000;
		base->mFloors  = mBox;
		base->_250[1]  = 0;
		base->_250[0]  = 0;
		fn_8008C8A4(base);
		SetFloors(this, lbl_8065A6D0[1]);
		vf12C(1, 0, 0, 0, 0.5f);
		SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
}

static inline void PlayLetickerSound(XObjLeticker& obj)
{
	s32 anim  = obj.mAnimId;
	f32 frame = GetAnimFrame(obj.mJObj);
	obj.mSfx.Update(anim, frame, fn_8001DF00(&lbl_8025C400.mSound, &obj.mMoveTranslate, 0));
}

/**
 * @note Address: 0x800E5E78
 * @note Size: 0x598
 */
void XObjLeticker::vfA4(u32 frames)
{
	if (mAnimId == 1) {
		if (GetAnimEndFrame(mJObj) <= frames + GetAnimFrame(mJObj)) {
			SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
	} else if (mAnimId == 3) {
		if (GetAnimEndFrame(mJObj) <= frames + GetAnimFrame(mJObj)) {
			SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
	}
	fn_80090ED4(this, frames);
	mFuncTime += frames;
	if (mFunc) {
		(this->*mFunc)();
	}
	s32 path[3]    = { 0, 0, 0 };
	HSD_JObj* jobj = FindJointByPath(mJObj, path, 2);
	XObj* player   = lbl_8025C400.GetPlayer();
	f32 angle      = atan2(player->mMoveTranslate.x - mMoveTranslate.x, player->mMoveTranslate.z - mMoveTranslate.z);
	HSD_JObjSetRotationY(jobj, angle - HSD_JObjGetRotationY(mJObj));
	PlayLetickerSound(*this);
}

/**
 * @note Address: 0x800E6410
 * @note Size: 0x98
 */
void XObjLeticker::vf98()
{
	fn_800928E8(this);
	mLinkedCount = 0;
	mFunc        = NULL;
	_99C         = 0;
	mFuncTime    = 0;
	_9F8         = 0.0f;
	mHeight      = 0.0f;
	mPtcl        = TRUE;
	_A2D         = FALSE;
	_A30         = 0;
	mState       = 0;
	mHeading     = 0.0f;
	mBox[0].mId  = -1;
	mBox[1].mId  = -1;
	mSfx.mEvents = sSfx;
}

/**
 * @note Address: 0x800E64A8
 * @note Size: 0x2E8
 */
void CCameraActionLeticker::Update(u32 step)
{
	CPad pad(0);
	u64 buttons     = lbl_8025C400.mStatus.mFlags & 0x80000000;
	u32 leftButton  = buttons ? HSD_PAD_DPADLEFT : HSD_PAD_CSTICK_LEFT;
	u32 rightButton = buttons ? HSD_PAD_DPADRIGHT : HSD_PAD_CSTICK_RIGHT;

	if (mTurnFrames == 0) {
		if (pad.mPad->mTrigger & leftButton) {
			if (CanTurn(this, mTurnTo - 1.5707964f)) {
				mTurnTo -= 1.5707964f;
				mTurnSpeed  = -0.08726646f;
				mTurnFrames = 18;
			}
		} else if (pad.mPad->mTrigger & rightButton) {
			if (CanTurn(this, 1.5707964f + mTurnTo)) {
				mTurnTo += 1.5707964f;
				mTurnSpeed  = 0.08726646f;
				mTurnFrames = 18;
			}
		}
	} else {
		mTurnFrames -= step;
		if (mTurnFrames <= 0) {
			mAngle      = mTurnTo;
			mTurnFrames = 0;
		} else {
			mAngle += mTurnSpeed * step;
		}
	}
	CCameraActionLadr::Update(step);
}

/**
 * @note Address: 0x800E6790
 * @note Size: 0x68
 */
void CCameraActionLeticker::vf14()
{
#if !defined(VERSION_PAL)
	OSReport("CCameraActionLeticker::Reset\n");
#endif
	CCameraActionLadr::vf14();
	mTurnTo     = 0.0f;
	mTurnFrames = 0;
	mBounded    = false;
	lbl_8025C400.mStatus.mFlags |= 0x80;
}

/**
 * @note Address: 0x800E67F8
 * @note Size: 0x20
 */
Vec CCameraActionLeticker::GetCameraTarget()
{
	return *mTarget;
}

static CCameraActionLeticker sCamera;
