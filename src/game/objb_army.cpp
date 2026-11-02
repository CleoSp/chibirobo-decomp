#include "game/xobjarmy.h"
#include "game/xchr.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/texturecopy.h"
#include "game/animframe.h"
#include <math.h>
extern "C" {
#include <dolphin/mtx/vec.h>
}

// XObjArmy, the toy soldiers, and the shots they fire

extern "C" {
s32 fn_8006EBC8(s32 id);
f32 HSD_Randf(void);
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

// nothing reads it
static s32 sUnused[26] = { 0, 3, 0, 0, 3, 0, 0, 2, 0, 0, 2, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 3, 0, 0, 3, 0 };

// the points the soldier stands on, upright, falling over and lying down
static XObjFloor sFloorsStand[2] = {
	{ 0, 0, { 0, 0x226, 0 }, 0, { 0x200, 0x200, 0x200 } },
	{ -1 },
};
static XObjFloor sFloorsFall[2] = {
	{ 0, 0, { 0, 0x100, -0xD4 }, 0, { 0x200, 0x100, 0x200 } },
	{ -1 },
};
static XObjFloor sFloorsDown[2] = {
	{ 0, 0, { 0, 0x100, 0 }, 0, { 0x200, 0x100, 0x200 } },
	{ -1 },
};
static XObjFloor* sFloors[3] = { sFloorsStand, sFloorsFall, sFloorsDown };

static AnimSfxEvent sSfxF7[] = {
	{ 0x02, 18.0f, 36.0f, 1, 0x11000E, 0xFF, 4, 0, 0 }, { 0x03, 14.0f, 27.0f, 1, 0x11000E, 0xFF, 4, 0, 0 },
	{ 0x11, 11.0f, 22.0f, 1, 0x11000E, 0xFF, 4, 0, 0 }, { 0x12, 5.0f, -1.0f, 1, 0x11000D, 0xFF, 4, 0, 0 },
	{ 0x0F, 0.0f, -1.0f, 1, 0x11001C, 0xFF, 0, 0, 0 },  { 0x07, 28.0f, -1.0f, 1, 0x110028, 0xFF, 0, 0, 0 },
	{ 0x28, 18.0f, 36.0f, 1, 0x11000E, 0xFF, 0, 0, 0 }, { -1 },
};
static AnimSfxEvent sSfxF8[] = {
	{ 0x02, 18.0f, 36.0f, 1, 0x11000E, 0xFF, 4, 0, 0 },
	{ 0x03, 14.0f, 27.0f, 1, 0x11000E, 0xFF, 4, 0, 0 },
	{ 0x11, 11.0f, 22.0f, 1, 0x11000E, 0xFF, 4, 0, 0 },
	{ 0x12, 5.0f, -1.0f, 1, 0x11000D, 0xFF, 4, 0, 0 },
	{ 0x0F, 0.0f, -1.0f, 1, 0x11001C, 0xFF, 0, 0, 0 },
	{ 0x07, 28.0f, -1.0f, 1, 0x110028, 0xFF, 0, 0, 0 },
	{ -1 },
};

// the y angle that looks from to at from, 0 when they're on top of each other
static inline f32 GetAngleY(const Vec& from, const Vec& to)
{
	f32 dx = to.x - from.x;
	f32 dz = to.z - from.z;
	if (sqrtf(dx * dx + dz * dz) == 0.0f) {
		return 0.0f;
	}
	f32 angle = atan2(from.x - to.x, from.z - to.z);
	while (angle < 0.0f) {
		angle += 6.2831855f;
	}
	while (6.2831855f <= angle) {
		angle -= 6.2831855f;
	}
	return angle;
}

static Vec sMin;
static Vec sMax;
static XObj* sLeader;   // the leader's model draws the shots
static s32 sShotSfx;    // the next of the four shot sounds
static s32 sHitCount;   // soldiers knocked over
static XObj* sTarget;   // shot at instead of chibi robo
static CStatus sStatus; // 1 once a shot hits, 2 keeps them from shooting

// fires a shot from pos along dir
static inline void FireShot(XObjArmyShot* shot, const CVec& pos, const CVec& dir, u8 quiet)
{
	shot->mActive = 1;
	shot->mLife   = 120;
	shot->mPos    = pos;
	shot->mPos.y += 6.0f;
	shot->mVel = dir;
	PSVECNormalize(&shot->mVel, &shot->mVel);
	shot->mSpeed = 3.0f;
	shot->mPhase = 0;
	if (!quiet) {
		if (sShotSfx < 0 || sShotSfx >= 4) {
			sShotSfx = 0;
		}
		fn_8001E904(&lbl_8025C400.mSound, sShotSfx + 0x110012, 0xFF);
		sShotSfx++;
	}
}

// starts what the soldier does between hits again
static inline void RestartMode(XObjArmy* army)
{
	army->mStep = 0;
	switch (army->mMode) {
	case 1:
		army->mStatus.mFlags |= 8;
		army->mFunc = &XObjArmy::StateMarch;
		break;
	case 2:
		army->mFunc = &XObjArmy::StateGuard;
		break;
	default:
		army->mFunc = NULL;
		break;
	}
}

static inline void Knock(XObjArmy* army)
{
	army->mStep = 0;
	army->mFunc = &XObjArmy::StateKnock;
}

static inline void SetMode(XObjArmy* army, s32 mode)
{
	army->mMode = mode;
	RestartMode(army);
}

/**
 * @note Address: 0x800E6A5C
 * @note Size: 0x114
 */
// NONMATCHING: regalloc
void XObjArmy::vf148()
{
	u8 volume;
	if (mFlags.mFlags & 0x100) {
		volume = 0xFF;
	} else {
		volume = fn_8001DF00(&lbl_8025C400.mSound, &mMoveTranslate, 0);
	}
	HSD_JObj* jobj = mJObj;
	s32 anim       = mAnimId;
	mSfx.Update(anim, GetAnimFrame(jobj), volume);
}

extern "C" {

/**
 * @note Address: 0x800E6B70
 * @note Size: 0x308
 */
void fn_800E6B70(XObjArmyShot* shot, u32 frames)
{
	if (!shot->mActive) {
		return;
	}
	CVec target;
	switch (shot->mPhase) {
	case 0: {
		if (--shot->mLife == 0) {
			shot->mActive = 0;
		}
		shot->Move();
		f32 height;
		if (sTarget != NULL) {
			target = sTarget->mMoveTranslate;
			height = 28.0f;
		} else {
			XObjChr* player = lbl_8025C400.GetPlayer();
			target          = player->mMoveTranslate;
			height          = player->GetAttachment() != 6 ? 10.0f : 28.0f;
		}
		f32 dy = target.y - shot->mPos.y;
		if (dy < 0.0f) {
			dy = -dy;
		}
		if (dy < height) {
			target.y = shot->mPos.y;
			if (PSVECDistance(&shot->mPos, &target) < 5.0f) {
				Vec hit = shot->mPos;
				fn_8001E904(&lbl_8025C400.mSound, 0x110029, 0xFF);
				sStatus.mFlags |= 1;
				lbl_8025C400.GetPlayer()->vf138(hit, 10.0f);
				shot->mVel.x = -1.0f * shot->mVel.x;
				shot->mVel.y = -1.0f;
				shot->mVel.z = -1.0f * shot->mVel.z;
				shot->mSpeed = 0.5f;
				shot->mPhase++;
			}
		}
		break;
	}
	case 1:
		shot->Move();
		shot->mVel.y += -0.15f;
		if (shot->mPos.y <= 1.5f) {
			shot->mVel.y = 0.0f;
			shot->mSpeed = 0.3f;
			shot->mLife  = 10;
			shot->mPhase++;
		}
		break;
	case 2:
		if (--shot->mLife == 0) {
			shot->mActive = 0;
		}
		shot->Move();
		break;
	}
}

} // extern "C"

/**
 * @note Address: 0x800E6E78
 * @note Size: 0x158
 */
void XObjArmy::vf138(const Vec& pos, f32 f)
{
	if (!(mFlags.mFlags & 1)) {
		_994 = 0.0f;
		fn_8001E904(&lbl_8025C400.mSound, 0x83, 0xFF);
		if (_994 <= 0.0f) {
			if (fn_8006EBC8(0x1F) / 100.0f <= f || PSVECDistance(&mMoveTranslate, &lbl_8025C400.GetPlayer()->mMoveTranslate) < 20.0f) {
				mStep = 0;
				mFunc = &XObjArmy::StateFall;
			} else {
				mStep = 0;
				mFunc = &XObjArmy::StateKnock;
			}
		} else {
			mStep = 0;
			mFunc = &XObjArmy::StateWait;
		}
	}
}

extern "C" {

/**
 * @note Address: 0x800E6FD0
 * @note Size: 0x560
 */
s32 vf124__8XObjArmyFlPl(XObjArmy* obj, s32 argc, s32* args)
{
	if (argc < 1) {
		return -1;
	}
	switch (args[0]) {
	case 0:
		sHitCount = 0;
		sMin.x    = args[1] / 100.0f;
		sMin.y    = args[2] / 100.0f;
		sMin.z    = args[3] / 100.0f;
		sMax.x    = args[4] / 100.0f;
		sMax.y    = args[5] / 100.0f;
		sMax.z    = args[6] / 100.0f;
		break;
	case 1:
		switch (args[1]) {
		case 0:
			return sHitCount;
		case 5:
			return sStatus.mFlags & 1;
		default:
			return sHitCount;
		}
	case 2:
		switch (args[1]) {
		case 0: {
			Vec pos;
			pos.x = sMin.x + (sMax.x - sMin.x) * HSD_Randf();
			pos.y = sMin.y + (sMax.y - sMin.y) * HSD_Randf();
			pos.z = sMin.z + (sMax.z - sMin.z) * HSD_Randf();
			obj->SetPos(pos, 0);
			obj->mStatus.mFlags |= 0x10000;
			SetMode(obj, 1);
			break;
		}
		case 1:
			Knock(obj);
			break;
		case 2:
			obj->mStep = 0;
			obj->mFunc = NULL;
			break;
		case 3:
			if (argc >= 2) {
				if (args[2] >= 0) {
					sTarget = lbl_8025C400.GetObject(args[2]);
				} else {
					sTarget = NULL;
				}
			}
			break;
		case 4:
			if (argc >= 3) {
				if (args[2]) {
					obj->mFlags.mFlags &= ~0x80;
				} else {
					obj->mFlags.mFlags |= 0x80;
				}
			} else {
				obj->mFlags.mFlags &= ~0x80;
			}
			break;
		case 5:
			sStatus.mFlags &= ~1;
			break;
		case 6:
			if (argc >= 3) {
				if (args[2]) {
					sStatus.mFlags &= ~2;
				} else {
					sStatus.mFlags |= 2;
				}
			} else {
				sStatus.mFlags |= 2;
			}
			break;
		}
		break;
	case 3:
		obj->mGuardPos.x = args[1] / 100.0f;
		obj->mGuardPos.y = args[2] / 100.0f;
		obj->mGuardPos.z = args[3] / 100.0f;
		SetMode(obj, 2);
		break;
	case 100:
		break;
	case 0x65:
		if (args[1] == 0) {
			obj->mFlags.mFlags &= ~0x100;
		} else {
			obj->mFlags.mFlags |= 0x100;
		}
		break;
	default:
		break;
	}
	return 0;
}

/**
 * @note Address: 0x800E7530
 * @note Size: 0x2C4
 */
// NONMATCHING: regalloc
void fn_800E7530(XObjArmy* army)
{
	CVec pos;
	CVec dir;
	s32 path[6] = { 0, 0, 4, 0, 0, 0 };
	FindJointByPath(army->mJObj, path, 6);
	pos        = army->mMoveTranslate;
	Vec facing = { 0.0f, 0.0f, 0.0f };
	facing.x   = sin(HSD_JObjGetRotationY(army->mJObj));
	facing.z   = cos(HSD_JObjGetRotationY(army->mJObj));
	dir        = CVec(facing);
	u32 i;
	for (i = 0; army->mShots[i].mActive && i < 5; i++) { }
	if (i < 5) {
		FireShot(&army->mShots[i], pos, dir, (army->mFlags.mFlags >> 7) & 1);
	}
}

} // extern "C"

/**
 * @note Address: 0x800E77F4
 * @note Size: 0x5E8
 */
void XObjArmy::StateGuard()
{
	switch (mStep) {
	case 0:
		SetAnim(9, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		SetRotateY(GetAngleY(mGuardPos, mMoveTranslate), 1.0f);
		mStep++;
		break;
	case 1:
		if (20.0f <= GetAnimFrame(mJObj)) {
			u32 i;
			for (i = 0; mShots[i].mActive && i < 5; i++) { }
			if (i < 5) {
				FireShot(&mShots[i], mMoveTranslate, mGuardPos - mMoveTranslate, (mFlags.mFlags & 0x80) != 0);
			}
			mStep++;
		}
		break;
	case 2:
		if (GetAnimEndFrame(mJObj) <= 2.0f + GetAnimFrame(mJObj)) {
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			SetMode(this, 0);
		}
		break;
	}
}

/**
 * @note Address: 0x800E7DDC
 * @note Size: 0x3C4
 */
void XObjArmy::StateFall()
{
	CVec pos;
	switch (mStep) {
	case 0: {
		mFlags.mFlags |= 1;
		sHitCount++;
		f32 angle = GetAngleY(mMoveTranslate, ((XObjChr*)lbl_8025C400.mXObjs.mPools[2].mObjects)->mMoveTranslate);
		pos       = mMoveTranslate;
		SetAnim(0x13, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		SetRotateY(angle - 3.1415927f, 1.0f);
		SetMoveDir(&pos, 0.5f);
		mStep++;
		break;
	}
	case 1:
		if (GetAnimEndFrame(mJObj) <= GetAnimFrame(mJObj)) {
			SetMoveDir(&mMoveTranslate, 1.0f);
			mStep++;
		}
		break;
	}
}

/**
 * @note Address: 0x800E81A0
 * @note Size: 0x3C4
 */
void XObjArmy::StateKnock()
{
	CVec pos;
	switch (mStep) {
	case 0: {
		mFlags.mFlags |= 1;
		sHitCount++;
		f32 angle = GetAngleY(mMoveTranslate, ((XObjChr*)lbl_8025C400.mXObjs.mPools[2].mObjects)->mMoveTranslate);
		pos       = mMoveTranslate;
		SetAnim(0xA, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		SetRotateY(angle - 3.1415927f, 1.0f);
		SetMoveDir(&pos, 0.1f);
		mStep++;
		break;
	}
	case 1:
		if (GetAnimEndFrame(mJObj) <= GetAnimFrame(mJObj)) {
			SetMoveDir(&mMoveTranslate, 0.0f);
			mStep++;
		}
		break;
	}
}

/**
 * @note Address: 0x800E8564
 * @note Size: 0x194
 */
void XObjArmy::StateWait()
{
	switch (mStep) {
	case 0:
		mTimer = 30;
		SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		SetMoveDir(&mMoveTranslate, 1.0f);
		SetRotateY(HSD_JObjGetRotationY(mJObj), 1.0f);
		mStep++;
		break;
	case 1:
		if (--mTimer == 0) {
			RestartMode(this);
		}
		break;
	}
}

/**
 * @note Address: 0x800E980C
 * @note Size: 0x370
 */
// NONMATCHING: regalloc and stack order
void XObjArmy::vfC0()
{
	CGs* gs = &lbl_8025C400.mGs;
	for (u32 i = 0; i < 5; i++) {
		if (mShots[i].mActive) {
			if (sLeader == NULL) {
				gs->_29C = 1;
				Vec pos  = mShots[i].mPos;
				fn_8019B56C(gs->mCurRenderMode, 1, 1, 1, 15);
				Vec rot = { 0.0f, 0.0f, 0.0f };
				fn_80198258(&lbl_8025C400.mGs, 16, 0x64FF64C8, 0xFF, pos.x, pos.y, pos.z, 3.0f, 3.0f, rot.x, rot.y, rot.z);
				gs->_29C = 0;
			} else {
				HSD_JObj* jobj = sLeader->mJObj;
				HSD_JObjSetTranslateX(jobj, mShots[i].mPos.x);
				HSD_JObjSetTranslateY(jobj, mShots[i].mPos.y);
				HSD_JObjSetTranslateZ(jobj, mShots[i].mPos.z);
				HSD_JObjDispAll(jobj, NULL, 7, 0);
			}
		}
	}
	InvalidateCaptureGraphics(gs);
}

/**
 * @note Address: 0x800E9B7C
 * @note Size: 0x600
 */
void XObjArmy::vfA4(u32 frames)
{
	vfA4__4XObjFUl(this, frames);
	if (mFunc) {
		(this->*mFunc)();
	}
	switch (mAnimId) {
	case 4:
		if (!(mFlags.mFlags & 2) && 20.0f <= GetAnimFrame(mJObj)) {
			SetFloors(this, sFloors[2]);
			mFlags.mFlags |= 2;
		}
		if (50.0f < GetAnimFrame(mJObj)) {
			SetMoveDir(&mMoveTranslate, 1.0f);
		}
		break;
	case 5:
	case 6:
		if (!(mFlags.mFlags & 2) && 15.0f <= GetAnimFrame(mJObj)) {
			SetFloors(this, sFloors[2]);
			mFlags.mFlags |= 2;
		}
		if (50.0f < GetAnimFrame(mJObj)) {
			SetMoveDir(&mMoveTranslate, 1.0f);
		}
		break;
	case 10:
		if (!(mFlags.mFlags & 2) && GetAnimEndFrame(mJObj) <= GetAnimFrame(mJObj)) {
			SetFloors(this, sFloors[1]);
			mFlags.mFlags |= 2;
		}
		break;
	case 17:
		if (!(mFlags.mFlags & 2)) {
			SetFloors(this, sFloors[1]);
			mFlags.mFlags |= 2;
		}
		break;
	case 18:
		if (!(mFlags.mFlags & 2)) {
			SetFloors(this, sFloors[1]);
			mFlags.mFlags |= 2;
		}
		break;
	default:
		if (mFlags.mFlags & 2) {
			SetFloors(this, sFloors[0]);
			mFlags.mFlags &= ~2;
		}
		break;
	}
	vf148();
	for (u32 i = 0; i < 5; i++) {
		fn_800E6B70(&mShots[i], frames);
	}
}

/**
 * @note Address: 0x800EA17C
 * @note Size: 0x144
 */
void XObjArmy::vf98()
{
	vf98__4XObjFv(this);
	sHitCount      = 0;
	sTarget        = NULL;
	sStatus.mFlags = 0;
	mFunc          = NULL;
	if (_258 == 0) {
		sLeader = NULL;
		for (s32 i = 0; sLeader == NULL && i < lbl_8025C400.mXObjs.mObjectCount; i++) {
			XObj* obj = lbl_8025C400.GetObject(i);
			if ((obj->mStatus.mFlags & 1) && obj->mCoord._00->_00 == 0x12A) {
				sLeader = obj;
			}
		}
	}
	switch (GetKind()) {
	case 0xF7:
		mSfx.mEvents = sSfxF7;
		break;
	case 0xF8:
		mSfx.mEvents = sSfxF8;
		break;
	}
	for (s32 i = 0; i < 5; i++) {
		mShots[i].mActive = 0;
	}
}
