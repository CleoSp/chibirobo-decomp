#include "game/objb_leticker.h"

// XObjArmy, the toy soldiers, and the shots they fire

// the robot's fields these read, game/objb.h has the robot
struct UnkChrMode {
	u8 _00[0x95D0]; // _00
	s32 _95D0;      // _95D0
	s32 _95D4;      // _95D4

	s32 GetMode() const { return _95D4 >= 0 ? _95D4 : _95D0; }
};

// one shot a soldier fires, it flies at the robot or the leader and drops when it hits
class ArmyShot {
public:
	s32 mActive; // _00
	s32 mLife;   // _04, frames left
	CVec mPos;   // _08
	CVec mVel;   // _18
	f32 mSpeed;  // _28
	s32 mPhase;  // _2C, 0 flying, 1 bouncing off, 2 on the floor

	void Update(u32 frames);

	void Move()
	{
		mPos.x += mSpeed * mVel.x;
		mPos.y += mSpeed * mVel.y;
		mPos.z += mSpeed * mVel.z;
	}
};

// nothing reads it
static s32 sUnused[26] = { 0, 3, 0, 0, 3, 0, 0, 2, 0, 0, 2, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 3, 0, 0, 3, 0 };

// the points the soldier stands on, upright, falling over and lying down
static XObjPoint sPointsStand[2] = {
	{ 0, 0, { 0, 0x226, 0 }, 0, { 0x200, 0x200, 0x200 } },
	{ -1 },
};
static XObjPoint sPointsFall[2] = {
	{ 0, 0, { 0, 0x100, -0xD4 }, 0, { 0x200, 0x100, 0x200 } },
	{ -1 },
};
static XObjPoint sPointsDown[2] = {
	{ 0, 0, { 0, 0x100, 0 }, 0, { 0x200, 0x100, 0x200 } },
	{ -1 },
};
static XObjPoint* sPoints[3] = { sPointsStand, sPointsFall, sPointsDown };

static AnimSfxEntry sSfxF7[] = {
	{ 0x02, 18.0f, 36.0f, 1, 0x11000E, 0xFF, 4, 0, 0 }, { 0x03, 14.0f, 27.0f, 1, 0x11000E, 0xFF, 4, 0, 0 },
	{ 0x11, 11.0f, 22.0f, 1, 0x11000E, 0xFF, 4, 0, 0 }, { 0x12, 5.0f, -1.0f, 1, 0x11000D, 0xFF, 4, 0, 0 },
	{ 0x0F, 0.0f, -1.0f, 1, 0x11001C, 0xFF, 0, 0, 0 },  { 0x07, 28.0f, -1.0f, 1, 0x110028, 0xFF, 0, 0, 0 },
	{ 0x28, 18.0f, 36.0f, 1, 0x11000E, 0xFF, 0, 0, 0 }, { -1 },
};
static AnimSfxEntry sSfxF8[] = {
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

// __vt__8XObjArmy
class XObjArmy : public XObj {
public:
	typedef void (XObjArmy::*StateFunc)();

	StateFunc mState;   // _980
	s32 mStep;          // _98C
	s32 mTimer;         // _990
	f32 _994;           // _994
	u8 _998[0x8];       // _998
	CStatus mStatus;    // _9A0
	CVec mGuardPos;     // _9A8
	s32 mMode;          // _9B8
	ArmyShot mShots[5]; // _9BC
	AnimSfx mSfx;       // _AAC
	u8 _ABC[0x4];       // _ABC

	virtual void Reset();                      // _98
	virtual void Update(u32 frames);           // _A4
	virtual void vfC0();                       // _C0
	virtual void vf138(const Vec* pos, f32 f); // _138
	virtual void vf148();                      // _148

	void Attack()
	{
		mStep  = 0;
		mState = &XObjArmy::State81A0;
	}

	// what the soldier does, 1 marches, 2 stands guard
	void SetMode(s32 mode)
	{
		mMode = mode;
		RestartMode();
	}

	void RestartMode()
	{
		mStep = 0;
		switch (mMode) {
		case 1:
			mFlags |= 8;
			mState = &XObjArmy::State86F8;
			break;
		case 2:
			mState = &XObjArmy::State77F4;
			break;
		default:
			mState = NULL;
			break;
		}
	}

	void Shoot();

	void State77F4();
	void State7DDC();
	void State81A0();
	void State8564();
	void State86F8();
};
typedef char XObjArmy_size_check[sizeof(XObjArmy) == 0xAC0 ? 1 : -1];

static Vec sMin;
static Vec sMax;
static XObj* sLeader;
static s32 sB2A4;
static s32 sB2A8;
static XObj* sTarget;
static CStatus sStatus;

extern "C" {
s32 fn_8006EBC8(s32 id);
f32 HSD_Randf(void);
}

// ---- functions ----

/**
 * @note Address: 0x800E6A5C
 * @note Size: 0x114
 */
// NONMATCHING: pan, the anim id and the jobj take other saved registers
void XObjArmy::vf148()
{
	s32 pan;
	if (mStatus.mFlags & 0x100) {
		pan = 0xFF;
	} else {
		pan = fn_8001DF00(GAME_SOUND, &mMoveTranslate, 0);
	}
	HSD_JObj* jobj = mJObj;
	s32 anim       = mAnimId;
	mSfx.Play(anim, pan, GetAnimFrame(jobj));
}

/**
 * @note Address: 0x800E6B70
 * @note Size: 0x308
 */
void ArmyShot::Update(u32 frames)
{
	if (mActive) {
		CVec target;
		switch (mPhase) {
		case 0: {
			if (--mLife == 0) {
				mActive = 0;
			}
			Move();
			f32 range;
			if (sTarget != NULL) {
				target = sTarget->mMoveTranslate;
				range  = 28.0f;
			} else {
				XObj* player = GAME_PLAYER;
				target       = player->mMoveTranslate;
				if (((UnkChrMode*)player)->GetMode() != 6) {
					range = 10.0f;
				} else {
					range = 28.0f;
				}
			}
			f32 dy = target.y - mPos.y;
			if (dy < 0.0f) {
				dy = -dy;
			}
			if (dy < range) {
				target.y = mPos.y;
				if (PSVECDistance(&mPos, &target) < 5.0f) {
					Vec pos = mPos;
					fn_8001E904(GAME_SOUND, 0x110029, 0xFF);
					sStatus.mFlags |= 1;
					GAME_PLAYER->vf138(&pos, 10.0f);
					mVel.x = -1.0f * mVel.x;
					mVel.y = -1.0f;
					mVel.z = -1.0f * mVel.z;
					mSpeed = 0.5f;
					mPhase++;
				}
			}
			break;
		}
		case 1:
			Move();
			mVel.y += -0.15f;
			if (mPos.y <= 1.5f) {
				mVel.y = 0.0f;
				mSpeed = 0.3f;
				mLife  = 10;
				mPhase++;
			}
			break;
		case 2:
			if (--mLife == 0) {
				mActive = 0;
			}
			Move();
			break;
		}
	}
}

/**
 * @note Address: 0x800E6E78
 * @note Size: 0x158
 */
void XObjArmy::vf138(const Vec* pos, f32 f)
{
	if (!(mStatus.mFlags & 1)) {
		_994 = 0.0f;
		fn_8001E904(GAME_SOUND, 0x83, 0xFF);
		if (_994 <= 0.0f) {
			if (fn_8006EBC8(0x1F) / 100.0f <= f || PSVECDistance(&mMoveTranslate, &GAME_PLAYER->mMoveTranslate) < 20.0f) {
				mStep  = 0;
				mState = &XObjArmy::State7DDC;
			} else {
				mStep  = 0;
				mState = &XObjArmy::State81A0;
			}
		} else {
			mStep  = 0;
			mState = &XObjArmy::State8564;
		}
	}
}

extern "C" {

/**
 * @note Address: 0x800E6FD0
 * @note Size: 0x560
 */
s32 fn_800E6FD0(XObjArmy* obj, s32 argc, s32* args)
{
	if (argc < 1) {
		return -1;
	}
	switch (args[0]) {
	case 0:
		sB2A8  = 0;
		sMin.x = args[1] / 100.0f;
		sMin.y = args[2] / 100.0f;
		sMin.z = args[3] / 100.0f;
		sMax.x = args[4] / 100.0f;
		sMax.y = args[5] / 100.0f;
		sMax.z = args[6] / 100.0f;
		break;
	case 1:
		switch (args[1]) {
		case 0:
			return sB2A8;
		case 5:
			return sStatus.mFlags & 1;
		default:
			return sB2A8;
		}
	case 2:
		switch (args[1]) {
		case 0: {
			Vec pos;
			pos.x = sMin.x + (sMax.x - sMin.x) * HSD_Randf();
			pos.y = sMin.y + (sMax.y - sMin.y) * HSD_Randf();
			pos.z = sMin.z + (sMax.z - sMin.z) * HSD_Randf();
			obj->SetMoveTranslate(&pos, 0);
			obj->mFlags |= 0x10000;
			obj->SetMode(1);
			break;
		}
		case 1:
			obj->Attack();
			break;
		case 2:
			obj->mStep  = 0;
			obj->mState = NULL;
			break;
		case 3:
			if (argc >= 2) {
				if (args[2] >= 0) {
					sTarget = GAME_OBJTABLE[args[2]];
				} else {
					sTarget = NULL;
				}
			}
			break;
		case 4:
			if (argc >= 3) {
				if (args[2]) {
					obj->mStatus.mFlags &= ~0x80;
				} else {
					obj->mStatus.mFlags |= 0x80;
				}
			} else {
				obj->mStatus.mFlags &= ~0x80;
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
		obj->SetMode(2);
		break;
	case 100:
		break;
	case 0x65:
		if (args[1] == 0) {
			obj->mStatus.mFlags &= ~0x100;
		} else {
			obj->mStatus.mFlags |= 0x100;
		}
		break;
	default:
		break;
	}
	return 0;
}

} // extern "C"

/**
 * @note Address: 0x800E7530
 * @note Size: 0x2C4
 */
// NONMATCHING: the joint walk swaps two registers
void XObjArmy::Shoot()
{
	CVec pos;
	CVec dir;
	s32 path[6] = { 0, 0, 4, 0, 0, 0 };
	FindJoint(mJObj, path, 6);
	pos        = mMoveTranslate;
	Vec facing = { 0.0f, 0.0f, 0.0f };
	facing.x   = sin(HSD_JObjGetRotationY(mJObj));
	facing.z   = cos(HSD_JObjGetRotationY(mJObj));
	dir        = CVec(facing);
	u32 i;
	for (i = 0; mShots[i].mActive && i < 5; i++) { }
	if (i < 5) {
		u8 quiet          = (mStatus.mFlags >> 7) & 1;
		mShots[i].mActive = 1;
		mShots[i].mLife   = 120;
		mShots[i].mPos    = pos;
		mShots[i].mPos.y += 6.0f;
		mShots[i].mVel = dir;
		PSVECNormalize(&mShots[i].mVel, &mShots[i].mVel);
		mShots[i].mSpeed = 3.0f;
		mShots[i].mPhase = 0;
		if (!quiet) {
			if (sB2A4 < 0 || sB2A4 >= 4) {
				sB2A4 = 0;
			}
			fn_8001E904(GAME_SOUND, sB2A4 + 0x110012, 0xFF);
			sB2A4++;
		}
	}
}

// Initialize a shot from the soldier's position and firing direction.
static inline void FireShot(ArmyShot* shot, const CVec& pos, const CVec& dir, u8 quiet)
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
		if (sB2A4 < 0 || sB2A4 >= 4) {
			sB2A4 = 0;
		}
		fn_8001E904(GAME_SOUND, sB2A4 + 0x110012, 0xFF);
		sB2A4++;
	}
}

/**
 * @note Address: 0x800E77F4
 * @note Size: 0x5E8
 */
void XObjArmy::State77F4()
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
				FireShot(&mShots[i], mMoveTranslate, mGuardPos - mMoveTranslate, (mStatus.mFlags & 0x80) != 0);
			}
			mStep++;
		}
		break;
	case 2:
		if (GetAnimEndFrame(mJObj) <= 2.0f + GetAnimFrame(mJObj)) {
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			SetMode(0);
		}
		break;
	}
}

/**
 * @note Address: 0x800E7DDC
 * @note Size: 0x3C4
 */
void XObjArmy::State7DDC()
{
	CVec pos;
	switch (mStep) {
	case 0: {
		mStatus.mFlags |= 1;
		sB2A8++;
		f32 angle = GetAngleY(mMoveTranslate, GAME_PLAYER->mMoveTranslate);
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
void XObjArmy::State81A0()
{
	CVec pos;
	switch (mStep) {
	case 0: {
		mStatus.mFlags |= 1;
		sB2A8++;
		f32 angle = GetAngleY(mMoveTranslate, GAME_PLAYER->mMoveTranslate);
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
void XObjArmy::State8564()
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
			RestartMode();
		}
		break;
	}
}

/**
 * @note Address: 0x800E980C
 * @note Size: 0x370
 */
// NONMATCHING: this, the index and the jobj take other saved registers, pos and rot swap stack slots
void XObjArmy::vfC0()
{
	UnkGameRender* game = (UnkGameRender*)lbl_8025C400;
	for (u32 i = 0; i < 5; i++) {
		if (mShots[i].mActive) {
			if (sLeader == NULL) {
				game->_29C = 1;
				Vec pos    = mShots[i].mPos;
				fn_8019B56C(game->_290, 1, 1, 1, 15);
				Vec rot = { 0.0f, 0.0f, 0.0f };
				fn_80198258(lbl_8025C400, 16, 0x64FF64C8, 0xFF, pos.x, pos.y, pos.z, 3.0f, 3.0f, rot.x, rot.y, rot.z);
				game->_29C = 0;
			} else {
				HSD_JObj* jobj = sLeader->mJObj;
				HSD_JObjSetTranslateX(jobj, mShots[i].mPos.x);
				HSD_JObjSetTranslateY(jobj, mShots[i].mPos.y);
				HSD_JObjSetTranslateZ(jobj, mShots[i].mPos.z);
				HSD_JObjDispAll(jobj, NULL, 7, 0);
			}
		}
	}
	UnkGameRender* game2 = (UnkGameRender*)lbl_8025C400;
	HSD_StateInvalidate(-1);
	if (game->_290 != NULL) {
		UnkRenderA* a = game2->_290;
		if (a->_0C == 0) {
			a->_0C = 1;
		}
		if (a->_10 == 0) {
			a->_10 = 1;
		}
		if (a->_14 == 0) {
			a->_14 = 1;
		}
		if (a->_18 == 0) {
			a->_18 = 1;
		}
	}
	UnkRenderB* b = game2->_294;
	if (b != NULL) {
		if (b->_74 == 0) {
			b->_74 = 1;
		}
		if (b->_78 == 0) {
			b->_78 = 1;
		}
		if (b->_7C == 0) {
			b->_7C = 1;
		}
	}
	UnkRenderC* c = game2->_298;
	if (c != NULL) {
		if (c->_30 == 0) {
			c->_30 = 1;
		}
	}
}

/**
 * @note Address: 0x800E9B7C
 * @note Size: 0x600
 */
void XObjArmy::Update(u32 frames)
{
	fn_80090ED4(this, frames);
	if (mState) {
		(this->*mState)();
	}
	switch (mAnimId) {
	case 4:
		if (!(mStatus.mFlags & 2) && 20.0f <= GetAnimFrame(mJObj)) {
			SetPoints(this, sPoints[2]);
			mStatus.mFlags |= 2;
		}
		if (50.0f < GetAnimFrame(mJObj)) {
			SetMoveDir(&mMoveTranslate, 1.0f);
		}
		break;
	case 5:
	case 6:
		if (!(mStatus.mFlags & 2) && 15.0f <= GetAnimFrame(mJObj)) {
			SetPoints(this, sPoints[2]);
			mStatus.mFlags |= 2;
		}
		if (50.0f < GetAnimFrame(mJObj)) {
			SetMoveDir(&mMoveTranslate, 1.0f);
		}
		break;
	case 10:
		if (!(mStatus.mFlags & 2) && GetAnimEndFrame(mJObj) <= GetAnimFrame(mJObj)) {
			SetPoints(this, sPoints[1]);
			mStatus.mFlags |= 2;
		}
		break;
	case 17:
		if (!(mStatus.mFlags & 2)) {
			SetPoints(this, sPoints[1]);
			mStatus.mFlags |= 2;
		}
		break;
	case 18:
		if (!(mStatus.mFlags & 2)) {
			SetPoints(this, sPoints[1]);
			mStatus.mFlags |= 2;
		}
		break;
	default:
		if (mStatus.mFlags & 2) {
			SetPoints(this, sPoints[0]);
			mStatus.mFlags &= ~2;
		}
		break;
	}
	vf148();
	for (u32 i = 0; i < 5; i++) {
		mShots[i].Update(frames);
	}
}

/**
 * @note Address: 0x800EA17C
 * @note Size: 0x144
 */
// NONMATCHING: the leader search takes other registers
void XObjArmy::Reset()
{
	fn_800928E8(this);
	sB2A8          = 0;
	sTarget        = NULL;
	sStatus.mFlags = 0;
	mState         = NULL;
	if (_258 == 0) {
		sLeader = NULL;
		for (s32 i = 0; sLeader == NULL && i < ((UnkGameObjs*)lbl_8025C400)->mObjectCount; i++) {
			XObj* obj = GAME_OBJS[i];
			if ((obj->mFlags & 1) && *(s32*)obj->_238 == 0x12A) {
				sLeader = obj;
			}
		}
	}
	switch (GetDescKind()) {
	case 0xF7:
		mSfx.mTable = sSfxF7;
		break;
	case 0xF8:
		mSfx.mTable = sSfxF8;
		break;
	}
	for (s32 i = 0; i < 5; i++) {
		mShots[i].mActive = 0;
	}
}
