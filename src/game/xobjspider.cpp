#include "game/particle_srt.h"
#include "game/xobjspider.h"
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
bool fn_8012CA54(void* obj);
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

void XObjSpider::Init(u32 id)
{
	fn_800929C4(this, id);
	fn_800F4350(this);
	SetState(0);
	mStatus.mFlags |= 0x4000000;
}

void XObjSpider::vf94()
{
	fn_800F4350(this);
	SetState(0);
	mStatus.mFlags |= 0x4000000;
}

// puts everything back to how it starts
extern "C" void fn_800F4350(XObjSpider* self)
{
	self->mDir           = 0.0f;
	self->mSpeed         = 0.0f;
	self->_988           = 0.0f;
	self->_98C           = 0.0f;
	self->mTurnSpeed     = 0.1f;
	self->mFallSpeed     = 0.0f;
	self->mState         = 0;
	self->mPrevState     = 0;
	self->mStep          = 0;
	self->mTimer         = 0;
	self->mStatus.mFlags = 0;
	self->mWait          = 0;
	self->_9C0           = 1.0f;
	self->_9C4           = 1.0f;
	Vec v0               = { 0.0f, 0.0f, 0.0f };
	self->mHitPos        = v0;
	Vec v1               = { 0.0f, 0.0f, 0.0f };
	self->mHome          = v1;
	Vec v2               = { 0.0f, 0.0f, 0.0f };
	self->mWallPos       = v2;
	Vec v3               = { 0.0f, 0.0f, 0.0f };
	self->_9FC           = v3;
	Vec v4               = { 0.0f, 0.0f, 0.0f };
	self->_A08           = v4;
	Vec v5               = { 0.0f, 0.0f, 0.0f };
	self->_A14           = v5;
	self->_A38           = 0.0f;
	self->_A40           = 0.0f;
	self->_A44           = 0;
	self->_A48           = 0;
	self->mHitDist       = 0.0f;
	self->mWall          = 0;
	self->mMaxLife       = 3;
	self->mLife          = self->mMaxLife;
	self->mArgs[0]       = 0;
	self->mArgs[1]       = 0;
	self->_A88           = 0;
	self->mThreadAlpha   = 1.0f;
	self->mFlags |= 8;
	self->mPart.mPos.x         = 0.0f;
	self->mPart.mPos.y         = 0.0f;
	self->mPart.mPos.z         = 0.0f;
	self->mPart._10.x          = 0.0f;
	self->mPart._10.y          = 0.0f;
	self->mPart._10.z          = 0.0f;
	self->mPart.mRotY          = 0.0f;
	self->mPart._24            = 0.0f;
	self->mPart.mState         = 0;
	self->mPart._2C            = 0;
	self->mPart.mStatus.mFlags = 0;
	self->mPart.mObj           = NULL;
	self->mRadius              = 20.0f;
	self->mPart2.mObj          = NULL;
	self->mCarry               = 0xFF;
	Vec v6                     = { 0.0f, 0.0f, 0.0f };
	self->mArea                = v6;
	self->mAreaW               = 0.0f;
	self->mAreaD               = 0.0f;
	Vec v7                     = { 0.0f, 0.0f, 0.0f };
	self->_A60                 = v7;
	self->_A6C                 = 0.0f;
	self->_B10                 = 2;
	self->_B14[0]              = 0;
	self->_B14[1]              = 0;
	self->_B14[2]              = 0;
	self->_B14[3]              = 0;
	self->_B24[0]              = 100;
	self->_B24[1]              = 100;
	self->_B24[2]              = 100;
	self->_B54                 = 0;
	self->_B50                 = 0;
	self->_B5C                 = NULL;
	self->_B58                 = 0;
	self->_B60                 = 0;
	for (int i = 0; i < 8; i++) {
		self->mTies[i].mObj   = NULL;
		self->mTies[i].mJoint = NULL;
	}
	if (self->_258 == 0) {
		ResetDrops();
	}
	self->_B94         = 0;
	self->_B98         = 0;
	self->_B9C         = -1;
	self->mRumble      = 0;
	self->mRumbleTimer = 0;
}

BOOL XObjSpider::vf9C(void* info)
{
	if (fn_80091C80(this, info)) {
		mStatus.mFlags &= ~0x4000000;
		fn_800F4350(this);
		fn_800F38F4(this);
		return TRUE;
	}
	return FALSE;
}

// NONMATCHING: the cleared tables and the object loop take other registers
void XObjSpider::Reset()
{
	fn_800928E8(this);
	mSe.mTable = lbl_8022AB90;
	if (_258 == 0) {
		XObj** list = lbl_8037E5C0;
		for (int i = 0; i < 16; i++) {
			lbl_8037E5C0[i] = NULL;
			lbl_8037E600[i] = -1;
		}
		lbl_8065B2C0        = 0;
		lbl_8065B2BC        = 0;
		s32 n               = 0;
		UnkSpiderGame* game = GAME;
		for (int i = 0; i < game->mObjCount; i++) {
			XObj* obj = game->mObjs[i];
			if ((obj->mFlags & 1) && *(s32*)obj->_238 == 0x13C) {
				n++;
				*list++ = obj;
				if (n >= 16) {
					return;
				}
			}
		}
	}
}

void XObjSpider::Update(u32 frames)
{
	CPad pad(0);

	switch (mRumble) {
	case 0:
		break;
	case 1:
		if (GAME->mFlags & 0x800000000ULL) {
			GAME->StartRumble();
			mRumbleTimer = 24 / frames;
			mRumble++;
		} else {
			mRumble = 0;
		}
		break;
	case 2:
		if (--mRumbleTimer < 0) {
			UnkSpiderGame* game = GAME;
			game->StopRumble();
			mRumble = 0;
		}
		break;
	}

	if (GAME->mFlags & 0x400) {
		return;
	}
	if (_258 == 0) {
		fn_800EA780(&lbl_8065B2B8, frames);
	}
	if (!(mFlags & 0x10000) && mState != 1) {
		return;
	}
	vf148();
	fn_800F1B10(this, frames);
	if (!(mStatus.mFlags & 0x8000000)) {
		(this->*lbl_8022AC50[mState])(frames);
		CVec v;
		if (mWall == 4) {
			mMoveTranslate.x = mWallPos.x;
			v.y              = mSpeed * (f32)sin(mDir);
			v.z              = mSpeed * (f32)cos(mDir);
			v.x              = 0.0f;
			mMoveRotate.z    = -1.5707964f;
			SetRotateY(mDir, mTurnSpeed);
		} else if (mWall == 5) {
			mMoveTranslate.x = mWallPos.x;
			v.y              = mSpeed * (f32)sin(mDir);
			v.z              = mSpeed * (f32)cos(mDir);
			v.x              = 0.0f;
			mMoveRotate.z    = 1.5707964f;
			SetRotateY(mDir, mTurnSpeed);
		} else if (mWall == 2) {
			mMoveTranslate.z = mWallPos.z;
			v.y              = mSpeed * (f32)sin(mDir);
			v.x              = mSpeed * (f32)cos(mDir);
			v.z              = 0.0f;
			mMoveRotate.x    = -1.5707964f;
			mMoveRotate.z    = mDir - 1.5707964f;
		} else if (mWall == 3) {
			mMoveTranslate.z = mWallPos.z;
			v.y              = mSpeed * (f32)sin(mDir);
			v.x              = mSpeed * (f32)cos(mDir);
			v.z              = 0.0f;
			mMoveRotate.x    = 1.5707964f;
			mMoveRotate.z    = 1.5707964f + mDir;
		} else if (mStatus.mFlags & 0x100) {
			v.x = 0.0f;
			v.z = 0.0f;
			v.y = mSpeed;
			SetRotateY(mDir, mTurnSpeed);
		} else {
			if (mStatus.mFlags & 8) {
				mMoveTranslate.y += mFallSpeed;
			} else {
				mMoveTranslate.y = mHome.y;
			}
			v.x = mSpeed * (f32)sin(mDir);
			v.z = mSpeed * (f32)cos(mDir);
			v.y = 0.0f;
			SetRotateY(mDir, mTurnSpeed);
		}
		SetNow(mMoveTranslate, mMoveTranslate + v * (f32)frames);
		SetMoveTranslate(&mMoveTranslate, 0);
	}
	fn_80090ED4(this, frames);
	fn_800EAF18(&mPart, mCarry, frames, 0);
	fn_800F1068(this, frames);
}

void XObjSpider::vfAC()
{
	vfB0(0);
	bool turned = false;
	if (mWall == 3) {
		turned = true;
	}
	fn_800EAB58(&mPart, mCarry, turned);
}

void XObjSpider::vfB8()
{
}

// NONMATCHING: the float registers of the ring loop are numbered the other way
// the shadow: a disc and a ring around it that fades out
void XObjSpider::vfBC(Vec* pos, f32 scale)
{
	if (!(mFlags & 8)) {
		return;
	}
	switch (Get25C()) {
	case 3:
	case 4:
	case 5:
	case 7:
	case 8:
	case 9:
		return;
	}
	Vec p    = *pos;
	Vec rot  = { -HALF_PI, 0.0f, 0.0f };
	Vec size = mMoveScale * (f32)(mInfo != NULL ? *(u32*)(mInfo->_00 + 0xC) : 0);
	size.x   = mRadius;
	size.y   = mRadius;
	size.z   = mRadius;
	fn_8019B56C(GAME_290, 1, 4, 5, 0xF);
	u32 alpha = 128.0f * scale;
	fn_80198258(lbl_8025C400, 0xC, 160.0f * scale, alpha, p.x, p.y, p.z, size.x, size.z, rot.x, rot.y, rot.z);
	UnkShadowVtx verts[26];
	for (int i = 0; i < 13; i++) {
		f32 a                   = 0.5235988f * i;
		verts[i * 2].mPos.x     = p.x - size.x * 0.5f * (f32)sin(a);
		verts[i * 2].mPos.z     = p.z + size.z * 0.5f * (f32)cos(a);
		verts[i * 2].mPos.y     = p.y;
		verts[i * 2].mColor     = alpha;
		verts[i * 2 + 1].mPos.x = p.x - (1.5f + size.x * 0.5f) * (f32)sin(a);
		verts[i * 2 + 1].mPos.z = p.z + (1.5f + size.z * 0.5f) * (f32)cos(a);
		verts[i * 2 + 1].mPos.y = p.y;
		verts[i * 2 + 1].mColor = 0;
	}
	fn_801988EC(lbl_8025C400, 0x98, GAME_44 + 0xE8, verts, 0x1A);
}

void XObjSpider::vfC8()
{
}

void XObjSpider::vfCC()
{
}

// NONMATCHING: the joint path and the two points are laid out differently on the stack
// the thread it hangs from, a line from its joint up to the ceiling
void XObjSpider::vfC0()
{
	if (mStatus.mFlags & 0x80) {
		Vec top;
		CJObj joint;
		u32 color = (u32)(90.0f * mThreadAlpha) | 0xFFFFFF00;
		joint.Attach(GetJoint(mJObj, sThreadPath, 1));
		top = GetJObjPos(joint.mJObj);
		CVec pos(top);
		UnkSpiderGame* game = GAME;
		HSD_StateInvalidate(-1);
		UnkSpiderGame* game2 = GAME;
		if (game2->_290 != NULL) {
			UnkSpiderRenderA* a = game->_290;
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
		UnkSpiderRenderB* b = game->_294;
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
		UnkSpiderRenderC* c = game->_298;
		if (c != NULL) {
			if (c->_30 == 0) {
				c->_30 = 1;
			}
		}
		UnkSpiderGame* game3 = GAME;
		if (game2->_290 != NULL) {
			fn_8019B278(game3->_290, 1);
		}
		if (game3->_294 != NULL) {
			fn_8019AED0(game3->_294, 1);
		}
		if (game3->_298 != NULL) {
			fn_8019AE94(game3->_298, 1);
		}
		fn_8019AFA0(GAME->_294, 4);
		fn_8019B34C(game2->_290, 0x12, 5);
		fn_801992BC(lbl_8025C400, 0, color, color, pos.x, pos.y, pos.z, top.x, 300.0f, top.z);
		fn_8019B34C(game2->_290, 0xC, 5);
	}
}

// NONMATCHING: argv[1] is read into another register
// the event commands: 100 ties another object to one of its joints, or lets go of all
// of them, anything else is kept for the states to read
extern "C" s32 fn_800F2454(XObjSpider* self, s32 argc, s32* argv)
{
	for (int i = 0; i < argc; i++) { }
	switch (argv[0]) {
	case 100:
		if (argv[1] == 0) {
			self->mStatus.mFlags &= ~0x20000000;
			for (u32 i = 0; i < 8; i++) {
				if (self->mTies[i].mObj != NULL) {
					HSD_JObjClearUserMtx(self->mTies[i].mObj->mJObj);
					self->mTies[i].mObj   = NULL;
					self->mTies[i].mJoint = NULL;
				}
			}
		} else {
			u32 i;
			for (i = 0; self->mTies[i].mObj != NULL && i < 8; i++) { }
			if (i < 8) {
				OSReport("id = %d, argv[1]=%d\n", i);
				self->mStatus.mFlags |= 0x20000000;
				if (argv[1] >= 10000) {
					self->mTies[i].mObj = FindEvtObj(argv[1]);
				} else {
					self->mTies[i].mObj = GetObj(argv[1]);
				}
				if (self->mTies[i].mObj == NULL) {
					OSReport("NULL==============\n");
				}
				s32 path[8];
				s32 depth = argv[2];
				for (int j = 0; j < depth; j++) {
					path[j] = argv[3 + j];
				}
				self->mTies[i].mJoint = GetJointByPath(self->mJObj, path, depth);
				Mtx m;
				XObj* obj = self->mTies[i].mObj;
				PSMTXTrans(m, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXConcat(HSD_JObjGetMtxPtr(self->mTies[i].mJoint), m, self->mTies[i].mMtx);
				HSD_JObjCopyMtx(self->mTies[i].mObj->mJObj, self->mTies[i].mMtx);
			}
		}
		break;
	default:
		if (argc < 1) {
			return 0;
		}
		self->mArgs[0] = argv[0];
		if (argc < 2) {
			self->mArgs[1] = 0;
		} else {
			self->mArgs[1] = argv[1];
		}
		if (argc < 3) {
			self->mArgs[2] = 0;
		} else {
			self->mArgs[2] = argv[2];
		}
		if (argc < 4) {
			self->mArgs[3] = 0;
		} else {
			self->mArgs[3] = argv[3];
		}
		if (argc < 5) {
			self->mArgs[4] = 0;
		} else {
			self->mArgs[4] = argv[4];
		}
		if (argc < 6) {
			self->mArgs[5] = 0;
		} else {
			self->mArgs[5] = argv[5];
		}
		break;
	}
	return 0;
}

void XObjSpider::vf138(const Vec* pos, f32 f)
{
	if (mStatus.mFlags & 2) {
		return;
	}
	if (mStatus.mFlags & 0x10) {
		return;
	}
	if (mStatus.mFlags & 0x8000) {
		return;
	}
	if (fn_8009C1BC((u8*)GAME->mChr + 0x12DFC, 2) < 2) {
		return;
	}
	fn_8008A6C0(this, pos, f);
	mStatus.mFlags |= 2;
	Vec v    = { 0.0f, 0.0f, 0.0f };
	v        = *pos;
	mHitPos  = v;
	mHitDist = f;
}

s32 XObjSpider::vf100(s32 a, s32 b)
{
	mStatus.mFlags |= 1;
	return b;
}

// NONMATCHING: the return temp and the target sit the other way on the stack
// shoots the part from its joint, ahead of it or at the robot
extern "C" void fn_800F0B7C(XObjSpider* self)
{
	CJObj joint;
	s32 path[8] = { 0, 0, 0, 0, 1, 1, 0, 0 };
	joint.Attach(GetJoint(self->mJObj, path, 8));
	CVec pos(GetJObjPos(joint.mJObj));
	switch (self->Get25C()) {
	case 0:
		self->mShotTo.x = pos.x + 72.0f * (f32)sin(self->mDir);
		self->mShotTo.y = pos.y;
		self->mShotTo.z = pos.z + 72.0f * (f32)cos(self->mDir);
		break;
	case 12:
	case 13:
		self->mShotTo.x = pos.x + 72.0f * (f32)sin(HALF_PI + self->mDir);
		self->mShotTo.y = pos.y + 72.0f * (f32)cos(HALF_PI + self->mDir);
		self->mShotTo.z = pos.z;
		break;
	default:
		Vec chr       = GAME->mChr->mMoveTranslate;
		self->mShotTo = chr;
		break;
	}
	self->_A44 = 1;
	self->_A48 = 10;
	self->mPart.mStatus.mFlags |= 1;
	self->mPart.mPos.x = pos.x;
	self->mPart.mPos.y = pos.y;
	self->mPart.mPos.z = pos.z;
	XObj* obj          = (XObj*)(GAME_POOL2->mData + self->_258 * GAME_POOL2->mSize);
	CVec to(self->mShotTo);
	self->mPart.mRotY  = atan2(to.y - self->mPart.mPos.y, to.x - self->mPart.mPos.x);
	self->mPart.mObj   = obj;
	self->mPart.mState = 1;
	fn_8001E904(GAME_SOUND, 0x12A, 0xFF);
}

// NONMATCHING: the matrix pointers and the tie loop take other registers
// reads the object it holds up and keeps it on its joint, and moves the tied objects
// along with their joints
extern "C" void fn_800F1068(XObjSpider* self, u32 frames)
{
	switch (self->_B60) {
	case 0:
		break;
	case 1:
		if (!fn_8012CA54(GAME->_97AC)) {
			break;
		}
		if (self->_B5C != NULL) {
			self->_B5C->Release();
		}
		if (self->_B58 >= 0) {
			self->_B5C = ReadObj(self->_B58);
		} else {
			self->_B5C = NULL;
		}
		self->_B60++;
	case 2:
		if (self->_B5C != NULL) {
			if (self->_B5C->mFlags & 0x0040000000000000ULL) {
				break;
			}
			GAME->mFlags |= 0x1000;
		}
		self->_B60++;
	case 3:
		if (self->_B58 < 0 || self->_B5C == NULL) {
			break;
		}
		self->_B5C->SetScale(1.0f, 1.0f, 1.0f, 0);
		CJObj joint;
		joint.Attach(GetJoint(self->mJObj, sHoldPath, 1));
		PSMTXCopy(HSD_JObjGetMtxPtr(joint.mJObj), self->_B64);
		Vec scale;
		HSD_MtxGetScale(self->_B64, &scale);
		scale.x = fn_800621E0(self->_B5C).x / scale.x;
		scale.y = fn_800621E0(self->_B5C).y / scale.y;
		scale.z = fn_800621E0(self->_B5C).z / scale.z;
		Mtx s;
		PSMTXScale(s, scale.x, scale.y, scale.z);
		Mtx t;
		Mtx m;
		PSMTXConcat(self->_B64, s, m);
		PSMTXTrans(t, 0.0f, 5.0f, 0.0f);
		PSMTXConcat(m, t, m);
		HSD_JObjCopyMtx(self->_B5C->mJObj, m);
		self->_B5C->mFlags &= ~0x0200000000000000ULL;
		break;
	}
	if (self->mStatus.mFlags & 0x20000000) {
		for (u32 i = 0; i < 8; i++) {
			if (self->mTies[i].mJoint != NULL && self->mTies[i].mObj != NULL) {
				Mtx m;
				XObj* obj = self->mTies[i].mObj;
				PSMTXTrans(m, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXConcat(HSD_JObjGetMtxPtr(self->mTies[i].mJoint), m, self->mTies[i].mMtx);
				HSD_JObjCopyMtx(self->mTies[i].mObj->mJObj, self->mTies[i].mMtx);
			}
		}
	}
}

// the hits: it loses life when the robot hits it, more for a hard hit, then blinks red
// for a while. and when the robot touches it, the robot gets hit
extern "C" void fn_800F1B10(XObjSpider* self, u32 frames)
{
	switch (self->_B94) {
	case 0:
		if ((self->mStatus.mFlags & 2) && !(self->mStatus.mFlags & 0x10)) {
			fn_8001E660(GAME_SOUND, 0x12A);
			self->mStatus.mFlags &= ~2;
			if (self->mHitDist >= fn_8006EBC8(0x1F) / 100.0f) {
				self->mLife -= 5;
				if (self->mLife <= 0) {
					self->mLife = 0;
				}
			} else if (--self->mLife <= 0) {
				self->mLife = 0;
			}
			if (self->mLife <= 0) {
				self->mSpeed = 0.0f;
				self->SetState(3);
				self->mStatus.mFlags |= 0x10;
			} else {
				self->_B94 = 1;
				self->_B98 = 0;
				self->mStatus.mFlags |= 0x10;
			}
		}
		break;
	case 1:
		if (self->mTimer % 30 < 15) {
			self->SetColor(1, 0x80, 0, 0, 0.0f);
		} else {
			self->SetColor(1, 0, 0, 0, 0.0f);
		}
		self->_B98++;
		if (self->_B98 > 20) {
			self->mStatus.mFlags &= ~0x10;
			self->SetColor(1, 0, 0, 0, 0.0f);
			self->_B98 = 0;
			self->_B94 = 0;
		}
		break;
	}
	Vec chr = GAME->mChr->mMoveTranslate;
	if (self->mWall == 3) {
		f32 dx = chr.x - self->_A14.x;
		f32 dy = 8.0f + chr.y - self->_A14.y;
		f32 dz = chr.z - chr.z;
		if (sqrtf(dx * dx + dy * dy + dz * dz) < 8.0f && (self->mStatus.mFlags & 0x800)) {
			GAME->mChr->vf138(&chr, 7.0f);
			HitCarried(self);
		}
	} else {
		f32 dx = chr.x - self->_A14.x;
		f32 dy = 0.0f;
		f32 dz = chr.z - self->_A14.z;
		f32 y  = chr.y;
		if (sqrtf(dx * dx + dy * dy + dz * dz) < 2.0f && (self->mStatus.mFlags & 0x800) && Abs(y - self->mMoveTranslate.y) < 4.0f) {
			GAME->mChr->vf138(&chr, 7.0f);
			HitCarried(self);
		}
	}
	fn_800F1730(self, frames);
	fn_800F0090(self, frames);
}

// does what the last event command said
extern "C" void fn_800F1730(XObjSpider* self, u32 frames)
{
	switch (self->mArgs[0]) {
	case 0:
		break;
	case 1:
		self->SetState(3);
		self->mArgs[0] = 0;
		break;
	case 2:
		if (self->mStatus.mFlags & 0x10000) {
			GAME_5D08C = 1;
		} else {
			GAME_5D08C = 0;
		}
		self->mArgs[0] = 0;
		break;
	case 3:
		if (self->mStatus.mFlags & 0x80000) {
			GAME_5D08C = 1;
		} else {
			GAME_5D08C = 0;
		}
		self->mArgs[0] = 0;
		break;
	case 4:
		self->mStatus.mFlags |= 0x200000;
		self->mArgs[0] = 0;
		break;
	case 5:
		self->mStatus.mFlags |= 0x400000;
		self->mArgs[0] = 0;
		break;
	case 6:
		self->mFlags |= 0x100000;
		self->mArgs[0] = 0;
		break;
	case 7:
		if (self->mArgs[1] == 1) {
			self->mStatus.mFlags &= ~0x8000000;
		} else {
			self->mStatus.mFlags |= 0x8000000;
		}
		self->mArgs[0] = 0;
		self->mArgs[1] = 0;
		break;
	case 8:
		fn_800F38F4(self);
		self->mArgs[0] = 0;
		break;
	case 9:
		self->mHome.x          = self->mArgs[1] / 100.0f;
		self->mHome.y          = self->mArgs[2] / 100.0f;
		self->mHome.z          = self->mArgs[3] / 100.0f;
		self->_A60.x           = self->mHome.x;
		self->_A60.y           = self->mHome.y;
		self->_A60.z           = self->mHome.z;
		self->mMoveTranslate.x = self->mHome.x;
		self->mMoveTranslate.y = (f32) * (s32*)&self->_25C[4] + self->mHome.y; // _260, the height it hangs at
		self->mMoveTranslate.z = self->mHome.z;
		self->mMoveTranslate.mTarget.Set(self->mMoveTranslate.x, self->mMoveTranslate.y, self->mMoveTranslate.z);
		self->mArgs[0] = 0;
		break;
	case 10:
		self->mArea.x  = self->mArgs[1] / 100.0f;
		self->mArea.y  = self->mArgs[2] / 100.0f;
		self->mArea.z  = self->mArgs[3] / 100.0f;
		self->mAreaW   = self->mArgs[4];
		self->mAreaD   = self->mArgs[5];
		self->mArgs[0] = 0;
		break;
	case 11:
		self->mArgs[0] = 0;
		break;
	case 12:
		self->mArgs[0] = 0;
		break;
	case 13:
		self->mStatus.mFlags |= 0x10000000;
		self->mArgs[0] = 0;
		break;
	case 14:
		self->_B58     = self->mArgs[1];
		self->_B60     = 1;
		self->mArgs[0] = 0;
		break;
	case 15:
		self->_B60 = 0;
		self->_B58 = -1;
		if (self->_B5C != NULL) {
			self->_B5C->Release();
		}
		self->mArgs[0] = 0;
		break;
	case 16:
		self->mArgs[0] = 0;
		break;
	case 17:
		self->mArgs[0] = 0;
		break;
	case 18:
		self->mArgs[0] = 0;
		break;
	case 19:
		self->SetState(5);
		self->mArgs[0] = 0;
		break;
	}
}

void XObjSpider::State0(u32 frames)
{
	if (Get25C() != 0xB) {
		return;
	}
	if (!(mStatus.mFlags & 0x400000)) {
		return;
	}
	SetState(4);
}

// hit: stops, then plays anim 0x15 and goes on, or goes to state 3 when it has no life left
void XObjSpider::State1(u32 frames)
{
	switch (mStep) {
	case 0:
		fn_8001E904(GAME_SOUND, 0x129, 0xFF);
		mStatus.mFlags &= ~0x800;
		mStatus.mFlags |= 0x10;
		mSpeed = 0.0f;
		if (mLife <= 0) {
			mStatus.mFlags |= 0x10000;
			SetState(3);
		} else {
			SetAnim(0x15, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mStep++;
		}
		break;
	case 1:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			mStatus.mFlags &= ~0x10;
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			SetState(4);
		}
		break;
	}
}

void XObjSpider::State2(u32 frames)
{
}

// dies: blinks, stops its sound, drops what it carries, shakes the screen, then
// leaves one of the drops where it was
void XObjSpider::State3(u32 frames)
{
	mTimer++;
	if (mTimer % 30 < 15) {
		SetColor(1, 0, 0x80, 0, 0.0f);
	} else {
		SetColor(1, 0, 0, 0, 0.0f);
	}
	Vec pos;
	switch (mStep) {
	case 0:
		fn_8001E660(GAME_SOUND, 0x132);
		SetAnim(0x19, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		if (mCarry != 0xFF) {
			if (mPart2.mObj != NULL) {
				mPart2.mObj->mFlags &= ~0x10000ULL;
			}
			lbl_8037E600[mCarry] = -1;
			mPart2.mObj          = NULL;
			mCarry               = 0xFF;
		}
		mWait = 0x2C;
		mStep++;
		break;
	case 1:
		mWait -= frames;
		if (mWait < 0) {
			fn_8010CDDC(mMoveTranslate.x, mMoveTranslate.y, mMoveTranslate.z);
			if (!(GAME->mFlags & 0x100)) {
				fn_800242A8(GAME_QUAKE, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.4f);
			}
			mStep++;
		}
		break;
	case 2:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			mStep++;
			mTimer = 0;
			mFlags &= ~0x10000ULL;
			mStatus.mFlags |= 0x80000;
			mStatus.mFlags &= ~0x100000;
			pos.x = mMoveTranslate.x;
			pos.y = mMoveTranslate.y;
			pos.z = mMoveTranslate.z;
			s32 anim;
			if (rand() % 2 > 0) {
				anim = 2;
			} else if (rand() % 4 > 1) {
				anim = 1;
			} else {
				anim = 0;
			}
			Vec to;
			to.x = mHome.x;
			to.y = 0.0f;
			to.z = mHome.z;
			fn_800EA554(&pos, &to, anim);
		}
		break;
	case 3:
		break;
	}
}

// lands: drops into its anim, then takes the first free one of the carried objects
void XObjSpider::State5(u32 frames)
{
	switch (mStep) {
	case 0:
		mStatus.mFlags |= 0x40;
		mStatus.mFlags |= 0x400;
		mStatus.mFlags |= 0x8000;
		SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		mStep++;
		break;
	case 1:
		if (GAME_5D080 == NULL) {
			mStatus.mFlags &= ~0x40;
			mStatus.mFlags &= ~0x80;
			mStatus.mFlags &= ~0x100;
			mStatus.mFlags &= ~0x8000;
			TakeCarried(this);
			SetState(4);
		}
		break;
	case 2:
		break;
	}
}

// NONMATCHING: the spider count loop isn't strength reduced, the robot position temp sits elsewhere
// hangs from the ceiling and swings, drops down its thread when the robot comes near,
// climbs back, then takes one of the carried objects and walks off
void XObjSpider::State6(u32 frames)
{
	CVec chr(GetChrPos());
	CPad pad(0);
	switch (mStep) {
	case 0:
		mTimer = rand() % 30;
		mStep  = 10;
		break;
	case 10:
		if (--mTimer < 0) {
			mTimer = rand() % 30;
			SetAnim(0x11, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mStep = 1;
		}
		break;
	case 1:
		mTimer++;
		mMoveTranslate.x = mHome.x + (f32)(8.0f * (f32)sin(DEG2RAD * (f32)((mTimer * 4) % 360))) - 4.0f;
		if (Get25C() == 11) {
			if (mStatus.mFlags & 0x200000) {
				fn_8001E904(GAME_SOUND, 0x131, 0xFF);
				mStatus.mFlags |= 0x100000;
				mStep++;
				SetAnim(0x10, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mSpeed = -1.6f;
			}
		} else {
			f32 dist;
			if (mWall == 3) {
				f32 dx = chr.x - mMoveTranslate.x;
				f32 dy = chr.y - mMoveTranslate.y;
				f32 dz = 0.0f;
				dist   = sqrtf(dx * dx + dy * dy + dz * dz);
			} else {
				f32 dx = chr.x - mMoveTranslate.x;
				f32 dy = 0.0f;
				f32 dz = chr.z - mMoveTranslate.z;
				dist   = sqrtf(dx * dx + dy * dy + dz * dz);
			}
			if (dist <= 68.0f && GAME->CanAct() && GAME_5D080 == NULL) {
				s32 n = 0;
				for (int i = 0; i < 16; i++) {
					XObjSpider* other = (XObjSpider*)GAME_POOL2->Get(i);
					if ((other->mFlags & 1) && (other->mStatus.mFlags & 0x100000)) {
						n++;
					}
				}
				if (n < 10) {
					fn_8001E904(GAME_SOUND, 0x131, 0xFF);
					mStatus.mFlags |= 0x100000;
					mStep++;
					SetAnim(0x10, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					mSpeed = -1.6f;
				}
			}
		}
		break;
	case 2:
		if (mMoveTranslate.y < 20.0f + mHome.y) {
			fn_8001E660(GAME_SOUND, 0x131);
			mStep++;
			SetAnim(0x12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mDir   = atan2(_9FC.x - mMoveTranslate.x, _9FC.z - mMoveTranslate.z);
			_988   = mDir;
			mSpeed = 0.0f;
		}
		break;
	case 3:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			mStep++;
			SetAnim(0x13, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mDir   = atan2(_9FC.x - mMoveTranslate.x, _9FC.z - mMoveTranslate.z);
			_988   = mDir;
			mSpeed = -0.5f;
		}
		break;
	case 4:
		if (mMoveTranslate.y < 8.0f + mHome.y) {
			mStep++;
			SetAnim(0x14, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_8001E904(GAME_SOUND, 0x128, 0xFF);
		}
		break;
	case 5:
		if (mMoveTranslate.y < mHome.y) {
			mMoveTranslate.y = mHome.y;
			mSpeed           = 0.0f;
		}
		f32 end2 = GetAnimEndFrame(mJObj);
		if (end2 - frames <= GetAnimFrame(mJObj)) {
			mStatus.mFlags &= ~0x40;
			mStatus.mFlags &= ~0x80;
			mStatus.mFlags &= ~0x100;
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mStatus.mFlags &= ~0x8000;
			TakeCarried(this);
			SetState(4);
		}
		break;
	case 6:
		if (pad.mPad->mTrigger & 0x200) {
			mStatus.mFlags = 0;
			mStatus.mFlags |= 0x40;
			mStatus.mFlags |= 0x80;
			mStatus.mFlags |= 0x100;
			mMoveTranslate.y = 48.0f + mHome.y;
			mStep            = 0;
		}
		break;
	}
}

// backs off to its home spot, its steps sounding, then waits out the event
void XObjSpider::State7(u32 frames)
{
	switch (mStep) {
	case 0:
		SetAnim(0xC, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		mSpeed = 0.0f;
		mStep++;
		break;
	case 1:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			SetAnim(8, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			_9FC.x = mHome.x;
			_9FC.y = mMoveTranslate.y;
			_9FC.z = mHome.z;
			mDir   = PI + (f32)atan2(_9FC.x - mMoveTranslate.x, _9FC.z - mMoveTranslate.z);
			mSpeed = -0.5f;
			mStep++;
		}
		break;
	case 2:
		if (1.0f <= GetAnimFrame(mJObj) && 1.0f >= GetAnimFrame(mJObj)) {
			PlayStep(this);
		} else if (15.0f <= GetAnimFrame(mJObj) && 15.0f >= GetAnimFrame(mJObj)) {
			PlayStep(this);
		}
		if (DistXZ(&_9FC, &mMoveTranslate) <= 2.0f) {
			mSpeed   = 0.0f;
			f32 end2 = GetAnimEndFrame(mJObj);
			if (end2 - frames <= GetAnimFrame(mJObj)) {
				SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				if (GAME_5D080 != NULL) {
					mStep++;
				} else {
					SetState(4);
				}
			}
		}
		break;
	case 3:
		if (GAME_5D080 == NULL) {
			SetState(4);
		}
		break;
	}
}

void XObjSpider::State8(u32 frames)
{
}

extern "C" u32 fn_800EBDB0(XObjSpider* self)
{
	return (self->mStatus.mFlags >> 19) & 1;
}

// whether another spider is closer than 24
extern "C" BOOL fn_800EBBF0(XObjSpider* self)
{
	for (int i = 0; i < 16; i++) {
		XObj* obj = GAME_POOL2->Get(i);
		if ((obj->mFlags & 1) && (obj->mFlags & 1) && (obj->mFlags & 0x10000)) {
			XObjSpider* other = (XObjSpider*)obj;
			if ((other->mStatus.mFlags & 0x100000) && other->_258 != self->_258) {
				f32 dx = other->mMoveTranslate.x - self->mMoveTranslate.x;
				f32 dy = other->mMoveTranslate.y - self->mMoveTranslate.y;
				f32 dz = other->mMoveTranslate.z - self->mMoveTranslate.z;
				if (sqrtf(dx * dx + dy * dy + dz * dz) < 24.0f) {
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

// the id of another spider closer than 24, -1 when there is none
extern "C" s32 fn_800EBA30(XObjSpider* self)
{
	for (int i = 0; i < 16; i++) {
		XObj* obj = GAME_POOL2->Get(i);
		if ((obj->mFlags & 1) && (obj->mFlags & 1) && (obj->mFlags & 0x10000)) {
			XObjSpider* other = (XObjSpider*)obj;
			if ((other->mStatus.mFlags & 0x100000) && other->_258 != self->_258) {
				f32 dx = other->mMoveTranslate.x - self->mMoveTranslate.x;
				f32 dy = other->mMoveTranslate.y - self->mMoveTranslate.y;
				f32 dz = other->mMoveTranslate.z - self->mMoveTranslate.z;
				if (sqrtf(dx * dx + dy * dy + dz * dz) < 24.0f) {
					return other->_258;
				}
			}
		}
	}
	return -1;
}

extern "C" void fn_800EB928(XObjSpider* self)
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

// NONMATCHING: retail has another 0x24 bytes of .rodata in front of the joint paths
// the part's anims: it opens, follows its joint on the carried object while the anim
// plays, then closes and hides
extern "C" void fn_800EAF18(UnkSpiderPart* part, u8 carry, u32 frames, s32 mode)
{
	if (carry == 0xFF) {
		part->mState = 0;
		part->mStatus.mFlags &= ~1;
		return;
	}
	if (lbl_8037E5C0[carry] == NULL) {
		part->mState = 0;
		part->mStatus.mFlags &= ~1;
		return;
	}
	switch (part->mState) {
	case 0:
		break;
	case 1:
		lbl_8037E5C0[carry]->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		part->mState++;
		break;
	case 2:
		if (part->mObj != NULL) {
			CJObj joint;
			s32 path[8] = { 0, 0, 0, 0, 1, 1, 0, 0 };
			joint.Attach(GetJoint(part->mObj->mJObj, path, 8));
			part->mPos = GetJObjPos(joint.mJObj);
		}
		f32 end = GetAnimEndFrame(lbl_8037E5C0[carry]->mJObj);
		if (end - frames <= GetAnimFrame(lbl_8037E5C0[carry]->mJObj)) {
			lbl_8037E5C0[carry]->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			part->mState++;
		}
		break;
	case 3:
		if (part->mObj != NULL) {
			CJObj joint;
			s32 path[8] = { 0, 0, 0, 0, 1, 1, 0, 0 };
			joint.Attach(GetJoint(part->mObj->mJObj, path, 8));
			part->mPos = GetJObjPos(joint.mJObj);
		}
		f32 end2 = GetAnimEndFrame(lbl_8037E5C0[carry]->mJObj);
		if (end2 - frames <= GetAnimFrame(lbl_8037E5C0[carry]->mJObj)) {
			lbl_8037E5C0[carry]->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			part->mState = 0;
			part->mStatus.mFlags &= ~1;
			part->mStatus.mFlags &= ~2;
		}
		break;
	}
}

// a test mode: the pad's direction sends it a step that way, up its thread and back
void XObjSpider::State9(u32 frames)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x20) {
		mStep            = 0;
		mSpeed           = 0.0f;
		mMoveTranslate.x = mHome.x;
		mMoveTranslate.y = mHome.y;
		mMoveTranslate.z = mHome.z;
		SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		mStatus.mFlags &= ~0x100;
	}
	switch (mStep) {
	case 0:
		if (pad.mPad->mTrigger & 0x200) {
			u32 button = pad.mPad->mButton;
			if (button & 8) {
				_9FC.x = mHome.x;
				_9FC.y = mHome.y;
				_9FC.z = mHome.z - 60.0f;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mStep++;
				mSpeed = 0.3f;
				mDir   = PI;
			} else if (button & 4) {
				_9FC.x = mHome.x;
				_9FC.y = mHome.y;
				_9FC.z = 60.0f + mHome.z;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mStep++;
				mSpeed = 0.3f;
				mDir   = PI;
			} else if (button & 1) {
				_9FC.x = mHome.x - 60.0f;
				_9FC.y = mHome.y;
				_9FC.z = mHome.z;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mStep++;
				mSpeed = 0.3f;
				mDir   = PI;
			} else if (button & 2) {
				_9FC.x = 60.0f + mHome.x;
				_9FC.y = mHome.y;
				_9FC.z = mHome.z;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mStep++;
				mSpeed = 0.3f;
				mDir   = PI;
			}
		}
		break;
	case 1:
		if (HeadTo(this, _9FC) < 20.0f) {
			mStep++;
			SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		break;
	case 2:
		if (HeadTo(this, _9FC) < 1.0f) {
			mStatus.mFlags |= 0x100;
			mSpeed = 0.0f;
		}
		HSD_AObj* aobj = GetAObj(mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(mJObj);
		if (end - frames <= (aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f)) {
			mStatus.mFlags |= 0x100;
			mSpeed = 0.0f;
			mStep++;
			SetAnim(5, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mSpeed = 0.3f;
		}
		break;
	case 3:
		if (mMoveTranslate.y >= 80.0f) {
			mStep++;
			mSpeed = -0.3f;
		}
		break;
	case 4:
		if (mMoveTranslate.y <= 20.0f + mHome.y) {
			mStep++;
			mMoveTranslate.y = 0.0f;
			mStatus.mFlags &= ~0x100;
			_9FC.x = mHome.x;
			_9FC.y = mHome.y;
			_9FC.z = mHome.z;
			SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mSpeed = 0.3f;
		}
		break;
	case 5: {
		f32 distance = HeadTo(this, _9FC);
		s32 done     = 0;
		if (distance < 1.0f) {
			mSpeed = 0.0f;
			done   = 1;
		}
		f32 end2 = GetAnimEndFrame(mJObj);
		if (end2 - frames <= GetAnimFrame(mJObj)) {
			done++;
		}
		if (done >= 2) {
			mStep = 0;
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		break;
	}
	}
}

// draws the carried object where the part is
extern "C" void fn_800EAB58(UnkSpiderPart* part, u8 carry, bool turned)
{
	if (carry == 0xFF) {
		return;
	}
	if (!(part->mStatus.mFlags & 1)) {
		return;
	}
	XObj* obj = lbl_8037E5C0[carry];
	if (obj == NULL) {
		return;
	}
	HSD_JObj* jobj = obj->mJObj;
	HSD_JObjSetTranslateX(jobj, part->mPos.x);
	HSD_JObjSetTranslateY(jobj, part->mPos.y);
	HSD_JObjSetTranslateZ(jobj, part->mPos.z);
	if (turned) {
		HSD_JObjSetRotationX(jobj, HALF_PI);
		HSD_JObjSetRotationZ(jobj, HALF_PI + part->mRotY);
	} else {
		HSD_JObjSetRotationY(jobj, part->mRotY);
	}
	HSD_JObjDispAll(jobj, NULL, 7, 0);
}

extern "C" void fn_800EA99C(void* mgr)
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

// NONMATCHING: the drop pointer is set up through r0, the height sum is commuted
// moves the drops: shown, bobbing, then falling to where they land
extern "C" void fn_800EA780(void* mgr, u32 frames)
{
	Vec pos;
	f32 fall         = 0.8f * (f32)frames;
	UnkSpiderDrop* d = lbl_8037E640;
	for (int i = 0; i < 11; i++, d++) {
		XObj* obj = d->mObj;
		if (obj != NULL) {
			s32 state = d->mState;
			s32 timer = d->mTimer;
			f32 toY   = d->mTo.y;
			pos       = d->mPos;
			switch (state) {
			case 3:
				break;
			case 0:
				obj->mFlags |= 0x10000;
				state++;
				break;
			case 1:
				timer++;
				f32 bob = 10.0f * (f32)sin(DEG2RAD * (f32)((timer * 16) % 360)) + 0.2f;
				if (bob < 0.2f) {
					obj->SetTranslateY(pos.y + 0.2f, 0);
					state++;
				} else {
					obj->SetTranslateY(pos.y + bob, 0);
				}
				break;
			case 2:
				pos.y -= fall;
				if (pos.y <= toY) {
					obj->mFlags &= ~0x100000ULL;
					pos.y = toY;
					state++;
				}
				obj->SetTranslateY(pos.y + 0.2f, 0);
				break;
			}
			d->mState = state;
			d->mTimer = timer;
			d->mPos   = pos;
		}
	}
}

// NONMATCHING: the object loop multiplies instead of stepping the offset
// puts one of the free drops at pos and plays its anim
extern "C" void fn_800EA554(Vec* pos, Vec* to, s32 anim)
{
	if (lbl_8065B2C4 >= 11) {
		return;
	}
	UnkObjPool* pool = GAME_ITEMS;
	for (u32 i = 0; i < 96; i++) {
		XObj* obj = pool->Get(i);
		if ((obj->mFlags & 1) && obj->GetDescKind() == 0x104 && (obj->mFlags & 1) && ((u8*)obj)[0x9E8] == 0) {
			((u8*)obj)[0x9E8]                 = 1;
			lbl_8037E640[lbl_8065B2C4].mState = 0;
			lbl_8037E640[lbl_8065B2C4].mTimer += 2;
			lbl_8037E640[lbl_8065B2C4].mPos.x = pos->x;
			lbl_8037E640[lbl_8065B2C4].mPos.y = pos->y + 0.2f;
			lbl_8037E640[lbl_8065B2C4].mPos.z = pos->z;
			lbl_8037E640[lbl_8065B2C4].mTo.x  = to->x;
			lbl_8037E640[lbl_8065B2C4].mTo.y  = to->y;
			lbl_8037E640[lbl_8065B2C4].mTo.z  = to->z;
			lbl_8037E640[lbl_8065B2C4].mObj   = obj;
			lbl_8037E640[lbl_8065B2C4].mObj->SetTranslateX(pos->x, 0);
			lbl_8037E640[lbl_8065B2C4].mObj->SetTranslateY(pos->y + 0.2f, 0);
			lbl_8037E640[lbl_8065B2C4].mObj->SetTranslateZ(pos->z, 0);
			lbl_8037E640[lbl_8065B2C4].mObj->mFlags |= 0x100000;
			lbl_8037E640[lbl_8065B2C4].mObj->SetAnim(anim, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			lbl_8065B2C4++;
			return;
		}
	}
}

// NONMATCHING: the jobj and the anim id swap registers
void XObjSpider::vf148()
{
	if ((mFlags & 1) && (mFlags & 0x10000)) {
		s32 anim  = mAnimId;
		f32 frame = GetAnimFrame(mJObj);
		mSe.Play(anim, fn_8001DF00(GAME_SOUND, &mMoveTranslate, 0), frame);
	}
}
