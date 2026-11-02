#include "game/xchr.h"
#include "game/hsd_jobj.h"
#include "game/hsd_dobj.h"
#include "game/hsd_cobj.h"
#include "game/xobjmap.h"
#include "game/xpad.h"
#include "game/xact.h"
#include "game/octamenu.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/cvec.h"
#include "game/cshadow.h"
#include "game/cparticle.h"
#include "game/cmotioncontrolcamera.h"
#include "game/cjobjinlineattach.h"
#include "game/cjobjset.h"
#include "game/coordinfo.h"
#include "game/infoscrn.h"
#include "game/evt.h"
#include "game/video.h"
#include "game/globals.h"
#include "game/ut.h"
#include <math.h>
#include <stdlib.h>
extern "C" {
#include <dolphin/mtx/vec.h>
#include <sysdolphin/aobj.h>
#include <sysdolphin/fobj.h>
#include <sysdolphin/mtx.h>
#include "game/gmath.h"
}

// XObjChr, chibi robo himself, and the menus he opens. most of the small functions at the end
// are out of line copies of the inlines the player code uses

// one point of a spiral trail
struct EffTrailPoint {
	f32 mX;      // _00
	f32 mY;      // _04
	f32 mZ;      // _08
	f32 mAmount; // _0C, fades to 0
	u32 _10;     // _10
	f32 mPhase;  // _14
};
STATIC_ASSERT(sizeof(EffTrailPoint) == 0x18);

// two spirals rising off chibi robo, the newest point first
struct EffTrail {
	BOOL mActive;                // _00
	EffTrailPoint mPoint[2][10]; // _04
};
STATIC_ASSERT(sizeof(EffTrail) == 0x1E4);

struct PointPtclOne {
	s32 mState;   // _00
	u8 _04[0x20]; // _04
};
STATIC_ASSERT(sizeof(PointPtclOne) == 0x24);

// the sparks that fly in when points change, a pool of 10 at lbl_80378300. type 0, 2 and 3
// have their own colors, a negative amount draws them black
struct PointPtcl {
	u8 _00[0x8];             // _00
	s32 mType;               // _08
	s32 mState;              // _0C
	s32 mAmount;             // _10
	u8 _14[0x54];            // _14
	u32 mColor;              // _68
	u32 mColor2;             // _6C
	u8 _70[0x4];             // _70
	s32 _74;                 // _74
	s32 mNum;                // _78
	PointPtclOne mPtcls[40]; // _7C

	u8 IsFree() const { return mState == 0 && _74 == 0; }
};
STATIC_ASSERT(sizeof(PointPtcl) == 0x61C);

// fades in and out over 0.3 seconds
#if defined(VERSION_PAL)
#define OX_FADE_STEP (1.0f / (0.3f * lbl_80660D4C))
#else
#define OX_FADE_STEP (1.0f / 18.0f)
#endif

extern "C" {
void fn_800D5874(XObjOX* object);
void fn_800D5804(XObjOX* object);
}

// the globals label and icon ids are 2 lower in japan
#if defined(VERSION_JP)
#define DRS_LABEL(id) ((id) - 2)
#else
#define DRS_LABEL(id) (id)
#endif

// the globals label and icon ids are 2 lower in japan
#if defined(VERSION_JP)
#define ATC_LABEL(id) ((id) - 2)
#else
#define ATC_LABEL(id) (id)
#endif

extern "C" s32 fn_800991FC(XObjChr* chr, s32 type);

extern "C" const char lbl_8065D4AC[1]; // ""

// todo: cleanup
static inline s32 FindAttachment(const AttachmentValues* values, s32 id)
{
	for (s32 i = 0; i < 9; ++i) {
		if (values->mEntries[i].mId == id)
			return i;
	}
	return -1;
}

extern "C" {
void fn_8009C8E0(AttachmentValues* values);
}

// 0.3 seconds
#if defined(VERSION_PAL)
#define LOOK_FADE_STEP (1.0f / (0.3f * lbl_80660D4C))
#else
#define LOOK_FADE_STEP (1.0f / 18.0f)
#endif

#if defined(VERSION_PAL)
#define IDLE_SECOND lbl_80660D4C
#else
#define IDLE_SECOND 60
#endif

// todo: cleanup
static inline u8 DiskBusy(CGame* game)
{
	return (u8)((game->mDvdStatus || game->mPrevDvdStatus) && lbl_8025C400.mDiskOk == 0);
}

// todo: cleanup
static inline u8 InputEnabled(CGame* game)
{
	return (u8)((game->mStatus.mFlags & 1) && !(game->mStatus.mFlags & 0x22) && !DiskBusy(game));
}

extern "C" {
s32 fn_800E4AAC(XObj* target, const Vec* position);
void fn_800A0014(XObjChr* chr, BOOL start, BOOL cancel, f32 value);
}

// todo: cleanup
static inline f32 CurrentFrame(HSD_JObj* joint)
{
	HSD_AObj* animation = GetAObj(joint);
	return animation != NULL ? HSD_AObjGetCurrentFrame(animation) : 0.0f;
}

// joints that follow an item's own anim
struct XObjChrItemAnim {
	s32 mItem;     // _00, -1 ends the table
	s32 mDepth;    // _04
	s32 mPath[17]; // _08
};
STATIC_ASSERT(sizeof(XObjChrItemAnim) == 0x4C);

extern "C" {
extern XObjChrItemAnim lbl_80226130[];
}

// anims from 3000 up are the suit's, from 2000 up the attachment's, from 1000 up XObj's second archive
static inline s32 SuitAnimGroup(XObjChr* chr, s32 id)
{
	bool ret = false;
	if (id >= 3000 && chr->mSuitAnim.mArchive != NULL) {
		ret = true;
	}
	return ret ? id / 3000 : 0;
}

static inline s32 AtcAnimGroup(XObjChr* chr, s32 id)
{
	bool ret = false;
	if (id >= 2000 && chr->mAttachmentAnim.mArchive != NULL) {
		ret = true;
	}
	return ret ? id / 2000 : 0;
}

static inline s32 AnimGroup(XObjChr* chr, s32 id)
{
	bool extra = id >= 1000;
	if (extra)
		extra = chr->mAnim.mArchive != NULL;
	return extra ? id / 1000 : 0;
}

// a particle an anim frame table asks for
struct PlayerFrameParticle {
	s32 mCode;      // _00, -1 for none
	Vec mTranslate; // _04
	Vec mScale;     // _10
	Vec mRotate;    // _1C
};
STATIC_ASSERT(sizeof(PlayerFrameParticle) == 0x28);

extern "C" {
void PSMTXRotRad(Mtx mtx, char axis, f32 angle);
PlayerFrameParticle fn_800A7E1C(XObj* object, f32* frame, const void* table);
extern const u8 lbl_80226324[];
extern const u8 lbl_802276D4[];
}

struct FrameEvent;

// the frame pair fn_800A7E1C steps, XObjChr _94F8
struct FrameEventTime {
	f32 mFrame;     // _00
	f32 mLastFrame; // _04
};

// a frame window check, 1 starts the event, -1 stops it, 0 leaves it
typedef s32 (*FrameEventFunc)(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event);

// one entry of an anim's frame event table, like lbl_80226324
struct FrameEvent {
	s32 mAnim;          // _00
	f32 mStart;         // _04
	f32 mEnd;           // _08
	FrameEventFunc _0C; // _0C
	FrameEventFunc _10; // _10
	s32 mId;            // _14, a sound or particle
};
STATIC_ASSERT(sizeof(FrameEvent) == 0x18);

// todo: cleanup
static inline u8* AnimationHead(HSD_JObj* joint)
{
	HSD_AObj* animation = joint->aobj;
	HSD_FObj* frame     = animation ? animation->fobj : NULL;
	return frame ? frame->ad_head : NULL;
}

extern "C" {
extern Vec lbl_80378258;
extern Vec lbl_8037824C;
}

static inline Vec FirstEdgePoint(const XObj* object, s32 index, f32 height)
{
	Vec point = lbl_80378258;
	point.x   = object->mMoveTranslate.x + object->mEdges[index].mFrom.x;
	point.y   = height;
	point.z   = object->mMoveTranslate.z + object->mEdges[index].mFrom.z;
	return point;
}

static inline Vec SecondEdgePoint(const XObj* object, s32 index, f32 height)
{
	Vec point = lbl_8037824C;
	point.x   = object->mMoveTranslate.x + object->mEdges[index].mTo.x;
	point.y   = height;
	point.z   = object->mMoveTranslate.z + object->mEdges[index].mTo.z;
	return point;
}

// out of line copies of the small inlines the player code uses, on XObjChr, its actions
// and the objects around it

extern "C" {
u32 CheckStageFlag(s32 stage, u32 mask);
}

// todo: cleanup
static inline bool TestStatus(const XObj* object, u64 mask)
{
	return (u8)((object->mStatus.mFlags & mask) != 0);
}

// the first floor at or under a height, the floors are sorted from the highest down
static inline s32 FindFloorStart(const XObjMap* map, f32 y)
{
	for (int i = 0; i < map->mTrigonCount; i++) {
		if (map->mTrigon[map->mTrigonOrder[i]].mMinY <= y) {
			return i;
		}
	}
	return 0;
}

struct XObjMapHit {
	s32 mTriangle; // _00
	s32 mTrigon;   // _04
	f32 mHeight;   // _08
};

extern "C" s32 lbl_8065B260;

// out of battery, fn_8009FEDC
static inline bool IsStopped(const XObjChr* chr)
{
	bool ret = false;
	if (chr->mParams.mBattery <= 0.0f && fn_800665E0(&lbl_8025C400.mInfoScrn)) {
		ret = true;
	}
	return ret;
}

// out of battery and slumped over
static inline bool IsDown(const XObjChr* chr)
{
	bool ret = false;
	if (IsStopped(chr)) {
		if (chr->_158A4 != 0) {
			ret = true;
		}
	}
	return ret;
}

static inline bool IsEventHeld(const XObjChr* chr)
{
	bool ret = false;
	if ((lbl_8025C400.mStatus.mFlags & 0x40000000) && IsDown(chr)) {
		ret = true;
	}
	return ret;
}

static inline bool IsTumbling(const XObjChr* chr)
{
	bool ret = false;
	if (chr->mTumble.mState == 4 || chr->mTumble.mState == 5 || chr->mTumble.mState == 6) {
		ret = true;
	}
	return ret;
}

// events wait while he lies there out of battery, fn_8009FDE4
static inline bool IsEventBlocked(const XObjChr* chr)
{
	bool ret = false;
	if (IsEventHeld(chr) && IsTumbling(chr)) {
		ret = true;
	}
	return ret;
}

static inline u8 IsMenuUp(XObjChr* chr)
{
	return (u8)(chr->mMenu.mAtc.mOpen > 0 || chr->mMenu.mDrs.mOpen > 0);
}

// he stops where he is, and lets go of what he pushes
static inline void StopMoving(XObjChr* chr)
{
	chr->_988.Set(0.0f, 0.0f, 0.0f);
	chr->_988.mTarget.Set(0.0f, 0.0f, 0.0f);
	chr->_988.mSpeed.Set(0.0f, 0.0f, 0.0f);
	chr->mMoveRotate.Set(chr->mMoveRotate.mTarget);
	if (chr->mPush.mState != 0) {
		fn_800C5A00(&chr->mPush);
		chr->mPush.mState = 0;
		chr->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		chr->_A2C = 3;
	}
}

// the y angle a direction faces
static inline f32 GetYAngle(const Vec& dir)
{
	return atan2(dir.x, dir.z);
}

// OXMenu's marks start at random turns
static inline void SpinMarks(XObjChr& chr)
{
	for (int i = 0; i < 2; i++) {
		chr.mOXMenu.mMark[i].mSpin  = DEG2RAD * (rand() % 360);
		chr.mOXMenu.mMark[i].mTilt  = DEG2RAD * (rand() % 360);
		chr.mOXMenu.mMark[i].mScale = 0.5f;
	}
}

static inline void OpenMenus(XObjChr* chr)
{
	lbl_8025C400.mStatus.mFlags |= 0x400ULL;
	fn_8001E904(&lbl_8025C400.mSound, 0x86, 255);
	if (chr->mMenu.mSel == 0) {
		chr->mMenu.mAtc.Open(0);
		chr->mMenu.mDrs.Open(1);
	} else {
		chr->mMenu.mAtc.Open(1);
		chr->mMenu.mDrs.Open(0);
	}
}

extern "C" {
void fn_80091C44(CXObj* object, u32* flags);
void fn_8008B0D8(CXObj* object, s32 mode);
extern PointPtcl lbl_80378300[10];
extern PointPtcl* lbl_8037C018[10];
}

// todo: cleanup
static inline u8 IsRopeState(XObjChr* chr, s32 state)
{
	return chr->mRope.mState == state;
}

// the command line's picks, the attachment slots are one more than the pick.
// todo: cleanup, cgame.cpp has its own CmdLineArgs
struct CmdLineArgs {
	s32 mStage;    // _00
	s32 mSlot;     // _04, the card slot, 0 for A
	u8 _08_0 : 2;  // _08
	u8 mPick1 : 2; // _08
	u8 mPick2 : 2; // _08
	u8 mPick3 : 2; // _08
	u8 _09[0x17];  // _09
};

extern "C" {
extern CmdLineArgs lbl_8020FBC0;
s32 fn_8006E940(s32 index);
// stage_data.cpp returns a bool, this file tests the whole word
BOOL fn_8006EC34();
void fn_800C17BC(XActPopperCharge* charge);
void fn_800C2D18(XActPopperShot* shot);
void fn_8009BF14(OctaMenu* menu, s32 focus);
void fn_8009C2C8(AttachmentValues* values, s32 id, s32 value);
}

// the item glows brighter and darker while the cursor is on it
static inline void AddPulse(s32& red, s32& green, s32& blue, const OctaMenu* menu, s32 index)
{
	red += (s32)(25.0f * (f32)sin(1.2f * menu->mItems[index]._08) + 25.0f);
	green += (s32)(25.0f * (f32)sin(1.2f * menu->mItems[index]._08) + 25.0f);
	blue += (s32)(25.0f * (f32)sin(1.2f * menu->mItems[index]._08) + 25.0f);
}

// he has the attachment on, or asked for it
static inline BOOL IsAttachment(const XObjChr* chr, s32 id)
{
	return id == chr->GetAttachment();
}

// DrsMenu::vf2C, he wears the item's costume
static inline BOOL IsWorn(DrsMenu* menu, s32 index)
{
	return menu->mItems[index].mType == menu->mChr->mSuit;
}

// he carries the item
static inline BOOL HasItem(XObjChr* chr, s16 id)
{
	return fn_800DCD30(chr->mParams.mInventory.mItems, id) < 0 ? FALSE : TRUE;
}

// stops every popper shot he has flying
static inline void StopShots(XObjChr* chr)
{
	XActPopperShot* shot = chr->mPopper.mShot;
	for (int i = 0; i < 10; i++, shot++) {
		fn_800C2D18(shot);
	}
}

// puts the joint at an angle and keeps it there
static inline void SetTurn(CJointRot* rot, f32 angle)
{
	rot->mMode   = 1;
	rot->mTarget = angle;
	rot->mAngle  = angle;
}

// turns the joint to an angle
static inline void TurnJoint(CJointRot* rot, f32 angle)
{
	rot->mMode   = 1;
	rot->mTarget = angle;
}

// how the head turns for each cursor, from no cursor through the eight items
static f32 sMenuTurnY[9] = { 0.0f, -20.0f, 0.0f, 20.0f, 20.0f, 20.0f, 0.0f, -20.0f, -20.0f };
static f32 sMenuTurnX[9] = { -20.0f, -20.0f, -30.0f, -20.0f, -10.0f, 0.0f, 10.0f, 0.0f, -10.0f };

// one of his tools is out
static inline BOOL IsToolOut(XObjChr* chr, s16 item)
{
	switch (item) {
	case 0x10:
		return fn_800D4F0C(&chr->mLift);
	case 0x16:
		return fn_800D4D54(&chr->mLift);
	case 0x15:
		return fn_800D4E30(&chr->mLift);
	default:
		return fn_800D4C78(&chr->mLift);
	}
}

// he can use the tool, he has it or it is out
static inline bool CanUseTool(XObjChr* chr, s16 item)
{
	u8 has = 1;
	if (fn_800DCD30(chr->mParams.mInventory.mItems, item) < 0 && !IsToolOut(chr, item))
		has = 0;
	return has;
}

static inline void SetAttachmentOn(AttachmentValues* values, s32 id, bool on)
{
	s32 index = FindAttachment(values, id);
	if (index >= 0)
		values->mEntries[index].mValue = on;
}

// the lift's timer ran out
static inline bool IsLiftDone(const XObjChr* chr)
{
	bool ret = false;
	if (chr->mLift.mState == 4 && fn_800D4FE8((XActLift*)&chr->mLift)) {
		ret = true;
	}
	return ret;
}

// he takes a tool out, or puts it away when he came back to it
static inline void TakeOutTool(XObjChr* chr, s16 item)
{
	if (chr->_95DC == 0) {
		chr->mLift.mState = 20;
		chr->mLift._48    = item;
	} else {
		chr->mLift.mState = 24;
		chr->mLift._48    = item;
	}
}

// the anim archive each attachment needs
static const char* sAttachmentAnims[9] = {
	NULL, "cb_robo_6.dat", "cb_robo_7.dat", "cb_robo_8.dat", "cb_robo_9.dat", "cb_robo_10.dat", "cb_robo_11.dat", "cb_robo_12.dat", NULL,
};

/**
 * @note Address: 0x80093B7C
 * @note Size: 0x5C
 */
DrsMenu::~DrsMenu()
{
}

// todo: weak copy
/**
 * @note Address: 0x80093BD8
 * @note Size: 0x30
 */
extern "C" void fn_80093BD8(OctaMenu* menu)
{
	menu->Open(0);
}

// todo: weak copy
/**
 * @note Address: 0x80093C08
 * @note Size: 0xC
 */
extern "C" void fn_80093C08(OctaMenu* menu)
{
	menu->mOpen = 0;
}

/**
 * @note Address: 0x800940A4
 * @note Size: 0x298
 */
extern "C" void fn_800940A4(EffTrail* trail, u32 frames)
{
	if (trail->mActive != 0) {
		f32 total = 0.0f;
		for (int i = 0; i < 2; i++) {
			EffTrailPoint* point = trail->mPoint[i];
			for (int j = 9; j > 0; j--) {
				point[j].mX      = point[j - 1].mX;
				point[j].mY      = point[j - 1].mY;
				point[j].mZ      = point[j - 1].mZ;
				point[j].mAmount = point[j - 1].mAmount;
				point[j].mPhase  = point[j - 1].mPhase;
			}
			if ((point[0].mAmount -= 0.02f * frames) <= 0.0f)
				point[0].mAmount = 0.0f;
			point[0].mPhase += DEG2RAD * 9.0f * frames;
			f32 sine    = sin(point[0].mPhase);
			point[0].mX = point[0].mAmount * (15.0f * ((1.0f - point[0].mAmount) * sine));
			f32 cosine  = cos(point[0].mPhase);
			point[0].mZ = point[0].mAmount * (15.0f * ((1.0f - point[0].mAmount) * cosine));
			point[0].mY += 0.3f * frames;
			for (int j = 0; j < 10; j++)
				total += point[j].mAmount;
		}
		if (total == 0.0f)
			trail->mActive = 0;
	}
}

/**
 * @note Address: 0x80095AD8
 * @note Size: 0x1F8
 */
extern "C" void fn_80095AD8(PointPtcl* ptcl, s32 type, s32 amount)
{
	ptcl->mType   = type;
	ptcl->mState  = 1;
	ptcl->mAmount = amount;
	switch (ptcl->mType) {
	case 0:
		ptcl->mColor  = amount >= 0 ? 0xCECEFF00 : 0xFF000000;
		ptcl->mColor2 = amount >= 0 ? 0xE037EE00 : 0xFF000000;
		ptcl->mNum    = __abs(amount);
		if (ptcl->mNum > 40)
			ptcl->mNum = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = -1;
		break;
	case 2:
		ptcl->mColor  = amount >= 0 ? 0xEFEBC500 : 0xFF000000;
		ptcl->mColor2 = amount >= 0 ? 0xEDA83600 : 0xFF000000;
		ptcl->mNum    = __abs(amount);
		if (ptcl->mNum > 40)
			ptcl->mNum = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = -2;
		break;
	case 3:
		ptcl->mColor  = amount >= 0 ? 0xCECEFF00 : 0xFF000000;
		ptcl->mColor2 = amount >= 0 ? 0x5959FF00 : 0xFF000000;
		ptcl->mNum    = __abs(amount);
		if (ptcl->mNum > 40)
			ptcl->mNum = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = -3;
		break;
	default:
		ptcl->mColor  = 0xFFFFFF00;
		ptcl->mColor2 = 0xFFFFFF00;
		ptcl->mNum    = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = 5;
		break;
	}
}

// OXMenu's update
/**
 * @note Address: 0x80096B90
 * @note Size: 0x2A8
 */
extern "C" void fn_80096B90(OXMenu* menu, u32 frames)
{
	if (menu->mStarted > 0) {
		if (menu->mFade < 1.0f) {
			f32 fade    = menu->mFade + OX_FADE_STEP * frames;
			menu->mFade = fade;
			if (fade >= 1.0f)
				menu->mFade = 1.0f;
		}
	} else if (menu->mStarted < 0) {
		if (menu->mFade > 0.0f) {
			f32 fade    = menu->mFade - OX_FADE_STEP * frames;
			menu->mFade = fade;
			if (fade <= 0.0f)
				menu->mFade = 0.0f;
		}
		if (0.0f == menu->mFade) {
			s32 anim = menu->mChr->mAnimId;
			if (anim != 0x78 && anim != 0xEF && anim != 0x13B) {
				menu->mStarted = 0;
				lbl_8025C400.mStatus.mFlags &= ~0x800ULL;
				menu->mChr->SetRotateY(menu->mStartRot, DEG2RAD * 12.0f);
				XObjChr* chr = lbl_8025C400.GetPlayer();
				((XPad*)chr->mPad)->Update(0);
			}
		}
	}
	if (menu->mFade > 0.0f) {
		f32 spin = DEG2RAD * 8.0f * frames;
		f32 tilt = DEG2RAD * 3.0f * frames;
		f32 grow = 0.05f * frames;
		for (s32 i = 0; i < 2; ++i) {
			OXMenuMark* mark = &menu->mMark[i];
			mark->mSpin += spin;
			mark->mTilt += tilt;
			if (i == menu->_0C) {
				f32 scale    = mark->mScale + grow;
				mark->mScale = scale;
				if (scale > 0.8f)
					mark->mScale = 0.8f;
			} else {
				f32 scale    = mark->mScale - grow;
				mark->mScale = scale;
				if (scale < 0.5f)
					mark->mScale = 0.5f;
			}
#if !defined(VERSION_JP)
			mark->mObj.vfA4(frames);
#endif
		}
	}
}

// OXMenu's input, left and right pick, a confirms
/**
 * @note Address: 0x80096E38
 * @note Size: 0x2AC
 */
extern "C" void fn_80096E38(OXMenu* menu, CPad* pad)
{
	if (menu->mStarted > 0) {
		if (!(lbl_8025C400.mStatus.mFlags & 0x100ULL)) {
			menu->mChr->SetRotateY(PI + lbl_8025C400.mMotionControlCamera.mCamera.mAngle.y, DEG2RAD * 8.0f);
		}
		u32 next     = (lbl_8025C400.mStatus.mFlags & 0x80000000ULL) ? 0x80000 : 0x80002;
		u32 previous = (lbl_8025C400.mStatus.mFlags & 0x80000000ULL) ? 0x40000 : 0x40001;
		if (pad->mPad->mRepeat & next) {
			if (menu->_0C + 1 < 2) {
				menu->_0C = menu->_0C + 1;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 255);
			}
		} else if (pad->mPad->mRepeat & previous) {
			if (menu->_0C - 1 >= 0) {
				menu->_0C = menu->_0C - 1;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 255);
			}
		}
		f32 angle                 = menu->_0C == 0 ? -(DEG2RAD * 20.0f) : DEG2RAD * 20.0f;
		XObjChr* chr              = (XObjChr*)menu->mChr;
		chr->mJointRot[1].mMode   = 1;
		chr->mJointRot[1].mTarget = angle;
		if (!(lbl_8025C400.mStatus.mFlags & 0x20ULL) && 1.0f == menu->mFade && (pad->mPad->mTrigger & 0x100)) {
			menu->mStarted = -1;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 255);
			menu->_1340 = menu->_0C;
			menu->mChr->SetAnim(0x78, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			if (!(menu->_08 & (1U << menu->_1340))) {
				fn_800D5874(&((XObjChr*)menu->mChr)->mOXObj);
			} else {
				fn_800D5804(&((XObjChr*)menu->mChr)->mOXObj);
			}
			chr                       = (XObjChr*)menu->mChr;
			chr->mJointRot[1].mMode   = 2;
			chr->mJointRot[1].mTarget = 0.0f;
			chr->mJointRot[1].mLookAt = NULL;
		}
	}
}

// joint walk callback, args are the diffuse and ambient colors and the alpha
/**
 * @note Address: 0x800970E4
 * @note Size: 0xE0
 */
extern "C" void fn_800970E4(HSD_JObj* joint, f32** arguments, s32 type)
{
	GXColor diffuse = *(GXColor*)arguments[0];
	GXColor ambient = *(GXColor*)arguments[1];
	f32 alpha       = *arguments[2];
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			HSD_MObjSetDiffuseColor(material, diffuse.r, diffuse.g, diffuse.b);
			fn_801BA3AC(material, ambient.r, ambient.g, ambient.b);
			HSD_MObjSetAlpha(material, alpha);
		}
	}
}

// joint walk callback, makes every material translucent
/**
 * @note Address: 0x800971C4
 * @note Size: 0xC0
 */
extern "C" void fn_800971C4(HSD_JObj* joint, f32** arguments, s32 type)
{
	HSD_JObjModifyFlags(joint, 0x20080000, 0x701C0000);
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_DObjModifyFlags(draw, 4, 0xE);
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			HSD_MObjSetFlags(material, 0x60000000);
			HSD_MObjClearFlags(material, 8);
		}
		if (draw != NULL)
			HSD_MObjCompileTev(draw->mobj);
	}
}

// closes both rings, 0 cancels, above 0 confirms, below 0 backs out
/**
 * @note Address: 0x80097284
 * @note Size: 0xF4
 */
extern "C" void fn_80097284(XObjChrMenu* menu, s32 result)
{
	if (result == 0) {
		fn_8001E904(&lbl_8025C400.mSound, 0x86, 255);
		if (menu->mAtc._280 != 3)
			menu->mSel = menu->mPrevSel;
	} else if (result > 0) {
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 255);
	} else if (result < 0) {
		fn_8001E904(&lbl_8025C400.mSound, 0x86, 255);
		if (menu->mAtc._280 != 3)
			menu->mSel = menu->mPrevSel;
	}
	menu->mAtc.Select(result);
	menu->mDrs.Select(result);
}

// DrsMenu::vf28, the item's color, pulsing while selected
/**
 * @note Address: 0x80098204
 * @note Size: 0x1B4
 */
extern "C" u32 fn_80098204(DrsMenu* menu, s32 index)
{
	s32 red, green, blue;
	if (menu->mItems[index].mAvailable == 0) {
		blue  = 32;
		green = 32;
		red   = 32;
	} else if (menu->_280 == 3) {
		if (IsWorn(menu, index)) {
			red   = 50;
			green = 200;
			blue  = 200;
		} else {
			red   = 50;
			green = 150;
			blue  = 50;
		}
	} else if (index == menu->_04 && IsWorn(menu, index)) {
		red   = 200;
		green = 100;
		blue  = 150;
	} else if (index == menu->_04) {
		red   = 200;
		green = 100;
		blue  = 100;
	} else if (IsWorn(menu, index)) {
		red   = 50;
		green = 200;
		blue  = 200;
	} else {
		red   = 50;
		green = 150;
		blue  = 50;
	}
	if (index == menu->_04)
		AddPulse(red, green, blue, menu, index);
	return (red << 24) | ((u8)green << 16) | ((u8)blue << 8);
}

// DrsMenu::vf24, the item's label
/**
 * @note Address: 0x800983B8
 * @note Size: 0x64
 */
extern "C" const char* fn_800983B8(DrsMenu* menu, s32 index)
{
	if (menu->mItems[index].mAvailable > 0) {
		if (IsWorn(menu, index))
			return fn_8006DBF0(DRS_LABEL(13));
		return (const char*)fn_8006E034(menu->mItems[index].mType);
	}
	return fn_8006DBF0(DRS_LABEL(14));
}

// DrsMenu::vf20, the item's icon
/**
 * @note Address: 0x8009841C
 * @note Size: 0xB0
 */
extern "C" s32 fn_8009841C(DrsMenu* menu, s32 index)
{
	s32 icons[9] = { DRS_LABEL(126), DRS_LABEL(98),  DRS_LABEL(100), DRS_LABEL(99), DRS_LABEL(101),
		             DRS_LABEL(126), DRS_LABEL(102), DRS_LABEL(103), DRS_LABEL(104) };
	if (index == -1)
		return DRS_LABEL(107);
	if (index == -2)
		return DRS_LABEL(109);
	if (menu->mItems[index].mAvailable > 0)
		return icons[menu->mItems[index].mType];
	return DRS_LABEL(105);
}

// DrsMenu::vf2C
/**
 * @note Address: 0x800984CC
 * @note Size: 0x28
 */
extern "C" BOOL fn_800984CC(DrsMenu* menu, s32 index)
{
	return IsWorn(menu, index);
}

// puts the selected costume on, or takes the worn one off
/**
 * @note Address: 0x800984F4
 * @note Size: 0xC0
 */
void DrsMenu::vf1C()
{
	s8 selectable[9] = { 0, 1, 1, 1, 1, 0, 1, 1, 1 };
	s32 type         = mItems[_04].mType;
	if (selectable[type]) {
		if (type == mChr->mSuit) {
			XObjChr* chr = mChr;
			if (chr->mSuitChanged == 0) {
				chr->mSuitRequest = 0;
				chr->mSuitChanged = 1;
			}
		} else {
			XObjChr* chr = mChr;
			if (chr->mSuitChanged == 0) {
				chr->mSuitRequest = type;
				chr->mSuitChanged = 1;
			}
		}
	} else {
		XObjChr* chr = mChr;
		if (chr->mSuitChanged == 0) {
			chr->mSuitRequest = 0;
			chr->mSuitChanged = 1;
		}
	}
}

// OctaMenu::Open, ahead of the two menus that inline it
/**
 * @note Address: 0x8009BF14
 * @note Size: 0x2A8
 */
extern "C" void fn_8009BF14(OctaMenu* menu, s32 focus)
{
	if (menu->mOpen == 0) {
		menu->_16C = menu->mChr->mMoveRotate.y;
		SetTurn(&menu->mChr->mJointRot[1], 0.0f);
		SetTurn(&menu->mChr->mJointRot[2], -1.52f);
	}
	menu->mOpen  = 1;
	menu->mFocus = focus;
	if (menu->mFocus != 0) {
		menu->_0C = 1.0f;
	}
	Vec offset = { 0.0f, 8.8f, 0.0f };
	menu->mChr->vf10C(&offset, &menu->_158, &menu->_15C, NULL, 0);
	for (int i = 0; i < 8; i++) {
		menu->mItems[i]._08 = DEG2RAD * (rand() % 360);
		menu->mItems[i]._0C = DEG2RAD * (rand() % 360);
		menu->mItems[i]._10 = menu->mFocus != 0 ? 0.3f : 0.5f;
	}
	menu->_04  = -1;
	menu->_170 = 0;
	for (int i = 0; i < (int)ARRAY_SIZE(menu->_174); i++) {
		menu->_174[i]._04 = DEG2RAD * (rand() % 360);
		menu->_174[i]._08 = DEG2RAD * (rand() % 360);
	}
	menu->mChr->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

// DrsMenu::Open, a costume is there when he has it
/**
 * @note Address: 0x800985B4
 * @note Size: 0x488
 */
extern "C" void fn_800985B4(OctaMenu* menu, s32 focus)
{
	// NONMATCHING: the costume ids are read from the unit's rodata
	if (menu->_280 == 3) {
		s16 ids[8] = { 0x18, 0x20, 0x22, 0x19, 0x1A, 0x1E, 0x1F, 0x21 };
		for (s32 i = 0; i < 8; ++i) {
			if (menu->_280 == 2)
				menu->mItems[i].mAvailable = 1;
			else
				menu->mItems[i].mAvailable = HasItem(menu->mChr, ids[i]);
		}
	} else {
		s16 ids[8] = { 0x19, 0x21, 0x20, 0x18, 0x22, 0x1F, 0x1E, 0x1A };
		for (s32 i = 0; i < 8; ++i) {
			if (menu->_280 == 2)
				menu->mItems[i].mAvailable = 1;
			else
				menu->mItems[i].mAvailable = HasItem(menu->mChr, ids[i]);
		}
	}
	fn_8009BF14(menu, focus);
	if (menu->_280 == 3) {
		if (menu->_28C >= 0) {
			menu->_288 = menu->_28C;
			menu->_28C = -1;
		} else {
			// one past the items
			for (s32 i = 0; i < 9; ++i) {
				if (menu->mItems[i].mType == menu->mChr->mSuit) {
					menu->_288 = i - 3;
					if (menu->_288 < 0)
						menu->_288 += 7;
				}
			}
		}
	}
}

// AtcMenu::vf28, the item's color, pulsing while selected
/**
 * @note Address: 0x80098A3C
 * @note Size: 0x1E4
 */
extern "C" u32 fn_80098A3C(AtcMenu* menu, s32 index)
{
	s32 red, green, blue;
	if (menu->mItems[index].mAvailable == 0) {
		blue  = 32;
		green = 32;
		red   = 32;
	} else if (menu->_280 == 3) {
		if (IsAttachment(menu->mChr, menu->mItems[index].mType)) {
			red   = 50;
			green = 200;
			blue  = 200;
		} else {
			red   = 50;
			green = 50;
			blue  = 200;
		}
	} else if (index == menu->_04 && IsAttachment(menu->mChr, menu->mItems[index].mType)) {
		red   = 200;
		green = 100;
		blue  = 150;
	} else if (index == menu->_04) {
		red   = 200;
		green = 100;
		blue  = 100;
	} else if (IsAttachment(menu->mChr, menu->mItems[index].mType)) {
		red   = 50;
		green = 200;
		blue  = 200;
	} else {
		red   = 50;
		green = 50;
		blue  = 200;
	}
	if (index == menu->_04)
		AddPulse(red, green, blue, menu, index);
	return (red << 24) | ((u8)green << 16) | ((u8)blue << 8);
}

// AtcMenu::vf24, the item's label. the last one stays up while a switch is pending or an
// action that holds the attachment is running
/**
 * @note Address: 0x80098C20
 * @note Size: 0xCC
 */
extern "C" const char* fn_80098C20(AtcMenu* menu, s32 index)
{
	static const char* sLabel = "";
	if (menu->mItems[index].mAvailable > 0) {
		XObjChr* chr = menu->mChr;
		if (chr->mAttachmentRequest < 0 && chr->mLift.mState != 20 && chr->mLift.mState != 21 && chr->mLift.mState != 22) {
			if (menu->mItems[index].mType == chr->GetAttachment())
				sLabel = fn_8006DBF0(ATC_LABEL(12));
			else
				sLabel = (const char*)fn_8006E160(menu->mItems[index].mType);
		}
	} else {
		sLabel = fn_8006DBF0(ATC_LABEL(14));
	}
	return sLabel;
}

// AtcMenu::vf20, the item's icon
/**
 * @note Address: 0x80098CEC
 * @note Size: 0xB0
 */
extern "C" s32 fn_80098CEC(AtcMenu* menu, s32 index)
{
	s32 icons[9] = { ATC_LABEL(126), ATC_LABEL(91), ATC_LABEL(92), ATC_LABEL(93), ATC_LABEL(94),
		             ATC_LABEL(96),  ATC_LABEL(95), ATC_LABEL(97), ATC_LABEL(126) };
	if (index == -1)
		return ATC_LABEL(106);
	if (index == -2)
		return ATC_LABEL(108);
	if (menu->mItems[index].mAvailable > 0)
		return icons[menu->mItems[index].mType];
	return ATC_LABEL(105);
}

// AtcMenu::vf2C
/**
 * @note Address: 0x80098D9C
 * @note Size: 0x38
 */
extern "C" u8 fn_80098D9C(AtcMenu* menu, s32 index)
{
	return menu->mChr->GetAttachment() == menu->mItems[index].mType;
}

// AtcMenu::vf1C, switches to the selected attachment, or puts the current one away
/**
 * @note Address: 0x80098DD4
 * @note Size: 0x74
 */
extern "C" void fn_80098DD4(AtcMenu* menu)
{
	XObjChr* chr = menu->mChr;
	s32 current  = chr->GetAttachment();
	s32 type     = menu->mItems[menu->_04].mType;
	if (type == current) {
		if (chr->mAttachment != 0) {
			chr->mAttachmentRequest = 0;
			chr->_95D8              = 0;
		}
	} else {
		if (type != chr->mAttachment) {
			chr->mAttachmentRequest = type;
			chr->_95D8              = 0;
		}
	}
}

// AtcMenu::Open, OctaMenu's plus the item counts and the start item
/**
 * @note Address: 0x80098E48
 * @note Size: 0x3B4
 */
extern "C" void fn_80098E48(OctaMenu* menu, s32 focus)
{
	for (s32 i = 0; i < 8; ++i) {
		if (menu->_280 == 2)
			menu->mItems[i].mAvailable = 1;
		else
			menu->mItems[i].mAvailable = fn_800991FC(menu->mChr, menu->mItems[i].mType);
	}
	fn_8009BF14(menu, focus);
	if (menu->_280 == 3) {
		if (menu->_28C >= 0) {
			menu->_288 = menu->_28C;
			menu->_28C = -1;
		} else {
			for (s32 i = 0; i < 8; ++i) {
				if (menu->mItems[i].mType == menu->mChr->GetAttachment()) {
					menu->_288 = i - 3;
					if (menu->_288 < 0)
						menu->_288 += 7;
				}
			}
		}
	}
}

// how many of an attachment chibi robo has
/**
 * @note Address: 0x800991FC
 * @note Size: 0x28
 */
extern "C" s32 fn_800991FC(XObjChr* chr, s32 type)
{
	return fn_8009C1BC(&chr->mAttachments, type);
}

// OctaMenu::vf24, the base menu has no labels
/**
 * @note Address: 0x8009AFD4
 * @note Size: 0x8
 */
extern "C" const char* fn_8009AFD4(OctaMenu* menu, s32 index)
{
	return lbl_8065D4AC;
}

// OctaMenu::vf2C
/**
 * @note Address: 0x8009AFDC
 * @note Size: 0x8
 */
extern "C" u8 fn_8009AFDC(OctaMenu* menu, s32 index)
{
	return 0;
}

// OctaMenu::vf20
/**
 * @note Address: 0x8009AFE4
 * @note Size: 0x8
 */
extern "C" s32 fn_8009AFE4(OctaMenu* menu, s32 index)
{
	return 0;
}

// OctaMenu::vf28, the item's color, pulsing while selected
/**
 * @note Address: 0x8009AFEC
 * @note Size: 0x130
 */
extern "C" u32 fn_8009AFEC(OctaMenu* menu, s32 index)
{
	s32 red, green, blue;
	if (menu->mItems[index].mAvailable == 0) {
		blue  = 32;
		green = 32;
		red   = 32;
	} else if (menu->_280 == 3) {
		red   = 50;
		green = 50;
		blue  = 200;
	} else if (index == menu->_04) {
		red   = 200;
		green = 100;
		blue  = 100;
	} else {
		red   = 50;
		green = 50;
		blue  = 200;
	}
	if (index == menu->_04)
		AddPulse(red, green, blue, menu, index);
	return (red << 24) | ((u8)green << 16) | ((u8)blue << 8);
}

// OctaMenu::vf14, the stick or the dpad moves the cursor, a picks, b closes. 1 when it picked,
// -1 when it closed
/**
 * @note Address: 0x8009B9E0
 * @note Size: 0x438
 */
extern "C" s32 fn_8009B9E0(OctaMenu* menu, BOOL active)
{
	// NONMATCHING: the turn table and chibi robo swap r3 and r4
	if (menu->mOpen > 0 && menu->mFocus == 0) {
		CPad pad(0);
		s32 cursor;
		if (menu->_280 == 3) {
			u32 dir = active ? pad.mPad->mDirRepeat & 0xF : 0;
			if (dir & 1) {
				if (--menu->_288 < 0)
					menu->_288 = 6;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 255);
			} else if (dir & 2) {
				if (++menu->_288 >= 7)
					menu->_288 = 0;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 255);
			}
			if (menu->mFocus != 0)
				cursor = menu->_04 = -1;
			else
				cursor = menu->_04 = (menu->_288 + 3) % 7;
		} else {
			u32 stick = active ? pad.mPad->mButton & 0xF0000 : 0;
			if (stick == 0x50000)
				cursor = 0;
			else if (stick == 0x10000)
				cursor = 1;
			else if (stick == 0x90000)
				cursor = 2;
			else if (stick == 0x80000)
				cursor = 3;
			else if (stick == 0xA0000)
				cursor = 4;
			else if (stick == 0x20000)
				cursor = 5;
			else if (stick == 0x60000)
				cursor = 6;
			else if (stick == 0x40000)
				cursor = 7;
			else
				cursor = menu->_04;
		}
		if (menu->_280 != 3) {
			TurnJoint(&menu->mChr->mJointRot[1], DEG2RAD * sMenuTurnY[cursor + 1]);
			if (fpclassify(menu->mHeadTurn) == FP_NAN)
				TurnJoint(&menu->mChr->mJointRot[2], DEG2RAD * sMenuTurnX[cursor + 1] + -1.52f);
			else
				TurnJoint(&menu->mChr->mJointRot[2], menu->mHeadTurn);
		}
		if (menu->_04 != cursor)
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 255);
		menu->_04 = cursor;
		if (active && menu->mFade >= 1.0f) {
			if (pad.mPad->mTrigger & 0x100) {
				if (menu->_04 < 0) {
					fn_8001E904(&lbl_8025C400.mSound, 0x85, 255);
				} else if (menu->mItems[menu->_04].mAvailable == 0) {
					fn_8001E904(&lbl_8025C400.mSound, 0x85, 255);
				} else {
					menu->vf1C();
					StopShots(menu->mChr);
					return 1;
				}
			} else if (pad.mPad->mTrigger & 0x200) {
				return -1;
			}
		}
	}
	return 0;
}

// OctaMenu::vf1C, nothing to decide on the base menu
/**
 * @note Address: 0x8009BE18
 * @note Size: 0x4
 */
extern "C" void fn_8009BE18(OctaMenu* menu)
{
}

// OctaMenu::Select, closes the ring and turns chibi robo back
/**
 * @note Address: 0x8009BE1C
 * @note Size: 0xF8
 */
extern "C" void fn_8009BE1C(OctaMenu* menu, s32 result)
{
	if (result > 0 && menu->mFocus == 0)
		menu->mOpen = -1;
	else
		menu->mOpen = -2;
	menu->_28C   = -1;
	menu->mFocus = 0;
	if (menu->mChr->mTumble.mState == 0) {
		menu->mChr->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		menu->mChr->SetRotateY(menu->_16C, DEG2RAD * 12.0f);
		CJointRot* neck = &menu->mChr->mJointRot[1];
		neck->mMode     = 2;
		neck->mTarget   = 0.0f;
		neck->mLookAt   = NULL;
		CJointRot* head = &menu->mChr->mJointRot[2];
		head->mMode     = 2;
		head->mTarget   = -1.52f;
		head->mLookAt   = NULL;
	}
}

// the count for an attachment id, -1 when it isn't in the list
/**
 * @note Address: 0x8009C1BC
 * @note Size: 0x10C
 */
extern "C" s32 fn_8009C1BC(AttachmentValues* values, s32 id)
{
	s32 index = FindAttachment(values, id);
	if (index >= 0) {
		fn_8009C398(values);
		return values->mEntries[index].mValue;
	}
	return -1;
}

/**
 * @note Address: 0x8009C2C8
 * @note Size: 0xD0
 */
extern "C" void fn_8009C2C8(AttachmentValues* values, s32 id, s32 value)
{
	s32 index = FindAttachment(values, id);
	if (index >= 0)
		values->mEntries[index].mValue = value;
}

// which of his tools he can use, he has them or one of them is going
/**
 * @note Address: 0x8009C398
 * @note Size: 0x548
 */
extern "C" void fn_8009C398(AttachmentValues* values)
{
	XObjChr* chr = lbl_8025C400.GetPlayer();
	SetAttachmentOn(values, 4, CanUseTool(chr, 0x10));
	SetAttachmentOn(values, 6, CanUseTool(chr, 0x16));
	SetAttachmentOn(values, 5, CanUseTool(chr, 0x15));
	SetAttachmentOn(values, 7, CanUseTool(chr, 0x38));
	SetAttachmentOn(values, 8, false);
}

// the first three start at what the stage says, or at what the command line picked
/**
 * @note Address: 0x8009C8E0
 * @note Size: 0x5C0
 */
extern "C" void fn_8009C8E0(AttachmentValues* values)
{
	if (fn_8006EC34()) {
		fn_8009C2C8(values, 1, lbl_8020FBC0.mPick1 != 0 ? lbl_8020FBC0.mPick1 - 1 : fn_8006E940(0));
		fn_8009C2C8(values, 2, lbl_8020FBC0.mPick2 != 0 ? lbl_8020FBC0.mPick2 - 1 : fn_8006E940(1));
		fn_8009C2C8(values, 3, lbl_8020FBC0.mPick3 != 0 ? lbl_8020FBC0.mPick3 - 1 : fn_8006E940(2));
	} else {
		fn_8009C2C8(values, 1, lbl_8020FBC0.mPick1 != 0 ? lbl_8020FBC0.mPick1 - 1 : 0);
		fn_8009C2C8(values, 2, lbl_8020FBC0.mPick2 != 0 ? lbl_8020FBC0.mPick2 - 1 : 0);
		fn_8009C2C8(values, 3, lbl_8020FBC0.mPick3 != 0 ? lbl_8020FBC0.mPick3 - 1 : 0);
	}
}

/**
 * @note Address: 0x8009CEA0
 * @note Size: 0x34
 */
extern "C" void fn_8009CEA0(AttachmentValues* values)
{
	fn_8009C8E0(values);
	fn_8009C398(values);
}

// the eight attachments in ring order, then a refresh
/**
 * @note Address: 0x8009CED4
 * @note Size: 0x60
 */
extern "C" void fn_8009CED4(AttachmentValues* values)
{
	values->mEntries[0].mId = 1;
	values->mEntries[1].mId = 2;
	values->mEntries[2].mId = 3;
	values->mEntries[3].mId = 4;
	values->mEntries[4].mId = 5;
	values->mEntries[5].mId = 6;
	values->mEntries[6].mId = 7;
	values->mEntries[7].mId = 8;
	fn_8009CEA0(values);
}

// steps the marker on what chibi robo looks at
/**
 * @note Address: 0x8009D600
 * @note Size: 0x178
 */
extern "C" void fn_8009D600(XObjChrLook* look, u32 frames)
{
	if (look->mActive) {
		if (look->mFade < 1.0f) {
			f32 fade    = look->mFade + LOOK_FADE_STEP * frames;
			look->mFade = fade;
			if (fade >= 1.0f)
				look->mFade = 1.0f;
			look->mPulse = look->mFade;
		}
	} else if (look->mFade > 0.0f) {
		f32 fade    = look->mFade - LOOK_FADE_STEP * frames;
		look->mFade = fade;
		if (fade <= 0.0f)
			look->mFade = 0.0f;
	}
	if (look->mFade > 0.0f) {
		look->mPhase = fmod(look->mPhase + (f32)(frames * 6), 360.0);
		if (look->mFade >= 1.0f) {
			f32 pulse    = look->mPulse + LOOK_FADE_STEP * frames;
			look->mPulse = pulse;
			if (pulse >= 2.0f)
				look->mPulse = fmod(look->mPulse, 2.0);
		}
	}
}

// XObjChr::vf144, what chibi robo does when he's left alone: looks at the camera first,
// then two longer idles at 3000 and 6000
/**
 * @note Address: 0x8009D778
 * @note Size: 0x330
 */
extern "C" void fn_8009D778(XObjChr* chr, s32 input, s32 elapsed)
{
	s32 idle;
	if (lbl_8025C400.mEvt.mData.mScript != NULL || !InputEnabled(&lbl_8025C400) || (lbl_8025C400.mStatus.mFlags & 0xC00)
	    || (lbl_8025C400.mStatus.mFlags & 0x100) || !(chr->_980.mFlags & 0x20000000) || chr->mCopter.mState != 0
	    || (chr->mPopper.mState != 0 && chr->mPopper.mState != 4) || chr->mRadar.mState != 0
	    || (!(idle = (u8)(chr->mLift.mState == 0)) && chr->mLift.mState != 4) || chr->mHang.mState != 0 || chr->mJump.mState != 0
	    || chr->mLadder.mState != 0 || chr->mRope.mState != 0 || chr->mPush.mState != 0 || chr->mTumble.mState != 0
	    || chr->mAttachmentRequest != -1 || chr->mSuitChanged != 0)
		return;
	if (input != -1)
		return;
	if (elapsed > 6000 && (idle || (chr->mLift.mState == 4 && fn_800D4FE8(&chr->mLift)))) {
		u8 release = chr->mLift.mState == 4 && fn_800D4FE8(&chr->mLift);
		if (release) {
			if (chr->mLift.mTarget) {
				chr->mLift.mTarget->mStatus.mFlags &= 0xFBFFFFFFFFFFFFFFULL;
				chr->mLift.mTarget = NULL;
			}
			chr->mLift.mState = 0;
		}
		chr->mJointRot[1].mMode    = 2;
		chr->mJointRot[1].mTarget  = 0.0f;
		chr->mJointRot[1].mLookAt  = NULL;
		chr->mTumble.mState        = 15;
		chr->mTumble.mFade.mActive = 1;
		chr->mTumble.mFade.mAmount = 0.0f;
		chr->_158BC                = 3 * IDLE_SECOND;
	} else if (elapsed > 3000) {
		chr->mTumble.mState       = 12;
		chr->mJointRot[1].mMode   = 2;
		chr->mJointRot[1].mTarget = 0.0f;
		chr->mJointRot[1].mLookAt = NULL;
		chr->_158B8               = 2;
		chr->_158BC               = IDLE_SECOND;
	} else {
		f32 angle = (f32)atan2(lbl_8025C400.mMotionControlCamera.mCamera.mEye.x - chr->mMoveTranslate.x,
		                       lbl_8025C400.mMotionControlCamera.mCamera.mEye.z - chr->mMoveTranslate.z)
		          - chr->mMoveRotate.y;
		chr->mJointRot[1].mMode   = 1;
		chr->mJointRot[1].mTarget = angle;
		chr->_158B8               = 1;
		chr->_158BC               = IDLE_SECOND;
	}
}

// the ground type under chibi robo, from what he hangs or climbs on, the object he stands
// on, or else the map's floor color. 13 and up count as none
/**
 * @note Address: 0x8009DAA8
 * @note Size: 0x1DC
 */
extern "C" s32 fn_8009DAA8(XObjChr* chr)
{
	s32 type = 0;
	if (chr->mHang.mState == 5 && chr->mHang.mTarget != NULL) {
		XObj* owner = chr->mHang.mTarget;
		type        = owner->mFloors->mType;
	} else if (chr->mRope.mState == 5 && chr->mRope.mTarget != NULL) {
		XObj* owner = chr->mRope.mTarget;
		type        = owner->mFloors->mType;
	} else if (chr->mLadder.mState == 5 && chr->mLadder.mTarget != NULL) {
		XObj* owner = chr->mLadder.mTarget;
		type        = owner->mFloors->mType;
	} else if (chr->_9D4[0] >= 0) {
		XObj* owner = lbl_8025C400.mXObjs.mObjects[chr->_9D4[0]];
		f32 best    = NAN;
		for (s32 i = 0; owner->mFloors[i].mId != -1; ++i) {
			if ((owner->_250[0] & (1U << i)) && (__fpclassifyf(best) == FP_NAN || owner->_6FC[i] > best)) {
				f32 height = owner->_6FC[i];
				if (height <= 1.0f + chr->mMoveTranslate.y) {
					best = height;
					type = owner->mFloors[i].mType;
				}
			}
		}
	}
	type = (u8)type;
	if (type == 0 && chr->_788 >= 0 && (s32)chr->_78C >= 0) {
		type = fn_80093210((XObjMap*)lbl_8025C400.mXObjs.mPools[3].mObjects, (s32)chr->_78C, chr->_788);
	}
	if (type >= 13)
		type = 0;
	return type;
}

// the highest floor of the object under a floor group that's still below chibi robo,
// stops early on the preferred one, -1 for none
/**
 * @note Address: 0x8009DC84
 * @note Size: 0x134
 */
extern "C" s32 fn_8009DC84(XObjChr* chr, s32 group, s32 preferred)
{
	s32 result = -1;
	if (chr->_9D4[group] >= 0) {
		XObj* owner = lbl_8025C400.mXObjs.mObjects[chr->_9D4[group]];
		f32 best    = NAN;
		for (int i = 0; owner->mFloors[i].mId != -1; i++) {
			if (owner->_250[group] & (1 << i)) {
				if ((fpclassify(best) == FP_NAN || owner->_6FC[i] >= best) && owner->_6FC[i] <= 1.0f + chr->mMoveTranslate.y) {
					best   = owner->_6FC[i];
					result = i;
					if (i == preferred)
						break;
				}
			}
		}
	}
	return result;
}

/**
 * @note Address: 0x8009F4F8
 * @note Size: 0x2D8
 */
extern "C" void fn_8009F4F8(CJObj* object, s32 depth, s32* path, f32 x, f32 y, f32 z)
{
	CJObj joint(GetJointByPath(object->mJObj, path, depth));
	joint.CJObj::SetTranslate(x, y, z, TRUE);
}

// pool 9 objects decide for themselves whether chibi robo can reach them
/**
 * @note Address: 0x8009FC60
 * @note Size: 0x64
 */
extern "C" s32 fn_8009FC60(const XObjChr* chr, const XObj* target)
{
	if (target->GetCategory() == 9)
		return fn_800E4AAC((XObj*)target, &chr->mMoveTranslate);
	return 1;
}

// resets the plug, keeping its rotation while it's carried
/**
 * @note Address: 0x8009FCC4
 * @note Size: 0x120
 */
extern "C" void fn_8009FCC4(XObjChr* chr)
{
	if (chr->mPlug.mStatus.mFlags & 0x0400000000000000ULL) {
		Vec rotation = (Vec&)chr->mPlug.mMoveRotate;
		chr->mPlug.mStatus.mFlags &= ~0x0400000000000000ULL;
		chr->mPlug.vf98();
		chr->mPlug.vfA4(10);
		XObj* plug = &chr->mPlug;
		plug->SetRot(rotation, 0);
		chr->mPlug.mStatus.mFlags |= 0x0400000000000000ULL;
	} else {
		chr->mPlug.vf98();
		chr->mPlug.vfA4(10);
	}
}

// todo: cleanup, xobjchr.h has it as void*
/**
 * @note Address: 0x8009FDE4
 * @note Size: 0xF8
 */
extern "C" BOOL fn_8009FDE4(void* object)
{
	return IsEventBlocked((XObjChr*)object);
}

// out of battery
/**
 * @note Address: 0x8009FEDC
 * @note Size: 0x64
 */
extern "C" BOOL fn_8009FEDC(const XObjChr* chr)
{
	return IsStopped(chr);
}

// XObjChr::vf138, hit from a position. flashes what he carries instead when he lifts something
/**
 * @note Address: 0x8009FF40
 * @note Size: 0xC0
 */
extern "C" void fn_8009FF40(XObjChr* chr, f32 strength)
{
	if (fn_800D4D54(&chr->mLift)) {
		chr->mLift.mTarget->vf12C(1, 150, 150, 0, 0.0f);
		chr->mLift.mTarget->vf12C(1, 0, 0, 0, 0.2f);
	} else if (chr->_A34 <= 0) {
		fn_800A0014(chr, TRUE, TRUE, strength);
	}
}

// still flashing from a hit
/**
 * @note Address: 0x800A0000
 * @note Size: 0x14
 */
extern "C" u8 fn_800A0000(XObjChr* chr)
{
	return chr->_A34 > 0;
}

// hit, flinches when he's free to and starts the flash timer
/**
 * @note Address: 0x800A0014
 * @note Size: 0x220
 */
extern "C" void fn_800A0014(XObjChr* chr, BOOL start, BOOL cancel, f32 value)
{
	if (!(lbl_8025C400.mStatus.mFlags & 0xC00ULL) && chr->mAttachmentRequest < 0 && lbl_8025C400.mEvt.mData.mScript == NULL) {
		BOOL idle;
		if (start && ((idle = chr->mLift.mState == 0) || chr->mLift.mState == 4) && chr->mHang.mState == 0 && chr->mJump.mState == 0
		    && chr->mLadder.mState == 0 && chr->mRope.mState == 0 && chr->mPush.mState == 0 && chr->mTumble.mState == 0
		    && chr->mCopter.mState == 0 && (chr->mPopper.mState == 0 || chr->mPopper.mState == 4 || chr->mPopper.mState == 8)
		    && chr->mRadar.mState == 0) {
			if (idle)
				chr->SetAnim(0xD4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			else
				chr->SetAnim(0x94, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->mTumble.mState = 8;
		} else if (cancel) {
			if (chr->mHang.mState == 5)
				chr->mHang.mState = 8;
			else if (chr->mLadder.mState == 5)
				chr->mLadder.mState = 8;
			else if (chr->mRope.mState == 5)
				chr->mRope.mState = 7;
		}
	}
	if (0.0f != value) {
#if defined(VERSION_PAL)
		chr->_A34 = lbl_80660D4C;
#else
		chr->_A34 = 60;
#endif
		chr->_A38 = 0;
		chr->_A3C = value;
	}
}

// XObjChr::vf12C, the color goes to everything he wears and carries too. while a costume
// change is running, blend 2 turns into 1 and 1 is dropped
/**
 * @note Address: 0x800A0234
 * @note Size: 0x1E0
 */
extern "C" void fn_800A0234(XObjChr* chr, s32 blend, u8 red, u8 green, u8 blue, f32 time)
{
	if (chr->mSuitChanged != 0) {
		if (blend == 2)
			blend = 1;
		else if (blend == 1)
			return;
	}
	fn_8008A820(chr, blend, red, green, blue, time);
	chr->mPlug.vf12C(blend, red, green, blue, time);
	chr->mCopterObj.vf12C(blend, red, green, blue, time);
	chr->mPopperObj.vf12C(blend, red, green, blue, time);
	chr->mRadarObj.vf12C(blend, red, green, blue, time);
	chr->mOXObj.vf12C(blend, red, green, blue, time);
	for (s32 i = 0; i < 9; ++i)
		chr->mSuitObj[i].vf12C(blend, red, green, blue, time);
	if (fn_800D4D54(&chr->mLift))
		chr->mLift.mTarget->vf12C(blend, red, green, blue, time);
}

// his radius, bigger while he holds something up, by the scale of the held joint
/**
 * @note Address: 0x800A0414
 * @note Size: 0x280
 */
extern "C" f32 fn_800A0414(XObjChr* chr)
{
	f32 size = 1.5f * chr->mMoveScale.x;
	// the plug anims pull it in and out
	if (fn_800D4D54(&chr->mLift) && !(chr->mAnimId == 11 && CurrentFrame(chr->mJObj) < 45.0f)
	    && !(chr->mAnimId == 10 && CurrentFrame(chr->mJObj) > 22.0f)) {
		CVec scale;
		s32 path[2] = { 0, 0 };
		HSD_JObjGetScale(FindJointByPath(chr->mJObj, path, 2), &scale);
		size += 4.0f * scale.x;
	}
	return size;
}

// XObjChr::SetMoveDir, stops whatever push is running and keeps the heading he walks towards
/**
 * @note Address: 0x800A0694
 * @note Size: 0x1BC
 */
extern "C" void fn_800A0694(XObjChr* chr, const Vec* direction, f32 speed)
{
	StopMoving(chr);
	chr->CXObj::SetMoveDir(direction, speed);
	CVec horizontal = chr->mMoveTranslate.mSpeed;
	horizontal.y    = 0.0f;
	if (horizontal.Length() > 0.0f)
		chr->_12DF8 = WrapAngle(GetYAngle(chr->mMoveTranslate.mTarget - chr->mMoveTranslate));
}

/**
 * @note Address: 0x800A0850
 * @note Size: 0x44
 */
extern "C" CVec fn_800A0850(CJObj* object)
{
	s32 path[2] = { 0, 0 };
	return fn_800A0894(object, 2, path);
}

// a joint's local translation
/**
 * @note Address: 0x800A0894
 * @note Size: 0xE4
 */
extern "C" CVec fn_800A0894(const CJObj* object, s32 depth, s32* path)
{
	Vec translate;
	HSD_JObj* jobj = GetJointByPath(object->mJObj, path, depth);
	HSD_JObjGetTranslation(jobj, &translate);
	return CVec(translate);
}

/**
 * @note Address: 0x800A0978
 * @note Size: 0x44
 */
extern "C" CVec fn_800A0978(CJObj* object)
{
	s32 path[2] = { 0, 0 };
	return fn_8007E7F4(object, 2, path);
}

// halfway between two of his joints, pulled back a little along his heading
/**
 * @note Address: 0x800A09BC
 * @note Size: 0x21C
 */
extern "C" Vec fn_800A09BC(XObjChr* chr)
{
	s32 left[8]  = { 0, 1, 0, 1, 2, 0, 0, 0 };
	CVec first   = fn_8007E7F4(chr, 8, left);
	s32 right[8] = { 0, 1, 0, 1, 2, 0, 0, 2 };
	CVec second  = fn_8007E7F4(chr, 8, right);
	Vec point    = first + (second - first) / 2.0f;
	point.x -= 1.5f * (f32)sin(chr->mMoveRotate.y);
	point.z -= 1.5f * (f32)cos(chr->mMoveRotate.y);
	return point;
}

// todo: weak copy
/**
 * @note Address: 0x800A2BD0
 * @note Size: 0x64
 */
extern "C" s32 fn_800A2BD0(f32 value)
{
	s32 bits = *(s32*)&value;
	switch (bits & 0x7F800000) {
	case 0x7F800000:
		return (bits & 0x007FFFFF) ? FP_NAN : FP_INFINITE;
	case 0:
		return (bits & 0x007FFFFF) ? FP_SUBNORMAL : FP_ZERO;
	}
	return FP_NORMAL;
}

// todo: weak copy
/**
 * @note Address: 0x800A2C34
 * @note Size: 0x14
 */
extern "C" BOOL fn_800A2C34(const XAction* action, s32 state)
{
	return state == action->mState;
}

// todo: weak copy
/**
 * @note Address: 0x800A2C48
 * @note Size: 0x28
 */
extern "C" CVec fn_800A2C48(const Vec& value)
{
	return CVec(value);
}

// todo: weak copy
/**
 * @note Address: 0x800A2C70
 * @note Size: 0x14
 */
extern "C" BOOL fn_800A2C70(const XAction* action, s32 state)
{
	return state == action->mState;
}

// todo: weak copy
/**
 * @note Address: 0x800A2C84
 * @note Size: 0x14
 */
extern "C" BOOL fn_800A2C84(const XAction* action, s32 state)
{
	return state == action->mState;
}

// todo: weak copy
/**
 * @note Address: 0x800A2C98
 * @note Size: 0x1C
 */
extern "C" void fn_800A2C98(Vec* dst, const Vec* src)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

// todo: weak copy
/**
 * @note Address: 0x800A2CB4
 * @note Size: 0x14
 */
extern "C" BOOL fn_800A2CB4(const XAction* action, s32 state)
{
	return state == action->mState;
}

// todo: weak copy
/**
 * @note Address: 0x800A2CC8
 * @note Size: 0x1C
 */
extern "C" void fn_800A2CC8(Vec* dst, const Vec* src)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

// todo: weak copy
/**
 * @note Address: 0x800A2CE4
 * @note Size: 0xA4
 */
extern "C" f32 fn_800A2CE4(CJObj* object)
{
	HSD_AObj* aobj = GetAObj(object->mJObj);
	return aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
}

// todo: weak copy
/**
 * @note Address: 0x800A2D88
 * @note Size: 0xA4
 */
extern "C" f32 fn_800A2D88(CJObj* object)
{
	HSD_AObj* aobj = GetAObj(object->mJObj);
	return aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
}

// todo: weak copy
/**
 * @note Address: 0x800A2E2C
 * @note Size: 0x28
 */
extern "C" CVec fn_800A2E2C(const CVec& vector)
{
	return vector;
}

// todo: weak copy
/**
 * @note Address: 0x800A2E54
 * @note Size: 0x34
 */
extern "C" BOOL fn_800A2E54(XObj* object)
{
	return (object->mStatus.mFlags & 1ULL) != 0;
}

// hides or shows one joint subtree of chibi robo's model
/**
 * @note Address: 0x800A2E88
 * @note Size: 0x264
 */
extern "C" void fn_800A2E88(XObj* chr, BOOL hidden)
{
	if (chr->mStatus.mFlags & 1ULL) {
		s32 path[5] = { 0, 1, 0, 1, 3 };
		CJObj joint(FindJointByPath(chr->mJObj, path, 5));
		if (hidden)
			HSD_JObjSetFlagsAll(joint.mJObj, 0x10);
		else
			HSD_JObjClearFlagsAll(joint.mJObj, 0x10);
	}
}

// XObjChr::SetAnimRate, 1.135 times faster in lift state 4, and pushed onto the item's joints too
/**
 * @note Address: 0x800A36E8
 * @note Size: 0x1A4
 */
extern "C" void fn_800A36E8(XObjChr* chr, f32 rate, u32 mask)
{
	f32 multiplier;
	if (chr->mLift.mState == 4 && fn_800D4FE8(&chr->mLift)) {
		multiplier = 1.135f;
	} else {
		multiplier = 1.0f;
	}
#if defined(VERSION_PAL)
	multiplier *= 60.0f / (f32)lbl_80660D4C;
#endif
	chr->CXObj::SetAnimRate(rate * multiplier, mask);
	for (s32 i = 0; lbl_80226130[i].mItem >= 0; i++) {
		XObjChrItemAnim* entry = &lbl_80226130[i];
		if (entry->mItem == chr->mSuit) {
			HSD_JObj* joint = GetJointByPath(chr->mJObj, entry->mPath, entry->mDepth);
			if (GetAObj(joint) != NULL)
				chr->Push(joint, 0x200, 1.0f);
		}
	}
}

// XObjChr::vfF8, the shape anim for an id, 3000 up from the suit, 2000 up from the attachment,
// 1000 up from XObj's second archive
/**
 * @note Address: 0x800A388C
 * @note Size: 0x254
 */
extern "C" HSD_ShapeAnimJoint* fn_800A388C(XObjChr* chr, s32 id)
{
	if (SuitAnimGroup(chr, id)) {
		HSD_ShapeAnimJoint** anims = chr->mSuitAnim.mShapeAnimJoint;
		return anims != NULL ? anims[(id % 3000) % 1000] : NULL;
	}
	if (AtcAnimGroup(chr, id)) {
		HSD_ShapeAnimJoint** anims = chr->mAttachmentAnim.mShapeAnimJoint;
		return anims != NULL ? anims[(id % 2000) % 1000] : NULL;
	}
	if (AnimGroup(chr, id)) {
		HSD_ShapeAnimJoint** anims = chr->mAnim.mShapeAnimJoint;
		return anims != NULL ? anims[(id % 1000) % 1000] : NULL;
	}
	HSD_ShapeAnimJoint** anims = chr->mArchive.mShapeAnimJoint;
	return anims != NULL ? anims[id % 1000] : NULL;
}

// XObjChr::vfF4, the material anim
/**
 * @note Address: 0x800A3AE0
 * @note Size: 0x254
 */
extern "C" HSD_MatAnimJoint* fn_800A3AE0(XObjChr* chr, s32 id)
{
	if (SuitAnimGroup(chr, id)) {
		HSD_MatAnimJoint** anims = chr->mSuitAnim.mMatAnimJoint;
		return anims != NULL ? anims[(id % 3000) % 1000] : NULL;
	}
	if (AtcAnimGroup(chr, id)) {
		HSD_MatAnimJoint** anims = chr->mAttachmentAnim.mMatAnimJoint;
		return anims != NULL ? anims[(id % 2000) % 1000] : NULL;
	}
	if (AnimGroup(chr, id)) {
		HSD_MatAnimJoint** anims = chr->mAnim.mMatAnimJoint;
		return anims != NULL ? anims[(id % 1000) % 1000] : NULL;
	}
	HSD_MatAnimJoint** anims = chr->mArchive.mMatAnimJoint;
	return anims != NULL ? anims[id % 1000] : NULL;
}

// XObjChr::vfF0, the joint anim
/**
 * @note Address: 0x800A3D34
 * @note Size: 0x254
 */
extern "C" HSD_AnimJoint* fn_800A3D34(XObjChr* chr, s32 id)
{
	if (SuitAnimGroup(chr, id)) {
		HSD_AnimJoint** anims = chr->mSuitAnim.mAnimJoint;
		return anims != NULL ? anims[(id % 3000) % 1000] : NULL;
	}
	if (AtcAnimGroup(chr, id)) {
		HSD_AnimJoint** anims = chr->mAttachmentAnim.mAnimJoint;
		return anims != NULL ? anims[(id % 2000) % 1000] : NULL;
	}
	if (AnimGroup(chr, id)) {
		HSD_AnimJoint** anims = chr->mAnim.mAnimJoint;
		return anims != NULL ? anims[(id % 1000) % 1000] : NULL;
	}
	HSD_AnimJoint** anims = chr->mArchive.mAnimJoint;
	return anims != NULL ? anims[id % 1000] : NULL;
}

// the anim rate SetAnimRate would give him
/**
 * @note Address: 0x800A4574
 * @note Size: 0x44
 */
extern "C" f32 fn_800A4574(XObjChr* chr)
{
	f32 rate;
	if (chr->mLift.mState == 4 && fn_800D4FE8(&chr->mLift)) {
		rate = 1.135f;
	} else {
		rate = 1.0f;
	}
#if defined(VERSION_PAL)
	return rate * (60.0f / (f32)lbl_80660D4C);
#else
	return rate;
#endif
}

// XObjChr::vfD0, his attachments cast shadows too, the plug only under game flag 0x0040000000000000
/**
 * @note Address: 0x800A45B8
 * @note Size: 0xC0
 */
extern "C" void fn_800A45B8(XObjChr* chr, CShadow* shadow)
{
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mJObj);
	if (lbl_8025C400.mStatus.mFlags & 0x0040000000000000ULL) {
		HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mPlug.mJObj);
	}
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mCopterObj.mJObj);
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mPopperObj.mJObj);
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mRadarObj.mJObj);
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mOXObj.mJObj);
}

// XObjChr::UpdateAnim
/**
 * @note Address: 0x800A54FC
 * @note Size: 0x20
 */
extern "C" void fn_800A54FC(XObjChr* chr, u32 frames)
{
	chr->CXObjIntp::UpdateAnim(frames);
}

// XObjChr::vfB8, nothing up close to the camera
/**
 * @note Address: 0x800A77D4
 * @note Size: 0xC0
 */
extern "C" void fn_800A77D4(XObjChr* player)
{
	CMotionControlCamera* camera = &lbl_8025C400.mMotionControlCamera;
	if (camera->GetMapDistance() < 1.0f)
		return;
	if (player->mStatus.mFlags & 4ULL) {
		if (lbl_8025C400.mStatus.mFlags & 0x0040000000000000ULL) {
			XObj* plug = &player->mPlug;
			plug->vfB8();
		}
	} else {
		fn_80090864(player);
	}
}

// XObjChr::vfB4
/**
 * @note Address: 0x800A7894
 * @note Size: 0x198
 */
extern "C" void fn_800A7894(XObjChr* player, Mtx view)
{
	CMotionControlCamera* camera = &lbl_8025C400.mMotionControlCamera;
	if (camera->GetMapDistance() < 1.0f)
		return;
	if (lbl_8025C400.mStatus.mFlags & 0x100ULL)
		fn_800A30EC(player, 0);
	player->vfB0(view);
	if (lbl_8025C400.mStatus.mFlags & 0x100ULL)
		fn_800A30EC(player, 1);
	if (lbl_8025C400.mStatus.mFlags & 0x0040000000000000ULL)
		player->mPlug.vfB4(view);
	player->mCopterObj.vfB4(view);
	player->mPopperObj.vfB4(view);
	player->mRadarObj.vfB4(view);
	player->mOXObj.vfB4(view);
}

// XObjChr::vfB0, himself then the attachments, the popper turned to face the camera on flag 0x100
/**
 * @note Address: 0x800A7A2C
 * @note Size: 0x218
 */
extern "C" void fn_800A7A2C(XObjChr* player, Mtx view)
{
	CMotionControlCamera* camera = &lbl_8025C400.mMotionControlCamera;
	if (camera->GetMapDistance() < 1.0f)
		return;
	fn_800908C0(player, view);
	if ((lbl_8025C400.mStatus.mFlags & 0x100ULL) && player->mLift.mState == 4 && fn_800D4FE8(&player->mLift)) {
		// no plug in this lift state
	} else if (lbl_8025C400.mStatus.mFlags & 0x0040000000000000ULL) {
		player->mPlug.vfB0(view);
	}
	player->mCopterObj.vfB0(view);
	if (lbl_8025C400.mStatus.mFlags & 0x100ULL) {
		Mtx rotation, combined;
		PSMTXRotRad(rotation, 'x', -camera->mCamera.mAngle.x);
		if (view != NULL) {
			PSMTXConcat(rotation, view, combined);
		} else {
			CCamera* current = ((CBase*)&lbl_8025C400)->GetCamera();
			PSMTXConcat(rotation, current->mViewMtx, combined);
		}
		player->mPopperObj.vfB0(combined);
	} else {
		player->mPopperObj.vfB0(view);
	}
	player->mRadarObj.vfB0(view);
	player->mOXObj.vfB0(view);
}

// XObjAttachment::vfB0
/**
 * @note Address: 0x800A7C44
 * @note Size: 0x2C
 */
extern "C" void fn_800A7C44(XObjAttachment* object, Mtx view)
{
	if (object->mVisible != 0)
		fn_800908C0(object, view);
}

// steps the anim frame and fires the particles the frame tables ask for
/**
 * @note Address: 0x800A7C70
 * @note Size: 0x1AC
 */
extern "C" void fn_800A7C70(XObjChr* player)
{
	player->_94FC                = player->_94F8;
	HSD_AObj* aobj               = GetAObj(player->mJObj);
	player->_94F8                = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
	PlayerFrameParticle particle = fn_800A7E1C(player, &player->_94F8, lbl_80226324);
	if (particle.mCode >= 0 && !(lbl_8025C400.mStatus.mFlags & 0x100ULL)) {
		(&lbl_8025C400.mParticle)
		    ->CreateSRT((u32)particle.mCode | 0x10000, 1, particle.mTranslate, particle.mScale, particle.mRotate, FALSE);
	}
	if (player->mCopterObj.mVisible != 0)
		fn_800A7E1C(&player->mCopterObj, NULL, lbl_802276D4);
}

// stop once the sound is playing
/**
 * @note Address: 0x800A8454
 * @note Size: 0x40
 */
extern "C" s32 fn_800A8454(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	return fn_8001E6C8(&lbl_8025C400.mSound, event->mId) ? -1 : 0;
}

// start while the sound isn't playing
/**
 * @note Address: 0x800A8494
 * @note Size: 0x3C
 */
extern "C" bool fn_800A8494(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	return !fn_8001E6C8(&lbl_8025C400.mSound, event->mId);
}

// keep the sound playing inside the window only
/**
 * @note Address: 0x800A84D0
 * @note Size: 0x90
 */
extern "C" s32 fn_800A84D0(const u32* aobjFlags, FrameEventTime* time, const FrameEvent* event)
{
	if (time->mFrame >= event->mStart && time->mFrame < event->mEnd) {
		if (!fn_8001E6C8(&lbl_8025C400.mSound, event->mId))
			return 1;
	} else {
		if (fn_8001E6C8(&lbl_8025C400.mSound, event->mId))
			return -1;
	}
	return 0;
}

// keep the sound playing until the anim ends
/**
 * @note Address: 0x800A8560
 * @note Size: 0x7C
 */
extern "C" s32 fn_800A8560(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	if (*aobjFlags & 0x44000000) {
		if (fn_8001E6C8(&lbl_8025C400.mSound, event->mId))
			return -1;
	} else {
		if (!fn_8001E6C8(&lbl_8025C400.mSound, event->mId))
			return 1;
	}
	return 0;
}

// fires on the frame it crosses mStart or mEnd, a loop back counts as crossing
/**
 * @note Address: 0x800A85DC
 * @note Size: 0x64
 */
extern "C" s32 fn_800A85DC(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	if (((time->mLastFrame < event->mStart || time->mLastFrame > time->mFrame) && time->mFrame >= event->mStart)
	    || ((time->mLastFrame < event->mEnd || time->mLastFrame > time->mFrame) && time->mFrame >= event->mEnd))
		return 1;
	return 0;
}

// fires on the frame it crosses mStart
/**
 * @note Address: 0x800A8640
 * @note Size: 0x3C
 */
extern "C" s32 fn_800A8640(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	if ((time->mLastFrame < event->mStart || time->mLastFrame > time->mFrame) && time->mFrame >= event->mStart)
		return 1;
	return 0;
}

// loops the four idle joints' own anims, restarting them together after a random pause
/**
 * @note Address: 0x800A9DBC
 * @note Size: 0x2B4
 */
extern "C" void fn_800A9DBC(XObjChr* chr, u32 frames)
{
	XObjChrIdleJoint* joints = chr->_9500;
	if (joints[0].mJObj != NULL && joints[1].mJObj != NULL && joints[2].mJObj != NULL && joints[3].mJObj != NULL) {
		if (chr->_9530 > 0) {
			if ((chr->_9530 -= frames) < 0)
				chr->_9530 = 0;
		}
		u8* heads[4];
		for (int i = 0; i < 4; i++)
			heads[i] = AnimationHead(joints[i].mJObj);
		if (heads[0] == joints[0]._08 && heads[1] == joints[1]._08 && heads[2] == joints[2]._08 && heads[3] == joints[3]._08) {
			s32 started = 0;
			for (int i = 0; i < 4; i++) {
				if (chr->_9530 == 0) {
					if (joints[i].mJObj->aobj == NULL) {
						HSD_JObjAddAnim(joints[i].mJObj, joints[i].mAnimJoint, NULL, NULL);
						HSD_JObjReqAnim(joints[i].mJObj, 0.0f);
						started++;
						joints[i]._08 = joints[i].mJObj->aobj->fobj->ad_head;
					} else if (HSD_AObjGetFlags(joints[i].mJObj->aobj) & 0x44000000) {
						HSD_JObjReqAnim(joints[i].mJObj, 0.0f);
						started++;
					}
				}
				chr->Push(joints[i].mJObj, 0x200, 1.0f);
			}
			if (started > 0) {
#if defined(VERSION_PAL)
				s32 fps    = lbl_80660D4C;
				chr->_9530 = 0.5f * fps + rand() % fps;
#else
				chr->_9530 = 30.0f + rand() % 60;
#endif
			}
		} else {
			for (int i = 0; i < 4; i++)
				joints[i]._08 = NULL;
		}
	}
}

/**
 * @note Address: 0x800B1B0C
 * @note Size: 0x8
 */
extern "C" void fn_800B1B0C(XActPush* act, s32 value)
{
	act->_24 = value;
}

/**
 * @note Address: 0x800B1B14
 * @note Size: 0x8
 */
extern "C" void fn_800B1B14(XActPush* act, s32 state)
{
	act->mState = state;
}

/**
 * @note Address: 0x800B1B1C
 * @note Size: 0x8
 */
extern "C" void fn_800B1B1C(XActHang* act, s32 state)
{
	act->mState = state;
}

/**
 * @note Address: 0x800B1B24
 * @note Size: 0x4C
 */
extern "C" BOOL fn_800B1B24(const XObjChr* chr)
{
	return chr->mLift.mState == 4 && fn_800D4FE8((XActLift*)&chr->mLift);
}

/**
 * @note Address: 0x800B1B70
 * @note Size: 0x14
 */
extern "C" void fn_800B1B70(XAction* action, XObj* target)
{
	action->SetTarget(target);
}

/**
 * @note Address: 0x800B1B84
 * @note Size: 0x48
 */
extern "C" f32 fn_800B1B84(void*)
{
	return (f32)(s32)fn_8006E9AC(2) / 100.0f;
}

/**
 * @note Address: 0x800B1BCC
 * @note Size: 0x48
 */
extern "C" f32 fn_800B1BCC(void*)
{
	return (f32)(s32)fn_8006E9AC(1) / 100.0f;
}

/**
 * @note Address: 0x800B1C14
 * @note Size: 0x48
 */
extern "C" f32 fn_800B1C14(void*)
{
	return (f32)(s32)fn_8006E9AC(0) / 100.0f;
}

/**
 * @note Address: 0x800B1C5C
 * @note Size: 0xF4
 */
extern "C" f32 fn_800B1C5C(const CPadOne* pad)
{
	f32 x = pad->mStickX;
	f32 y = pad->mStickY;
	return sqrtf(x * x + y * y);
}

/**
 * @note Address: 0x800B1D50
 * @note Size: 0x34
 */
extern "C" bool fn_800B1D50(XObj* object)
{
	return (u8)((object->mStatus.mFlags & 0x0000000400000000ULL) != 0);
}

/**
 * @note Address: 0x800B1D84
 * @note Size: 0x8
 */
extern "C" CVec* fn_800B1D84(CCamera* camera)
{
	return &camera->mAngle;
}

/**
 * @note Address: 0x800B1D8C
 * @note Size: 0x50
 */
extern "C" void fn_800B1D8C(XObjChrLook* look, f32 x, f32 y, f32 z)
{
	Vec target    = { 0.0f, 0.0f, 0.0f };
	target.x      = x;
	target.y      = y;
	target.z      = z;
	look->mTarget = target;
}

/**
 * @note Address: 0x800B1DDC
 * @note Size: 0x8
 */
extern "C" void fn_800B1DDC(XObjChrLook* look, s32 mode)
{
	look->mMode = mode;
}

/**
 * @note Address: 0x800B1DE4
 * @note Size: 0xC
 */
extern "C" void fn_800B1DE4(XObjChrLook* look)
{
	look->mActive = TRUE;
}

/**
 * @note Address: 0x800B1DF0
 * @note Size: 0xC
 */
extern "C" u32 fn_800B1DF0(XObj* object, u32 mask)
{
	return object->_8B0 & mask;
}

/**
 * @note Address: 0x800B1DFC
 * @note Size: 0x8
 */
extern "C" u32 fn_800B1DFC(XObj* object)
{
	return object->_8B0;
}

/**
 * @note Address: 0x800B1E04
 * @note Size: 0x8
 */
extern "C" s32 fn_800B1E04(void*)
{
	return 0;
}

/**
 * @note Address: 0x800B1E0C
 * @note Size: 0xAC
 */
// is position behind the object, within 60 degrees either side
extern "C" BOOL fn_800B1E0C(void*, const Vec* pos, CXObj* object)
{
	f32 angle = atan2(object->mMoveTranslate.x - pos->x, object->mMoveTranslate.z - pos->z);
	angle     = WrapAngle(angle - object->mMoveRotate.y);
	return angle >= 2.0f * PI / 3.0f && angle <= 4.0f * PI / 3.0f;
}

/**
 * @note Address: 0x800B1EB8
 * @note Size: 0x8
 */
extern "C" f32 fn_800B1EB8(XObj* object)
{
	return object->_77C;
}

/**
 * @note Address: 0x800B1EC0
 * @note Size: 0x144
 */
extern "C" f32 fn_800B1EC0(const XObj* object, Vec* point, s32 index)
{
	Vec first  = FirstEdgePoint(object, index, point->y);
	Vec second = SecondEdgePoint(object, index, point->y);
	CVec direction;
	fn_8018E824(point, &first, &second, &direction);
	f32 angle = atan2(direction.x, direction.z);
	angle     = WrapAngle(angle);
	return angle;
}

/**
 * @note Address: 0x800B2004
 * @note Size: 0x8
 */
extern "C" s32 fn_800B2004(const XObj* object)
{
	return object->_784;
}

/**
 * @note Address: 0x800B200C
 * @note Size: 0x8
 */
extern "C" bool fn_800B200C()
{
	return false;
}

/**
 * @note Address: 0x800B2014
 * @note Size: 0x34
 */
extern "C" bool fn_800B2014(const XObj* object)
{
	return TestStatus(object, 0x0000008000000000ULL);
}

/**
 * @note Address: 0x800B2048
 * @note Size: 0x34
 */
extern "C" bool fn_800B2048(const XObj* object)
{
	return TestStatus(object, 0x0000000100000000ULL);
}

/**
 * @note Address: 0x800B207C
 * @note Size: 0xC
 */
extern "C" void fn_800B207C(XObjChrLook* look)
{
	look->mActive = FALSE;
}

/**
 * @note Address: 0x800B2088
 * @note Size: 0x18
 */
extern "C" bool fn_800B2088(const CObjVec* value)
{
	return value->mSpeed.y > 0.0f;
}

/**
 * @note Address: 0x800B20A0
 * @note Size: 0x14
 */
extern "C" s32 fn_800B20A0(const CGame* game, s32 index)
{
	return game->mXObjs.mPools[index].mUsed;
}

/**
 * @note Address: 0x800B20B4
 * @note Size: 0x10
 */
extern "C" UnkStageObj* fn_800B20B4(const CoordInfo* info, s32 index)
{
	return info->_00 + index;
}

/**
 * @note Address: 0x800B20C4
 * @note Size: 0x8
 */
extern "C" CoordInfo* fn_800B20C4(XObj* object)
{
	return &object->mCoord;
}

/**
 * @note Address: 0x800B20CC
 * @note Size: 0x3C
 */
extern "C" f32 fn_800B20CC(const Vec* from, const Vec* to)
{
	return atan2(to->x - from->x, to->z - from->z);
}

/**
 * @note Address: 0x800B2108
 * @note Size: 0x20
 */
extern "C" void* fn_800B2108(const CGame* game, s32 pool, s32 index)
{
	return game->mXObjs.mPools[pool].mObjects + index * game->mXObjs.mPools[pool].mStride;
}

/**
 * @note Address: 0x800B2128
 * @note Size: 0xC
 */
extern "C" s32 fn_800B2128(const CGame* game)
{
	return game->mXObjs.mObjectCount;
}

/**
 * @note Address: 0x800B2134
 * @note Size: 0x8
 */
extern "C" f32 fn_800B2134(const XObj* object)
{
	return object->_780;
}

/**
 * @note Address: 0x800B213C
 * @note Size: 0x8
 */
extern "C" void fn_800B213C(XAction* action, s32 state)
{
	action->mState = state;
}

/**
 * @note Address: 0x800B2144
 * @note Size: 0x20
 */
extern "C" void fn_800B2144(XObjChr* chr, s32 state, XObj* target)
{
	chr->mRope.SetState(state, target);
}

/**
 * @note Address: 0x800B2164
 * @note Size: 0x34
 */
extern "C" bool fn_800B2164(const XObj* object)
{
	return TestStatus(object, 0x0000001000000000ULL);
}

/**
 * @note Address: 0x800B2198
 * @note Size: 0x20
 */
extern "C" void fn_800B2198(XObjChr* chr, s32 state, XObj* target)
{
	chr->mLadder.SetState(state, target);
}

/**
 * @note Address: 0x800B21B8
 * @note Size: 0x34
 */
extern "C" bool fn_800B21B8(const XObj* object)
{
	return TestStatus(object, 0x0000000800000000ULL);
}

/**
 * @note Address: 0x800B21EC
 * @note Size: 0x44
 */
extern "C" void fn_800B21EC(XObjChr* chr)
{
	XObj* target = chr->mLift.mTarget;
	if (target != NULL) {
		target->mStatus.mFlags &= ~0x0400000000000000ULL;
		chr->mLift.mTarget = NULL;
	}
	chr->mLift.mState = 0;
}

/**
 * @note Address: 0x800B2230
 * @note Size: 0x20
 */
extern "C" void fn_800B2230(XObjChr* chr, s32 state, XObj* target)
{
	chr->mHang.SetState(state, target);
}

/**
 * @note Address: 0x800B2250
 * @note Size: 0x34
 */
extern "C" bool fn_800B2250(const XObj* object)
{
	return TestStatus(object, 0x0000000200000000ULL);
}

/**
 * @note Address: 0x800B2284
 * @note Size: 0x10
 */
extern "C" s32 fn_800B2284(const XObj* object, s32 index)
{
	const s32* entries = &object->_25C;
	return entries[index];
}

/**
 * @note Address: 0x800B2294
 * @note Size: 0x34
 */
extern "C" bool fn_800B2294(const XObj* object)
{
	return TestStatus(object, 0x0000002000000000ULL);
}

/**
 * @note Address: 0x800B22C8
 * @note Size: 0x8
 */
extern "C" void fn_800B22C8(XAction* action, s32 state)
{
	action->mState = state;
}

/**
 * @note Address: 0x800B22D0
 * @note Size: 0x28
 */
extern "C" u8 fn_800B22D0(const XObjChrMenu* menu)
{
	return menu->IsOpen();
}

/**
 * @note Address: 0x800B22F8
 * @note Size: 0x48
 */
extern "C" f32 fn_800B22F8()
{
	return (s32)fn_8006EAF0(2) / 100.0f;
}

/**
 * @note Address: 0x800B2340
 * @note Size: 0x48
 */
extern "C" f32 fn_800B2340()
{
	return (s32)fn_8006EAF0(1) / 100.0f;
}

/**
 * @note Address: 0x800B2388
 * @note Size: 0x8
 */
extern "C" void fn_800B2388(CObjVec* value, f32 y)
{
	value->mTarget.y = y;
}

/**
 * @note Address: 0x800B2390
 * @note Size: 0x8
 */
extern "C" bool fn_800B2390()
{
	return true;
}

/**
 * @note Address: 0x800B2398
 * @note Size: 0xE4
 */
extern "C" f32 fn_800B2398(f32 value)
{
	return sqrtf(value);
}

/**
 * @note Address: 0x800B247C
 * @note Size: 0x40
 */
extern "C" f32 fn_800B247C(XObjFloor* const* floors, s32 index)
{
	return DEG2RAD * (*floors)[index]._10;
}

/**
 * @note Address: 0x800B24BC
 * @note Size: 0x8
 */
extern "C" s32 fn_800B24BC(const XAction* action)
{
	return action->mTargetFloor;
}

/**
 * @note Address: 0x800B24C4
 * @note Size: 0x8
 */
extern "C" XObjFloor** fn_800B24C4(XObj* object)
{
	return &object->mFloors;
}

/**
 * @note Address: 0x800B24CC
 * @note Size: 0x8
 */
extern "C" CObjVec* fn_800B24CC(CXObj* object)
{
	return &object->mMoveScale;
}

/**
 * @note Address: 0x800B24D4
 * @note Size: 0x8
 */
extern "C" CObjVec* fn_800B24D4(CXObj* object)
{
	return &object->mMoveRotate;
}

/**
 * @note Address: 0x800B24DC
 * @note Size: 0x28
 */
extern "C" s32 fn_800B24DC(const XObj* object)
{
	return object->GetPool();
}

/**
 * @note Address: 0x800B2504
 * @note Size: 0x14
 */
extern "C" s32 fn_800B2504(const CGame* game, s32 list)
{
	return game->mDrawLists[list].mCount;
}

/**
 * @note Address: 0x800B2518
 * @note Size: 0x24
 */
extern "C" void fn_800B2518(XObj* object, u64 mask)
{
	object->mStatus.mFlags &= ~mask;
}

/**
 * @note Address: 0x800B253C
 * @note Size: 0xC
 */
extern "C" u32 fn_800B253C(const XObjChr* chr, u32 mask)
{
	return chr->_980.mFlags & mask;
}

/**
 * @note Address: 0x800B2548
 * @note Size: 0x1C
 */
extern "C" s32 fn_800B2548(const CGame* game, s32 list, s32 index)
{
	return game->mDrawLists[list].mIndices[index];
}

/**
 * @note Address: 0x800B2564
 * @note Size: 0x1C
 */
extern "C" void fn_800B2564(XObj* object, u64 mask)
{
	object->mStatus.mFlags |= mask;
}

/**
 * @note Address: 0x800B2580
 * @note Size: 0x14
 */
extern "C" f32 fn_800B2580(const XObj* object)
{
	return object->_6FC[object->_274];
}

/**
 * @note Address: 0x800B2594
 * @note Size: 0x8
 */
extern "C" f32 fn_800B2594(const XObj* object)
{
	return object->_6F8;
}

/**
 * @note Address: 0x800B259C
 * @note Size: 0x104
 */
extern "C" f32 fn_800B259C(const Vec* a, const Vec* b)
{
	f32 z = a->z - b->z;
	f32 x = a->x - b->x;
	return sqrtf(x * x + z * z);
}

/**
 * @note Address: 0x800B26A0
 * @note Size: 0x14
 */
extern "C" BOOL fn_800B26A0(const XAction* action, s32 wanted)
{
	return action->_0C == wanted;
}

/**
 * @note Address: 0x800B26B4
 * @note Size: 0x8
 */
extern "C" u32 fn_800B26B4(const CXObj* object)
{
	return object->mId;
}

/**
 * @note Address: 0x800B26BC
 * @note Size: 0x14
 */
extern "C" u64 fn_800B26BC(const XObj* object, u64 mask)
{
	return object->mStatus.mFlags & mask;
}

/**
 * @note Address: 0x800B26D0
 * @note Size: 0x1C
 */
extern "C" XObj* fn_800B26D0(const CGame* game, s32 list, s32 index)
{
	return game->mDrawLists[list].mObjects[index];
}

/**
 * @note Address: 0x800B26EC
 * @note Size: 0x78
 */
extern "C" void fn_800B26EC(CJObj*, HSD_JObj* jobj, Vec* scale)
{
	HSD_JObjGetScale(jobj, scale);
}

/**
 * @note Address: 0x800B2764
 * @note Size: 0x7C
 */
extern "C" HSD_JObj* fn_800B2764(const CJObj* object, s32 depth, s32* path)
{
	return GetJointByPath(object->mJObj, path, depth);
}

/**
 * @note Address: 0x800B27E0
 * @note Size: 0xBC
 */
extern "C" Vec fn_800B27E0(XObjFloor* const* floors, s32 index)
{
	Vec value = lbl_8037827C;
	value.x   = (*floors)[index]._04[0] / 100.0f;
	value.y   = (*floors)[index]._04[1] / 100.0f;
	value.z   = (*floors)[index]._04[2] / 100.0f;
	return value;
}

/**
 * @note Address: 0x800B289C
 * @note Size: 0xBC
 */
extern "C" Vec fn_800B289C(XObjFloor* const* floors, s32 index)
{
	Vec value = lbl_80378288;
	value.x   = (*floors)[index]._14[0] / 100.0f;
	value.y   = (*floors)[index]._14[1] / 100.0f;
	value.z   = (*floors)[index]._14[2] / 100.0f;
	return value;
}

/**
 * @note Address: 0x800B2958
 * @note Size: 0x1C
 */
extern "C" void fn_800B2958(CObjVec* value, const Vec& target)
{
	value->mTarget.Set(target);
}

/**
 * @note Address: 0x800B2974
 * @note Size: 0x10
 */
extern "C" void fn_800B2974(CVec* value, f32 x, f32 y, f32 z)
{
	value->Set(x, y, z);
}

/**
 * @note Address: 0x800B2984
 * @note Size: 0x10
 */
extern "C" void fn_800B2984(CObjVec* value, f32 x, f32 y, f32 z)
{
	value->mSpeed.Set(x, y, z);
}

/**
 * @note Address: 0x800B2994
 * @note Size: 0x24
 */
extern "C" void fn_800B2994(Vec* value, f32 scale)
{
	PSVECScale(value, value, scale);
}

/**
 * @note Address: 0x800B29B8
 * @note Size: 0x8
 */
extern "C" bool fn_800B29B8()
{
	return true;
}

/**
 * @note Address: 0x800B29C0
 * @note Size: 0x8
 */
extern "C" bool fn_800B29C0()
{
	return true;
}

/**
 * @note Address: 0x800B29C8
 * @note Size: 0x2C
 */
extern "C" u8 fn_800B29C8(const XActCopter* act)
{
	return act->mState == 0 || act->mState == 6 || act->mState == 9;
}

/**
 * @note Address: 0x800B29F4
 * @note Size: 0x8
 */
extern "C" bool fn_800B29F4()
{
	return true;
}

/**
 * @note Address: 0x800B29FC
 * @note Size: 0x8
 */
extern "C" bool fn_800B29FC()
{
	return true;
}

/**
 * @note Address: 0x800B2A04
 * @note Size: 0x24
 */
extern "C" u8 fn_800B2A04(const XActRope* act)
{
	return act->mState == 0 || act->mState == 10;
}

/**
 * @note Address: 0x800B2A28
 * @note Size: 0x24
 */
extern "C" u8 fn_800B2A28(const XActLadder* act)
{
	return act->mState == 0 || act->mState == 11;
}

/**
 * @note Address: 0x800B2A4C
 * @note Size: 0x24
 */
extern "C" u8 fn_800B2A4C(const XActJump* act)
{
	return act->mState == 0 || act->mState == 4;
}

/**
 * @note Address: 0x800B2A70
 * @note Size: 0x10
 */
extern "C" bool fn_800B2A70(const XActHang* act)
{
	return (u8)(act->mState == 0);
}

/**
 * @note Address: 0x800B2A80
 * @note Size: 0x8
 */
extern "C" bool fn_800B2A80()
{
	return true;
}

/**
 * @note Address: 0x800B2A88
 * @note Size: 0x34
 */
extern "C" CVec* fn_800B2A88(CVec* value)
{
	PSVECNormalize(value, value);
	return value;
}

/**
 * @note Address: 0x800B2ABC
 * @note Size: 0xF4
 */
extern "C" BOOL fn_800B2ABC(const XObjMap* map, const Vec* pos, XObjMapHit* hit, Vec* tri)
{
	s32 start = FindFloorStart(map, 0.2f + pos->y);
	for (int i = 0; i < map->mTrigonCount; i++) {
		s32 index    = map->mTrigonOrder[(start + i) % map->mTrigonCount];
		int triangle = ((CTrigon*)&map->mTrigon[index])->Check((Vec*)pos, &hit->mHeight, tri);
		if (triangle >= 0) {
			hit->mTrigon   = index;
			hit->mTriangle = triangle;
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @note Address: 0x800B2BB0
 * @note Size: 0x20
 */
extern "C" void fn_800B2BB0(XObjChr* chr, s32 state, BOOL restart)
{
	chr->mTumble.mState = state;
	if (restart) {
		chr->mTumble.mFade.mActive = TRUE;
		chr->mTumble.mFade.mAmount = 0.0f;
	}
}

/**
 * @note Address: 0x800B2BD0
 * @note Size: 0x12C
 */
extern "C" void fn_800B2BD0(XObjPlug* plug, BOOL resetJoint)
{
	plug->_D3C = 0;
	if (resetJoint) {
		s32 path       = lbl_8065B260;
		HSD_JObj* jobj = GetJointByPath(plug->mJObj, &path, 1);
		HSD_JObjSetRotationX(jobj, 0.0f);
	}
}

/**
 * @note Address: 0x800B2CFC
 * @note Size: 0xC
 */
extern "C" XSound* fn_800B2CFC(CGame* game)
{
	return &game->mSound;
}

/**
 * @note Address: 0x800B2D08
 * @note Size: 0x24
 */
extern "C" u32 fn_800B2D08(const CGame* game, u32 mask)
{
	return CheckStageFlag(game->mStage, mask);
}

/**
 * @note Address: 0x800B2D2C
 * @note Size: 0x14
 */
extern "C" bool fn_800B2D2C(const XActRadar* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B2D40
 * @note Size: 0x14
 */
extern "C" bool fn_800B2D40(const XActPopper* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B2D54
 * @note Size: 0x8
 */
extern "C" CVec* fn_800B2D54(XObjPlug* plug)
{
	return &plug->mCord._6C;
}

/**
 * @note Address: 0x800B2D5C
 * @note Size: 0x8
 */
extern "C" bool fn_800B2D5C()
{
	return true;
}

/**
 * @note Address: 0x800B2D64
 * @note Size: 0x8
 */
extern "C" bool fn_800B2D64()
{
	return true;
}

/**
 * @note Address: 0x800B2D6C
 * @note Size: 0x54
 */
extern "C" u8 fn_800B2D6C(const XActCopter* act)
{
	s32 state = act->mState;
	return (state == 0 || state == 3 || state == 4 || state == 7 || state == 8 || state == 5 || state == 6 || state == 9);
}

/**
 * @note Address: 0x800B2DC0
 * @note Size: 0x24
 */
extern "C" u8 fn_800B2DC0(const XActTumble* act)
{
	return (act->mState != 8 && act->mState != 9);
}

/**
 * @note Address: 0x800B2DE4
 * @note Size: 0x34
 */
extern "C" u8 fn_800B2DE4(const XActRope* act)
{
	s32 state = act->mState;
	return (state == 0 || state == 9 || state == 10 || state == 11);
}

/**
 * @note Address: 0x800B2E18
 * @note Size: 0x2C
 */
extern "C" u8 fn_800B2E18(const XActLadder* act)
{
	s32 state = act->mState;
	return (state == 0 || state == 10 || state == 11);
}

/**
 * @note Address: 0x800B2E44
 * @note Size: 0x3C
 */
extern "C" u8 fn_800B2E44(const XActJump* act)
{
	s32 state = act->mState;
	return (state == 0 || state == 2 || state == 3 || state == 4 || state == 17);
}

/**
 * @note Address: 0x800B2E80
 * @note Size: 0x24
 */
extern "C" u8 fn_800B2E80(const XActHang* act)
{
	return (act->mState == 0 || act->mState == 10);
}

/**
 * @note Address: 0x800B2EA4
 * @note Size: 0xC
 */
extern "C" s32 fn_800B2EA4(const CGame* game)
{
	return game->mMenu.mActive;
}

/**
 * @note Address: 0x800B2EB0
 * @note Size: 0x8
 */
extern "C" u32 fn_800B2EB0(const XObjPlug* plug)
{
	return plug->_D3C;
}

/**
 * @note Address: 0x800B2EB8
 * @note Size: 0x18
 */
extern "C" void* fn_800B2EB8(const CGame* game, s32 index)
{
	s32 stride  = game->mXObjs.mPools[3].mStride;
	u8* entries = game->mXObjs.mPools[3].mObjects;
	return entries + index * stride;
}

/**
 * @note Address: 0x800B2ED0
 * @note Size: 0x8
 */
extern "C" void fn_800B2ED0(CObjVec* value, f32 y)
{
	value->mSpeed.y = y;
}

/**
 * @note Address: 0x800B2ED8
 * @note Size: 0x28
 */
extern "C" void fn_800B2ED8(CObjVec* value, const Vec& v)
{
	f32 x = v.x, y = v.y, z = v.z;
	value->x         = x;
	value->y         = y;
	value->z         = z;
	value->mTarget.x = x;
	value->mTarget.y = y;
	value->mTarget.z = z;
}

/**
 * @note Address: 0x800B2F00
 * @note Size: 0xC
 */
extern "C" u32 fn_800B2F00(const CPadOne* pad, u32 mask)
{
	return pad->mButton & mask;
}

/**
 * @note Address: 0x800B2F0C
 * @note Size: 0x14
 */
extern "C" CPadOne* fn_800B2F0C(CGame* game, u8 port)
{
	CPadOne* pads = game->mPad.mPad;
	return &pads[port];
}

/**
 * @note Address: 0x800B2F20
 * @note Size: 0xC
 */
extern "C" void fn_800B2F20(CObjVec* value, f32 z)
{
	value->z         = z;
	value->mTarget.z = z;
}

/**
 * @note Address: 0x800B2F2C
 * @note Size: 0xC
 */
extern "C" void fn_800B2F2C(CObjVec* value, f32 x)
{
	value->x         = x;
	value->mTarget.x = x;
}

/**
 * @note Address: 0x800B2F38
 * @note Size: 0x14
 */
extern "C" bool fn_800B2F38(const XActLift* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B2F4C
 * @note Size: 0x28
 */
extern "C" bool fn_800B2F4C(const XObjChr* chr, s32 wanted)
{
	return (u8)(wanted == chr->GetAttachment());
}

/**
 * @note Address: 0x800B2F74
 * @note Size: 0x18
 */
extern "C" bool fn_800B2F74(const CGame* game)
{
	return game->mEvt.mData.mScript != NULL;
}

/**
 * @note Address: 0x800B2F8C
 * @note Size: 0x14
 */
extern "C" bool fn_800B2F8C(const XActTumble* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B2FA0
 * @note Size: 0x20
 */
extern "C" f32 fn_800B2FA0(const Vec* a, const Vec* b)
{
	return PSVECDistance((Vec*)a, (Vec*)b);
}

/**
 * @note Address: 0x800B2FC0
 * @note Size: 0x4
 */
extern "C" void fn_800B2FC0()
{
}

/**
 * @note Address: 0x800B2FC4
 * @note Size: 0x14
 */
extern "C" u64 fn_800B2FC4(const CGame* game, u64 mask)
{
	return game->mStatus.mFlags & mask;
}

/**
 * @note Address: 0x800B2FD8
 * @note Size: 0xC
 */
extern "C" void fn_800B2FD8(CObjVec* value, f32 y)
{
	value->y         = y;
	value->mTarget.y = y;
}

/**
 * @note Address: 0x800B2FE4
 * @note Size: 0x8
 */
extern "C" s32 fn_800B2FE4(const CXObj* object)
{
	return object->mAnimId;
}

/**
 * @note Address: 0x800B2FEC
 * @note Size: 0x1C
 */
extern "C" void fn_800B2FEC(CObjVec* value, f32 x, f32 y, f32 z)
{
	value->x         = x;
	value->y         = y;
	value->z         = z;
	value->mTarget.x = x;
	value->mTarget.y = y;
	value->mTarget.z = z;
}

/**
 * @note Address: 0x800B3008
 * @note Size: 0x8
 */
extern "C" void fn_800B3008(CVec* value, f32 z)
{
	value->z = z;
}

/**
 * @note Address: 0x800B3010
 * @note Size: 0x8
 */
extern "C" void fn_800B3010(CVec* value, f32 x)
{
	value->x = x;
}

/**
 * @note Address: 0x800B3018
 * @note Size: 0x34
 */
extern "C" f32 fn_800B3018(f32 angle)
{
	angle = WrapAngle(angle);
	return angle;
}

/**
 * @note Address: 0x800B304C
 * @note Size: 0x8
 */
extern "C" u32 fn_800B304C(const XAction* action)
{
	return action->_0C;
}

/**
 * @note Address: 0x800B3054
 * @note Size: 0x8
 */
extern "C" f32 fn_800B3054(const XActHang* act)
{
	return act->_48;
}

/**
 * @note Address: 0x800B305C
 * @note Size: 0x8
 */
extern "C" Vec* fn_800B305C(XActHang* act)
{
	return &act->_3C;
}

/**
 * @note Address: 0x800B3064
 * @note Size: 0x24
 */
extern "C" bool fn_800B3064(const XAction* action, s32 wanted)
{
	return (u8)(action->mTarget->mFloors[action->mTargetFloor].mId == wanted);
}

/**
 * @note Address: 0x800B3088
 * @note Size: 0x8
 */
extern "C" void fn_800B3088(CObjVec* value, f32 z)
{
	value->mSpeed.z = z;
}

/**
 * @note Address: 0x800B3090
 * @note Size: 0x8
 */
extern "C" void fn_800B3090(CObjVec* value, f32 x)
{
	value->mSpeed.x = x;
}

/**
 * @note Address: 0x800B3098
 * @note Size: 0x8
 */
extern "C" CVec* fn_800B3098(CObjVec* value)
{
	return &value->mSpeed;
}

/**
 * @note Address: 0x800B30A0
 * @note Size: 0x20
 */
extern "C" f32 fn_800B30A0(const Vec* value)
{
	return PSVECMag((Vec*)value);
}

/**
 * @note Address: 0x800B30C0
 * @note Size: 0x8
 */
extern "C" CVec* fn_800B30C0(CObjVec* value)
{
	return &value->mTarget;
}

/**
 * @note Address: 0x800B30C8
 * @note Size: 0x8
 */
extern "C" void fn_800B30C8(CObjVec* value, f32 z)
{
	value->mTarget.z = z;
}

/**
 * @note Address: 0x800B30D0
 * @note Size: 0x8
 */
extern "C" void fn_800B30D0(CObjVec* value, f32 x)
{
	value->mTarget.x = x;
}

/**
 * @note Address: 0x800B30D8
 * @note Size: 0x24
 */
extern "C" f32 fn_800B30D8(f32 angle)
{
	return cos(angle);
}

/**
 * @note Address: 0x800B30FC
 * @note Size: 0x8
 */
extern "C" f32 fn_800B30FC(const XObjChr* chr)
{
	return chr->_A40;
}

/**
 * @note Address: 0x800B3104
 * @note Size: 0x24
 */
extern "C" f32 fn_800B3104(f32 angle)
{
	return sin(angle);
}

/**
 * @note Address: 0x800B3128
 * @note Size: 0x8
 */
extern "C" XObj* fn_800B3128(const XAction* action)
{
	return action->mTarget;
}

/**
 * @note Address: 0x800B3130
 * @note Size: 0x14
 */
extern "C" bool fn_800B3130(const XActHang* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B3144
 * @note Size: 0x8
 */
extern "C" CVec* fn_800B3144(XObj* object)
{
	return &object->_264;
}

/**
 * @note Address: 0x800B314C
 * @note Size: 0x8
 */
extern "C" CObjVec* fn_800B314C(CXObj* object)
{
	return &object->mMoveTranslate;
}

/**
 * @note Address: 0x800B3154
 * @note Size: 0x14
 */
extern "C" XObj* fn_800B3154(const CGame* game, s32 index)
{
	return game->mXObjs.mObjects[index];
}

/**
 * @note Address: 0x800B3168
 * @note Size: 0x14
 */
extern "C" bool fn_800B3168(const XAction* action)
{
	return action->mTarget != NULL;
}

/**
 * @note Address: 0x800B317C
 * @note Size: 0x14
 */
extern "C" bool fn_800B317C(const XActPush* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B3190
 * @note Size: 0x1C
 */
extern "C" CVec fn_800B3190(f32 x, f32 y, f32 z)
{
	return CVec(x, y, z);
}

/**
 * @note Address: 0x800B31AC
 * @note Size: 0xC
 */
extern "C" u32 fn_800B31AC(const XObjChr* chr, u32 mask)
{
	return chr->_980.mFlags & mask;
}

// switches to the attachment he asked for, he stands still meanwhile
/**
 * @note Address: 0x800B31B8
 * @note Size: 0x414
 */
extern "C" void fn_800B31B8(XObjChr* chr, f32* speed)
{
	if (chr->mAttachmentRequest >= 0) {
		switch (chr->_95D8) {
		case 0:
			if (IsLiftDone(chr)) {
				if (chr->mLift.mTarget != NULL) {
					chr->mLift.mTarget->mStatus.mFlags &= ~0x0400000000000000ULL;
					chr->mLift.mTarget = NULL;
				}
				chr->mLift.mState = 0;
			}
			switch (chr->mAttachment) {
			case 1:
				chr->mCopter.mState = 2;
				break;
			case 2:
				chr->mPopper.mState = 9;
				fn_800C17BC(&chr->mPopper.mCharge);
				break;
			case 3:
				chr->mRadar.mState = 2;
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				chr->mLift.mState = 19;
				break;
			default:
				if (fn_800D4F0C(&chr->mLift) || fn_800D4E30(&chr->mLift) || fn_800D4D54(&chr->mLift) || fn_800D4C78(&chr->mLift))
					chr->mLift.mState = 19;
				break;
			}
			chr->_95D8 = 10;
		case 10:
			if (chr->mCopter.mState != 0 || chr->mPopper.mState != 0 || chr->mRadar.mState != 0 || chr->mLift.mState != 0)
				break;
			if (chr->mAttachmentAnim.mLoading == 0) {
				chr->mAttachment = chr->mAttachmentRequest;
				chr->_95D8       = 15;
				chr->mAttachmentAnim.Release();
			}
		case 15: {
			BOOL done;
			if (sAttachmentAnims[chr->mAttachment] != NULL) {
				if (fn_8008A38C(&chr->mAttachmentAnim, sAttachmentAnims[chr->mAttachment])) {
					chr->SetAnim(17, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					done = TRUE;
				} else {
					done = FALSE;
				}
			} else {
				done = TRUE;
			}
			if (!done)
				break;
			chr->_95D8 = 20;
		}
		case 20:
			switch (chr->mAttachmentRequest) {
			case 1:
				chr->mCopter.mState = 1;
				break;
			case 2:
				chr->mPopper.mState = 1;
				break;
			case 3:
				chr->mRadar.mState = 1;
				break;
			case 4:
				TakeOutTool(chr, 0x10);
				break;
			case 5:
				TakeOutTool(chr, 0x15);
				break;
			case 6:
				TakeOutTool(chr, 0x16);
				break;
			case 7:
				TakeOutTool(chr, 0x38);
				break;
			case 8:
				chr->mAttachment = 0;
				break;
			}
			chr->_95D8 = 30;
			chr->_95DC = 0;
			break;
		case 30:
			if (chr->mCopter.mState == 0 && chr->mPopper.mState == 0 && chr->mRadar.mState == 0) {
				chr->mAttachmentRequest = -1;
				chr->_95D8              = 0;
				chr->_95DC              = 0;
				chr->_95E0              = chr->_95E4;
				chr->_95E4              = 0;
			}
			break;
		}
		*speed = 0.0f;
	}
}

/**
 * @note Address: 0x800B4F28
 * @note Size: 0x40
 */
extern "C" BOOL fn_800B4F28(const Vec* a, const Vec* b)
{
	BOOL different = FALSE;
	if (a->x != b->x || a->y != b->y || a->z != b->z)
		different = TRUE;
	return different;
}

/**
 * @note Address: 0x800B4F68
 * @note Size: 0xB4
 */
extern "C" Vec fn_800B4F68(void*, HSD_JObj* joint)
{
	Vec position;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(joint), &position);
	return position;
}

/**
 * @note Address: 0x800B501C
 * @note Size: 0x8
 */
extern "C" void fn_800B501C(XActRadar* act, s32 state)
{
	act->mState = state;
}

/**
 * @note Address: 0x800B5024
 * @note Size: 0x8
 */
extern "C" void fn_800B5024(XActPopper* act, s32 state)
{
	act->mState = state;
}

/**
 * @note Address: 0x800B502C
 * @note Size: 0x8
 */
extern "C" void fn_800B502C(XActRope* act, s32 state)
{
	act->mState = state;
}

/**
 * @note Address: 0x800B5034
 * @note Size: 0x8
 */
extern "C" void fn_800B5034(XActLadder* act, s32 state)
{
	act->mState = state;
}

/**
 * @note Address: 0x800B503C
 * @note Size: 0x8
 */
extern "C" void fn_800B503C(XActLift* act, s32 value)
{
	act->_24 = value;
}

/**
 * @note Address: 0x800B5044
 * @note Size: 0x8
 */
extern "C" void fn_800B5044(XActLift* act, s32 state)
{
	act->mState = state;
}

/**
 * @note Address: 0x800B504C
 * @note Size: 0x130
 */
extern "C" void fn_800B504C(CGame* game, const void* script, void* context, s16 first, s16 second)
{
	if (!IsEventBlocked(game->GetPlayer())) {
		fn_8007EEA4(&game->mEvt, (u8*)script, (u32)context, first, second);
	}
}

/**
 * @note Address: 0x800B517C
 * @note Size: 0x80
 */
extern "C" u8 fn_800B517C(CGame* game)
{
	return game->IsRunning();
}

/**
 * @note Address: 0x800B51FC
 * @note Size: 0x1A4
 */
// opens the yes/no marks
extern "C" void fn_800B51FC(XObjChr* chr, s32 first, s32 second)
{
	if (!chr->mOXMenu.mStarted) {
		chr->mOXMenu.mStarted  = TRUE;
		chr->mOXMenu._08       = first;
		chr->mOXMenu.mStartRot = chr->mOXMenu.mChr->mMoveRotate.y;
		chr->mOXMenu._0C       = second;
		SpinMarks(*chr);
		fn_8001E904(&lbl_8025C400.mSound, 0x86, 255);
		chr->mOXMenu.mChr->SetAnim(0x5D, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		lbl_8025C400.mStatus.mFlags |= 0x800ULL;
	}
}

/**
 * @note Address: 0x800B53A0
 * @note Size: 0x104
 */
extern "C" void fn_800B53A0(XObjChr* chr, s32 dir)
{
	fn_80097284(&chr->mMenu, dir);
}

/**
 * @note Address: 0x800B54A4
 * @note Size: 0xC8
 */
extern "C" void fn_800B54A4(XObjChr* chr, s32 selection)
{
	if (selection >= 0) {
		chr->mMenu.mPrevSel = chr->mMenu.mSel;
		chr->mMenu.mSel     = selection;
	}
	if (IsMenuUp(chr)) {
		if (chr->mMenu.mSel == 0) {
			chr->mMenu.mAtc.mFocus = 0;
			chr->mMenu.mDrs.mFocus = 1;
			chr->mMenu.mDrs._04    = -1;
		} else {
			chr->mMenu.mAtc.mFocus = 1;
			chr->mMenu.mAtc._04    = -1;
			chr->mMenu.mDrs.mFocus = 0;
		}
		fn_8001E904(&lbl_8025C400.mSound, 0x86, 255);
	} else {
		fn_800B558C(chr);
	}
}

/**
 * @note Address: 0x800B556C
 * @note Size: 0x20
 */
extern "C" void fn_800B556C(XObjChr* chr)
{
	if (chr->mAttachment != 0) {
		chr->mAttachmentRequest = 0;
		chr->_95D8              = 0;
	}
}

/**
 * @note Address: 0x800B558C
 * @note Size: 0x27C
 */
// stops him and brings up the attachment and costume rings
extern "C" void fn_800B558C(XObjChr* chr)
{
	if (chr->mAttachmentRequest != -1)
		return;
	if (chr->mSuitChanged != 0)
		return;
	if (!chr->mMenu.IsOpen()) {
		StopMoving(chr);
		OpenMenus(chr);
	} else if (chr->mMenu.IsClosing()) {
		OpenMenus(chr);
	}
}

/**
 * @note Address: 0x800B5808
 * @note Size: 0x4
 */
extern "C" void fn_800B5808()
{
}

/**
 * @note Address: 0x800B580C
 * @note Size: 0x168
 */
extern "C" void fn_800B580C(XObjChr* chr, BOOL enabled, u8 value)
{
	fn_80091834(chr, enabled, value);
	if (chr->mPlug.mJObj != NULL)
		chr->mPlug.vfDC(enabled, value);
	if (chr->mCopterObj.mJObj != NULL)
		chr->mCopterObj.vfDC(enabled, value);
	if (chr->mPopperObj.mJObj != NULL)
		chr->mPopperObj.vfDC(enabled, value);
	if (chr->mRadarObj.mJObj != NULL)
		chr->mRadarObj.vfDC(enabled, value);
	if (chr->mOXObj.mJObj != NULL)
		chr->mOXObj.vfDC(enabled, value);
	for (s32 i = 0; i < 9; ++i) {
		if (chr->mSuitObj[i].mJObj != NULL)
			chr->mSuitObj[i].vfDC(enabled, value);
	}
}

/**
 * @note Address: 0x800B5974
 * @note Size: 0x144
 */
extern "C" void fn_800B5974(XObjChr* chr, u32* flags)
{
	fn_80091C44(chr, flags);
	if (chr->mPlug.mJObj != NULL)
		chr->mPlug.vfD8(flags);
	if (chr->mCopterObj.mJObj != NULL)
		chr->mCopterObj.vfD8(flags);
	if (chr->mPopperObj.mJObj != NULL)
		chr->mPopperObj.vfD8(flags);
	if (chr->mRadarObj.mJObj != NULL)
		chr->mRadarObj.vfD8(flags);
	if (chr->mOXObj.mJObj != NULL)
		chr->mOXObj.vfD8(flags);
	for (s32 i = 0; i < 9; ++i) {
		if (chr->mSuitObj[i].mJObj != NULL)
			chr->mSuitObj[i].vfD8(flags);
	}
}

/**
 * @note Address: 0x800B5AB8
 * @note Size: 0xC4
 */
extern "C" void fn_800B5AB8(XObjChr* chr, s32 mode)
{
	fn_8008B0D8(chr, mode);
	chr->mPlug.vf120(mode);
	chr->mCopterObj.vf120(mode);
	chr->mPopperObj.vf120(mode);
	chr->mRadarObj.vf120(mode);
	chr->mOXObj.vf120(mode);
}

/**
 * @note Address: 0x800B5B7C
 * @note Size: 0xC0
 */
extern "C" void fn_800B5B7C(XObjChr*, s32 type, s32 value)
{
	for (s32 i = 0; i < 10; ++i) {
		if (lbl_80378300[i].IsFree()) {
			for (s32 j = 0; j < 10; ++j) {
				if (lbl_8037C018[j] == NULL) {
					lbl_8037C018[j] = &lbl_80378300[i];
					fn_80095AD8(lbl_8037C018[j], type, value);
					return;
				}
			}
			return;
		}
	}
}

/**
 * @note Address: 0x800B5C3C
 * @note Size: 0x1E0
 */
extern "C" void fn_800B5C3C(XObjChr* chr, u32 frames)
{
	if (chr->mHeadOffsetReady == 0 && lbl_8025C400.mEvt.mData.mScript == NULL)
		chr->mHeadOffsetReady = 1;
	f32 target, rate;
	if (chr->mPush.mState == 1 || chr->mPush.mState == 2) {
		if (chr->mPush._0C == 1)
			target = 8.0f;
		else if (chr->mPush._0C == -1)
			target = -5.0f;
		else
			target = 0.0f;
		rate = 0.4f;
	} else if (lbl_8025C400.mEvt.mData.mScript != NULL && chr->mHeadOffsetReady != 0) {
		target = 0.0f;
		rate   = 0.2f;
	} else if (IsRopeState(chr, 1) || IsRopeState(chr, 2) || IsRopeState(chr, 3) || IsRopeState(chr, 4) || IsRopeState(chr, 5)
	           || IsRopeState(chr, 6) || IsRopeState(chr, 7) || IsRopeState(chr, 8)) {
		target = 0.0f;
		rate   = 0.2f;
	} else {
		target = 5.0f;
		rate   = 0.2f;
	}
	if (__fpclassifyf(target) != FP_NAN) {
		f32 current = chr->mHeadOffset;
		if (current > target) {
			current -= rate * frames;
			chr->mHeadOffset = current;
			if (current < target)
				chr->mHeadOffset = target;
		} else if (current < target) {
			current += rate * frames;
			chr->mHeadOffset = current;
			if (current > target)
				chr->mHeadOffset = target;
		}
	}
}

/**
 * @note Address: 0x800B5E1C
 * @note Size: 0x18
 */
extern "C" void fn_800B5E1C(XObjChr* chr)
{
	chr->mHeadOffset      = 5.0f;
	chr->mHeadOffsetReady = 0;
}
