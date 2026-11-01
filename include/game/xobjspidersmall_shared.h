#ifndef _GAME_XOBJSPIDERSMALL_SHARED_H
#define _GAME_XOBJSPIDERSMALL_SHARED_H

#include "game/hsd_jobj.h"
#include "game/cparticle.h"
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

#endif
