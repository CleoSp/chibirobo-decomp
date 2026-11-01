#include "game/objb_leticker.h"

// XObjLeticker, and the camera that turns around it in steps, CCameraActionLeticker

class CCamera;

// __vt__12XObjLeticker
class XObjLeticker : public XObj {
public:
	typedef void (XObjLeticker::*StateFunc)();

	StateFunc mState;  // _980
	s32 mStep;         // _98C
	s32 mParts[2];     // _990, indices in the game's object table
	s32 mPartCount;    // _998
	s32 _99C;          // _99C
	s32 _9A0;          // _9A0
	s32 _9A4[4];       // _9A4
	s32 mStateTime;    // _9B4
	XObjPoint mBox[2]; // _9B8, the point the parts stand on
	f32 _9F8;          // _9F8
	f32 _9FC;          // _9FC
	f32 _A00;          // _A00
	Vec _A04;          // _A04
	u8 _A10[0x14];     // _A10
	u8 _A24;           // _A24
	u8 _A25[0x3];      // _A25
	f32 _A28;          // _A28
	u8 mParticleOn;    // _A2C
	u8 _A2D;           // _A2D
	u8 _A2E[0x2];      // _A2E
	s32 _A30;          // _A30
	f32 _A34;          // _A34
	s32 mMode;         // _A38
	AnimSfx mSfx;      // _A3C
	u8 _A4C[0x4];      // _A4C

	virtual void Reset();                      // _98
	virtual void Update(u32 frames);           // _A4
	virtual void vfAC();                       // _AC
	virtual void vfC0();                       // _C0
	virtual void vf138(const Vec* pos, f32 f); // _138
	virtual void vf148();                      // _148

	BOOL TurnParts(f32 angle);

	void StateLift();
	void StateCount();
	void StateRide();
};
typedef char XObjLeticker_size_check[sizeof(XObjLeticker) == 0xA50 ? 1 : -1];

// __vt__21CCameraActionLeticker, turns around the leticker a quarter at a time
class CCameraActionLeticker : public CCameraActionLadr {
public:
	Vec* mTarget;   // _44
	f32 mTurnTo;    // _48
	f32 mTurnSpeed; // _4C
	s32 mTurnTime;  // _50
	u8 mBounded;    // _54
	f32 mMinX;      // _58
	f32 mMaxX;      // _5C
	f32 mMinZ;      // _60
	f32 mMaxZ;      // _64
	u8 _68[0x4];    // _68

	CCameraActionLeticker() { mTarget = NULL; }

	virtual void Update(u32 step);      // _10
	virtual void vf14();                // _14
	virtual u32 vf50() { return 0; }    // _50
	virtual u32 Get_38() { return 11; } // _54
	virtual f32 vf58() { return 0.1f; } // _58
	virtual Vec GetCameraTarget();      // _64

	// whether the eye at that angle stays inside the room
	bool CanTurn(f32 angle)
	{
		if (!mBounded) {
			return true;
		}
		Vec eye;
		CalcEye(&eye, angle);
		OSReport("%4.2f %4.2f\n", eye.x, eye.z);
		if (eye.x < mMinX || mMaxX < eye.x || eye.z < mMinZ || mMaxZ < eye.z) {
			OSReport("CCameraActionLeticker::Update %4.2f %4.2f %d\n", eye.x, eye.z, (s32)(57.295776f * mTurnTo));
			return false;
		}
		return true;
	}
};
typedef char CCameraActionLeticker_size_check[sizeof(CCameraActionLeticker) == 0x6C ? 1 : -1];

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

static AnimSfxEntry sSfx[] = {
	{ 7, 10.0f, -1.0f, 1, 0x190, 0xFF, 0, 0, 0 },
	{ -1 },
};

extern "C" XObjPoint* lbl_8022A008[];
extern "C" bool fn_800E4790(XObj* obj, HSD_JObj* jobj);
extern "C" XObjPoint* lbl_8065A6D0[2];

// ---- functions ----

/**
 * @note Address: 0x800E260C
 * @note Size: 0x3AC
 */
// NONMATCHING: this and part swap saved registers
BOOL XObjLeticker::TurnParts(f32 angle)
{
	XObj* part = GAME_OBJTABLE[mParts[0]];
	f32 my     = HSD_JObjGetRotationY(mJObj);
	f32 c      = cos(HSD_JObjGetRotationY(part->mJObj) - (angle + my));
	if (c < (f32)cos(0.08726646f)) {
		_9F8 = 0.0f;
		_A30 = 0;
		if (part->mAnimId == 0) {
			part->SetRotateY(angle + HSD_JObjGetRotationY(mJObj), 0.1f);
			_A28 = angle;
		}
	}
	if (GetAnimFrame(part->mJObj) < GetAnimEndFrame(part->mJObj)) {
		return TRUE;
	}
	s32 path[9]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	HSD_JObj* jobj = FindJoint(part->mJObj, path, _99C + 1);
	if (jobj != NULL) {
		MtxPtr mtx = HSD_JObjGetMtxPtr(jobj);
		Vec pos;
		pos.x = pos.y = pos.z = 0.0f;
		PSMTXMultVec(mtx, &pos, &_A04);
	}
	return TRUE;
}

extern "C" {

/**
 * @note Address: 0x800E2EAC
 * @note Size: 0x1A4
 */
// NONMATCHING: the loop index, pointer and part take other saved registers
void fn_800E2EAC(XObjLeticker* obj, Vec* dir)
{
	XObj* part;
	for (s32 i = 0; i < obj->mPartCount; i++) {
		part = GAME_OBJS[obj->mParts[i]];
		part->SetTranslateX(obj->mMoveTranslate.x, 0);
		part->SetTranslateZ(obj->mMoveTranslate.z, 0);
	}
	if (0.0f < PSVECMag(dir)) {
		obj->SetColor(1, 0, 0, 0, 0.5f);
		obj->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	} else {
		obj->SetColor(1, 0xFF, 0, 0, 1.0f);
		obj->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
	if (obj->mParticleOn) {
		GAME_PARTICLE->Kill(0xA5);
		obj->mParticleOn = FALSE;
	}
}

} // extern "C"

/**
 * @note Address: 0x800E3050
 * @note Size: 0x1DC
 */
void XObjLeticker::vf148()
{
	if (mMode != 3) {
		GAME_FLAGS &= ~0x80ULL;
		GAME_CAMERA->PopAction();
	}
	_A2D = FALSE;
	_A30 = 0;
	if (mMode == 2) {
		_9A0 = _9A4[GetSide(_A28)];
		_9FC = 10.0f * _9A0;
		_9F8 = 10.0f * _99C;
	}
	SetColor(1, 0, 0, 0, 0.5f);
	SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800E3AF0
 * @note Size: 0x4
 */
void XObjLeticker::vf138(const Vec* pos, f32 f)
{
}

/**
 * @note Address: 0x800E54E0
 * @note Size: 0x1A8
 */
void XObjLeticker::StateRide()
{
	XObj* player   = GAME_PLAYER;
	s32 path[2]    = { 0, 9 };
	HSD_JObj* jobj = FindJoint(mJObj, path, 2);
	switch (mStep) {
	case 0:
		break;
	case 10:
		Vec pos;
		pos.x = pos.y = pos.z = 0.0f;
		pos.y                 = 0.5f;
		PSMTXMultVec(HSD_JObjGetMtxPtr(jobj), &pos, &pos);
		player->SetMoveTranslate(&pos, 0);
		player->SetRotationY(player->mMoveRotate.y - 0.5235988f, 0);
		break;
	}
}

extern "C" {

/**
 * @note Address: 0x800E4AAC
 * @note Size: 0x3AC
 */
s32 fn_800E4AAC(XObj* obj, const Vec* pos)
{
	f32 angle = WrapAngle((f32)atan2(pos->x - obj->mMoveTranslate.x, pos->z - obj->mMoveTranslate.z) - obj->mMoveRotate.y);
	if (3.0f + obj->mMoveTranslate.y < GAME_PLAYER->mMoveTranslate.y) {
		return 0;
	}
	if (Cos(0.7853982f) < Cos(angle)) {
		s32 path[2] = { 0, 2 };
		if (fn_800E4790(obj, FindJoint(obj->mJObj, path, 2))) {
			return -1;
		}
	} else if (Cos(angle) < Cos(3.9269907f)) {
		s32 path[2] = { 0, 6 };
		if (fn_800E4790(obj, FindJoint(obj->mJObj, path, 2))) {
			return -2;
		}
	} else if (0.0f < Sin(angle)) {
		s32 path[2] = { 0, 4 };
		if (fn_800E4790(obj, FindJoint(obj->mJObj, path, 2))) {
			return -3;
		}
	} else {
		s32 path[2] = { 0, 8 };
		if (fn_800E4790(obj, FindJoint(obj->mJObj, path, 2))) {
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
// NONMATCHING: part and the two loop pointers take other saved registers
void XObjLeticker::vfC0()
{
	XObj* part = GAME_OBJTABLE[mParts[0]];
	switch (mMode) {
	case 3:
		if (!(0.0f < _9F8) || (!_A2D && part->mAnimId != 1)) {
			break;
		}
		Mtx mtx;
		u32 colors[50];
		Vec pts[50];
		Vec pos = mMoveTranslate;
		PSMTXTrans(mtx, pos.x, pos.y, pos.z);
		f32 twist = 6.0f * (_9F8 / _9FC) * (f32)sin(0.017453292f * (mStateTime * 5)) + 72.0f;
		u32 i;
		for (i = 0; i < 50; i++) {
			f32 angle = 0.017453292f * mStateTime + 0.017453292f * (twist * (s32)i);
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
		fn_80198F1C(lbl_8025C400, pts, colors, 50);
		if (!fn_8001E6C8(GAME_SOUND, 0xB0)) {
			fn_8001E904(GAME_SOUND, 0xB0, 0xFF);
			HSD_AudioSFXSetPitchByID(0xB0, 600.0f * (_A34 - 1.0f));
		} else {
			fn_8001E468(GAME_SOUND, 0xB0, fn_8001DF00(GAME_SOUND, &part->mMoveTranslate, 0), 0);
		}
		break;
	}
}

/**
 * @note Address: 0x800E51BC
 * @note Size: 0x324
 */
// NONMATCHING: the dol has one more branch to the end of the switch
void XObjLeticker::vfAC()
{
	vfB0(0);
	switch (mMode) {
	case 1: {
		XObj* part     = GAME_OBJTABLE[mParts[0]];
		s32 path[8]    = { 0, 0, 0, 0, 0, 0, 0, 0 };
		HSD_JObj* jobj = FindJoint(part->mJObj, path, 7);
		HSD_JObjClearFlags(jobj, 0x10);
		f32 y;
		for (s32 i = 0; 0.0f < (y = _A00 - 24.0f * i); i++) {
			HSD_JObjSetTranslateY(jobj, y);
			HSD_JObjDispAll(jobj, NULL, 7, 0);
		}
		HSD_JObjSetFlags(jobj, 0x10);
		HSD_JObjSetTranslateY(jobj, 0.0f);
		break;
	}
	case 2:
		GAME_OBJTABLE[mParts[0]]->mFlags &= ~0x80000000ULL;
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
	part = GAME_OBJTABLE[mParts[0]];
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
					PushMsg(part, 0, 0, 100);
				}
			} else {
				_9F8 = (_99C - 1) * 10;
			}
			if (_A2D) {
				fn_8001E904(GAME_SOUND, 0xAC, 0xFF);
				HSD_AudioSFXSetPitchByID(0xAC, 600.0f * (_A34 - 1.0f));
			}
		}
		part->mPoints       = lbl_8022A008[_99C];
		part->mPointMaskOld = 0;
		part->mPointMask    = 0;
		fn_8008D220(part);
		fn_8008C8A4(part);
	}
	if (!(mFlags & 0x0800000000000000ULL)) {
		SetColor(1, 0, 0, 0, 0.5f);
		SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
}

/**
 * @note Address: 0x800E5A98
 * @note Size: 0x3E0
 */
void XObjLeticker::StateLift()
{
	XObj* base = GAME_OBJTABLE[mParts[0]];
	XObj* top  = GAME_OBJTABLE[mParts[1]];
	if (base == NULL || top == NULL) {
		return;
	}
	f32 height = 4.0f * (s32)(_9F8 / 3.0f) + 0.5f;
	if (_A00 == height) {
		_A24 = TRUE;
	} else if (_A00 < height) {
		_A00 += 0.5f;
		if (height < _A00) {
			_A00 = height;
		}
	} else if (height < _A00 && 0.0f < _A00) {
		_A00 -= 0.5f;
		if (_A00 < 0.0f) {
			_A00 = 0.0f;
		}
	}
	if (_A00 <= 2.5f) {
		_A00 = 2.5f;
	}
	s32 step = _A00 / 4.0f;
	if (_99C != step) {
		if (!fn_8001E6C8(GAME_SOUND, 0xA8)) {
			fn_8001E904(GAME_SOUND, 0xA8, 0xFF);
		}
		HSD_AudioSFXSetPitchByID(0xA8, 600.0f * (_A34 - 1.0f));
		_99C = step;
		_A24 = FALSE;
	}
	top->SetTranslateY(9.0f + (_A00 + HSD_JObjGetTranslationY(mJObj)), 0);
	_A04.x = top->mMoveTranslate.x;
	_A04.y = top->mMoveTranslate.y;
	_A04.z = top->mMoveTranslate.z;
	if (mFlags & 0x0800000000000000ULL) {
		f32 h;
		if (_A00 < 10.0f) {
			h = 19.0f;
		} else {
			h = 5.0f + (9.0f + _A00) - 2.0f;
		}
		lbl_8065A6D0[0]->mPos2[1] = (s32)h * 100;
		lbl_8065A6D0[0]->mPos[1]  = (s32)(h / sqrtf(3.0f)) * 100;
		mPoints                   = lbl_8065A6D0[0];
		mPointMaskOld             = 0;
		mPointMask                = 0;
		fn_8008D220(this);
		fn_8008C8A4(this);
	} else {
		s32 k               = (_A00 - 12.5f) / 4.0f;
		mBox[0].mPos[1]     = k * 0xA9 + 0x654;
		mBox[0].mPos2[1]    = k * 400 + 1000;
		base->mPoints       = mBox;
		base->mPointMaskOld = 0;
		base->mPointMask    = 0;
		fn_8008C8A4(base);
		mPoints       = lbl_8065A6D0[1];
		mPointMaskOld = 0;
		mPointMask    = 0;
		fn_8008D220(this);
		fn_8008C8A4(this);
		SetColor(1, 0, 0, 0, 0.5f);
		SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
}

static inline void PlayLetickerSound(XObjLeticker& obj)
{
	s32 anim  = obj.mAnimId;
	f32 frame = GetAnimFrame(obj.mJObj);
	obj.mSfx.Play(anim, fn_8001DF00(GAME_SOUND, &obj.mMoveTranslate, 0), frame);
}

/**
 * @note Address: 0x800E5E78
 * @note Size: 0x598
 */
void XObjLeticker::Update(u32 frames)
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
	mStateTime += frames;
	if (mState) {
		(this->*mState)();
	}
	s32 path[3]    = { 0, 0, 0 };
	HSD_JObj* jobj = FindJoint(mJObj, path, 2);
	XObj* player   = GAME_PLAYER;
	f32 angle      = atan2(player->mMoveTranslate.x - mMoveTranslate.x, player->mMoveTranslate.z - mMoveTranslate.z);
	HSD_JObjSetRotationY(jobj, angle - HSD_JObjGetRotationY(mJObj));
	PlayLetickerSound(*this);
}

/**
 * @note Address: 0x800E6410
 * @note Size: 0x98
 */
void XObjLeticker::Reset()
{
	fn_800928E8(this);
	mPartCount    = 0;
	mState        = NULL;
	_99C          = 0;
	mStateTime    = 0;
	_9F8          = 0.0f;
	_A00          = 0.0f;
	mParticleOn   = TRUE;
	_A2D          = FALSE;
	_A30          = 0;
	mMode         = 0;
	_A28          = 0.0f;
	mBox[0].mKind = -1;
	mBox[1].mKind = -1;
	mSfx.mTable   = sSfx;
}

/**
 * @note Address: 0x800E64A8
 * @note Size: 0x2E8
 */
void CCameraActionLeticker::Update(u32 step)
{
	CPad pad(0);
	u64 buttons     = GAME_FLAGS & 0x80000000;
	u32 leftButton  = buttons ? HSD_PAD_DPADLEFT : HSD_PAD_CSTICK_LEFT;
	u32 rightButton = buttons ? HSD_PAD_DPADRIGHT : HSD_PAD_CSTICK_RIGHT;

	if (mTurnTime == 0) {
		if (pad.mPad->mTrigger & leftButton) {
			if (CanTurn(mTurnTo - 1.5707964f)) {
				mTurnTo -= 1.5707964f;
				mTurnSpeed = -0.08726646f;
				mTurnTime  = 18;
			}
		} else if (pad.mPad->mTrigger & rightButton) {
			if (CanTurn(1.5707964f + mTurnTo)) {
				mTurnTo += 1.5707964f;
				mTurnSpeed = 0.08726646f;
				mTurnTime  = 18;
			}
		}
	} else {
		mTurnTime -= step;
		if (mTurnTime <= 0) {
			mAngle    = mTurnTo;
			mTurnTime = 0;
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
	OSReport("CCameraActionLeticker::Reset\n");
	CCameraActionLadr::vf14();
	mTurnTo   = 0.0f;
	mTurnTime = 0;
	mBounded  = FALSE;
	GAME_FLAGS |= 0x80;
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
