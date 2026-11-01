#ifndef _GAME_XOBJSPIDER_SHARED_H
#define _GAME_XOBJSPIDER_SHARED_H

#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/scr.h"

// XObjSpider, game/xobjspider.cpp, the small spiders that walk the floor and the walls.
// the class name is from its vtable, nothing else has a name in the dol. offsets are usa

// __vt__7AnimSfx, plays the sounds an animation has at its frames. game/sound.cpp has the
// methods, and no destructor, so the objects that hold one don't destroy it
// one entry of an AnimSfx table, the sound an animation plays at a frame
struct AnimSfxEntry {
	s32 mAnim;    // _00, -1 ends the table
	f32 mFrame;   // _04
	u8 _08[0x10]; // _08
};

class AnimSfx {
public:
	AnimSfxEntry* mTable; // _00
	s32 mAnim;            // _04
	f32 mFrame;           // _08
	// _0C vtable

	virtual void vf08();                           // _08
	virtual void Play(s32 id, s32 vol, f32 frame); // _0C
};

// what an object is read from, game/xobjs.h has it but needs game/cobj.h, __vt__9CoordInfo
class CoordInfo {
public:
	void* _00; // _00
	// _04 vtable

	CoordInfo(void* value)
	    : _00(value)
	{
	}
	virtual ~CoordInfo() { } // _08
};

// another object the spider holds on one of its joints
struct UnkSpiderTie {
	HSD_JObj* mJoint; // _00
	XObj* mObj;       // _04
	Mtx mMtx;         // _08
};

// one thing the spider moves and draws through another object's model, 0x3C bytes
struct UnkSpiderPart {
	CVec mPos;       // _00
	CVec _10;        // _10
	f32 mRotY;       // _20
	f32 _24;         // _24
	s32 mState;      // _28
	s32 _2C;         // _2C
	CStatus mStatus; // _30, 1 while it's shown
	XObj* mObj;      // _38
};

class XObjSpider : public XObj {
public:
	f32 mDir;              // _980, the way it walks
	f32 mSpeed;            // _984
	f32 _988;              // _988
	f32 _98C;              // _98C
	f32 mTurnSpeed;        // _990
	u8 _994[0x4];          // _994
	f32 mThreadAlpha;      // _998, 0 to 1
	f32 mRadius;           // _99C
	f32 mFallSpeed;        // _9A0
	s32 mState;            // _9A4
	s32 mStep;             // _9A8
	s32 mTimer;            // _9AC
	CStatus mStatus;       // _9B0
	s32 mPrevState;        // _9B8
	s32 mWait;             // _9BC
	f32 _9C0;              // _9C0
	f32 _9C4;              // _9C4
	Vec mHitPos;           // _9C8
	Vec mHome;             // _9D4, y is the floor
	f32 mHitDist;          // _9E0
	s32 mWall;             // _9E4, 2 to 5 while it's on a wall
	s32 mLife;             // _9E8
	s32 mMaxLife;          // _9EC
	Vec mWallPos;          // _9F0
	Vec _9FC;              // _9FC
	Vec _A08;              // _A08
	Vec _A14;              // _A14
	u8 _A20[0xC];          // _A20
	Vec mShotTo;           // _A2C, where the part is shot at
	f32 _A38;              // _A38
	u8 _A3C[0x4];          // _A3C
	f32 _A40;              // _A40
	s32 _A44;              // _A44
	s32 _A48;              // _A48
	Vec mArea;             // _A4C, the corner of the floor it stays on
	f32 mAreaW;            // _A58
	f32 mAreaD;            // _A5C
	Vec _A60;              // _A60
	f32 _A6C;              // _A6C
	s32 mArgs[6];          // _A70, what the last event command said
	s32 _A88;              // _A88
	UnkSpiderPart mPart;   // _A8C
	UnkSpiderPart mPart2;  // _AC8
	u8 mCarry;             // _B04, which of the carried objects, 0xFF for none
	u8 _B05[0xB];          // _B05
	s16 _B10;              // _B10
	s32 _B14[4];           // _B14
	s32 _B24[3];           // _B24
	u8 _B30[0x20];         // _B30
	s16 _B50;              // _B50
	s32 _B54;              // _B54
	s16 _B58;              // _B58
	XObj* _B5C;            // _B5C
	s32 _B60;              // _B60
	Mtx _B64;              // _B64
	s32 _B94;              // _B94
	s32 _B98;              // _B98
	s32 _B9C;              // _B9C
	s32 mRumble;           // _BA0
	s32 mRumbleTimer;      // _BA4
	AnimSfx mSe;           // _BA8
	UnkSpiderTie mTies[8]; // _BB8

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
	virtual void vf138(const Vec* pos, f32 f); // _138
	virtual void vf148();                      // _148, plays the sounds along the anim

	// the states, sStates has them by number
	void State0(u32 frames);
	void State1(u32 frames);
	void State2(u32 frames);
	void State3(u32 frames);
	void State4(u32 frames);
	void State5(u32 frames);
	void State6(u32 frames);
	void State7(u32 frames);
	void State8(u32 frames);
	void State9(u32 frames);

	// XObj's _25C, scr.h has it in its padding
	s32 Get25C() const { return *(s32*)&_25C[0]; }

	void SetState(s32 state)
	{
		mStep      = 0;
		mTimer     = 0;
		mPrevState = mState;
		mState     = state;
	}
};
typedef char XObjSpider_size_check[sizeof(XObjSpider) == 0xD78 ? 1 : -1];
#include "game/gmath.h"
extern "C" {
#include <dolphin/mtx/mtx.h>
}

// XObjSpider, the small spiders. the class name is from its vtable, the functions that
// aren't its virtuals or states have no names, so they are extern "C" under their address

extern "C" {
s32 fn_8009C1BC(void* slots, s32 id);
f32 fn_800A2D88(const CJObj* obj); // the frame of the first anim
void fn_800EAB58(UnkSpiderPart* part, u8 carry, bool turned);
void fn_800F4350(XObjSpider* self);
void fn_800F38F4(XObjSpider* self);
}

extern "C" {
void HSD_PadRumbleOn(u8 no);
void HSD_PadRumbleOffN(u8 no);
void HSD_PadRumbleRemove(u8 no);
}

#define GAME_44    (*(u8**)(lbl_8025C400 + 0x44))
#define GAME_290   (*(void**)(lbl_8025C400 + 0x290))
#define GAME_QUAKE (lbl_8025C400 + 0x965C)
#define GAME_5D08C (*(s32*)(lbl_8025C400 + 0x5D08C))
#define GAME_5D080 (*(void**)(lbl_8025C400 + 0x5D080))

#define GAME_ITEMS ((UnkObjPool*)(lbl_8025C400 + 0x9720)) // 96 of them

// one of the things that drop down, kind 0x104 objects
struct UnkSpiderDrop {
	XObj* mObj; // _00
	Vec mPos;   // _04
	s32 mState; // _10
	s32 mTimer; // _14
	Vec mTo;    // _18, it falls to mTo.y
};

// one point of the shadow's ring
struct UnkShadowVtx {
	Vec mPos;   // _00
	u32 mColor; // _0C
};

// the three render objects the game keeps at _290, an id count of 0 means not set
struct UnkSpiderRenderA {
	u8 _00[0xC]; // _00
	s32 _0C;     // _0C
	s32 _10;     // _10
	s32 _14;     // _14
	s32 _18;     // _18
};

struct UnkSpiderRenderB {
	u8 _00[0x74]; // _00
	s32 _74;      // _74
	s32 _78;      // _78
	s32 _7C;      // _7C
};

struct UnkSpiderRenderC {
	u8 _00[0x30]; // _00
	s32 _30;      // _30
};

// the game object, the parts the spiders reach
struct UnkSpiderGame {
	u8 _0000[0x290];           // _0000
	UnkSpiderRenderA* _290;    // _0290
	UnkSpiderRenderB* _294;    // _0294
	UnkSpiderRenderC* _298;    // _0298
	u8 _029C[0x19B4 - 0x29C];  // _029C
	u8 mPadNo;                 // _19B4, the pad that rumbles
	u8 _19B5[0x4B];            // _19B5
	BOOL mRumbleOn;            // _1A00
	u8 _1A04[0x10C];           // _1A04
	u64 mFlags;                // _1B10
	u8 _1B18[0x331C - 0x1B18]; // _1B18
	s32 _331C;                 // _331C
	s32 _3320;                 // _3320
	s32 _3324;                 // _3324
	u8 _3328[0x96BC - 0x3328]; // _3328
	XObj* mChr;                // _96BC, the robot
	u8 _96C0[0x97AC - 0x96C0]; // _96C0
	void* _97AC;               // _97AC
	u8 _97B0[0x9860 - 0x97B0]; // _97B0
	XObj* mObjs[0x400];        // _9860
	s32 mObjCount;             // _A860

	// an event or a menu holds the robot
	bool IsBusy()
	{
		bool result = false;
		if ((_331C != 0 || _3320 != 0) && ((UnkSpiderGame*)lbl_8025C400)->_3324 == 0) {
			result = true;
		}
		return result;
	}
	bool CanAct()
	{
		bool result = false;
		if ((mFlags & 1) && !(mFlags & 0x22) && !IsBusy()) {
			result = true;
		}
		return result;
	}

	void StartRumble()
	{
		if (mRumbleOn) {
			HSD_PadRumbleOn(mPadNo);
		}
	}
	void StopRumble()
	{
		if (mRumbleOn) {
			HSD_PadRumbleOffN(mPadNo);
		}
		if (mRumbleOn) {
			HSD_PadRumbleRemove(mPadNo);
		}
	}
};
#define GAME         ((UnkSpiderGame*)lbl_8025C400)
#define GAME_OBJLIST (lbl_8025C400 + 0x9694)

// puts a CObjVec at a value and its target there too
static inline void SetNow(CObjVec& vec, const Vec& v)
{
	vec.Set(v);
	vec.mTarget.Set(v);
}

extern "C" {
void fn_800EA554(Vec* pos, Vec* to, s32 anim);
void fn_800EA780(void* mgr, u32 frames);
void fn_800EA99C(void* mgr);
void fn_800EB928(XObjSpider* self);
s32 fn_800EBA30(XObjSpider* self);
BOOL fn_800EBBF0(XObjSpider* self);
u32 fn_800EBDB0(XObjSpider* self);
void fn_800EAF18(UnkSpiderPart* part, u8 carry, u32 frames, s32 mode);
void fn_800F1068(XObjSpider* self, u32 frames);
void fn_800F1B10(XObjSpider* self, u32 frames);
void fn_800F1730(XObjSpider* self, u32 frames);
void fn_800F0090(XObjSpider* self, u32 frames);
void fn_800F0B7C(XObjSpider* self);
s32 fn_8006EBC8(s32 id);
bool IsFree__12XObjActorTaoFv(void* obj);
void* fn_8006EDB4(s16 id);
XObj* fn_80132698(void* list, const CoordInfo& info, s32 unused, BOOL async);
CVec fn_800621E0(XObj* obj);
void HSD_MtxGetScale(Mtx mat, Vec* vec);
void PSMTXScale(Mtx mtx, f32 x, f32 y, f32 z);
void fn_8010CDDC(f32 x, f32 y, f32 z);
void fn_800242A8(void* quake, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g);
void fn_8001E660(CSound* sound, s32 sfx);
int rand();
void fn_8019B56C(void* gx, s32 a, s32 b, s32 c, s32 d);
void fn_80198258(void* game, s32 kind, u32 a, u32 b, f32 x, f32 y, f32 z, f32 sx, f32 sz, f32 rx, f32 ry, f32 rz);
void fn_801988EC(void* game, s32 kind, void* tex, void* verts, s32 count);
void HSD_MtxGetTranslate(Mtx mat, Vec* vec);
void OSReport(const char* fmt, ...);
XObj* fn_8007F570(void* list, s16 id);
void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
void HSD_StateInvalidate(s32 state);
void fn_8019B278(void* mode, BOOL force);
void fn_8019AED0(void* stage, BOOL force);
void fn_8019AE94(void* obj, BOOL force);
void fn_8019AFA0(void* stage, s32 mode);
void fn_8019B34C(void* mode, s32 a, s32 b);
void fn_801992BC(void* game, s32 kind, u32 color0, u32 color1, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1);

extern AnimSfxEntry lbl_8022AB90[]; // the spider's sounds
extern XObj* lbl_8037E5C0[16];      // the objects the spiders carry, kind 0x13C
extern s32 lbl_8037E600[16];        // which spider has each of them, -1 for none
extern u8 lbl_8065B2B8;             // the drops' manager, it has no data of its own
extern UnkSpiderDrop lbl_8037E640[11];
extern s32 lbl_8065B2C4; // how many drops are out
extern s32 lbl_8065B2BC;
extern u8 lbl_8065B2C0;
}

// the carried object plays its anim 2 from where its anim ends, once
static inline void HitCarried(XObjSpider* self)
{
	if (!(self->mPart.mStatus.mFlags & 2)) {
		XObj* obj = lbl_8037E5C0[self->mCarry];
		if (obj != NULL && self->mPart.mState == 2) {
			f32 end = GetAnimEndFrame(obj->mJObj);
			obj->SetAnim(2, TRUE, NULL, NULL, NULL, end - GetAnimFrame(obj->mJObj), 1.0f, 0.0f, 10.0f);
			self->mPart.mState++;
			self->mPart.mStatus.mFlags |= 2;
		}
	}
}

// starts reading the object a stage table entry places
static inline XObj* ReadObj(s16 id)
{
	return fn_80132698(GAME_OBJLIST, CoordInfo(fn_8006EDB4(id)), 0, TRUE);
}

// turns it toward a point and says how far that is
static inline f32 HeadTo(XObjSpider* self, const Vec& to)
{
	self->mDir = atan2(to.x - self->mMoveTranslate.x, to.z - self->mMoveTranslate.z);
	f32 dx     = to.x - self->mMoveTranslate.x;
	f32 dy     = to.y - self->mMoveTranslate.y;
	f32 dz     = to.z - self->mMoveTranslate.z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}

// where the robot is
static inline Vec GetChrPos()
{
	return GAME->mChr->mMoveTranslate;
}

// takes the first of the carried objects nobody has
static inline void TakeCarried(XObjSpider* self)
{
	for (int i = 0; i < 16; i++) {
		if (lbl_8037E600[i] == -1) {
			self->mPart2.mObj = lbl_8037E5C0[i];
			self->mCarry      = i;
			lbl_8037E600[i]   = self->_258;
			return;
		}
	}
}

// a step sound, which one of the five goes by the spider's index
static inline void PlayStep(XObjSpider* self)
{
	switch (self->_258 % 5) {
	case 0:
		fn_8001E904(GAME_SOUND, 0x12B, 0xFF);
		break;
	case 1:
		fn_8001E904(GAME_SOUND, 0x12C, 0xFF);
		break;
	case 2:
		fn_8001E904(GAME_SOUND, 0x12D, 0xFF);
		break;
	case 3:
		fn_8001E904(GAME_SOUND, 0x12E, 0xFF);
		break;
	case 4:
		fn_8001E904(GAME_SOUND, 0x12F, 0xFF);
		break;
	}
}

// how far apart two points are on the floor
static inline f32 DistXZ(const Vec* a, const Vec* b)
{
	f32 dx = a->x - b->x;
	f32 dy = 0.0f;
	f32 dz = a->z - b->z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}

// the event's object with the id, ids from 10000 up
static inline XObj* FindEvtObj(s16 id)
{
	return GAME_5D080 != NULL ? fn_8007F570(&GAME_5D080, id) : NULL;
}

static inline XObj* GetObj(s16 id)
{
	return GAME->mObjs[id];
}

// hands the matrix back to the srt values
static inline void HSD_JObjClearUserMtx(HSD_JObj* jobj)
{
	JOBJ_H_ASSERT(1199, jobj);
	jobj->flags &= ~0x03800000;
	HSD_JObjSetMtxDirty(jobj);
}

// the joint at the end of a path of child and next steps
static inline HSD_JObj* GetJoint(HSD_JObj* jobj, s32* path, s32 depth)
{
	if (jobj == NULL) {
		return NULL;
	}
	for (s32 i = 0; i < depth; i++, path++) {
		jobj = HSD_JObjGetChild(jobj);
		for (s32 j = 0; j < *path; j++) {
			jobj = HSD_JObjGetNext(jobj);
			if (jobj == NULL) {
				return NULL;
			}
		}
	}
	return jobj;
}

// the jobj's place in the world
static inline Vec GetJObjPos(HSD_JObj* jobj)
{
	Vec v;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &v);
	return v;
}

// the states by number, Update calls them
// NONMATCHING: __sinit, retail has 0x1C8 bytes of .data in front of the initializer it copies
typedef void (XObjSpider::*XObjSpiderState)(u32 frames);
static XObjSpiderState lbl_8022AC50[10] = {
	&XObjSpider::State0, &XObjSpider::State1, &XObjSpider::State2, &XObjSpider::State3, &XObjSpider::State4,
	&XObjSpider::State5, &XObjSpider::State6, &XObjSpider::State7, &XObjSpider::State8, &XObjSpider::State9,
};

// ---- functions ----
// takes all the drops back, fn_800EA99C has it too
static inline void ResetDrops()
{
	lbl_8065B2C4 = 0;
	for (int i = 0; i < 11; i++) {
		lbl_8037E640[i].mObj   = NULL;
		lbl_8037E640[i].mState = 0;
		lbl_8037E640[i].mTimer = 0;
	}
	for (int i = 0; i < 96; i++) {
		XObj* obj = GAME_ITEMS->Get(i);
		if (obj->GetDescKind() == 0x104) {
			((u8*)obj)[0x9E8] = 0;
		}
	}
}

static s32 sThreadPath[] = { 0 };
static s32 sHoldPath[]   = { 0 };

#endif
