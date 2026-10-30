#include "game/objb.h"
#include "game/gmath.h"

// XObjFree, and XObjFlags with the dust, coin and item objects on it, and the hit box

class XObjDust;

extern "C" {
int rand();
double fmod(double x, double y);
s32 fn_8006F390(s32 kind);
BOOL fn_800DED24(XObjDust* obj, u32 frames);
BOOL fn_800DF2D8(XObjDust* obj, u32 frames);
s32 fn_8006EBC8(s32 id);
BOOL fn_8009FDE4(XObj* chr);
void fn_8007EEA4(void* evt, s32 pos, XObj* obj, s32 a, s32 b);
BOOL HSD_AudioSFXKeyOff(s32 vid);
}

// __vt__8XObjFree
class XObjFree : public XObj {
public:
	f32 mTime;  // _980, counts down, the object goes when it runs out
	u8 _984[4]; // _984
	s32 _988;   // _988
};

// __vt__9XObjFlags, keeps 4096 bits of flags
class XObjFlags : public XObj {
public:
	u32* mBits; // _980

	virtual s32 GetFlagNo(); // _148

	u32 IsBit(u32 i) const { return mBits[i >> 5] & (1 << (i & 31)); }
	u32 IsTaken() { return GetFlagNo() > 0 ? IsBit(GetFlagNo()) : 0; }
	void SetBit(u32 i) { mBits[i >> 5] |= 1 << (i & 31); }
};

// __vt__8XObjDust
class XObjDust : public XObjFlags {
public:
	s32 _988;      // _988
	u8 _98C[0x8];  // _98C
	f32 mBaseY;    // _994
	f32 mAngle;    // _998
	f32 mSpeed;    // _99C
	f32 _9A0;      // _9A0
	f32 _9A4;      // _9A4
	f32 _9A8;      // _9A8
	f32 _9AC;      // _9AC
	f32 _9B0;      // _9B0
	u8 _9B4[0x24]; // _9B4
	s16 _9D8;      // _9D8
	s16 _9DA;      // _9DA
};

// __vt__8XObjCoin
class XObjCoin : public XObjFlags {
public:
	f32 mTurn;     // _988
	f32 _98C;      // _98C
	s32 _990;      // _990
	s32 _994;      // _994
	s32 _998;      // _998
	u8 _99C[4];    // _99C
	s32 _9A0[8];   // _9A0
	u8 _9C0[0x28]; // _9C0
	u8 _9E8;       // _9E8
};

// __vt__8XObjItem
class XObjItem : public XObjFlags {
public:
};

// the box an XObjHBox hits with
struct XObjHBoxBody {
	XObj mObj; // _00
};

// __vt__8XObjHBox
class XObjHBox : public XObj {
public:
	s32 mState;         // _980
	f32 _984;           // _984
	XObjHBoxBody mBody; // _988
	f32 _1308;          // _1308
	u8 _130C[4];        // _130C
	f32 _1310;          // _1310
	u32 mSfx;           // _1314
};

// tells the event system something happened to the object, unless the robot is busy
static inline void SendEvt(XObj* obj, s32 msg)
{
	s32 pos = obj->GetSoundPos();
	if (!fn_8009FDE4(lbl_8025C400.mChr)) {
		fn_8007EEA4(lbl_8025C400.mEvt, pos, obj, -1, msg);
	}
}

// how far the stick is pushed
static inline f32 GetStickTilt(const UnkPadStatus* pad)
{
	f32 x = pad->_34;
	f32 y = pad->_38;
	return sqrtf(x * x + y * y);
}

static inline void PlayIdle(XObj* obj)
{
	if (obj->mJObj != NULL && (obj->mFlags & 0x1000ULL)) {
		obj->SetAnim(obj->mAnimSet->mIdle, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
	}
}

// ---- functions ----

extern "C" {

/**
 * @note Address: 0x800DD600
 * @note Size: 0x94
 */
void fn_800DD600(XObjFree* obj, f32 dist)
{
	fn_8008A6C0(obj, dist);
	if (obj->mFlags & 0x4000000ULL) {
		if ((obj->mTime -= dist) <= 0.0f) {
			obj->mFlags |= 0x2000000000000000ULL;
		}
	}
}

/**
 * @note Address: 0x800DE038
 * @note Size: 0x28
 */
void fn_800DE038(XObjFree* obj)
{
	obj->_988 = 0;
	fn_800911C0(obj);
}

/**
 * @note Address: 0x800DE060
 * @note Size: 0xEC
 */
void fn_800DE060(XObjFree* obj)
{
	fn_800928E8(obj);
	if (obj->mInfo != NULL) {
		obj->mTime = obj->GetKind();
	}
	obj->_988 = 0;
	PlayIdle(obj);
}

/**
 * @note Address: 0x800DECA8
 * @note Size: 0x7C
 */
void fn_800DECA8(XObjDust* obj, u32 frames)
{
	if (obj->GetKind() & 2) {
		fn_800908C0(obj, frames);
	} else if (obj->GetKind() & 4) {
		fn_800908C0(obj, frames);
	}
}

/**
 * @note Address: 0x800DE424
 * @note Size: 0x1E4
 */
void fn_800DE424(XObjDust* obj, f32 dist)
{
	if (obj->GetKind() & 1) {
		obj->mFlags |= 0x4000000000000000ULL;
		if (dist >= fn_8006EBC8(0x1F) / 100.0f) {
			obj->_9DA = 2001;
		} else {
			obj->_9DA = 2000;
		}
	} else if (obj->mFlags & 0x0000080000000000ULL) {
		if (dist >= fn_8006EBC8(0x1F) / 100.0f) {
			SendEvt(obj, 2001);
		} else {
			SendEvt(obj, 2000);
		}
	}
}

/**
 * @note Address: 0x800DE608
 * @note Size: 0x1EC
 */
u32 fn_800DE608(XObjDust* obj, s32 a, u32 msg)
{
	if ((msg & 3) == 1) {
		return msg;
	}
	if ((obj->GetKind() & 1) && obj->_9A0 == 0.0f) {
		CPad pad(0);
		obj->_9A0 = DEG2RAD;
		f32 tilt  = GetStickTilt(pad.mStatus);
		f32 scale = (rand() % 400 + 400) / 100.0f;
		obj->_9A4 = scale * tilt;
	}
	return 0;
}

/**
 * @note Address: 0x800DF588
 * @note Size: 0x148
 */
void fn_800DF588(XObjDust* obj, u32 frames)
{
	fn_80090ED4(obj, frames);
	if ((obj->GetKind() & 1) && fn_800DF2D8(obj, frames)) {
		obj->Release();
		obj->SetBit(obj->GetFlagNo());
	}
	if ((obj->GetKind() & 2) && fn_800DED24(obj, frames)) {
		obj->Release();
		obj->SetBit(obj->GetFlagNo());
	}
}

/**
 * @note Address: 0x800DF6D0
 * @note Size: 0x28
 */
void fn_800DF6D0(XObjDust* obj)
{
	obj->_988 = 0;
	fn_800911C0(obj);
}

/**
 * @note Address: 0x800DF9A0
 * @note Size: 0x184
 */
void fn_800DF9A0(XObjDust* obj)
{
	fn_800928E8(obj);
	if (obj->GetKind() & 1) {
		obj->mBaseY = obj->mMoveTranslate.y;
		obj->mAngle = fmod((f32)rand(), TWO_PI);
		obj->mSpeed = (rand() % 200 + 200) / 10000.0f;
		obj->_9A0   = 0.0f;
		obj->_9A4   = 0.0f;
		obj->_9A8   = NAN;
		obj->_9AC   = 0.0f;
		obj->_9B0   = 1.0f;
	}
	obj->_9D8 = -1;
	obj->_9DA = -1;
	obj->_988 = 0;
	PlayIdle(obj);
}

/**
 * @note Address: 0x800DFB24
 * @note Size: 0x44
 */
s32 fn_800DFB24(XObjDust* obj)
{
	s32 n = obj->mCount;
	if (n > 0) {
		s32 base = fn_8006F390(2);
		return base + n;
	}
	return 0;
}

/**
 * @note Address: 0x800DFB68
 * @note Size: 0x100
 */
u32 fn_800DFB68(XObjHBox* obj, s32 a, u32 msg)
{
	if ((msg & 3) == 1) {
		return msg;
	}
	if (obj->mAnimId == 0) {
		return msg;
	}
	lbl_8025C400.mChr->_9A90 = 0.0f;
	obj->mFlags |= 0x4000000000000000ULL;
	obj->_1308 = 1.0f;
	fn_8001E904(GAME_SOUND, 0x11D, 0xFF);
	SendEvt(obj, -1);
	HSD_AudioSFXKeyOff(obj->mSfx);
	return 0;
}

/**
 * @note Address: 0x800DFC68
 * @note Size: 0xBC
 */
void fn_800DFC68(XObjHBox* obj, f32 dist)
{
	if (obj->mAnimId <= 0) {
		fn_8008A6C0(obj, dist);
		if (obj->mFlags & 0x4000000ULL) {
			if (dist >= fn_8006EBC8(0x1F) / 100.0f) {
				obj->mFlags |= 0x2000000000000000ULL;
			}
		}
	}
}

/**
 * @note Address: 0x800DFF14
 * @note Size: 0x48
 */
void fn_800DFF14(XObjHBox* obj, Vec* pos, f32 scale)
{
	if (obj->mBody.mObj.mFlags & 0x10000ULL) {
		fn_8009056C(obj, pos, scale);
	}
}

/**
 * @note Address: 0x800DFF5C
 * @note Size: 0x78
 */
void fn_800DFF5C(XObjHBox* obj, u32 frames)
{
	fn_800908C0(obj, frames);
	if (obj->mBody.mObj.mFlags & 0x10000ULL) {
		obj->mBody.mObj.vfB0(frames);
	}
}

/**
 * @note Address: 0x800E0F40
 * @note Size: 0x4C
 */
void fn_800E0F40(XObjHBox* obj)
{
	obj->mBody.mObj.Release();
	fn_800911C0(obj);
	obj->mState = 0;
}

/**
 * @note Address: 0x800E10D8
 * @note Size: 0x60
 */
void fn_800E10D8(XObjHBox* obj)
{
	fn_800928E8(obj);
	obj->mState = 0;
	obj->_984   = 0.0f;
	obj->mBody.mObj.Reset();
	obj->_1308 = 0.0f;
	obj->_1310 = 5.0f;
}

/**
 * @note Address: 0x800E1138
 * @note Size: 0x30
 */
void fn_800E1138(XObjHBox* obj)
{
	obj->mBody.mObj.vf94();
}

/**
 * @note Address: 0x800E1168
 * @note Size: 0x50
 */
void fn_800E1168(XObjHBox* obj, u32 id)
{
	fn_800929C4(obj, id);
	obj->mBody.mObj.Init(id);
}

/**
 * @note Address: 0x800E13C4
 * @note Size: 0x7C
 */
s32 fn_800E13C4(XObjCoin* obj, s32 n, s32* args)
{
	if (n < 1) {
		return 0;
	}
	obj->_994 = args[0];
	if (n < 2) {
		obj->_998 = 0;
	} else {
		obj->_998 = args[1];
	}
	obj->_9A0[0] = args[2];
	obj->_9A0[1] = args[3];
	obj->_9A0[2] = args[4];
	obj->_9A0[3] = args[5];
	obj->_9A0[4] = args[6];
	obj->_9A0[5] = args[7];
	obj->_9A0[6] = args[8];
	obj->_9A0[7] = args[9];
	return 0;
}

/**
 * @note Address: 0x800E1E60
 * @note Size: 0xD0
 */
BOOL fn_800E1E60(XObjCoin* obj, const CoordInfo& info)
{
	obj->mCount = ((XObjSpawn*)info._00)->mCount;
	if (obj->IsTaken()) {
		return TRUE;
	}
	if (fn_80091C80(obj, info)) {
		obj->mFlags |= 0x10000000ULL;
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x800E1F30
 * @note Size: 0x1C8
 */
void fn_800E1F30(XObjCoin* obj)
{
	fn_800928E8(obj);
	s32 dir        = (rand() & 1) ? 1 : -1;
	f32 turn       = DEG2RAD * (rand() % 360);
	obj->mTurn     = turn * dir;
	HSD_AObj* aobj = GetAObj(obj->mJObj);
	f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
	if (end > 0.0f) {
		obj->SetAnim(obj->mAnimSet->mIdle, TRUE, NULL, NULL, NULL, fmod((f32)rand(), end), 1.0f, 1.0f, 10.0f);
	}
	obj->_98C = 0.0f;
	obj->_990 = 0;
	obj->_994 = 0;
	obj->_9E8 = 0;
}

/**
 * @note Address: 0x800E20F8
 * @note Size: 0x44
 */
s32 fn_800E20F8(XObjCoin* obj)
{
	s32 n = obj->mCount;
	if (n > 0) {
		s32 base = fn_8006F390(1);
		return base + n;
	}
	return 0;
}

/**
 * @note Address: 0x800E213C
 * @note Size: 0xC0
 */
// NONMATCHING: obj and result swap saved registers
BOOL fn_800E213C(XObjItem* obj, const CoordInfo& info)
{
	BOOL result = fn_80091C80(obj, info);
	if (result) {
		if (obj->IsTaken()) {
			obj->mFlags &= ~0x10000ULL;
		}
	}
	return result;
}

/**
 * @note Address: 0x800E21FC
 * @note Size: 0x44
 */
s32 fn_800E21FC(XObjItem* obj)
{
	s32 n = obj->mCount;
	if (n > 0) {
		s32 base = fn_8006F390(0);
		return base + n;
	}
	return 0;
}

/**
 * @note Address: 0x800E2300
 * @note Size: 0x9C
 */
void fn_800E2300(XObjFlags* obj)
{
	u32* bits = obj->mBits;
	for (int i = 0; i < 128; i++) {
		bits[i] = 0;
	}
}

XObjHBoxBody* fn_800E240C(XObjHBoxBody* body, s16 del);

/**
 * @note Address: 0x800E240C
 * @note Size: 0x54
 */
XObjHBoxBody* fn_800E240C(XObjHBoxBody* body, s16 del)
{
	if (body) {
		fn_8000877C(&body->mObj, -1);
		if (del > 0) {
			operator delete(body);
		}
	}
	return body;
}

} // extern "C"
