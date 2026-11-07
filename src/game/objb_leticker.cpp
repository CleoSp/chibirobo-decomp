#include "game/xobjleticker.h"
#include "game/xchr.h"
#include "game/ccameraaction.h"
#include "game/cmotioncontrolcamera.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/cpad.h"
#include "game/cparticle.h"
#include "game/gmath.h"
#include "game/animframe.h"
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
bool IsLetickerJointClearOfObjects(XObj* obj, HSD_JObj* jobj);
extern char lbl_8020AABC[];
void SelectHighestActiveFloor(XObj* obj);
extern CCameraActionLeticker gLetickerCameraAction;
extern XObjFloor* lbl_8022A008[];
extern XObjFloor* lbl_8065A6D0[2];
}

// sets the object's floor boxes, they take effect at once
static inline void SetFloors(XObj* obj, XObjFloor* floors)
{
	obj->mFloors = floors;
	obj->_250[1] = 0;
	obj->_250[0] = 0;
	fn_8008D220(obj);
	UpdateObjectFloorHeights(obj);
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
	XObj* part = gGame.GetObject(leticker->mLinked[0]);
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
 * @note Address: 0x800E29B8
 * @note Size: 0x4F4
 */
BOOL TurnLetickerAndCheckClearance(XObjLeticker* lift, f32 angle)
{
	CPad pad(0);
	XObj* first    = gGame.GetObject(lift->mLinked[0]);
	XObj* second   = gGame.GetObject(lift->mLinked[1]);
	lift->mHeading = angle;
	first->SetRotateY(angle + HSD_JObjGetRotationY(lift->mJObj), 0.1f);
	second->SetRotateY(angle + HSD_JObjGetRotationY(lift->mJObj), 0.1f);
	if (pad.mPad->mButton & HSD_PAD_STICK_DOWN)
		return TRUE;

	CVec pos(second->mMoveTranslate);
	f32 offset = 4.0f * (s32)(lift->_9F8 / 3.0f) + 0.5f;
	pos.y      = 6.0f + (3.0f + (9.0f + (offset + HSD_JObjGetTranslationY(lift->mJObj))));
	for (s32 i = 0; i < gGame.mXObjs.mObjectCount; ++i) {
		XObj* target = gGame.GetObject(i);
		if (!(target->mStatus.mFlags & 1) || !(target->mStatus.mFlags & 0x10000))
			continue;
		if (target->mId == lift->mId || target->mId == first->mId || target->mId == second->mId)
			continue;
		u32 attr = target->mAttr._00 != NULL ? target->mAttr._00->_00 : 0;
		if (attr == 0)
			continue;
		f32 dx = target->mMoveTranslate.x - pos.x;
		f32 dz = target->mMoveTranslate.z - pos.z;
		if (sqrtf(dx * dx + dz * dz) > 100.0f + (5.0f + target->_6F8))
			continue;
		CVec hit, normal;
		u32 flags = target->vfFC(&pos, &pos, 5.5f, 6.0f, &hit, &normal, 0, 0);
		if (flags & 2)
			return FALSE;
		if (flags & 1) {
			f32 floor = target->mFloorHeights[target->_274];
			if (__fpclassifyf(floor) == FP_NAN)
				return FALSE;
			if (pos.y <= floor)
				return FALSE;
		}
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
		part = gGame.GetObject(obj->mLinked[i]);
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
		(&gGame.mParticle)->Kill(0xA5);
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
		gGame.mStatus.mFlags &= ~0x80ULL;
		gGame.mMotionControlCamera.PopAction();
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
 * @note Address: 0x800E322C
 * @note Size: 0x8C4
 */
BOOL XObjLeticker::vf134(f32 amount)
{
	if (gGame.GetPlayer()->mMoveTranslate.y < mMoveTranslate.y - 5.0f)
		return FALSE;
	CPad pad(0);
	f32 heading
	    = (f32)atan2(gGame.GetPlayer()->mMoveTranslate.x - mMoveTranslate.x, gGame.GetPlayer()->mMoveTranslate.z - mMoveTranslate.z);
	f32 angle = heading - HSD_JObjGetRotationY(mJObj);
	s32 side  = GetSide(angle);
	switch (side) {
	case 0:
		angle = 0.0f;
		break;
	case 1:
		angle = HALF_PI;
		break;
	case 2:
		angle = PI;
		break;
	case 3:
		angle = -HALF_PI;
		break;
	}
	if (!_A2D) {
		_A2D     = TRUE;
		mWaiting = FALSE;
		if (mState != 3) {
			CCameraActionLeticker& camera = gLetickerCameraAction;
			gGame.mMotionControlCamera.PushAction(&camera);
			camera.mTarget = &_A04;
			if (mCameraBounded) {
				camera.mBounded = true;
				camera.mMinX    = mCameraMinX;
				camera.mMaxX    = mCameraMaxX;
				camera.mMinZ    = mCameraMinZ;
				camera.mMaxZ    = mCameraMaxZ;
				for (s32 i = 0; i < 4 && !CanTurn(&camera, camera.mAngle); ++i)
					camera.mTurnTo = camera.mAngle += HALF_PI;
			}
		} else {
			gGame.GetObject(mLinked[0])->mStatus.mFlags &= ~0x10000ULL;
			gGame.GetObject(mLinked[0])->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		if (mState == 2) {
			_9A0 = _9A4[side];
			_9FC = 10.0f * _9A0;
		}
		if (mPtcl) {
			gGame.mParticle.Kill(0xA5);
			mPtcl = FALSE;
		}
	}
	if (mWaiting) {
		if (!(pad.mPad->mButton & HSD_PAD_STICK_DOWN)) {
			if (--_A30 <= 0)
				return FALSE;
			return TRUE;
		}
		mWaiting = FALSE;
	}
	++_A30;
	f32 delta     = amount * _A34;
	BOOL starting = _A30 < 40;
	switch (mState) {
	case 1:
		if (!(u8)TurnLetickerAndCheckClearance(this, angle)) {
			if (mHeight == 4.0f * (s32)(_9F8 / 3.0f) + 0.5f) {
				XObj* part = gGame.GetObject(mLinked[1]);
				fn_8001E904(&gGame.mSound, 0x191, 255);
				vf12C(1, 255, 0, 0, 0.5f);
				SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				part->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				PushAnim(part, 0, 0, 100);
				_A30     = 30;
				mWaiting = TRUE;
			}
			return TRUE;
		}
		break;
	case 2:
		if (!(u8)fn_800E260C(this, angle))
			return starting;
		break;
	case 3:
		_A04 = mMoveTranslate;
		gGame.GetObject(mLinked[0])->SetPos(mMoveTranslate, 0);
		gGame.GetObject(mLinked[0])->SetRotateY(mMoveRotate.y, 1.0f);
		if (_9FC <= _9F8 + delta) {
			gGame.GetObject(mLinked[0])->mStatus.mFlags |= 0x10000ULL;
			gGame.GetObject(mLinked[0])->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		} else {
			XObj* part = gGame.GetObject(mLinked[0]);
			if (part->mAnimId != 0)
				part->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		break;
	}
	if (gGame.GetPlayer()->mParams.mBattery <= 0.0f)
		return FALSE;
	if (pad.mPad->mButton & HSD_PAD_STICK_DOWN) {
		_9F8 -= delta;
		if (_9F8 < 0.0f) {
			_9F8 = 0.0f;
			return starting;
		}
	} else {
		_9F8 += delta;
		if (_9FC <= _9F8) {
			_9F8 = _9FC;
			return starting;
		}
	}
	if (mAnimId != 1 && mAnimId != 2)
		SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	return TRUE;
}

/**
 * @note Address: 0x800E3AF0
 * @note Size: 0x4
 */
void XObjLeticker::vf138(const Vec& pos, f32 f)
{
}

static inline void StartLetickerRide(XObjLeticker* lift, XObj* part, XObjChr* player, s32 anim, s32 partAnim, s32 sound)
{
	lift->SetAnim(anim, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	part->mStatus.mFlags |= 0x10000ULL;
	part->SetAnim(partAnim, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	part->SetPos(lift->mMoveTranslate, 0);
	player->_980.mFlags = 0;
	player->_980.mFlags |= 0x40000000;
	player->mPad.mReset = FALSE;
	StopGameSfx(&gGame.mSound, 0xB0);
	fn_8001E904(&gGame.mSound, sound, 255);
	lift->mStep = 10;
}

/**
 * @note Address: 0x800E3AF4
 * @note Size: 0xC9C
 */
s32 XObjLeticker::vf124(s32 count, s32* args)
{
	XObjChr* player = gGame.GetPlayer();
	switch (args[0]) {
	case 0:
		mLinkedCount = 0;
		_9A0         = args[1];
		for (s32 i = 0; i < 4; ++i)
			_9A4[i] = _9A0;
		for (s32 i = 2; i < count && (u32)mLinkedCount < 2; ++i) {
			mLinked[mLinkedCount] = args[i];
			++mLinkedCount;
		}
		_A04 = mMoveTranslate;
		_A34 = 1.0f;
		if (mLinkedCount > 0) {
			XObj* part = gGame.GetObject(mLinked[0]);
			switch (part->mCoord._00->_00) {
			case 0xEE: {
				mState = 1;
				mFunc  = &XObjLeticker::StateLift;
				if (_9A0 > 30)
					_9A0 = 30;
				mBox[0].mId    = 2;
				mBox[0]._04[0] = 0;
				mBox[0]._04[1] = 1620;
				mBox[0]._04[2] = 0;
				mBox[0]._10    = 0;
				mBox[0]._14[0] = 400;
				mBox[0]._14[1] = 1000;
				mBox[0]._14[2] = 200;
				s32 path[]     = { 0 };
				HSD_JObjSetFlagsAll(FindJointByPath(part->mJObj, path, 1), 0x10);
				break;
			}
			case 0x114:
				mState = 2;
				mFunc  = &XObjLeticker::StateCount;
				if (_9A0 > 10)
					_9A0 = 10;
				break;
			case 0x266:
				mState = 3;
				mFunc  = NULL;
				for (s32 i = 0; mFloors[i].mId != -1; ++i)
					_250[0] |= 1U << i;
				SelectHighestActiveFloor(this);
				break;
			}
		}
		_9FC           = 10.0f * _9A0;
		mHeight        = 0.0f;
		_A24           = FALSE;
		mCameraBounded = FALSE;
		_99C           = 0;
		break;
	case 1: {
		_9F8           = count < 2 || !args[1] ? 0.0f : 10.0f * args[1];
		mHeading       = count < 3 || !args[2] ? 0.0f : DEG2RAD * args[2];
		bool setHeight = count >= 4 && args[3] != 0;
		if (mLinkedCount == 0) {
#if !defined(VERSION_PAL)
			OSReport(lbl_8020AABC);
#endif
			break;
		}
		switch (mState) {
		case 1: {
			XObj* base = gGame.GetObject(mLinked[0]);
			XObj* top  = gGame.GetObject(mLinked[1]);
			if (setHeight) {
				mHeight = 4.0f * (s32)(_9F8 / 3.0f) + 0.5f;
				_99C    = mHeight * 0.25f;
			}
			base->SetRotateY(mHeading + HSD_JObjGetRotationY(mJObj), 0.1f);
			top->SetRotateY(mHeading + HSD_JObjGetRotationY(mJObj), 0.1f);
			break;
		}
		case 2: {
			XObj* part = gGame.GetObject(mLinked[0]);
			_99C       = 0;
			part->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			part->SetRotateY(mHeading + HSD_JObjGetRotationY(mJObj), 0.1f);
			break;
		}
		case 3:
			SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			_9F8  = 0.0f;
			mStep = 0;
			break;
		}
		break;
	}
	case 2:
		if (args[1]) {
			mPtcl      = TRUE;
			s32 path[] = { 0 };
			gGame.mParticle.Create(0xA5, 0, FindJointByPath(mJObj, path, 1), TRUE);
		} else {
			mPtcl = FALSE;
		}
		break;
	case 3:
		switch (args[1]) {
		case 0:
			return (s32)((9.0f + _9F8) / 10.0f);
		case 1:
			return 90 * GetSide(mHeading);
		case 2:
			return !(_9F8 < _9FC);
		case 3:
			return _A2D != 0;
		case 4:
			return (s32)(100.0f * _A34);
		}
		break;
	case 4:
		switch (args[1]) {
		case 0:
			_A34 = args[2] / 100.0f;
			break;
		case 1:
			for (s32 i = 0; i < 4; ++i)
				_9A4[i] = args[i + 2];
#if !defined(VERSION_PAL)
			for (u32 i = 0; i < 4; ++i)
				OSReport("m_level_table[%d]=%d\n", i, _9A4[i]);
#endif
			break;
		case 2:
			mCameraBounded = TRUE;
			mCameraMinX    = args[2] / 100.0f;
			mCameraMaxX    = args[3] / 100.0f;
			mCameraMinZ    = args[4] / 100.0f;
			mCameraMaxZ    = args[5] / 100.0f;
			break;
		}
		break;
	case 5:
		if (mState == 3) {
			XObj* part = gGame.GetObject(mLinked[0]);
			switch (args[1]) {
			case 0:
				_9F8 = 0.0f;
				SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				part->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				player->_980.mFlags = 0;
				player->_980.mFlags |= 0x20000000;
				player->_980.mFlags |= 7;
				player->mPad.mReset = TRUE;
				player->SetRotationY(mMoveRotate.y, 0);
				mStep = 0;
				break;
			case 1:
				StartLetickerRide(this, part, player, 4, 2, 0xAE);
				break;
			case 2:
				StartLetickerRide(this, part, player, 5, 3, 0xAF);
				break;
			case 3:
				if (count <= 2 || args[2]) {
					player->mLift.SetState(1, &player->mPlug);
					player->mLift._24 = 0;
				} else if (player->mLift.mTarget) {
					player->mLift.mState = 6;
				}
				break;
			}
			mFunc = &XObjLeticker::StateRide;
		}
		break;
	}
	return 0;
}

/**
 * @note Address: 0x800E54E0
 * @note Size: 0x1A8
 */
void XObjLeticker::StateRide()
{
	XObj* player   = gGame.GetPlayer();
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
bool IsLetickerJointClearOfObjects(XObj* obj, HSD_JObj* jobj)
{
	CVec pos;
	if (jobj != NULL) {
		MtxPtr mtx = HSD_JObjGetMtxPtr(jobj);
		pos.x = pos.y = pos.z = 0.0f;
		PSMTXMultVec(mtx, &pos, &pos);
	}
	pos.y -= 6.0f;
	for (s32 i = 0; i < gGame.mXObjs.mObjectCount; ++i) {
		XObj* target = gGame.GetObject(i);
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
	if (3.0f + obj->mMoveTranslate.y < gGame.GetPlayer()->mMoveTranslate.y) {
		return 0;
	}
	if (Cos(0.7853982f) < Cos(angle)) {
		s32 path[2] = { 0, 2 };
		if (IsLetickerJointClearOfObjects(obj, FindJointByPath(obj->mJObj, path, 2))) {
			return -1;
		}
	} else if (Cos(angle) < Cos(3.9269907f)) {
		s32 path[2] = { 0, 6 };
		if (IsLetickerJointClearOfObjects(obj, FindJointByPath(obj->mJObj, path, 2))) {
			return -2;
		}
	} else if (0.0f < Sin(angle)) {
		s32 path[2] = { 0, 4 };
		if (IsLetickerJointClearOfObjects(obj, FindJointByPath(obj->mJObj, path, 2))) {
			return -3;
		}
	} else {
		s32 path[2] = { 0, 8 };
		if (IsLetickerJointClearOfObjects(obj, FindJointByPath(obj->mJObj, path, 2))) {
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
	XObj* part = gGame.GetObject(mLinked[0]);
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
		DrawGsColoredLineStrip(&gGame.mGs, pts, colors, 50);
		if (!fn_8001E6C8(&gGame.mSound, 0xB0)) {
			fn_8001E904(&gGame.mSound, 0xB0, 0xFF);
			HSD_AudioSFXSetPitchByID(0xB0, 600.0f * (_A34 - 1.0f));
		} else {
			SetGameSfxVolume(&gGame.mSound, 0xB0, GetSoundVolumeAtPosition(&gGame.mSound, &part->mMoveTranslate, 0), 0);
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
		XObj* part     = gGame.GetObject(mLinked[0]);
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
		gGame.GetObject(mLinked[0])->mStatus.mFlags &= ~0x80000000ULL;
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
	part = gGame.GetObject(mLinked[0]);
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
				fn_8001E904(&gGame.mSound, 0xAC, 0xFF);
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
	XObj* base = gGame.GetObject(mLinked[0]);
	XObj* top  = gGame.GetObject(mLinked[1]);
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
		if (!fn_8001E6C8(&gGame.mSound, 0xA8)) {
			fn_8001E904(&gGame.mSound, 0xA8, 0xFF);
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
		UpdateObjectFloorHeights(base);
		SetFloors(this, lbl_8065A6D0[1]);
		vf12C(1, 0, 0, 0, 0.5f);
		SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
}

static inline void PlayLetickerSound(XObjLeticker& obj)
{
	s32 anim  = obj.mAnimId;
	f32 frame = GetAnimFrame(obj.mJObj);
	obj.mSfx.Update(anim, frame, GetSoundVolumeAtPosition(&gGame.mSound, &obj.mMoveTranslate, 0));
}

/**
 * @note Address: 0x800E5E78
 * @note Size: 0x598
 */
void XObjLeticker::UpdateObject(u32 frames)
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
	UpdateObject__4XObjFUl(this, frames);
	mFuncTime += frames;
	if (mFunc) {
		(this->*mFunc)();
	}
	s32 path[3]    = { 0, 0, 0 };
	HSD_JObj* jobj = FindJointByPath(mJObj, path, 2);
	XObj* player   = gGame.GetPlayer();
	f32 angle      = atan2(player->mMoveTranslate.x - mMoveTranslate.x, player->mMoveTranslate.z - mMoveTranslate.z);
	HSD_JObjSetRotationY(jobj, angle - HSD_JObjGetRotationY(mJObj));
	PlayLetickerSound(*this);
}

/**
 * @note Address: 0x800E6410
 * @note Size: 0x98
 */
void XObjLeticker::ResetObjectState()
{
	ResetObjectState__4XObjFv(this);
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
	u64 buttons     = gGame.mStatus.mFlags & 0x80000000;
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
	gGame.mStatus.mFlags |= 0x80;
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
