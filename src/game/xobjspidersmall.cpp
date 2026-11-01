#include "game/particle_srt.h"
#include "game/scr.h"
#include <stdlib.h>

// XObjSpiderSmall, the small spiders. the class name is from its vtable, nothing else has a
// name in the dol, so the methods that aren't virtual are extern "C" under their addresses.
// offsets are usa

// plays the sfx of an object's animations as the frames go by, __vt__7AnimSfx. its
// destructor is trivial, the owner's never resets its vtable
class AnimSfx {
public:
	const void* mTable; // _00
	s32 mAnim;          // _04
	f32 mFrame;         // _08
	// _0C vtable

	virtual void vf08();                             // _08
	virtual void Play(s32 anim, f32 frame, s32 vol); // _0C
};

// the web shot the spider spits, it flies from mPos to mTarget
class SpiderShot {
public:
	CVec mPos;       // _00
	CVec mTarget;    // _10
	f32 _20;         // _20
	f32 _24;         // _24
	u32 _28;         // _28, the web anim step
	s32 _2C;         // _2C
	CStatus mStatus; // _30

	void Set(const CVec& pos, u32 step)
	{
		mStatus.mFlags |= 1;
		mPos = pos;
		_28  = step;
	}
};

class XObjSpiderSmall : public XObj {
public:
	f32 _980;         // _980
	f32 _984;         // _984
	f32 _988;         // _988
	f32 _98C;         // _98C
	f32 _990;         // _990
	f32 _994;         // _994
	f32 _998;         // _998
	f32 _99C;         // _99C
	f32 _9A0;         // _9A0
	f32 _9A4;         // _9A4
	s32 mState;       // _9A8
	s32 mStep;        // _9AC
	s32 mTimer;       // _9B0
	CStatus mStatus;  // _9B4
	s32 mPrevState;   // _9BC
	s32 _9C0;         // _9C0
	f32 _9C4;         // _9C4
	f32 _9C8;         // _9C8
	Vec _9CC;         // _9CC
	Vec _9D8;         // _9D8
	f32 _9E4;         // _9E4
	s32 _9E8;         // _9E8
	s32 _9EC;         // _9EC
	Vec _9F0;         // _9F0
	Vec _9FC;         // _9FC
	Vec _A08;         // _A08, the area's corner
	f32 _A14;         // _A14
	f32 _A18;         // _A18
	Vec _A1C;         // _A1C
	f32 _A28;         // _A28
	s32 _A2C;         // _A2C
	s32 _A30[6];      // _A30
	s32 _A48;         // _A48
	SpiderShot mShot; // _A4C
	XObj* _A84;       // _A84, the web it shoots
	u8 _A88;          // _A88
	f32 _A8C;         // _A8C
	f32 _A90;         // _A90
	s32 _A94;         // _A94
	f32 _A98;         // _A98
	Vec _A9C;         // _A9C
	u8 _AA8;          // _AA8
	s32 _AAC;         // _AAC
	u8 _AB0;          // _AB0
	s32 _AB4;         // _AB4
	s32 _AB8;         // _AB8
	s32 _ABC;         // _ABC
	s32 _AC0;         // _AC0
	s32 _AC4;         // _AC4
	AnimSfx mSfx;     // _AC8

	virtual void Release();                    // _54
	virtual void Init(u32 id);                 // _90
	virtual void vf94();                       // _94
	virtual void Reset();                      // _98
	virtual BOOL vf9C(void* info);             // _9C
	virtual void Update(u32 frames);           // _A4
	virtual void vfAC();                       // _AC
	virtual void vfB8();                       // _B8
	virtual void vfBC(Vec* pos, f32 scale);    // _BC
	virtual void vfC0();                       // _C0
	virtual void vfC8();                       // _C8
	virtual void vfCC();                       // _CC
	virtual s32 vf100(s32 a, s32 b);           // _100
	virtual s32 vf124(s32 n, s32* args);       // _124
	virtual void vf138(const Vec* pos, f32 f); // _138
	virtual void vf148();                      // _148, plays the anim's sfx

	void Hide() { mStatus.mFlags |= 0x80000; }
	u32 IsDead() const { return (mStatus.mFlags >> 19) & 1; }

	// whether pos is inside the spider's area, margin in from its sides
	// pos is inside the spider's area, _998 in from its sides
	bool InRect(const Vec& pos)
	{
		f32 x    = pos.x;
		f32 z    = pos.z;
		f32 minX = _A08.x + _998;
		f32 maxX = _A08.x + _A14 - _998;
		f32 minZ = _A08.z + _998;
		f32 maxZ = _A08.z + _A18 - _998;
		if (minX > x || maxX < x || minZ > z || maxZ < z) {
			return false;
		}
		return true;
	}

	// walks to a random spot around home
	void Wander(s32 step)
	{
		_9FC.x = 12.0f + _9D8.x - rand() % 2400 / 100.0f;
		_9FC.y = _9D8.y;
		_9FC.z = 12.0f + _9D8.z - rand() % 2400 / 100.0f;
		mStep  = step;
		_984   = 0.3f;
		SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}

	// _984 within the walk speeds
	void ClampSpeed()
	{
		if (_984 < 0.08f) {
			_984 = 0.08f;
		} else if (_984 > 1.1f) {
			_984 = 1.1f;
		}
	}

	bool IsInArea(const Vec& pos, f32 margin)
	{
		if (pos.y < HSD_JObjGetTranslationY(mJObj) - 20.0f || pos.y > 20.0f + HSD_JObjGetTranslationY(mJObj)) {
			return false;
		}
		f32 minX = _A08.x + margin;
		f32 maxX = _A08.x + _A14 - margin;
		f32 minZ = _A08.z + margin;
		f32 maxZ = _A08.z + _A18 - margin;
		if (minX > pos.x || maxX < pos.x || minZ > pos.z || maxZ < pos.z) {
			return false;
		}
		return true;
	}

	void SetState(s32 state)
	{
		mStep      = 0;
		mTimer     = 0;
		mPrevState = mState;
		mState     = state;
	}
};
#if !defined(VERSION_JP)
typedef char XObjSpiderSmall_size_check[sizeof(XObjSpiderSmall) == 0xAD8 ? 1 : -1];
#endif

#include "game/gmath.h"

#define PI 3.1415927f
#include <stdlib.h>

// game object fields, by offset
#define GAME_FLAGS      (*(u64*)(lbl_8025C400 + 0x1B10))
#define GAME_PLAYER     (*(UnkSpiderPlayer**)(lbl_8025C400 + 0x96BC))
#define GAME_DEMO       (*(void**)(lbl_8025C400 + 0x5D080))
#define GAME_WORD(o)    (*(s32*)(lbl_8025C400 + (o)))
#define GAME_FLOAT(o)   (*(f32*)(lbl_8025C400 + (o)))
#define GAME_SPIDERS    ((UnkObjPool*)(lbl_8025C400 + 0x9720)) // 96 of them
#define GAME            ((UnkSpiderGame*)lbl_8025C400)
#define GAME_SCRNCOLOR  ((void*)(lbl_8025C400 + 0x4588))
#define GAME_RENDERMODE (*(void**)(lbl_8025C400 + 0x290))
#define GAME_VIEWMTX    ((MtxPtr)(*(u8**)(lbl_8025C400 + 0x44) + 0xE8))

// the game object, see game/cgame.h
struct UnkSpiderGame {
	u8 _0000[0x3444];          // _0000
	s32 mStage;                // _3444
	u8 _3448[0x9860 - 0x3448]; // _3448
	XObj* mObjects[0x400];     // _9860
	s32 mObjectCount;          // _A860
};

// chibi robo, only what the spiders touch
class UnkSpiderPlayer : public XObj {
public:
	u8 _980[0xA40 - 0x980];    // _980
	f32 _A40;                  // _A40
	u8 _A44[0x9658 - 0xA44];   // _A44
	f32 mBattery;              // _9658
	f32 mBatteryMax;           // _965C
	u8 _9660[0xD7E8 - 0x9660]; // _9660
	s32 _D7E8;                 // _D7E8

	void SetBattery(f32 v)
	{
		mBattery = v > 0.0f ? v : 0.0f;
		if (mBattery < 0.0f) {
			mBattery = 0.0f;
		}
		if (mBattery > mBatteryMax) {
			mBattery = mBatteryMax;
		}
	}

	void UseBattery(f32 n)
	{
		f32 v = mBattery - n;
		if (v < 0.0f) {
			v = 0.0f;
		}
		SetBattery(v);
	}
};

// the spiders of one stage, one status per spider
struct UnkSpiderStage {
	CStatus mSpiders[16]; // _00

	void Clear()
	{
		for (int i = 0; i < 16; i++) {
			mSpiders[i].mFlags = 0;
		}
	}
};

extern "C" {
void fn_80066190(void* mgr, s32 a, s32 b);
void fn_8001E660(CSound* sound, s32 sfx);
void fn_800EA99C(void* p);
BOOL fn_800D4B60(void* p);
void fn_80068C8C(f32* value, f32* target, f32 step);
void fn_800EA554(Vec* pos, Vec* home, s32 kind);
void fn_800242A8(void* p, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g);
void fn_800EA780(void* p, u32 frames);
s16 fn_80034E4C(s32 id);
s32 fn_8006F654(const char* name);
f32 fn_800A0414(XObj* player);
void fn_800A0014(XObj* player, BOOL react, BOOL stop, f32 time);
void fn_80023EE0(void* scrn, f32 time, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, u8 a1);
void fn_800FCF04(XObjSpiderSmall* self, u32 frames);
void fn_800F54B4(XObjSpiderSmall* self);
void fn_800FFB00(XObjSpiderSmall* self);
void fn_800FF168(XObjSpiderSmall* self);
void fn_8019B56C(void* mode, s32 type, s32 src, s32 dst, s32 op);
void fn_80198258(void* gs, u32 segments, u32 centerColor, u32 edgeColor, f32 x, f32 y, f32 z, f32 w, f32 h, f32 rx, f32 ry, f32 rz);
void fn_801988EC(void* gs, s32 type, MtxPtr view, void* vtx, u16 count);

extern const UnkActorSeEntry lbl_8022B170[];
extern XObjPoint* lbl_8065A798[2];
extern u8 lbl_8065B2C8[4];
extern s32 lbl_8065B2CC[2];
extern u8 lbl_8065B2D4;
extern s32 lbl_8065B2D8;
extern XObj* lbl_8065B2DC;
extern u8 lbl_8065B2E0;

u8 lbl_8037E7D0[0x140];
XObj* lbl_8037E910[12]; // the webs
s32 lbl_8037E940[12];
UnkSpiderStage lbl_8037E97C[0x27];
}

// the states' updates, by mState
extern void (XObjSpiderSmall::*lbl_8022B230[])(u32);

static inline bool IsStage(const char* name)
{
	s32 stage = GAME->mStage;
	s32 id    = fn_8006F654(name);
	return id == stage;
}

// the player is in control, no talk or menu holds it
static inline bool IsPlayerFree()
{
	bool active = false;
	if ((GAME_FLAGS & 1) && !(GAME_FLAGS & 0x22)) {
		bool held = false;
		if (GAME_WORD(0x331C) != 0 || GAME_WORD(0x3320) != 0) {
			if (GAME_WORD(0x3324) == 0) {
				held = true;
			}
		}
		if (!held) {
			active = true;
		}
	}
	return active;
}

// and a sound effect may play
static inline bool CanPlaySe()
{
	bool play = false;
	if (IsPlayerFree() && GAME_DEMO == NULL && GAME_WORD(0x8580) == 0 && !(GAME_FLAGS & 0xC00) && GAME_FLOAT(0xE5D30) > 0.0f
	    && GAME_WORD(0xE5D34) == 0) {
		play = true;
	}
	return play;
}

// NONMATCHING: anim and jobj swap r29 and r30
void XObjSpiderSmall::vf148()
{
	if ((mFlags & 1) && (mFlags & 0x10000) && !fn_800665E0(GAME_EVTMGR)) {
		s32 anim = mAnimId;
		mSfx.Play(anim, GetAnimFrame(mJObj), fn_8001DF00(GAME_SOUND, &mMoveTranslate, 0));
	}
}

extern "C" {

bool fn_800F4C10(SpiderShot* shot, u8 i, u32 frames)
{
	if (lbl_8037E910[i] == NULL) {
		return TRUE;
	}
	f32 end = GetAnimEndFrame(lbl_8037E910[i]->mJObj);
	if (end - frames <= GetAnimFrame(lbl_8037E910[i]->mJObj)) {
		return TRUE;
	}
	return FALSE;
}

void fn_800F4DB8(SpiderShot* shot, const CVec& pos)
{
	shot->mStatus.mFlags |= 1;
	shot->mPos = pos;
	shot->_28  = 1;
	if (CanPlaySe()) {
		fn_8001E904(GAME_SOUND, 0x184, 0xFF);
		fn_8001DF00(GAME_SOUND, &shot->mPos, 0);
	}
}

// plays the web's anims, 1 and 2 shoot it, 10 drops it
void fn_800F4F48(SpiderShot* shot, u8 i, u32 frames, BOOL arg)
{
	if (GAME_FLAGS & 0x400) {
		return;
	}
	if (i == 0xFF) {
		shot->_28 = 0;
		shot->mStatus.mFlags &= ~1;
		return;
	}
	if (lbl_8037E910[i] == NULL) {
		shot->_28 = 0;
		shot->mStatus.mFlags &= ~1;
		return;
	}
	if (!(shot->mStatus.mFlags & 1)) {
		return;
	}
	switch (shot->_28) {
	case 0:
		break;
	case 1:
		lbl_8037E910[i]->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		shot->_28++;
		break;
	case 2: {
		f32 end = GetAnimEndFrame(lbl_8037E910[i]->mJObj);
		if (end - frames <= GetAnimFrame(lbl_8037E910[i]->mJObj)) {
			lbl_8037E910[i]->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			shot->_28 = 0;
			shot->mStatus.mFlags &= ~1;
		}
		break;
	}
	case 3:
		lbl_8037E910[i]->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		shot->_28++;
		break;
	case 4: {
		f32 end = GetAnimEndFrame(lbl_8037E910[i]->mJObj);
		if (end - frames <= GetAnimFrame(lbl_8037E910[i]->mJObj)) {
			lbl_8037E910[i]->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			shot->_28 = 0;
			shot->mStatus.mFlags &= ~1;
		}
		break;
	}
	case 10:
		lbl_8037E910[i]->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		shot->_28 = 0;
		shot->mStatus.mFlags &= ~1;
		break;
	}
}

void fn_800F53F8(u8 on)
{
	for (s32 i = 0; i < 0x60; i++) {
		XObj* obj = GAME_SPIDERS->Get(i);
		if ((obj->mFlags & 1) && obj->GetDescKind() == 0x104) {
			if (on) {
				obj->mFlags |= 0x10000;
			} else {
				obj->mFlags &= ~0x10000ULL;
			}
		}
	}
}

s32 fn_800F5588()
{
	s32 n;
	switch (GAME_PLAYER->_D7E8) {
	case 1:
	case 6:
		n = rand() % 10 + 10;
		break;
	case 2:
	case 3:
		n = rand() % 2 + 2;
		break;
	case 4:
	case 8:
		n = rand() % 20 + 20;
		break;
	default:
		n = rand() % 10 + 10;
		break;
	}
	return n;
}

u8 fn_800F5678(XObjSpiderSmall* self)
{
	return (self->mStatus.mFlags >> 6) & 1;
}

u8 fn_800F5684(XObjSpiderSmall* self)
{
	return (self->mStatus.mFlags >> 23) & 1;
}

bool fn_800F5690(XObjSpiderSmall* self)
{
	f32 dx = self->_A1C.x - self->mMoveTranslate.x;
	f32 dy = self->_A1C.y - self->mMoveTranslate.y;
	f32 dz = self->_A1C.z - self->mMoveTranslate.z;
	return sqrtf(dx * dx + dy * dy + dz * dz) <= self->_A28 + self->_998;
}

// the player stands in the spider's area
bool fn_800F57C0(XObjSpiderSmall* self)
{
	Vec pos = GAME_PLAYER->mMoveTranslate;
	f32 y   = pos.y;
	f32 z   = pos.z;
	CVec unused;
	if (y < HSD_JObjGetTranslationY(self->mJObj) - 20.0f || y > 20.0f + HSD_JObjGetTranslationY(self->mJObj)) {
		return false;
	}
	f32 minX = self->_A08.x + self->_998;
	f32 maxX = self->_A08.x + self->_A14 - self->_998;
	f32 minZ = self->_A08.z + self->_998;
	f32 maxZ = self->_A08.z + self->_A18 - self->_998;
	if (minX > pos.x || maxX < pos.x || minZ > z || maxZ < z) {
		return false;
	}
	return true;
}

// the distances on the ground and in space. their out of line copies, fn_800F841C and
// fn_800F8520, set up the frame before the loads, which these don't, so those stay asm
static inline f32 DistanceXZ(const Vec* a, const Vec* b)
{
	f32 dx = a->x - b->x;
	f32 dy = 0.0f;
	f32 dz = a->z - b->z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}

static inline f32 Distance(const Vec* a, const Vec* b)
{
	f32 dx = a->x - b->x;
	f32 dy = a->y - b->y;
	f32 dz = a->z - b->z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}

// state 2, dead
void fn_800FC930(XObjSpiderSmall* self)
{
	self->mFlags &= ~0x10000ULL;
	self->mStatus.mFlags |= 0x80000;
	self->mStatus.mFlags &= ~0x100000;
}

// jumps onto the robot
static inline void Catch(XObjSpiderSmall* self)
{
	self->mFlags |= 0x4000000000000ULL;
	self->_984             = 0.0f;
	self->_99C             = 0.0f;
	self->mMoveTranslate.y = 10.0f + self->_A9C.y;
	self->_A90             = 1.0f;
	self->_A98             = 40.0f;
	self->mStep            = 4;
	self->mStatus.mFlags |= 0x800000;
	lbl_8065B2D8++;
	GAME_PLAYER->_A40 = 0.4f;
	self->SetAnim(5, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

// state 8: chases the robot and spits webs at it
void fn_800F6408(XObjSpiderSmall* self, u32 frames)
{
	CPad pad(0);
	UnkSpiderPlayer* player = GAME_PLAYER;
	Vec ppos                = player->mMoveTranslate;
	Vec prot                = player->mMoveRotate;
	switch (self->mStep) {
	case 0:
		self->_98C = 0.0f;
		if (Distance(&ppos, &self->mMoveTranslate) < 48.0f) {
			self->_9FC.x = ppos.x;
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z;
			self->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep++;
		} else {
			self->mStep = 100;
		}
		break;
	case 1: {
		self->_984   = 0.3f;
		self->_9FC.x = ppos.x;
		self->_9FC.y = ppos.y;
		self->_9FC.z = ppos.z;
		self->_980   = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		f32 dist     = Distance(&self->_9FC, &self->mMoveTranslate);
		if (dist < 20.0f) {
			self->_984 = 0.0f;
			self->SetAnim(3, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = fn_800F5588();
			self->mStep++;
		} else if (dist >= 64.0f) {
			self->mStep = 100;
		} else if (12.0f + self->_9D8.y < ppos.y || self->_9D8.y - 12.0f > ppos.y) {
			self->mStep = 100;
		}
		break;
	}
	case 2: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			if (--self->mTimer < 0) {
				self->_9FC.x = ppos.x;
				self->_9FC.y = ppos.y;
				self->_9FC.z = ppos.z;
				if (Distance(&self->_9FC, &self->mMoveTranslate) >= 20.0f) {
					self->SetAnim(12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					self->mStatus.mFlags |= 8;
					self->_9FC.x = self->mMoveTranslate.x;
					self->_9FC.y = self->mMoveTranslate.y;
					self->_9FC.z = self->mMoveTranslate.z;
					self->_984   = 0.0f;
					self->ClampSpeed();
					self->_99C = 4.0f;
					self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
					self->mStatus.mFlags &= ~1;
					self->mStep = 22;
				} else {
					f32 size = fn_800A0414(GAME_PLAYER);
					self->SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					self->mStatus.mFlags |= 8;
					self->_A8C   = DEG2RAD * (rand() % 360);
					self->_A9C.x = size * (f32)sin(self->_A8C);
					self->_A9C.y = -rand() % 800 / 100;
					self->_A9C.z = size * (f32)cos(self->_A8C);
					self->_9FC.x = ppos.x + size * (f32)sin(self->_A8C);
					self->_9FC.y = ppos.y + -rand() % 800 / 100;
					self->_9FC.z = ppos.z + size * (f32)cos(self->_A8C);
					self->_984   = DistanceXZ(&self->_9FC, &self->mMoveTranslate) / 20.0f;
					self->ClampSpeed();
					self->_99C = 4.0f;
					self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
					self->mStatus.mFlags &= ~1;
					self->mFlags |= 0x4000000000000ULL;
					self->mStep = 10;
				}
			}
		}
		break;
	}
	case 4: {
		self->_AA8             = 0;
		f32 size               = fn_800A0414(GAME_PLAYER);
		self->mMoveTranslate.x = ppos.x + size * (f32)sin(prot.y + self->_A8C);
		self->mMoveTranslate.y = 10.0f + ppos.y + self->_A9C.y;
		self->mMoveTranslate.z = ppos.z + size * (f32)cos(prot.y + self->_A8C);
		self->_980             = atan2(ppos.x - self->mMoveTranslate.x, ppos.z - self->mMoveTranslate.z);
		if (IsPlayerFree() && (pad.mPad->mTrigger & 0xF0000)) {
			self->_A98 = self->_A98 - self->_A90;
			self->_A90 = 8.0f;
		}
		if (self->_A90 > 1.0f) {
			self->_A90 -= 1.0f;
		}
		if (GAME_DEMO != NULL) {
			self->_9FC.x = ppos.x + size * (f32)sin(prot.y + self->_A8C);
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z + size * (f32)cos(prot.y + self->_A8C);
			fn_800F54B4(self);
			self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mFlags |= 0x4000000000000ULL;
			self->mStep = 30;
		} else if (Abs(ppos.y - self->_9D8.y) > 10.0f) {
			fn_800F54B4(self);
			self->mStatus.mFlags |= 0x2000;
			self->SetState(7);
		} else if (self->_A98 <= 0.0f || fn_800D4B60(GAME_PLAYER->_A44)) {
			f32 ang;
			f32 dist     = 20.0f + size + rand() % 3000 / 100.0f;
			ang          = PI + (f32)atan2(ppos.x - self->mMoveTranslate.x, ppos.z - self->mMoveTranslate.z);
			self->_9FC.x = ppos.x + dist * (f32)sin(ang);
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z + dist * (f32)cos(ang);
			fn_800F54B4(self);
			self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 1.0f;
			self->mFlags |= 0x4000000000000ULL;
			if (size >= 2.0f) {
				self->_AA8 = 1;
			}
			self->mStep++;
		}
		break;
	}
	case 5: {
		s8 wait = 2;
		self->_99C -= 0.08f * frames;
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			wait                   = 1;
			self->mMoveTranslate.y = self->_9D8.y;
		}
		self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 0.5f * frames) {
			self->_984 = 0.0f;
			wait--;
		}
		if (wait == 0) {
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep++;
		}
		break;
	}
	case 6: {
		self->mTimer++;
		if (self->mTimer % 30 < 15) {
			self->SetColor(1, 0, 0x80, 0, 0.0f);
			self->SetColor(1, 0x80, 0, 0, 0.0f);
		} else {
			self->SetColor(1, 0, 0, 0, 0.0f);
		}
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetColor(1, 0, 0, 0, 0.0f);
			self->mTimer = 0;
			if (--self->_9E8 <= 0) {
				self->_9E8 = 0;
			}
			self->mStatus.mFlags &= ~0x8000;
			if (self->_9E8 <= 0) {
				self->SetState(1);
			} else {
				self->mStep = 0;
				self->mStatus.mFlags &= ~8;
				self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->mTimer = rand() % 30 + 30;
			}
		}
		break;
	}
	case 10: {
		f32 dist = Distance(&ppos, &self->mMoveTranslate);
		if (dist < 12.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			self->mFlags |= 0x4000000000000ULL;
		}
		if (dist < 8.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			Catch(self);
		} else {
			if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
				self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->mStep++;
			} else if (self->_99C >= -2.3f) {
				self->_99C -= 0.36f * frames;
			} else {
				self->_99C = -2.3f;
			}
		}
		break;
	}
	case 11: {
		f32 dist = Distance(&ppos, &self->mMoveTranslate);
		if (dist < 12.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			self->mFlags |= 0x4000000000000ULL;
		}
		if (dist < 8.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			Catch(self);
		} else {
			if (self->_99C >= -2.3f) {
				self->_99C -= 0.36f * frames;
			} else {
				self->_99C = -2.3f;
			}
			if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
				self->_99C             = 0.0f;
				self->mMoveTranslate.y = self->_9D8.y;
				self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->_984 = 0.0f;
				self->mStatus.mFlags &= ~8;
				self->mFlags &= ~0x4000000000000ULL;
				self->mStep++;
			}
		}
		break;
	}
	case 12: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->mStep = 0;
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = rand() % 30 + 30;
		}
		break;
	}
	case 22:
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
			self->mStep++;
		} else if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		break;
	case 23:
		if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mStatus.mFlags &= ~8;
			self->mFlags &= ~0x4000000000000ULL;
			self->mStep++;
		}
		break;
	case 24: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = rand() % 30 + 30;
			self->mStep  = 0;
		}
		break;
	}
	case 30:
		self->_99C -= 0.08f;
		if (self->mMoveTranslate.y <= self->_9D8.y) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep = 31;
		}
		break;
	case 31: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->Wander(32);
		}
		break;
	}
	case 32:
		self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
			self->mStep  = 33;
			self->_984   = 0.0f;
			self->mTimer = rand() % 30 + 30;
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		} else if (GAME_DEMO == NULL) {
			self->mStep = 0;
		}
		break;
	case 33:
		if (GAME_DEMO == NULL) {
			self->mStep = 0;
		}
		break;
	case 100:
		self->_98C = 0.0f;
		self->_984 = 0.0f;
		self->mStatus.mFlags |= 0x8000;
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
		self->_9A0 = 1.0f;
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		{
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
		}
		self->mStep++;
		break;
	case 101:
		self->_9A0 -= 0.01f * frames;
		if (self->_9A0 <= 0.0f) {
			self->_9A0   = 0.0f;
			self->mTimer = 210 / frames;
			self->mStep++;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 102:
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			self->mTimer = rand() % 30 + 30;
			self->mStep++;
		}
		break;
	case 103:
		if (--self->mTimer < 0) {
			self->_AC0--;
			if (12.0f + self->_9D8.y < ppos.y || self->_9D8.y - 12.0f > ppos.y || GAME_DEMO != NULL || self->_AC0 < 0
			    || GAME_FLOAT(0xE5D30) == 0.0f) {
				self->mMoveTranslate.x = self->_9D8.x;
				self->mMoveTranslate.y = self->_9D8.y;
				self->mMoveTranslate.z = self->_9D8.z;
				self->_AC0             = 0;
				self->mTimer           = 0;
				if (self->mStatus.mFlags & 0x40000) {
					fn_800FC930(self);
					if (self->_A88 != 0xFF) {
						lbl_8037E940[self->_A88] = -1;
						self->_A84               = NULL;
						self->_A88               = 0xFF;
					}
					self->mStep = 1000;
				} else {
					self->SetState(5);
					self->mStatus.mFlags &= ~0x40;
					lbl_8037E97C[GAME->mStage].mSpiders[self->_AB4].mFlags &= ~0x40;
				}
			} else {
				f32 size               = fn_800A0414(GAME_PLAYER);
				self->_A8C             = DEG2RAD * (rand() % 360);
				f32 dist               = 10.0f + size;
				self->mMoveTranslate.x = ppos.x + dist * (f32)sin(prot.y + self->_A8C);
				self->mMoveTranslate.y = self->_9D8.y;
				self->mMoveTranslate.z = ppos.z + dist * (f32)cos(prot.y + self->_A8C);
				f32 y, x, z;
				x          = self->mMoveTranslate.x;
				z          = self->mMoveTranslate.z;
				y          = self->mMoveTranslate.y;
				self->_980 = atan2(ppos.x - x, ppos.z - z);
				fn_800F4DB8(&self->mShot, CVec(x, y, z));
				self->mStep++;
			}
		}
		break;
	case 104:
		self->_9A0 += 0.01f * frames;
		if (self->_9A0 >= 1.0f) {
			self->_9A0   = 1.0f;
			self->mTimer = 210 / frames;
			self->mStep++;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 105:
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			self->mStatus.mFlags &= ~0x8000;
			self->mStep = 0;
		}
		break;
	}
}

// state 3: wanders about home, chases the robot and jumps onto it
void fn_800FA4B0(XObjSpiderSmall* self, u32 frames)
{
	CPad pad(0);
	Vec ppos = GAME_PLAYER->mMoveTranslate;
	Vec prot = GAME_PLAYER->mMoveRotate;
	if (!fn_800F57C0(self)) {
		if (--self->_ABC < 0) {
			self->_ABC = 0;
		}
	} else {
		self->_ABC = 180 / frames;
	}
	switch (self->mStep) {
	case 2:
	case 10:
	case 11:
	case 22:
	case 23:
		self->mPoints       = lbl_8065A798[1];
		self->mPointMaskOld = 0;
		self->mPointMask    = 0;
		break;
	default:
		self->mPoints       = lbl_8065A798[0];
		self->mPointMaskOld = 0;
		self->mPointMask    = 0;
		break;
	}
	switch (self->mStep) {
	case 0:
		self->_98C = 0.0f;
		self->mTimer--;
		if (self->mTimer <= 0) {
			self->Wander(20);
			self->mTimer = rand() % 30 + 10;
		} else if (Distance(&ppos, &self->mMoveTranslate) < 64.0f && fn_800F5690(self)) {
			self->_9FC.x = ppos.x;
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z;
			self->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep++;
		}
		break;
	case 20:
		self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
			self->mStep  = 0;
			self->_984   = 0.0f;
			self->mTimer = rand() % 30 + 30;
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		} else {
			if (--self->mTimer < 0) {
				self->_984 = 0.0f;
				self->SetAnim(3, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->mStep = 21;
			}
			if (!self->InRect(self->mMoveTranslate)) {
				self->_9FC.x = self->_9D8.x;
				self->_9FC.y = self->_9D8.y;
				self->_9FC.z = self->_9D8.z;
			}
			if (self->_ABC <= 0) {
				if ((self->mStatus.mFlags & 0x20) && GAME_FLOAT(0xE5D30) != 0.0f) {
					self->_AC0 = rand() % 4 + 3;
					self->SetState(8);
				} else {
					self->SetState(7);
				}
			}
		}
		break;
	case 21: {
		HSD_AObj* aobj = GetAObj(self->mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(self->mJObj);
		if (end - frames <= (aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f)) {
			self->SetAnim(12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStatus.mFlags |= 8;
			self->_9FC.x = self->mMoveTranslate.x;
			self->_9FC.y = self->mMoveTranslate.y;
			self->_9FC.z = self->mMoveTranslate.z;
			self->_984   = 0.0f;
			self->ClampSpeed();
			self->mStep++;
			self->_99C = 4.0f;
			self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
			self->mStatus.mFlags &= ~1;
		}
		break;
	}
	case 22:
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
			self->mStep++;
		} else if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		break;
	case 23:
		if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mStatus.mFlags &= ~8;
			self->mFlags &= ~0x4000000000000ULL;
			self->mStep++;
		}
		break;
	case 24: {
		HSD_AObj* aobj = GetAObj(self->mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(self->mJObj);
		if (end - frames <= (aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f)) {
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = rand() % 30 + 30;
			self->mStep  = 0;
		}
		break;
	}
	case 30:
		self->_99C -= 0.08f;
		if (self->mMoveTranslate.y <= self->_9D8.y) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep = 31;
		}
		break;
	case 31: {
		HSD_AObj* aobj = GetAObj(self->mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(self->mJObj);
		if (end - frames <= (aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f)) {
			self->Wander(32);
		}
		break;
	}
	case 32:
		self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
			self->mStep  = 33;
			self->_984   = 0.0f;
			self->mTimer = rand() % 30 + 30;
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		} else if (GAME_DEMO == NULL) {
			self->mStep = 0;
		}
		break;
	case 33:
		if (GAME_DEMO == NULL) {
			self->mStep = 0;
		}
		break;
	case 1:
		self->_984   = 0.3f;
		self->_9FC.x = ppos.x;
		self->_9FC.y = ppos.y;
		self->_9FC.z = ppos.z;
		self->_980   = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (Distance(&self->_9FC, &self->mMoveTranslate) < 20.0f) {
			self->_984 = 0.0f;
			self->SetAnim(3, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = fn_800F5588();
			self->mStep++;
		}
		if (!fn_800F5690(self)) {
			self->Wander(20);
		}
		if (!self->InRect(self->mMoveTranslate)) {
			self->Wander(20);
		}
		break;
	case 2: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			if (--self->mTimer < 0) {
				self->_9FC.x = ppos.x;
				self->_9FC.y = ppos.y;
				self->_9FC.z = ppos.z;
				if (Distance(&self->_9FC, &self->mMoveTranslate) >= 20.0f) {
					self->SetAnim(12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					self->mStatus.mFlags |= 8;
					self->_9FC.x = self->mMoveTranslate.x;
					self->_9FC.y = self->mMoveTranslate.y;
					self->_9FC.z = self->mMoveTranslate.z;
					self->_984   = 0.0f;
					self->ClampSpeed();
					self->_99C = 4.0f;
					self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
					self->mStatus.mFlags &= ~1;
					self->mStep = 22;
				} else {
					f32 size = fn_800A0414(GAME_PLAYER);
					self->SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					self->mStatus.mFlags |= 8;
					self->_A8C   = DEG2RAD * (rand() % 360);
					self->_A9C.x = size * (f32)sin(self->_A8C);
					self->_A9C.y = -rand() % 800 / 100;
					self->_A9C.z = size * (f32)cos(self->_A8C);
					self->_9FC.x = ppos.x + size * (f32)sin(self->_A8C);
					self->_9FC.y = ppos.y + -rand() % 800 / 100;
					self->_9FC.z = ppos.z + size * (f32)cos(self->_A8C);
					self->_984   = DistanceXZ(&self->_9FC, &self->mMoveTranslate) / 20.0f;
					self->ClampSpeed();
					self->_99C = 4.0f;
					self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
					self->mStatus.mFlags &= ~1;
					self->mFlags |= 0x4000000000000ULL;
					self->mStep = 10;
				}
			}
		}
		break;
	}
	case 4: {
		self->_AA8             = 0;
		f32 size               = fn_800A0414(GAME_PLAYER);
		self->mMoveTranslate.x = ppos.x + size * (f32)sin(prot.y + self->_A8C);
		self->mMoveTranslate.y = 10.0f + ppos.y + self->_A9C.y;
		self->mMoveTranslate.z = ppos.z + size * (f32)cos(prot.y + self->_A8C);
		self->_980             = atan2(ppos.x - self->mMoveTranslate.x, ppos.z - self->mMoveTranslate.z);
		if (pad.mPad->mTrigger & 0xF0000) {
			self->_A98 = self->_A98 - self->_A90;
			self->_A90 = 8.0f;
		}
		if (self->_A90 > 1.0f) {
			self->_A90 -= 1.0f;
		}
		if (GAME_DEMO != NULL) {
			self->_9FC.x = ppos.x + size * (f32)sin(prot.y + self->_A8C);
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z + size * (f32)cos(prot.y + self->_A8C);
			fn_800F54B4(self);
			self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mFlags |= 0x4000000000000ULL;
			self->mStep = 30;
		} else if (Abs(ppos.y - self->_9D8.y) > 10.0f) {
			fn_800F54B4(self);
			self->mStatus.mFlags |= 0x2000;
			self->SetState(7);
		} else if (self->_A98 <= 0.0f || fn_800D4B60(GAME_PLAYER->_A44)) {
			f32 dist     = 20.0f + size + rand() % 3000 / 100.0f;
			f32 ang      = PI + (f32)atan2(ppos.x - self->mMoveTranslate.x, ppos.z - self->mMoveTranslate.z);
			self->_9FC.x = ppos.x + dist * (f32)sin(ang);
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z + dist * (f32)cos(ang);
			fn_800F54B4(self);
			self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 1.0f;
			self->mFlags |= 0x4000000000000ULL;
			if (size >= 2.0f) {
				self->_AA8 = 1;
			}
			self->mStep++;
		}
		break;
	}
	case 5: {
		s8 wait = 2;
		self->_99C -= 0.08f * frames;
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			wait                   = 1;
			self->mMoveTranslate.y = self->_9D8.y;
		}
		if (!self->InRect(self->mMoveTranslate)) {
			self->_9FC.x = self->mMoveTranslate.x;
			self->_9FC.z = self->mMoveTranslate.z;
			self->_984   = 0.0f;
		}
		self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 0.5f * frames) {
			self->_984 = 0.0f;
			wait--;
		}
		if (wait == 0) {
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep++;
		}
		break;
	}
	case 6: {
		self->mTimer++;
		if (self->mTimer % 30 < 15) {
			self->SetColor(1, 0, 0x80, 0, 0.0f);
			self->SetColor(1, 0x80, 0, 0, 0.0f);
		} else {
			self->SetColor(1, 0, 0, 0, 0.0f);
		}
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetColor(1, 0, 0, 0, 0.0f);
			self->mTimer = 0;
			if (--self->_9E8 <= 0) {
				self->_9E8 = 0;
			}
			self->mStatus.mFlags &= ~0x8000;
			if (self->_9E8 <= 0) {
				self->SetState(1);
			} else {
				self->mStep = 0;
				self->mStatus.mFlags &= ~8;
				self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->mTimer = rand() % 30 + 30;
			}
		}
		break;
	}
	case 10: {
		f32 dist = Distance(&ppos, &self->mMoveTranslate);
		if (dist < 12.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			self->mFlags |= 0x4000000000000ULL;
		}
		if (dist < 8.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			Catch(self);
		} else {
			if (!self->InRect(self->mMoveTranslate)) {
				self->_9FC.x = self->mMoveTranslate.x;
				self->_9FC.z = self->mMoveTranslate.z;
				self->_984   = 0.0f;
			}
			if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
				self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->mStep++;
			} else if (self->_99C >= -2.3f) {
				self->_99C -= 0.36f * frames;
			} else {
				self->_99C = -2.3f;
			}
		}
		break;
	}
	case 11: {
		f32 dist = Distance(&ppos, &self->mMoveTranslate);
		if (dist < 12.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			self->mFlags |= 0x4000000000000ULL;
		}
		if (dist < 8.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			Catch(self);
		} else {
			if (self->_99C >= -2.3f) {
				self->_99C -= 0.36f * frames;
			} else {
				self->_99C = -2.3f;
			}
			if (!self->InRect(self->mMoveTranslate)) {
				self->_984 = 0.0f;
			}
			if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
				self->_99C             = 0.0f;
				self->mMoveTranslate.y = self->_9D8.y;
				self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->_984 = 0.0f;
				self->mStatus.mFlags &= ~8;
				self->mFlags &= ~0x4000000000000ULL;
				self->mStep++;
			}
		}
		break;
	}
	case 12: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = rand() % 30 + 30;
			self->mStep  = 0;
		}
		break;
	}
	}
}

void fn_800F54B4(XObjSpiderSmall* self)
{
	self->mStatus.mFlags &= ~0x800000;
	if (self->mStatus.Check(0x1000000)) {
		self->mStatus.mFlags &= ~0x1000000;
		lbl_8065B2D4 = 0;
		GAME_PLAYER->SetColor(1, 0, 0, 0, 0.0f);
		fn_80066190(GAME_EVTMGR, 0, 1);
	}
	if (--lbl_8065B2D8 < 0) {
		lbl_8065B2D8 = 0;
	}
	if (lbl_8065B2D8 == 0) {
		GAME_PLAYER->_A40 = 1.0f;
	}
}

// state 9: falls off the wall, runs a bit and spits a last web
void fn_800F5934(XObjSpiderSmall* self, u32 frames)
{
	UnkSpiderPlayer* player = GAME_PLAYER;
	Vec ppos                = player->mMoveTranslate;
	Vec prot                = player->mMoveRotate;
	switch (self->mStep) {
	case 0:
		if ((self->mStatus.mFlags & 0x800000) || self->mMoveTranslate.y > self->_9D8.y) {
			f32 size     = fn_800A0414(player);
			self->_9FC.x = ppos.x + size * (f32)sin(prot.y + self->_A8C);
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z + size * (f32)cos(prot.y + self->_A8C);
			fn_800F54B4(self);
			self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mFlags |= 0x4000000000000ULL;
			if (self->_99C >= -2.3f) {
				self->_99C -= 0.36f * frames;
			} else {
				self->_99C = -2.3f;
			}
			self->mStep = 1;
		} else {
			self->SetAnim(2, TRUE, NULL, NULL, NULL, 0.0f, 2.0f, 0.0f, 10.0f);
			self->mTimer = 60 / frames;
			self->mStep  = 10;
		}
		break;
	case 1:
		if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mStatus.mFlags &= ~8;
			self->mFlags &= ~0x4000000000000ULL;
			self->mStep++;
		}
		break;
	case 2: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->_9FC.x = 12.0f + self->_9D8.x - rand() % 2400 / 100.0f;
			self->_9FC.y = self->_9D8.y;
			self->_9FC.z = 12.0f + self->_9D8.z - rand() % 2400 / 100.0f;
			self->SetAnim(2, TRUE, NULL, NULL, NULL, 0.0f, 2.0f, 0.0f, 10.0f);
			self->mTimer = 60 / frames;
			self->mStep  = 10;
		}
		break;
	}
	case 10:
		self->_984   = 1.0f;
		self->_9FC.x = ppos.x;
		self->_9FC.y = ppos.y;
		self->_9FC.z = ppos.z;
		self->_980   = PI + (f32)atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		Distance(&self->_9FC, &self->mMoveTranslate);
		if (--self->mTimer < 0) {
			self->_98C = 0.0f;
			self->_984 = 0.0f;
			self->mStatus.mFlags |= 0x8000;
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
			self->_9A0 = 1.0f;
			self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
			self->mStep++;
		}
		break;
	case 11:
		self->_9A0 -= 0.01f * frames;
		if (self->_9A0 <= 0.0f) {
			self->_9A0   = 0.0f;
			self->mTimer = 210 / frames;
			self->mStep++;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 12:
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			self->mTimer = 0;
			fn_800FC930(self);
			if (self->_A88 != 0xFF) {
				lbl_8037E940[self->_A88] = -1;
				self->_A84               = NULL;
				self->_A88               = 0xFF;
			}
			self->mStep++;
		}
		break;
	case 13:
		break;
	}
}

// state 4: jumps around the robot and spits webs at it
void fn_800F9AA0(XObjSpiderSmall* self, u32 frames)
{
	CPad pad(0);
	Vec ppos = GAME_PLAYER->mMoveTranslate;
	Vec prot = GAME_PLAYER->mMoveRotate;
	switch (self->mStep) {
	case 0:
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		if (Distance(&ppos, &self->mMoveTranslate) >= 30.0f) {
			self->_984   = 0.0f;
			self->mTimer = 1;
			self->mStep++;
		}
		break;
	case 1:
		if (--self->mTimer < 0) {
			self->_9A0 = 1.0f;
			self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
			self->mStep++;
		}
		break;
	case 2:
		self->_9A0 -= 0.01f * frames;
		if (self->_9A0 <= 0.0f) {
			self->_9A0   = 0.0f;
			self->mTimer = 210 / frames;
			self->mStep++;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 3:
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			self->mTimer = rand() % 30 + 30;
			self->mStep++;
		}
		break;
	case 4:
		if (--self->mTimer < 0) {
			f32 x, z, y, dist;
			f32 size               = fn_800A0414(GAME_PLAYER);
			self->_A8C             = DEG2RAD * (rand() % 360);
			dist                   = 10.0f + size;
			self->mMoveTranslate.x = ppos.x + dist * (f32)sin(prot.y + self->_A8C);
			self->mMoveTranslate.y = ppos.y;
			self->mMoveTranslate.z = ppos.z + dist * (f32)cos(prot.y + self->_A8C);
			x                      = self->mMoveTranslate.x;
			z                      = self->mMoveTranslate.z;
			y                      = self->mMoveTranslate.y;
			self->_980             = atan2(ppos.x - x, ppos.z - z);
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
			self->mStep++;
		}
		break;
	case 5:
		self->_9A0 += 0.01f * frames;
		if (self->_9A0 >= 1.0f) {
			self->_9A0   = 1.0f;
			self->mTimer = 210 / frames;
			self->mStep++;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 6:
		fn_800F4C10(&self->mShot, self->_A88, frames);
		break;
	}
}

// NONMATCHING: r9 and r10 swap around the web sound
// state 5: rises back up on its thread
void fn_800F9350(XObjSpiderSmall* self, u32 frames)
{
	CJObj obj;
	Vec ppos = GAME_PLAYER->mMoveTranslate;
	CPad pad(0);
	switch (self->mStep) {
	case 0:
		self->mStatus.mFlags |= 0x8000;
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
		self->_9A0 = 0.0f;
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		DistanceXZ(&ppos, &self->mMoveTranslate);
		if (fn_800F57C0(self) && GAME_DEMO == NULL) {
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
			self->mStep++;
		}
		break;
	case 1:
		self->_9A0 += 0.01f * frames;
		if (self->_9A0 >= 1.0f) {
			self->_9A0 = 1.0f;
			self->mStep++;
			self->mTimer = 210 / frames;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 2:
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			self->mStatus.mFlags &= ~0x8000;
			self->SetState(3);
			self->mStatus.mFlags |= 0x40;
			lbl_8037E97C[GAME->mStage].mSpiders[self->_AB4].mFlags |= 0x40;
			self->_ABC = 180 / frames;
			switch (GAME->mStage) {
			case 3:
			case 7:
			case 9:
				break;
			default:
				if (rand() % 4 == 0) {
					self->mStatus.mFlags |= 0x20;
				}
				break;
			}
		}
		break;
	}
}

// state 7: spits a web and shrinks away
void fn_800F8634(XObjSpiderSmall* self, u32 frames)
{
	CJObj obj;
	CPad pad(0);
	switch (self->mStep) {
	case 0:
		self->_98C = 0.0f;
		self->_984 = 0.0f;
		self->mStatus.mFlags |= 0x8000;
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
		self->_9A0 = 1.0f;
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		{
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
		}
		self->mStep++;
		break;
	case 1:
		self->_9A0 -= 0.01f * frames;
		if (self->_9A0 <= 0.0f) {
			self->_9A0 = 0.0f;
			self->mStep++;
			self->mTimer = 210 / frames;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 2:
		if (self->mStatus.mFlags & 0x40000) {
			if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
				self->mStep++;
				self->mTimer = 0;
				fn_800FC930(self);
				if (self->_A88 != 0xFF) {
					lbl_8037E940[self->_A88] = -1;
					self->_A84               = NULL;
					self->_A88               = 0xFF;
				}
			}
		} else if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			if (self->mStatus.mFlags & 0x2000) {
				self->mMoveTranslate.x = self->_9D8.x;
				self->mMoveTranslate.y = self->_9D8.y;
				self->mMoveTranslate.z = self->_9D8.z;
				self->mStatus.mFlags &= ~0x2000;
			}
			self->SetState(5);
			self->mStatus.mFlags &= ~0x40;
			self->mStatus.mFlags &= ~0x20;
			lbl_8037E97C[GAME->mStage].mSpiders[self->_AB4].mFlags &= ~0x40;
		}
		break;
	case 3:
		break;
	}
}

// state 6: drops down on its thread and lands
void fn_800F8DB8(XObjSpiderSmall* self, u32 frames)
{
	CJObj obj;
	CPad pad(0);
	switch (self->mStep) {
	case 0:
		self->mStatus.mFlags |= 0x8000;
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
		self->_9A0 = 0.0f;
		self->_9A4 = 0.0f;
		if (self->mStatus.mFlags & 0x40000) {
			self->mTimer = 2 / frames;
			self->mStep++;
		}
		break;
	case 1:
		if (--self->mTimer < 0) {
			self->_984 = 0.0f;
			self->_9A4 = 1.0f;
			self->SetAnim(12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStatus.mFlags |= 8;
			self->_9FC.x = self->mMoveTranslate.x;
			self->_9FC.y = self->mMoveTranslate.y;
			self->_9FC.z = self->mMoveTranslate.z;
			self->_984   = 0.0f;
			self->_99C   = 4.0f;
			self->_980   = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
			self->mStatus.mFlags &= ~1;
			self->mStep++;
		}
		break;
	case 2:
		if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mStatus.mFlags &= ~8;
			self->mFlags &= ~0x4000000000000ULL;
			self->mStep++;
		}
		break;
	case 3: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = rand() % 30 + 30;
			self->mStatus.mFlags &= ~0x8000;
			self->SetState(3);
			self->mFlags &= ~0x4000000000000ULL;
			self->mStatus.mFlags |= 0x40;
			self->mStep = 0;
		}
		break;
	}
	}
	fn_80068C8C(&self->_9A0, &self->_9A4, 0.08f * frames);
	self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
}

void fn_800FCF00(XObjSpiderSmall* self)
{
}

// NONMATCHING: register order
// state 1, hit: blinks green, drops an item and waits out the web
void fn_800FC970(XObjSpiderSmall* self, u32 frames)
{
	self->mTimer++;
	if (self->mTimer % 30 < 15) {
		self->SetColor(1, 0, 0x80, 0, 0.0f);
	} else {
		self->SetColor(1, 0, 0, 0, 0.0f);
	}
	switch (self->mStep) {
	case 0:
		if (self->mStatus.Check(0x800000)) {
			fn_800F54B4(self);
		}
		fn_8001E904(GAME_SOUND, 0x127, 0xFF);
		if (!(GAME_FLAGS & 0x100)) {
			fn_800242A8(lbl_8025C400 + 0x965C, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.4f);
		}
		lbl_8037E97C[GAME->mStage].mSpiders[self->_AB4].mFlags |= 0x80000;
		{
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			self->mShot.Set(CVec(x, y, z), 3);
		}
		self->mStep++;
		break;
	case 1: {
		self->mStep++;
		self->mTimer = 0;
		fn_800FC930(self);
		Vec pos;
		pos.x = self->mMoveTranslate.x;
		pos.y = self->mMoveTranslate.y;
		pos.z = self->mMoveTranslate.z;
		s32 kind;
		if (IsStage("stage07")) {
			switch (self->_258) {
			case 0:
			case 3:
			case 5:
			case 7:
			case 10:
				kind = 1;
				break;
			case 2:
			case 8:
				kind = 0;
				break;
			case 1:
			case 4:
			case 6:
			case 9:
			default:
				kind = 2;
				break;
			}
		} else if (rand() % 2 > 0) {
			kind = 2;
		} else if (rand() % 4 > 1) {
			kind = 1;
		} else {
			kind = 0;
		}
		if (self->_AA8) {
			kind = 2;
		}
		fn_800EA554(&pos, &self->_9D8, kind);
		break;
	}
	case 2:
		self->mFlags &= ~0x10000ULL;
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			if (self->_A88 != 0xFF) {
				lbl_8037E940[self->_A88] = -1;
				self->_A84               = NULL;
				self->_A88               = 0xFF;
			}
			self->mStep++;
		}
		break;
	case 3:
		break;
	}
}

// NONMATCHING: the web loop swaps r5 and r6
// runs the command an event gave it with vf124
void fn_800FCF04(XObjSpiderSmall* self, u32 frames)
{
	switch (self->_A30[0]) {
	case 0:
		break;
	case 1:
		if (!(self->mStatus.mFlags & 0x8000)) {
			self->SetState(1);
		}
		self->_A30[0] = 0;
		break;
	case 2:
		if (self->mStatus.mFlags & 0x10000) {
			GAME_WORD(0x5D08C) = 1;
		} else {
			GAME_WORD(0x5D08C) = 0;
		}
		self->_A30[0] = 0;
		break;
	case 3:
		if (self->mStatus.mFlags & 0x80000) {
			GAME_WORD(0x5D08C) = 1;
		} else {
			GAME_WORD(0x5D08C) = 0;
		}
		self->_A30[0] = 0;
		break;
	case 4:
		self->mStatus.mFlags |= 0x200000;
		self->_A30[0] = 0;
		break;
	case 5:
		self->mStatus.mFlags |= 0x400000;
		self->_A30[0] = 0;
		break;
	case 6:
		self->mFlags |= 0x4000000000000ULL;
		self->_A30[0] = 0;
		break;
	case 7:
		if (self->_A30[1] == 1) {
			self->mStatus.mFlags &= ~0x2000000;
		} else {
			self->mStatus.mFlags |= 0x2000000;
		}
		self->_A30[0] = 0;
		self->_A30[1] = 0;
		break;
	case 8:
		fn_800FFB00(self);
		fn_800FF168(self);
		self->_A30[0] = 0;
		break;
	case 9:
		self->_9D8.x           = self->_A30[1] / 100.0f;
		self->_9D8.y           = self->_A30[2] / 100.0f;
		self->_9D8.z           = self->_A30[3] / 100.0f;
		self->_A1C.x           = self->_9D8.x;
		self->_A1C.y           = self->_9D8.y;
		self->_A1C.z           = self->_9D8.z;
		self->mMoveTranslate.x = self->_9D8.x;
		self->mMoveTranslate.y = self->_9D8.y;
		self->mMoveTranslate.z = self->_9D8.z;
		self->mMoveTranslate.mTarget.Set(self->mMoveTranslate);
		self->_A30[0] = 0;
		break;
	case 10:
		self->_A08.x  = self->_A30[1] / 100.0f;
		self->_A08.y  = self->_A30[2] / 100.0f;
		self->_A08.z  = self->_A30[3] / 100.0f;
		self->_A14    = self->_A30[4];
		self->_A18    = self->_A30[5];
		self->_A30[0] = 0;
		break;
	case 11:
		self->_984 = 0.0f;
		self->SetState(5);
		self->mShot.mStatus.mFlags |= 1;
		self->mShot._28 = 10;
		self->_A30[0]   = 0;
		break;
	case 12:
		self->mStatus.mFlags |= 0x40000;
		self->mFlags |= 0x10000;
		self->mFlags |= 0x4000000000000ULL;
		self->SetState(6);
		self->mStatus.mFlags |= 0x8000;
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
		self->_9A0 = 0.0f;
		self->_9A4 = 0.0f;
		self->_984 = 0.0f;
		for (int i = 0; i < 12; i++) {
			if (lbl_8037E940[i] == -1) {
				self->_A84      = lbl_8037E910[i];
				self->_A88      = i;
				lbl_8037E940[i] = self->_258;
				break;
			}
		}
		self->_A30[0] = 0;
		break;
	case 21:
		if (!(self->mStatus.mFlags & 0x8000) && !(self->mStatus.mFlags & 0x80000) && (self->mFlags & 0x10000)) {
			self->SetState(9);
		}
		self->_A30[0] = 0;
		break;
	case 23:
		if (self->_A30[1] == 1) {
			fn_800F53F8(1);
		} else {
			fn_800F53F8(0);
		}
		self->_A30[0] = 0;
		self->_A30[1] = 0;
		break;
	}
}

// NONMATCHING: the battery clamp recomputes its address in other places
// caught in the web: drains the robot's battery and tints the screen red
void fn_800FD5E0(XObjSpiderSmall* self, u32 frames)
{
	CPad pad(0);
	if ((self->mStatus.mFlags & 2) && !(self->mStatus.mFlags & 0x10)) {
		fn_8001E660(GAME_SOUND, 0x12A);
		self->mStatus.mFlags &= ~2;
		self->SetState(1);
		if (self->_9E4 >= 10.0f) {
			self->_9E8 -= 5;
			if (self->_9E8 <= 0) {
				self->_9E8 = 0;
			}
		} else if (--self->_9E8 <= 0) {
			self->_9E8 = 0;
		}
	}
	{
		UnkSpiderPlayer* player = GAME_PLAYER;
		Vec ppos                = player->mMoveTranslate;
		CVec pos(ppos);
		f32 dist = Distance(&pos, &self->mMoveTranslate);
		if ((self->mStatus.mFlags & 0x800000) && GAME_DEMO == NULL && fn_800A0414(GAME_PLAYER) < 2.0f) {
			if (--self->_A94 <= 0) {
				GAME_PLAYER->UseBattery(3.0f);
				self->_A94 = 30;
				if (lbl_8065B2D4 == 0) {
					self->mStatus.mFlags |= 0x1000000;
					lbl_8065B2D4 = 1;
					fn_80066190(GAME_EVTMGR, 1, 1);
				}
				if (self->mStatus.mFlags & 0x1000000) {
					lbl_8065B2CC[0] = 60;
					lbl_8065B2CC[1] = 0;
				}
			}
		}
		if (self->mStatus.mFlags & 0x1000000) {
			fn_800A0014(player, FALSE, TRUE, 0.0f);
			if (lbl_8065B2CC[0] > 0) {
				if ((lbl_8065B2CC[0] -= frames) < 0) {
					lbl_8065B2CC[0] = 0;
				}
				lbl_8065B2CC[1]++;
				if (lbl_8065B2CC[0] > 0) {
					f32 red = 128.0f * (lbl_8065B2CC[0] / 60.0f);
					player->SetColor(1, red, 0, 0, 0.0f);
					fn_80023EE0(GAME_SCRNCOLOR, 0.0f, red, 0, 0, 0, red, 0, 0, 0);
				} else {
					player->SetColor(1, 0, 0, 0, 0.0f);
					fn_80023EE0(GAME_SCRNCOLOR, 0.0f, 0, 0, 0, 0, 100, 100, 200, 0);
				}
			}
		}
	}
	fn_800FCF04(self, frames);
}

} // extern "C"

s32 XObjSpiderSmall::vf100(s32 a, s32 b)
{
	mStatus.mFlags |= 1;
	return b;
}

void XObjSpiderSmall::vf138(const Vec* pos, f32 f)
{
	if (!(mStatus.mFlags & 2) && !(mStatus.mFlags & 0x10) && !(mStatus.mFlags & 0x8000)) {
		fn_8008A6C0(this, pos, f);
		mStatus.mFlags |= 2;
		Vec v = { 0.0f, 0.0f, 0.0f };
		v.x   = pos->x;
		v.y   = pos->y;
		v.z   = pos->z;
		_9CC  = v;
		_9E4  = f;
	}
}

// NONMATCHING: the empty loop's first test
extern "C" s32 fn_800FDABC(XObjSpiderSmall* self, s32 n, s32* args)
{
	for (s32 i = 0; i < n; i++) { }
	if (n < 1) {
		return 0;
	}
	self->_A30[0] = args[0];
	if (n < 2) {
		self->_A30[1] = 0;
	} else {
		self->_A30[1] = args[1];
	}
	if (n < 3) {
		self->_A30[2] = 0;
	} else {
		self->_A30[2] = args[2];
	}
	if (n < 4) {
		self->_A30[3] = 0;
	} else {
		self->_A30[3] = args[3];
	}
	if (n < 5) {
		self->_A30[4] = 0;
	} else {
		self->_A30[4] = args[4];
	}
	if (n < 6) {
		self->_A30[5] = 0;
	} else {
		self->_A30[5] = args[5];
	}
	return 0;
}

void XObjSpiderSmall::vfC0()
{
}

void XObjSpiderSmall::vfCC()
{
}

void XObjSpiderSmall::vfC8()
{
}

void XObjSpiderSmall::vfB8()
{
}

// a vertex of the shadow's rim
struct UnkSpiderVtx {
	Vec mPos;   // _00
	u32 mColor; // _0C
};

// NONMATCHING: float register order
void XObjSpiderSmall::vfBC(Vec* pos, f32 scale)
{
	if ((mFlags & 8) && (mStatus.mFlags & 0x40) && (mFlags & 0x10000)) {
		Vec p    = *pos;
		Vec rot  = { -HALF_PI, 0.0f, 0.0f };
		Vec size = mMoveScale * (mInfo != NULL ? *(u32*)&mInfo->_00[0xC] : 0);
		size.x   = _998;
		size.y   = _998;
		size.z   = _998;
		fn_8019B56C(GAME_RENDERMODE, 1, 4, 5, 15);
		u32 edge   = 128.0f * scale;
		u32 center = 160.0f * scale;
		fn_80198258(lbl_8025C400, 12, center, edge, p.x, p.y, p.z, size.x, size.z, rot.x, rot.y, rot.z);
		UnkSpiderVtx vtx[26];
		f32 hx = size.x * 0.5f;
		f32 hz = size.z * 0.5f;
		for (s32 i = 0; i < 13; i++) {
			f32 ang               = 0.5235988f * i;
			vtx[i * 2].mPos.x     = p.x - hx * (f32)sin(ang);
			vtx[i * 2].mPos.z     = p.z + hz * (f32)cos(ang);
			vtx[i * 2].mPos.y     = p.y;
			vtx[i * 2].mColor     = edge;
			vtx[i * 2 + 1].mPos.x = p.x - (1.5f + hx) * (f32)sin(ang);
			vtx[i * 2 + 1].mPos.z = p.z + (1.5f + hz) * (f32)cos(ang);
			vtx[i * 2 + 1].mPos.y = p.y;
			vtx[i * 2 + 1].mColor = 0;
		}
		fn_801988EC(lbl_8025C400, 0x98, GAME_VIEWMTX, vtx, 26);
	}
}

void XObjSpiderSmall::vfAC()
{
	vfB0(0);
	if (_A88 != 0xFF && (mShot.mStatus.mFlags & 1) && lbl_8037E910[_A88] != NULL) {
		HSD_JObj* jobj = lbl_8037E910[_A88]->mJObj;
		HSD_JObjSetTranslateX(jobj, mShot.mPos.x);
		HSD_JObjSetTranslateY(jobj, mShot.mPos.y);
		HSD_JObjSetTranslateZ(jobj, mShot.mPos.z);
		HSD_JObjSetRotationY(jobj, mShot._20);
		HSD_JObjDispAll(jobj, NULL, 7, 0);
	}
}

// the bgm playing, none while one is queued
static inline s32 GetBgm()
{
	return GAME_WORD(0xE80E8) <= 0 ? GAME_WORD(0xE80D8) : 0;
}

// the player stands in a live spider's area
static inline bool IsPlayerNear()
{
	Vec pos = GAME_PLAYER->mMoveTranslate;
	for (int i = 0; i < 16; i++) {
		XObjSpiderSmall* spider = (XObjSpiderSmall*)GAME_POOL->Get(i);
		if ((spider->mFlags & 1) && !spider->IsDead() && (spider->mFlags & 0x10000) && !(spider->mStatus.mFlags & 0x2000000)) {
			if (pos.y < HSD_JObjGetTranslationY(spider->mJObj) - 20.0f || pos.y > 20.0f + HSD_JObjGetTranslationY(spider->mJObj)) {
				continue;
			}
			f32 minX = 1.5f + spider->_A08.x;
			f32 maxX = spider->_A08.x + spider->_A14 - 1.5f;
			f32 minZ = 1.5f + spider->_A08.z;
			f32 maxZ = spider->_A08.z + spider->_A18 - 1.5f;
			if (minX > pos.x || maxX < pos.x || minZ > pos.z || maxZ < pos.z) {
				continue;
			}
			return true;
		}
	}
	return false;
}

static inline void SetObjVec(CObjVec& v, const Vec& pos)
{
	v.Set(pos);
	v.mTarget.Set(pos);
}

// NONMATCHING: register allocation
void XObjSpiderSmall::Update(u32 frames)
{
	CPad pad(0);
	bool music = true;
	if (IsStage("stage00") || IsStage("stage19")) {
		music = false;
	}
	if (_258 == 0 && music && !IsStage("stage07")) {
		switch (_AC4) {
		case 0:
			if (IsPlayerNear() && GAME_DEMO == NULL && GetBgm() != -0xFFF4) {
				_AAC = GetBgm();
				_AB0 = *(u8*)(lbl_8025C400 + 0xE80F0);
				fn_8001E904(GAME_SOUND, -0xFFF4, 0xFF);
				_AC4++;
			}
			break;
		case 1:
			if (fn_800665E0(GAME_EVTMGR)) {
				if (fn_80034E4C(0x9E) > 0) {
					if (fn_8001E6C8(GAME_SOUND, -0xFFF4)) {
						fn_8001E660(GAME_SOUND, -0xFFF4);
					}
					_AC4 = 2;
				} else {
					if (fn_8001E6C8(GAME_SOUND, -0xFFF4)) {
						fn_8001E660(GAME_SOUND, -0xFFF4);
					}
					_AC4 = 0;
				}
			} else if (!IsPlayerNear() || GAME_DEMO != NULL) {
				if (_AAC != 0) {
					fn_8001E904(GAME_SOUND, _AAC, _AB0);
					_AC4 = 0;
				} else if (GetBgm() == -0xFFF4) {
					fn_8001E660(GAME_SOUND, -0xFFF4);
					_AC4 = 0;
				}
			}
			break;
		case 2:
			if (!IsPlayerNear() || GAME_DEMO != NULL) {
				if (_AAC != 0) {
					fn_8001E904(GAME_SOUND, _AAC, _AB0);
					_AC4 = 0;
				} else if (GetBgm() == -0xFFF4) {
					fn_8001E660(GAME_SOUND, -0xFFF4);
					_AC4 = 0;
				}
			}
			break;
		}
	}
	if (GAME_FLAGS & 0x400) {
		return;
	}
	if (_258 == 0) {
		fn_800EA780(lbl_8065B2C8, frames);
	}
	if (!(mFlags & 0x10000) && mState != 1) {
		return;
	}
	vf148();
	fn_800FD5E0(this, frames);
	if (!(mStatus.mFlags & 0x2000000)) {
		(this->*lbl_8022B230[mState])(frames);
		CVec dir;
		if (mStatus.mFlags & 0x100) {
			dir.x = 0.0f;
			dir.z = 0.0f;
			dir.y = _984;
			SetRotateY(_980, _98C);
		} else {
			if (mStatus.mFlags & 0x10) {
				mMoveTranslate.y += _99C;
			} else {
				mMoveTranslate.y = _9D8.y;
			}
			dir.x = _984 * (f32)sin(_980);
			dir.z = _984 * (f32)cos(_980);
			dir.y = 0.0f;
			SetRotateY(_980, _98C);
		}
		SetObjVec(mMoveTranslate, mMoveTranslate + dir * frames);
		SetMoveTranslate(&mMoveTranslate, 0);
	}
	fn_80090ED4(this, frames);
	fn_800F4F48(&mShot, _A88, frames, FALSE);
}

void XObjSpiderSmall::Release()
{
	if (fn_8001E6C8(GAME_SOUND, -0xFFF4)) {
		fn_8001E660(GAME_SOUND, -0xFFF4);
	}
	fn_800911C0(this);
}

// finds the stage's webs, the objects of kind 0x1BC
static inline void FindWebs()
{
	for (int i = 0; i < 12; i++) {
		lbl_8037E910[i] = NULL;
		lbl_8037E940[i] = -1;
	}
	lbl_8065B2E0 = 0;
	lbl_8065B2DC = NULL;
	s32 n        = 0;
	for (s32 i = 0; lbl_8065B2DC == NULL && i < GAME->mObjectCount; i++) {
		XObj* obj = GAME->mObjects[i];
		if ((obj->mFlags & 1) && *(s32*)obj->_238 == 0x1BC) {
			lbl_8037E910[n] = obj;
			lbl_8037E910[n]->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			n++;
			if (n >= 12) {
				break;
			}
		}
	}
}

void XObjSpiderSmall::Reset()
{
	fn_800928E8(this);
	mSfx.mTable = lbl_8022B170;
	_AAC        = GAME_WORD(0xE80E8) <= 0 ? GAME_WORD(0xE80D8) : 0;
	if (_258 == 0) {
		FindWebs();
	}
}

// NONMATCHING: the spider pool loop folds 0x9B4 into the index
BOOL XObjSpiderSmall::vf9C(void* info)
{
	if (fn_80091C80(this, info)) {
		if (_258 == 0) {
			for (int i = 0; i < 16; i++) {
				((XObjSpiderSmall*)GAME_POOL->Get(i))->Hide();
			}
			if (_AB8 != GAME_WORD(0xE5D28)) {
				for (s32 i = 0; i < 0x27; i++) {
					lbl_8037E97C[i].Clear();
				}
			}
			_AB8 = GAME_WORD(0xE5D28);
		}
		_A2C = GAME_WORD(0x3444);
		fn_800FFB00(this);
		fn_800FF168(this);
		_AB4 = _258;
		if (lbl_8037E97C[_A2C].mSpiders[_258].mFlags & 0x80000) {
			SetState(2);
		}
		if (_258 == 0) {
			FindWebs();
		}
		return TRUE;
	}
	return FALSE;
}

void XObjSpiderSmall::vf94()
{
	_A2C = -1;
	fn_800FFB00(this);
	SetState(0);
	mStatus.mFlags |= 0x4000000;
	_AAC = -1;
	_AB4 = -1;
	if (_258 == 0) {
		for (s32 i = 0; i < 0x27; i++) {
			lbl_8037E97C[i].Clear();
		}
	}
	_AB8 = GAME_WORD(0xE5D28);
}

void XObjSpiderSmall::Init(u32 id)
{
	fn_800929C4(this, id);
	_A2C = -1;
	fn_800FFB00(this);
	SetState(0);
	mStatus.mFlags |= 0x4000000;
	_AAC = -1;
	_AB4 = -1;
	if (_258 == 0) {
		for (s32 i = 0; i < 0x27; i++) {
			lbl_8037E97C[i].Clear();
		}
	}
	_AB8 = GAME_WORD(0xE5D28);
}

// NONMATCHING: its Vec constants sit 0x18 lower in .rodata
extern "C" void fn_800FFB00(XObjSpiderSmall* self)
{
	self->_980           = 0.0f;
	self->_984           = 0.0f;
	self->_988           = 0.0f;
	self->_98C           = 0.1f;
	self->_99C           = 0.0f;
	self->_9A0           = 0.0f;
	self->_9A4           = 0.0f;
	self->mState         = 0;
	self->mPrevState     = 0;
	self->mStep          = 0;
	self->mTimer         = 0;
	self->mStatus.mFlags = 0;
	self->_9C0           = 0;
	self->_9C4           = 1.0f;
	self->_9C8           = 1.0f;
	Vec v0               = { 0.0f, 0.0f, 0.0f };
	self->_9CC           = v0;
	Vec v1               = { 0.0f, 0.0f, 0.0f };
	self->_9D8           = v1;
	Vec v2               = { 0.0f, 0.0f, 0.0f };
	self->_9F0           = v2;
	Vec v3               = { 0.0f, 0.0f, 0.0f };
	self->_9FC           = v3;
	self->_9E4           = 0.0f;
	self->_9EC           = 3;
	self->_9E8           = self->_9EC;
	self->_A30[0]        = 0;
	self->_A30[1]        = 0;
	self->_A48           = 0;
	self->_994           = 1.0f;
	self->mFlags |= 8;
	self->mFlags |= 0x10000000;
	self->mShot.mPos.x         = 0.0f;
	self->mShot.mPos.y         = 0.0f;
	self->mShot.mPos.z         = 0.0f;
	self->mShot.mTarget.x      = 0.0f;
	self->mShot.mTarget.y      = 0.0f;
	self->mShot.mTarget.z      = 0.0f;
	self->mShot._20            = 0.0f;
	self->mShot._24            = 0.0f;
	self->mShot._28            = 0;
	self->mShot._2C            = 0;
	self->mShot.mStatus.mFlags = 0;
	self->_998                 = 16.0f;
	if (self->_258 == 0) {
		fn_800EA99C(lbl_8065B2C8);
		lbl_8065B2CC[0] = 0;
		lbl_8065B2CC[1] = 0;
		lbl_8065B2D4    = 0;
		lbl_8065B2D8    = 0;
	}
	self->_A84 = NULL;
	self->_A88 = 0xFF;
	self->_A8C = 0.0f;
	self->_A90 = 0.0f;
	self->_A94 = 0;
	self->_A98 = 0.0f;
	Vec v4     = { 0.0f, 0.0f, 0.0f };
	self->_A9C = v4;
	Vec v5     = { 0.0f, 0.0f, 0.0f };
	self->_A08 = v5;
	self->_A14 = 0.0f;
	self->_A18 = 0.0f;
	Vec v6     = { 0.0f, 0.0f, 0.0f };
	self->_A1C = v6;
	self->_A28 = 0.0f;
	self->SetColor(1, 0, 0, 0, 0.0f);
	self->_ABC = 0;
	self->_AC0 = 0;
	self->_AC4 = 0;
	self->_AA8 = 0;
}
