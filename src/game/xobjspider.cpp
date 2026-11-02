#include "game/xobjspider.h"
#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/cpad.h"
#include "game/cscrn.h"
#include "game/xchr.h"
#include "game/xobjflags.h"
#include "game/ut.h"
#include <sysdolphin/rumble.h>
#include "game/globals.h"
#include "game/gmath.h"
extern "C" {
#include <dolphin/mtx/mtx.h>
}

// one of the coins a spider knocks loose, it hops up and drops to mTarget
struct CoinBounce {
	XObj* mCoin; // _00
	Vec mPos;    // _04
	s32 mPhase;  // _10
	s32 mCount;  // _14
	Vec mTarget; // _18
};
STATIC_ASSERT(sizeof(CoinBounce) == 0x24);

extern "C" {
void fn_800EA554(Vec* position, Vec* target, s32 anim);
void fn_800EA780(void* unused, u32 frames);
void fn_800EA99C(void* unused);
void fn_800EB928(XObjSpider* spider);
s32 fn_800EBA30(XObjSpider* spider);
BOOL fn_800EBBF0(XObjSpider* spider);
u32 fn_800EBDB0(XObjSpider* spider);
BOOL fn_800EBDBC(XObjSpider* spider);
void fn_800EAF18(CarryInfo* carry, u8 slot, u32 frames, s32 mode);
f32 fn_800EF854(const Vec* first, const Vec* second);
void fn_800F0090(XObjSpider* spider, u32 frames);
void fn_800F0B7C(XObjSpider* spider);
void fn_800F1068(XObjSpider* spider, u32 frames);
void fn_800F1730(XObjSpider* spider, u32 frames);
void fn_800F1B10(XObjSpider* spider, u32 frames);
void fn_800F38F4(XObjSpider* spider);
void fn_800F4350(XObjSpider* spider);

UnkStageObj* fn_8006EDB4(s32 index);
bool fn_8012CA54(void* obj);
XObj* fn_80132698(void* list, const CoordInfo& info, s32 unused, BOOL async);
CVec fn_800621E0(XObj* obj);
void fn_8010CDDC(f32 x, f32 y, f32 z);
s32 fn_8009C1BC(AttachmentValues* values, s32 id);
void HSD_MtxGetScale(Mtx mat, Vec* vec);
void HSD_StateInvalidate(s32 state);
int rand();
void OSReport(const char* fmt, ...);

extern AnimSfxEvent lbl_8022AB90[]; // the spider's anim sounds
extern u8 lbl_8065B2B8;             // the coins' manager, it has no data of its own
extern CoinBounce lbl_8037E640[11];
extern s32 lbl_8065B2C4; // how many coins are out
extern s32 lbl_8065B2BC;
extern u8 lbl_8065B2C0;
}

// the frame the jobj's first anim is at, and the frame it ends at
static inline f32 GetAnimFrame(HSD_JObj* jobj)
{
	HSD_AObj* aobj = GetAObj(jobj);
	return aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
}

static inline f32 GetAnimEndFrame(HSD_JObj* jobj)
{
	HSD_AObj* aobj = GetAObj(jobj);
	return aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
}

// puts a CObjVec at a value and its target there too
static inline void SetNow(CObjVec& vec, const Vec& v)
{
	vec.Set(v);
	vec.mTarget.Set(v);
}

// the slot's model plays its anim 2 from where its anim ends, once
static inline void HitCarried(XObjSpider* spider)
{
	if (!(spider->mCarry.mFlags.mFlags & 2)) {
		XObj* model = lbl_8037E5C0[spider->mSlot];
		if (model != NULL && spider->mCarry.mState == 2) {
			f32 end = GetAnimEndFrame(model->mJObj);
			model->SetAnim(2, TRUE, NULL, NULL, NULL, end - GetAnimFrame(model->mJObj), 1.0f, 0.0f, 10.0f);
			spider->mCarry.mState++;
			spider->mCarry.mFlags.mFlags |= 2;
		}
	}
}

// starts reading the object a stage table entry places
static inline XObj* ReadObj(s16 id)
{
	return fn_80132698(&lbl_8025C400.mXObjs, CoordInfo(fn_8006EDB4(id)), 0, TRUE);
}

// turns it toward a point and says how far that is
static inline f32 HeadTo(XObjSpider* spider, const Vec& to)
{
	spider->mDir = atan2(to.x - spider->mMoveTranslate.x, to.z - spider->mMoveTranslate.z);
	f32 x        = to.x - spider->mMoveTranslate.x;
	f32 y        = to.y - spider->mMoveTranslate.y;
	f32 z        = to.z - spider->mMoveTranslate.z;
	return sqrtf(x * x + y * y + z * z);
}

// where the robot is
static inline Vec GetChrPos()
{
	return lbl_8025C400.GetPlayer()->mMoveTranslate;
}

// takes the first of the slot models nobody has
static inline void TakeSlot(XObjSpider* spider)
{
	for (int i = 0; i < 16; i++) {
		if (lbl_8037E600[i] == -1) {
			spider->_AC8.mObj = lbl_8037E5C0[i];
			spider->mSlot     = i;
			lbl_8037E600[i]   = spider->_258;
			return;
		}
	}
}

// a step sound, which one of the five goes by the spider's index
static inline void PlayStep(XObjSpider* spider)
{
	switch (spider->_258 % 5) {
	case 0:
		fn_8001E904(&lbl_8025C400.mSound, 0x12B, 255);
		break;
	case 1:
		fn_8001E904(&lbl_8025C400.mSound, 0x12C, 255);
		break;
	case 2:
		fn_8001E904(&lbl_8025C400.mSound, 0x12D, 255);
		break;
	case 3:
		fn_8001E904(&lbl_8025C400.mSound, 0x12E, 255);
		break;
	case 4:
		fn_8001E904(&lbl_8025C400.mSound, 0x12F, 255);
		break;
	}
}

// how far apart two points are on the floor
static inline f32 DistXZ(const Vec* first, const Vec* second)
{
	f32 x = first->x - second->x;
	f32 y = 0.0f;
	f32 z = first->z - second->z;
	return sqrtf(x * x + y * y + z * z);
}

// the event's object with the id, ids from 10000 up
static inline XObj* FindEvtObj(s16 id)
{
	return lbl_8025C400.mEvt.mData.mScript != NULL ? fn_8007F570(&lbl_8025C400.mEvt.mData, id) : NULL;
}

// takes all the coins back
static inline void ResetCoins()
{
	lbl_8065B2C4 = 0;
	for (int i = 0; i < 11; i++) {
		lbl_8037E640[i].mCoin  = NULL;
		lbl_8037E640[i].mPhase = 0;
		lbl_8037E640[i].mCount = 0;
	}
	for (int i = 0; i < 96; i++) {
		XObjCoin* coin = (XObjCoin*)lbl_8025C400.mXObjs.mPools[7].Get(i);
		if (coin->GetKind() == 0x104) {
			coin->mBouncing = 0;
		}
	}
}

// the object with the id
static inline XObj* GetObj(s16 id)
{
	return lbl_8025C400.mXObjs.mObjects[id];
}

// the state handlers by number, Update calls them
// NONMATCHING: __sinit, retail has 0x1C8 bytes of .data in front of the initializer it copies
static XObjSpider::Func sStates[10] = {
	&XObjSpider::State0, &XObjSpider::State1, &XObjSpider::State2, &XObjSpider::State3, &XObjSpider::State4,
	&XObjSpider::State5, &XObjSpider::State6, &XObjSpider::State7, &XObjSpider::State8, &XObjSpider::State9,
};

static s32 sThreadPath[] = { 0 };
static s32 sHoldPath[]   = { 0 };

/**
 * @note Address: 0x800F4868
 * @note Size: 0x58
 */
void XObjSpider::vf90(u32 id)
{
	fn_800929C4(this, id);
	fn_800F4350(this);
	SetState(0);
	mFlags.mFlags |= 0x04000000;
}

/**
 * @note Address: 0x800F4818
 * @note Size: 0x50
 */
void XObjSpider::vf94()
{
	fn_800F4350(this);
	SetState(0);
	mFlags.mFlags |= 0x04000000;
}

/**
 * @note Address: 0x800F4350
 * @note Size: 0x4C8
 */
// NONMATCHING: .rodata, retail has six more Vecs in front of the zero Vecs
// puts everything back to how it starts
extern "C" void fn_800F4350(XObjSpider* spider)
{
	spider->mDir          = 0.0f;
	spider->mSpeed        = 0.0f;
	spider->_988          = 0.0f;
	spider->_98C          = 0.0f;
	spider->mTurnSpeed    = 0.1f;
	spider->mFallSpeed    = 0.0f;
	spider->mState        = 0;
	spider->mPrevState    = 0;
	spider->mPhase        = 0;
	spider->mCounter      = 0;
	spider->mFlags.mFlags = 0;
	spider->mWait         = 0;
	spider->_9C0          = 1.0f;
	spider->_9C4          = 1.0f;
	Vec hitPos            = { 0.0f, 0.0f, 0.0f };
	spider->mHitPos       = hitPos;
	Vec home              = { 0.0f, 0.0f, 0.0f };
	spider->mHome         = home;
	Vec wallPos           = { 0.0f, 0.0f, 0.0f };
	spider->mWallPos      = wallPos;
	Vec target            = { 0.0f, 0.0f, 0.0f };
	spider->mTarget       = target;
	Vec v08               = { 0.0f, 0.0f, 0.0f };
	spider->_A08          = v08;
	Vec v14               = { 0.0f, 0.0f, 0.0f };
	spider->_A14          = v14;
	spider->_A38          = 0.0f;
	spider->_A40          = 0.0f;
	spider->_A44          = 0;
	spider->_A48          = 0;
	spider->mHitStrength  = 0.0f;
	spider->mWall         = 0;
	spider->mMaxHits      = 3;
	spider->mHits         = spider->mMaxHits;
	spider->mCommand      = 0;
	spider->mArgs[0]      = 0;
	spider->_A88          = 0;
	spider->mThreadAlpha  = 1.0f;
	spider->mStatus.mFlags |= 8;
	spider->mCarry.mPos.x        = 0.0f;
	spider->mCarry.mPos.y        = 0.0f;
	spider->mCarry.mPos.z        = 0.0f;
	spider->mCarry._10.x         = 0.0f;
	spider->mCarry._10.y         = 0.0f;
	spider->mCarry._10.z         = 0.0f;
	spider->mCarry.mAngle        = 0.0f;
	spider->mCarry._24           = 0.0f;
	spider->mCarry.mState        = 0;
	spider->mCarry._2C           = 0;
	spider->mCarry.mFlags.mFlags = 0;
	spider->mCarry.mObj          = NULL;
	spider->mRadius              = 20.0f;
	spider->_AC8.mObj            = NULL;
	spider->mSlot                = 255;
	Vec areaPos                  = { 0.0f, 0.0f, 0.0f };
	spider->mAreaPos             = areaPos;
	spider->mAreaWidth           = 0.0f;
	spider->mAreaDepth           = 0.0f;
	Vec startPos                 = { 0.0f, 0.0f, 0.0f };
	spider->mStartPos            = startPos;
	spider->_A6C                 = 0.0f;
	spider->_B10                 = 2;
	spider->_B14[0]              = 0;
	spider->_B14[1]              = 0;
	spider->_B14[2]              = 0;
	spider->_B14[3]              = 0;
	spider->_B24[0]              = 100;
	spider->_B24[1]              = 100;
	spider->_B24[2]              = 100;
	spider->_B54                 = 0;
	spider->_B50                 = 0;
	spider->mEffect              = NULL;
	spider->mEffectId            = 0;
	spider->mEffectOn            = 0;
	for (int i = 0; i < 8; i++) {
		spider->mTies[i].mObj   = NULL;
		spider->mTies[i].mJoint = NULL;
	}
	if (spider->_258 == 0) {
		ResetCoins();
	}
	spider->mHitState    = 0;
	spider->mHitTimer    = 0;
	spider->_B9C         = -1;
	spider->mRumble      = 0;
	spider->mRumbleTimer = 0;
}

/**
 * @note Address: 0x800F3898
 * @note Size: 0x5C
 */
BOOL XObjSpider::vf9C(const CoordInfo& info)
{
	if (fn_80091C80(this, &info)) {
		mFlags.mFlags &= ~0x04000000U;
		fn_800F4350(this);
		fn_800F38F4(this);
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x800F373C
 * @note Size: 0x15C
 */
void XObjSpider::vf98()
{
	fn_800928E8(this);
	mSfx.mEvents = lbl_8022AB90;
	if (_258 == 0) {
		for (u32 i = 0; i < 16; ++i) {
			lbl_8037E5C0[i] = NULL;
			lbl_8037E600[i] = -1;
		}
		lbl_8065B2C0  = 0;
		lbl_8065B2BC  = 0;
		s32 found     = 0;
		s32 count     = lbl_8025C400.mXObjs.mObjectCount;
		XObj** output = lbl_8037E5C0;
		for (s32 i = 0; i < count; ++i) {
			XObj* candidate = lbl_8025C400.mXObjs.mObjects[i];
			if (candidate->mStatus.mFlags & 1ULL) {
				UnkStageObj* placement = candidate->mCoord._00;
				if (placement->_00 == 0x13C) {
					++found;
					*output++ = candidate;
					if (found >= 16)
						break;
				}
			}
		}
	}
}

/**
 * @note Address: 0x800F31E0
 * @note Size: 0x55C
 */
void XObjSpider::vfA4(u32 frames)
{
	CPad pad(0);

	switch (mRumble) {
	case 0:
		break;
	case 1:
		if (lbl_8025C400.mStatus.mFlags & 0x800000000ULL) {
			CPadOne* rumble = &lbl_8025C400.mPad.mPad[0];
			if (rumble->mConnected) {
				HSD_PadRumbleOn(rumble->mPort);
			}
			mRumbleTimer = 24 / frames;
			mRumble++;
		} else {
			mRumble = 0;
		}
		break;
	case 2:
		if (--mRumbleTimer < 0) {
			CPadOne* rumble = &lbl_8025C400.mPad.mPad[0];
			if (rumble->mConnected) {
				HSD_PadRumbleOffN(rumble->mPort);
			}
			if (rumble->mConnected) {
				HSD_PadRumbleRemove(rumble->mPort);
			}
			mRumble = 0;
		}
		break;
	}

	if (lbl_8025C400.mStatus.mFlags & 0x400) {
		return;
	}
	if (_258 == 0) {
		fn_800EA780(&lbl_8065B2B8, frames);
	}
	if (!(mStatus.mFlags & 0x10000) && mState != 1) {
		return;
	}
	vf148();
	fn_800F1B10(this, frames);
	if (!(mFlags.mFlags & 0x08000000)) {
		(this->*sStates[mState])(frames);
		CVec move;
		if (mWall == 4) {
			mMoveTranslate.x = mWallPos.x;
			move.y           = mSpeed * (f32)sin(mDir);
			move.z           = mSpeed * (f32)cos(mDir);
			move.x           = 0.0f;
			mMoveRotate.z    = -HALF_PI;
			SetRotateY(mDir, mTurnSpeed);
		} else if (mWall == 5) {
			mMoveTranslate.x = mWallPos.x;
			move.y           = mSpeed * (f32)sin(mDir);
			move.z           = mSpeed * (f32)cos(mDir);
			move.x           = 0.0f;
			mMoveRotate.z    = HALF_PI;
			SetRotateY(mDir, mTurnSpeed);
		} else if (mWall == 2) {
			mMoveTranslate.z = mWallPos.z;
			move.y           = mSpeed * (f32)sin(mDir);
			move.x           = mSpeed * (f32)cos(mDir);
			move.z           = 0.0f;
			mMoveRotate.x    = -HALF_PI;
			mMoveRotate.z    = mDir - HALF_PI;
		} else if (mWall == 3) {
			mMoveTranslate.z = mWallPos.z;
			move.y           = mSpeed * (f32)sin(mDir);
			move.x           = mSpeed * (f32)cos(mDir);
			move.z           = 0.0f;
			mMoveRotate.x    = HALF_PI;
			mMoveRotate.z    = HALF_PI + mDir;
		} else if (mFlags.mFlags & 0x100) {
			move.x = 0.0f;
			move.z = 0.0f;
			move.y = mSpeed;
			SetRotateY(mDir, mTurnSpeed);
		} else {
			if (mFlags.mFlags & 8) {
				mMoveTranslate.y += mFallSpeed;
			} else {
				mMoveTranslate.y = mHome.y;
			}
			move.x = mSpeed * (f32)sin(mDir);
			move.z = mSpeed * (f32)cos(mDir);
			move.y = 0.0f;
			SetRotateY(mDir, mTurnSpeed);
		}
		SetNow(mMoveTranslate, mMoveTranslate + move * (f32)frames);
		SetPos(mMoveTranslate, 0);
	}
	fn_80090ED4(this, frames);
	fn_800EAF18(&mCarry, mSlot, frames, 0);
	fn_800F1068(this, frames);
}

/**
 * @note Address: 0x800F3184
 * @note Size: 0x5C
 */
void XObjSpider::vfAC()
{
	vfB0(NULL);
	bool sideways = false;
	if (mWall == 3)
		sideways = true;
	fn_800EAB58(&mCarry, mSlot, sideways);
}

/**
 * @note Address: 0x800F3180
 * @note Size: 0x4
 */
void XObjSpider::vfB8()
{
}

/**
 * @note Address: 0x800F2E44
 * @note Size: 0x33C
 */
// NONMATCHING: the float registers of the ring loop are numbered the other way
// the shadow, a disc and a ring around it that fades out
void XObjSpider::vfBC(const Vec* origin, f32 opacity)
{
	if (!(mStatus.mFlags & 8ULL))
		return;
	switch (_25C) {
	case 3:
	case 4:
	case 5:
	case 7:
	case 8:
	case 9:
		return;
	}
	Vec pos  = *origin;
	Vec rot  = { -HALF_PI, 0.0f, 0.0f };
	Vec size = mMoveScale * (f32)(mAttr._00 != NULL ? mAttr._00->_0C : 0);
	size.x   = mRadius;
	size.y   = mRadius;
	size.z   = mRadius;
	fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
	u32 edge = 128.0f * opacity;
	fn_80198258(&lbl_8025C400.mGs, 12, 160.0f * opacity, edge, pos.x, pos.y, pos.z, size.x, size.z, rot.x, rot.y, rot.z);
	PrimVertex verts[26];
	for (int i = 0; i < 13; i++) {
		f32 angle              = (PI / 6) * i;
		verts[i * 2].pos.x     = pos.x - size.x * 0.5f * (f32)sin(angle);
		verts[i * 2].pos.z     = pos.z + size.z * 0.5f * (f32)cos(angle);
		verts[i * 2].pos.y     = pos.y;
		verts[i * 2].color     = edge;
		verts[i * 2 + 1].pos.x = pos.x - (1.5f + size.x * 0.5f) * (f32)sin(angle);
		verts[i * 2 + 1].pos.z = pos.z + (1.5f + size.z * 0.5f) * (f32)cos(angle);
		verts[i * 2 + 1].pos.y = pos.y;
		verts[i * 2 + 1].color = 0;
	}
	fn_801988EC(&lbl_8025C400.mGs, GX_TRIANGLESTRIP, lbl_8025C400.mGs.mCamera->mViewMtx, verts, 26);
}

/**
 * @note Address: 0x800F2E40
 * @note Size: 0x4
 */
void XObjSpider::vfC8()
{
}

/**
 * @note Address: 0x800F2E3C
 * @note Size: 0x4
 */
void XObjSpider::vfCC()
{
}

/**
 * @note Address: 0x800F29A4
 * @note Size: 0x498
 */
// NONMATCHING: the joint path and the two points are laid out differently on the stack
// the thread it hangs from, a line from its joint up to the ceiling
void XObjSpider::vfC0()
{
	if (mFlags.mFlags & 0x80) {
		Vec top;
		CJObj joint;
		u32 color = (u32)(90.0f * mThreadAlpha) | 0xFFFFFF00;
		joint.Attach(FindJointByPath(mJObj, sThreadPath, 1));
		top = GetJointPosition(joint.mJObj);
		CVec pos(top);
		CGs* gs = &lbl_8025C400.mGs;
		HSD_StateInvalidate(-1);
		CGs* gs2 = &lbl_8025C400.mGs;
		if (gs2->mCurRenderMode != NULL) {
			CGsRenderMode* mode = gs->mCurRenderMode;
			if (mode->mPEDirty == 0) {
				mode->mPEDirty = 1;
			}
			if (mode->mCullDirty == 0) {
				mode->mCullDirty = 1;
			}
			if (mode->mLineDirty == 0) {
				mode->mLineDirty = 1;
			}
			if (mode->mPointDirty == 0) {
				mode->mPointDirty = 1;
			}
		}
		CGsTevStage* stage = gs->mCurTevStage;
		if (stage != NULL) {
			if (stage->mStageDirty == 0) {
				stage->mStageDirty = 1;
			}
			if (stage->mNumGensDirty == 0) {
				stage->mNumGensDirty = 1;
			}
			if (stage->mTexGenDirty == 0) {
				stage->mTexGenDirty = 1;
			}
		}
		CGsChannel* channel = gs->mCurChannel;
		if (channel != NULL) {
			if (channel->mDirty == 0) {
				channel->mDirty = 1;
			}
		}
		CGs* gs3 = &lbl_8025C400.mGs;
		if (gs2->mCurRenderMode != NULL) {
			fn_8019B278(gs3->mCurRenderMode, 1);
		}
		if (gs3->mCurTevStage != NULL) {
			fn_8019AED0(gs3->mCurTevStage, 1);
		}
		if (gs3->mCurChannel != NULL) {
			fn_8019AE94(gs3->mCurChannel, 1);
		}
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		fn_8019B34C(gs2->mCurRenderMode, 0x12, 5);
		fn_801992BC(&lbl_8025C400.mGs, NULL, pos.x, pos.y, pos.z, color, top.x, 300.0f, top.z, color);
		fn_8019B34C(gs2->mCurRenderMode, 0xC, 5);
	}
}

/**
 * @note Address: 0x800F2454
 * @note Size: 0x550
 */
// NONMATCHING: argv[1] is read into another register
// the event commands, 100 ties another object to one of its joints or lets go of all
// of them, anything else is kept for fn_800F1730
extern "C" s32 fn_800F2454(XObjSpider* spider, s32 argc, s32* argv)
{
	for (int i = 0; i < argc; i++) { }
	switch (argv[0]) {
	case 100:
		if (argv[1] == 0) {
			spider->mFlags.mFlags &= ~0x20000000;
			for (u32 i = 0; i < 8; i++) {
				if (spider->mTies[i].mObj != NULL) {
					HSD_JObjClearMtxFlags(spider->mTies[i].mObj->mJObj);
					spider->mTies[i].mObj   = NULL;
					spider->mTies[i].mJoint = NULL;
				}
			}
		} else {
			u32 i;
			for (i = 0; spider->mTies[i].mObj != NULL && i < 8; i++) { }
			if (i < 8) {
				OSReport("id = %d, argv[1]=%d\n", i);
				spider->mFlags.mFlags |= 0x20000000;
				if (argv[1] >= 10000) {
					spider->mTies[i].mObj = FindEvtObj(argv[1]);
				} else {
					spider->mTies[i].mObj = GetObj(argv[1]);
				}
				if (spider->mTies[i].mObj == NULL) {
					OSReport("NULL==============\n");
				}
				s32 path[8];
				s32 depth = argv[2];
				for (s32 j = 0; j < depth; j++) {
					path[j] = argv[3 + j];
				}
				spider->mTies[i].mJoint = GetJointByPath(spider->mJObj, path, depth);
				Mtx mtx;
				XObj* obj = spider->mTies[i].mObj;
				PSMTXTrans(mtx, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXConcat(HSD_JObjGetMtxPtr(spider->mTies[i].mJoint), mtx, spider->mTies[i].mMtx);
				HSD_JObjCopyMtx(spider->mTies[i].mObj->mJObj, spider->mTies[i].mMtx);
			}
		}
		break;
	default:
		if (argc < 1) {
			return 0;
		}
		spider->mCommand = argv[0];
		if (argc < 2) {
			spider->mArgs[0] = 0;
		} else {
			spider->mArgs[0] = argv[1];
		}
		if (argc < 3) {
			spider->mArgs[1] = 0;
		} else {
			spider->mArgs[1] = argv[2];
		}
		if (argc < 4) {
			spider->mArgs[2] = 0;
		} else {
			spider->mArgs[2] = argv[3];
		}
		if (argc < 5) {
			spider->mArgs[3] = 0;
		} else {
			spider->mArgs[3] = argv[4];
		}
		if (argc < 6) {
			spider->mArgs[4] = 0;
		} else {
			spider->mArgs[4] = argv[5];
		}
		break;
	}
	return 0;
}

/**
 * @note Address: 0x800F2364
 * @note Size: 0xF0
 */
void XObjSpider::vf138(const Vec& from, f32 strength)
{
	if (mFlags.mFlags & 2)
		return;
	if (mFlags.mFlags & 0x10)
		return;
	if (mFlags.mFlags & 0x8000)
		return;
	if (fn_8009C1BC(&lbl_8025C400.GetPlayer()->mAttachments, 2) < 2)
		return;
	fn_8008A6C0(this, &from, strength);
	mFlags.mFlags |= 2;
	Vec pos      = { 0.0f, 0.0f, 0.0f };
	pos          = from;
	mHitPos      = pos;
	mHitStrength = strength;
}

/**
 * @note Address: 0x800F2350
 * @note Size: 0x14
 */
s32 XObjSpider::vf100(s32 unused, s32 value)
{
	mFlags.mFlags |= 1;
	return value;
}

/**
 * @note Address: 0x800F0B7C
 * @note Size: 0x4EC
 */
// NONMATCHING: the return temp and the target sit the other way on the stack
// shoots the carried model from its joint, ahead of it or at the robot
extern "C" void fn_800F0B7C(XObjSpider* spider)
{
	CJObj joint;
	s32 path[8] = { 0, 0, 0, 0, 1, 1, 0, 0 };
	joint.Attach(FindJointByPath(spider->mJObj, path, 8));
	CVec pos(GetJointPosition(joint.mJObj));
	switch (spider->_25C) {
	case 0:
		spider->mShotTo.x = pos.x + 72.0f * (f32)sin(spider->mDir);
		spider->mShotTo.y = pos.y;
		spider->mShotTo.z = pos.z + 72.0f * (f32)cos(spider->mDir);
		break;
	case 12:
	case 13:
		spider->mShotTo.x = pos.x + 72.0f * (f32)sin(HALF_PI + spider->mDir);
		spider->mShotTo.y = pos.y + 72.0f * (f32)cos(HALF_PI + spider->mDir);
		spider->mShotTo.z = pos.z;
		break;
	default:
		Vec chr         = lbl_8025C400.GetPlayer()->mMoveTranslate;
		spider->mShotTo = chr;
		break;
	}
	spider->_A44 = 1;
	spider->_A48 = 10;
	spider->mCarry.mFlags.mFlags |= 1;
	spider->mCarry.mPos.x = pos.x;
	spider->mCarry.mPos.y = pos.y;
	spider->mCarry.mPos.z = pos.z;
	XObj* self            = lbl_8025C400.mXObjs.mPools[17].Get(spider->_258);
	CVec to(spider->mShotTo);
	spider->mCarry.mAngle = atan2(to.y - spider->mCarry.mPos.y, to.x - spider->mCarry.mPos.x);
	spider->mCarry.mObj   = self;
	spider->mCarry.mState = 1;
	fn_8001E904(&lbl_8025C400.mSound, 0x12A, 255);
}

/**
 * @note Address: 0x800F1068
 * @note Size: 0x6C8
 */
// NONMATCHING: the matrix pointers and the tie loop take other registers
// reads the object it holds up and keeps it on its joint, and moves the tied objects
// along with their joints
extern "C" void fn_800F1068(XObjSpider* spider, u32 frames)
{
	switch (spider->mEffectOn) {
	case 0:
		break;
	case 1:
		if (!fn_8012CA54(lbl_8025C400.mXObjs.mPools[14].mObjects)) {
			break;
		}
		if (spider->mEffect != NULL) {
			spider->mEffect->Release();
		}
		if (spider->mEffectId >= 0) {
			spider->mEffect = ReadObj(spider->mEffectId);
		} else {
			spider->mEffect = NULL;
		}
		spider->mEffectOn++;
	case 2:
		if (spider->mEffect != NULL) {
			if (spider->mEffect->mStatus.mFlags & 0x0040000000000000ULL) {
				break;
			}
			lbl_8025C400.mStatus.mFlags |= 0x1000;
		}
		spider->mEffectOn++;
	case 3:
		if (spider->mEffectId < 0 || spider->mEffect == NULL) {
			break;
		}
		spider->mEffect->SetScale(1.0f, 1.0f, 1.0f, 0);
		CJObj joint;
		joint.Attach(FindJointByPath(spider->mJObj, sHoldPath, 1));
		PSMTXCopy(HSD_JObjGetMtxPtr(joint.mJObj), spider->mEffectMtx);
		Vec scale;
		HSD_MtxGetScale(spider->mEffectMtx, &scale);
		scale.x = fn_800621E0(spider->mEffect).x / scale.x;
		scale.y = fn_800621E0(spider->mEffect).y / scale.y;
		scale.z = fn_800621E0(spider->mEffect).z / scale.z;
		Mtx scaleMtx;
		PSMTXScale(scaleMtx, scale.x, scale.y, scale.z);
		Mtx up;
		Mtx mtx;
		PSMTXConcat(spider->mEffectMtx, scaleMtx, mtx);
		PSMTXTrans(up, 0.0f, 5.0f, 0.0f);
		PSMTXConcat(mtx, up, mtx);
		HSD_JObjCopyMtx(spider->mEffect->mJObj, mtx);
		spider->mEffect->mStatus.mFlags &= ~0x0200000000000000ULL;
		break;
	}
	if (spider->mFlags.mFlags & 0x20000000) {
		for (u32 i = 0; i < 8; i++) {
			if (spider->mTies[i].mJoint != NULL && spider->mTies[i].mObj != NULL) {
				Mtx mtx;
				XObj* obj = spider->mTies[i].mObj;
				PSMTXTrans(mtx, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXConcat(HSD_JObjGetMtxPtr(spider->mTies[i].mJoint), mtx, spider->mTies[i].mMtx);
				HSD_JObjCopyMtx(spider->mTies[i].mObj->mJObj, spider->mTies[i].mMtx);
			}
		}
	}
}

/**
 * @note Address: 0x800F1B10
 * @note Size: 0x840
 */
// the hits, it loses hits when the robot hits it, more for a hard hit, then blinks red
// for a while. and when the robot touches it, the robot gets hit
extern "C" void fn_800F1B10(XObjSpider* spider, u32 frames)
{
	switch (spider->mHitState) {
	case 0:
		if ((spider->mFlags.mFlags & 2) && !(spider->mFlags.mFlags & 0x10)) {
			fn_8001E660(&lbl_8025C400.mSound, 0x12A);
			spider->mFlags.mFlags &= ~2;
			if (spider->mHitStrength >= (s32)fn_8006EBC8(0x1F) / 100.0f) {
				spider->mHits -= 5;
				if (spider->mHits <= 0) {
					spider->mHits = 0;
				}
			} else if (--spider->mHits <= 0) {
				spider->mHits = 0;
			}
			if (spider->mHits <= 0) {
				spider->mSpeed = 0.0f;
				spider->SetState(3);
				spider->mFlags.mFlags |= 0x10;
			} else {
				spider->mHitState = 1;
				spider->mHitTimer = 0;
				spider->mFlags.mFlags |= 0x10;
			}
		}
		break;
	case 1:
		if (spider->mCounter % 30 < 15) {
			spider->vf12C(1, 0x80, 0, 0, 0.0f);
		} else {
			spider->vf12C(1, 0, 0, 0, 0.0f);
		}
		spider->mHitTimer++;
		if (spider->mHitTimer > 20) {
			spider->mFlags.mFlags &= ~0x10;
			spider->vf12C(1, 0, 0, 0, 0.0f);
			spider->mHitTimer = 0;
			spider->mHitState = 0;
		}
		break;
	}
	Vec chr = lbl_8025C400.GetPlayer()->mMoveTranslate;
	if (spider->mWall == 3) {
		f32 x = chr.x - spider->_A14.x;
		f32 y = 8.0f + chr.y - spider->_A14.y;
		f32 z = chr.z - chr.z;
		if (sqrtf(x * x + y * y + z * z) < 8.0f && (spider->mFlags.mFlags & 0x800)) {
			lbl_8025C400.GetPlayer()->vf138(chr, 7.0f);
			HitCarried(spider);
		}
	} else {
		f32 x    = chr.x - spider->_A14.x;
		f32 y    = 0.0f;
		f32 z    = chr.z - spider->_A14.z;
		f32 chrY = chr.y;
		if (sqrtf(x * x + y * y + z * z) < 2.0f && (spider->mFlags.mFlags & 0x800) && Abs(chrY - spider->mMoveTranslate.y) < 4.0f) {
			lbl_8025C400.GetPlayer()->vf138(chr, 7.0f);
			HitCarried(spider);
		}
	}
	fn_800F1730(spider, frames);
	fn_800F0090(spider, frames);
}

/**
 * @note Address: 0x800F1730
 * @note Size: 0x3E0
 */
// does what the last event command said
extern "C" void fn_800F1730(XObjSpider* spider, u32 frames)
{
	switch (spider->mCommand) {
	case 0:
		break;
	case 1:
		spider->SetState(3);
		spider->mCommand = 0;
		break;
	case 2:
		if (spider->mFlags.mFlags & 0x10000)
			lbl_8025C400.mEvt._10 = 1;
		else
			lbl_8025C400.mEvt._10 = 0;
		spider->mCommand = 0;
		break;
	case 3:
		if (spider->mFlags.mFlags & 0x80000)
			lbl_8025C400.mEvt._10 = 1;
		else
			lbl_8025C400.mEvt._10 = 0;
		spider->mCommand = 0;
		break;
	case 4:
		spider->mFlags.mFlags |= 0x00200000;
		spider->mCommand = 0;
		break;
	case 5:
		spider->mFlags.mFlags |= 0x00400000;
		spider->mCommand = 0;
		break;
	case 6:
		spider->mStatus.mFlags |= 0x00100000;
		spider->mCommand = 0;
		break;
	case 7:
		if (spider->mArgs[0] == 1)
			spider->mFlags.mFlags &= ~0x08000000;
		else
			spider->mFlags.mFlags |= 0x08000000;
		spider->mCommand = 0;
		spider->mArgs[0] = 0;
		break;
	case 8:
		fn_800F38F4(spider);
		spider->mCommand = 0;
		break;
	case 9:
		spider->mHome.x          = spider->mArgs[0] / 100.0f;
		spider->mHome.y          = spider->mArgs[1] / 100.0f;
		spider->mHome.z          = spider->mArgs[2] / 100.0f;
		spider->mStartPos.x      = spider->mHome.x;
		spider->mStartPos.y      = spider->mHome.y;
		spider->mStartPos.z      = spider->mHome.z;
		spider->mMoveTranslate.x = spider->mHome.x;
		spider->mMoveTranslate.y = (f32)spider->_260 + spider->mHome.y;
		spider->mMoveTranslate.z = spider->mHome.z;
		spider->mMoveTranslate.mTarget.Set(spider->mMoveTranslate.x, spider->mMoveTranslate.y, spider->mMoveTranslate.z);
		spider->mCommand = 0;
		break;
	case 10:
		spider->mAreaPos.x = spider->mArgs[0] / 100.0f;
		spider->mAreaPos.y = spider->mArgs[1] / 100.0f;
		spider->mAreaPos.z = spider->mArgs[2] / 100.0f;
		spider->mAreaWidth = spider->mArgs[3];
		spider->mAreaDepth = spider->mArgs[4];
		spider->mCommand   = 0;
		break;
	case 11:
		spider->mCommand = 0;
		break;
	case 12:
		spider->mCommand = 0;
		break;
	case 13:
		spider->mFlags.mFlags |= 0x10000000;
		spider->mCommand = 0;
		break;
	case 14:
		spider->mEffectId = spider->mArgs[0];
		spider->mEffectOn = 1;
		spider->mCommand  = 0;
		break;
	case 15:
		spider->mEffectOn = 0;
		spider->mEffectId = -1;
		if (spider->mEffect != NULL)
			spider->mEffect->Release();
		spider->mCommand = 0;
		break;
	case 16:
		spider->mCommand = 0;
		break;
	case 17:
		spider->mCommand = 0;
		break;
	case 18:
		spider->mCommand = 0;
		break;
	case 19:
		spider->SetState(5);
		spider->mCommand = 0;
		break;
	}
}

/**
 * @note Address: 0x800F0058
 * @note Size: 0x38
 */
void XObjSpider::State0(u32 frames)
{
	if (_25C == 11 && (mFlags.mFlags & 0x400000))
		SetState(4);
}

/**
 * @note Address: 0x800EFDB0
 * @note Size: 0x2A8
 */
// hit, stops, then plays anim 21 and goes on, or dies when it has no hits left
void XObjSpider::State1(u32 frames)
{
	switch (mPhase) {
	case 0:
		fn_8001E904(&lbl_8025C400.mSound, 0x129, 255);
		mFlags.mFlags &= ~0x800U;
		mFlags.mFlags |= 0x10;
		mSpeed = 0.0f;
		if (mHits <= 0) {
			mFlags.mFlags |= 0x10000;
			SetState(3);
		} else {
			SetAnim(21, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			++mPhase;
		}
		break;
	case 1:
		HSD_AObj* aobj = GetAObj(mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(mJObj);
		f32 current    = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
		if (end - (f32)frames <= current) {
			mFlags.mFlags &= ~0x10U;
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			SetState(4);
		}
		break;
	}
}

/**
 * @note Address: 0x800EFDAC
 * @note Size: 0x4
 */
void XObjSpider::State2(u32 frames)
{
}

/**
 * @note Address: 0x800EF958
 * @note Size: 0x454
 */
// dies, blinks, stops its sound, lets go of its slot, shakes the screen, then leaves
// one of the coins where it was
void XObjSpider::State3(u32 frames)
{
	mCounter++;
	if (mCounter % 30 < 15) {
		vf12C(1, 0, 0x80, 0, 0.0f);
	} else {
		vf12C(1, 0, 0, 0, 0.0f);
	}
	Vec pos;
	switch (mPhase) {
	case 0:
		fn_8001E660(&lbl_8025C400.mSound, 0x132);
		SetAnim(0x19, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		if (mSlot != 255) {
			if (_AC8.mObj != NULL) {
				_AC8.mObj->mStatus.mFlags &= ~0x10000ULL;
			}
			lbl_8037E600[mSlot] = -1;
			_AC8.mObj           = NULL;
			mSlot               = 255;
		}
		mWait = 44;
		mPhase++;
		break;
	case 1:
		mWait -= frames;
		if (mWait < 0) {
			fn_8010CDDC(mMoveTranslate.x, mMoveTranslate.y, mMoveTranslate.z);
			if (!(lbl_8025C400.mStatus.mFlags & 0x100)) {
				fn_800242A8(&lbl_8025C400.mQuake, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.4f);
			}
			mPhase++;
		}
		break;
	case 2:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			mPhase++;
			mCounter = 0;
			mStatus.mFlags &= ~0x10000ULL;
			mFlags.mFlags |= 0x80000;
			mFlags.mFlags &= ~0x100000;
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
			Vec target;
			target.x = mHome.x;
			target.y = 0.0f;
			target.z = mHome.z;
			fn_800EA554(&pos, &target, anim);
		}
		break;
	case 3:
		break;
	}
}

/**
 * @note Address: 0x800EF854
 * @note Size: 0x104
 */
// NONMATCHING: the sum is done in another order
extern "C" f32 fn_800EF854(const Vec* first, const Vec* second)
{
	return DistXZ(first, second);
}

/**
 * @note Address: 0x800EDEE4
 * @note Size: 0x16C
 */
// lands, drops into its anim, then takes the first free slot
void XObjSpider::State5(u32 frames)
{
	switch (mPhase) {
	case 0:
		mFlags.mFlags |= 0x40;
		mFlags.mFlags |= 0x400;
		mFlags.mFlags |= 0x8000;
		SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		mPhase++;
		break;
	case 1:
		if (lbl_8025C400.mEvt.mData.mScript == NULL) {
			mFlags.mFlags &= ~0x40;
			mFlags.mFlags &= ~0x80;
			mFlags.mFlags &= ~0x100;
			mFlags.mFlags &= ~0x8000;
			TakeSlot(this);
			SetState(4);
		}
		break;
	case 2:
		break;
	}
}

/**
 * @note Address: 0x800ED2A4
 * @note Size: 0xC40
 */
// NONMATCHING: the spider count loop isn't strength reduced, the robot position temp sits elsewhere
// hangs from the ceiling and swings, drops down its thread when the robot comes near,
// climbs back, then takes a slot and walks off
void XObjSpider::State6(u32 frames)
{
	CVec chr(GetChrPos());
	CPad pad(0);
	switch (mPhase) {
	case 0:
		mCounter = rand() % 30;
		mPhase   = 10;
		break;
	case 10:
		if (--mCounter < 0) {
			mCounter = rand() % 30;
			SetAnim(0x11, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mPhase = 1;
		}
		break;
	case 1:
		mCounter++;
		mMoveTranslate.x = mHome.x + (f32)(8.0f * (f32)sin(DEG2RAD * (f32)((mCounter * 4) % 360))) - 4.0f;
		if (_25C == 11) {
			if (mFlags.mFlags & 0x200000) {
				fn_8001E904(&lbl_8025C400.mSound, 0x131, 255);
				mFlags.mFlags |= 0x100000;
				mPhase++;
				SetAnim(0x10, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mSpeed = -1.6f;
			}
		} else {
			f32 dist;
			if (mWall == 3) {
				f32 x = chr.x - mMoveTranslate.x;
				f32 y = chr.y - mMoveTranslate.y;
				f32 z = 0.0f;
				dist  = sqrtf(x * x + y * y + z * z);
			} else {
				f32 x = chr.x - mMoveTranslate.x;
				f32 y = 0.0f;
				f32 z = chr.z - mMoveTranslate.z;
				dist  = sqrtf(x * x + y * y + z * z);
			}
			if (dist <= 68.0f && lbl_8025C400.IsRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
				s32 dropped = 0;
				for (int i = 0; i < 16; i++) {
					XObjSpider* other = (XObjSpider*)lbl_8025C400.mXObjs.mPools[17].Get(i);
					if ((other->mStatus.mFlags & 1) && (other->mFlags.mFlags & 0x100000)) {
						dropped++;
					}
				}
				if (dropped < 10) {
					fn_8001E904(&lbl_8025C400.mSound, 0x131, 255);
					mFlags.mFlags |= 0x100000;
					mPhase++;
					SetAnim(0x10, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					mSpeed = -1.6f;
				}
			}
		}
		break;
	case 2:
		if (mMoveTranslate.y < 20.0f + mHome.y) {
			fn_8001E660(&lbl_8025C400.mSound, 0x131);
			mPhase++;
			SetAnim(0x12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mDir   = atan2(mTarget.x - mMoveTranslate.x, mTarget.z - mMoveTranslate.z);
			_988   = mDir;
			mSpeed = 0.0f;
		}
		break;
	case 3:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			mPhase++;
			SetAnim(0x13, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mDir   = atan2(mTarget.x - mMoveTranslate.x, mTarget.z - mMoveTranslate.z);
			_988   = mDir;
			mSpeed = -0.5f;
		}
		break;
	case 4:
		if (mMoveTranslate.y < 8.0f + mHome.y) {
			mPhase++;
			SetAnim(0x14, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_8001E904(&lbl_8025C400.mSound, 0x128, 255);
		}
		break;
	case 5:
		if (mMoveTranslate.y < mHome.y) {
			mMoveTranslate.y = mHome.y;
			mSpeed           = 0.0f;
		}
		f32 landEnd = GetAnimEndFrame(mJObj);
		if (landEnd - frames <= GetAnimFrame(mJObj)) {
			mFlags.mFlags &= ~0x40;
			mFlags.mFlags &= ~0x80;
			mFlags.mFlags &= ~0x100;
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mFlags.mFlags &= ~0x8000;
			TakeSlot(this);
			SetState(4);
		}
		break;
	case 6:
		if (pad.mPad->mTrigger & 0x200) {
			mFlags.mFlags = 0;
			mFlags.mFlags |= 0x40;
			mFlags.mFlags |= 0x80;
			mFlags.mFlags |= 0x100;
			mMoveTranslate.y = 48.0f + mHome.y;
			mPhase           = 0;
		}
		break;
	}
}

/**
 * @note Address: 0x800EC91C
 * @note Size: 0x988
 */
// backs off to its home spot, its steps sounding, then waits out the event
void XObjSpider::State7(u32 frames)
{
	switch (mPhase) {
	case 0:
		SetAnim(0xC, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		mSpeed = 0.0f;
		mPhase++;
		break;
	case 1:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			SetAnim(8, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mTarget.x = mHome.x;
			mTarget.y = mMoveTranslate.y;
			mTarget.z = mHome.z;
			mDir      = PI + (f32)atan2(mTarget.x - mMoveTranslate.x, mTarget.z - mMoveTranslate.z);
			mSpeed    = -0.5f;
			mPhase++;
		}
		break;
	case 2:
		if (1.0f <= GetAnimFrame(mJObj) && 1.0f >= GetAnimFrame(mJObj)) {
			PlayStep(this);
		} else if (15.0f <= GetAnimFrame(mJObj) && 15.0f >= GetAnimFrame(mJObj)) {
			PlayStep(this);
		}
		if (DistXZ(&mTarget, &mMoveTranslate) <= 2.0f) {
			mSpeed      = 0.0f;
			f32 walkEnd = GetAnimEndFrame(mJObj);
			if (walkEnd - frames <= GetAnimFrame(mJObj)) {
				SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				if (lbl_8025C400.mEvt.mData.mScript != NULL) {
					mPhase++;
				} else {
					SetState(4);
				}
			}
		}
		break;
	case 3:
		if (lbl_8025C400.mEvt.mData.mScript == NULL) {
			SetState(4);
		}
		break;
	}
}

/**
 * @note Address: 0x800EC918
 * @note Size: 0x4
 */
void XObjSpider::State8(u32 frames)
{
}

/**
 * @note Address: 0x800EBDBC
 * @note Size: 0x64
 */
// whether it is inside its area, kept mRadius off the edges
extern "C" BOOL fn_800EBDBC(XObjSpider* spider)
{
	f32 left  = spider->mAreaPos.x + spider->mRadius;
	f32 right = (spider->mAreaPos.x + spider->mAreaWidth) - spider->mRadius;
	f32 near  = spider->mAreaPos.z + spider->mRadius;
	f32 far   = (spider->mAreaPos.z + spider->mAreaDepth) - spider->mRadius;
	if (left > spider->mMoveTranslate.x || right < spider->mMoveTranslate.x || near > spider->mMoveTranslate.z
	    || far < spider->mMoveTranslate.z)
		return FALSE;
	return TRUE;
}

/**
 * @note Address: 0x800EBDB0
 * @note Size: 0xC
 */
extern "C" u32 fn_800EBDB0(XObjSpider* spider)
{
	return (spider->mFlags.mFlags >> 19) & 1;
}

/**
 * @note Address: 0x800EBBF0
 * @note Size: 0x1C0
 */
// whether another spider that dropped down is closer than 24
extern "C" BOOL fn_800EBBF0(XObjSpider* spider)
{
	for (int i = 0; i < 16; i++) {
		XObj* obj = lbl_8025C400.mXObjs.mPools[17].Get(i);
		if ((obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000)) {
			XObjSpider* other = (XObjSpider*)obj;
			if ((other->mFlags.mFlags & 0x100000) && other->_258 != spider->_258) {
				f32 x = other->mMoveTranslate.x - spider->mMoveTranslate.x;
				f32 y = other->mMoveTranslate.y - spider->mMoveTranslate.y;
				f32 z = other->mMoveTranslate.z - spider->mMoveTranslate.z;
				if (sqrtf(x * x + y * y + z * z) < 24.0f) {
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/**
 * @note Address: 0x800EBA30
 * @note Size: 0x1C0
 */
// the index of another spider that dropped down closer than 24, -1 when there is none
extern "C" s32 fn_800EBA30(XObjSpider* spider)
{
	for (int i = 0; i < 16; i++) {
		XObj* obj = lbl_8025C400.mXObjs.mPools[17].Get(i);
		if ((obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000)) {
			XObjSpider* other = (XObjSpider*)obj;
			if ((other->mFlags.mFlags & 0x100000) && other->_258 != spider->_258) {
				f32 x = other->mMoveTranslate.x - spider->mMoveTranslate.x;
				f32 y = other->mMoveTranslate.y - spider->mMoveTranslate.y;
				f32 z = other->mMoveTranslate.z - spider->mMoveTranslate.z;
				if (sqrtf(x * x + y * y + z * z) < 24.0f) {
					return other->_258;
				}
			}
		}
	}
	return -1;
}

/**
 * @note Address: 0x800EB928
 * @note Size: 0x108
 */
extern "C" void fn_800EB928(XObjSpider* spider)
{
	switch (spider->_258 % 5) {
	case 0:
		fn_8001E904(&lbl_8025C400.mSound, 0x12B, 255);
		break;
	case 1:
		fn_8001E904(&lbl_8025C400.mSound, 0x12C, 255);
		break;
	case 2:
		fn_8001E904(&lbl_8025C400.mSound, 0x12D, 255);
		break;
	case 3:
		fn_8001E904(&lbl_8025C400.mSound, 0x12E, 255);
		break;
	case 4:
		fn_8001E904(&lbl_8025C400.mSound, 0x12F, 255);
		break;
	}
}

/**
 * @note Address: 0x800EAF18
 * @note Size: 0xA10
 */
// NONMATCHING: retail has another 0x24 bytes of .rodata in front of the joint paths
// the slot model's anims, it opens, follows mObj's joint while the anim plays, then
// closes and hides
extern "C" void fn_800EAF18(CarryInfo* carry, u8 slot, u32 frames, s32 mode)
{
	if (slot == 255) {
		carry->mState = 0;
		carry->mFlags.mFlags &= ~1;
		return;
	}
	if (lbl_8037E5C0[slot] == NULL) {
		carry->mState = 0;
		carry->mFlags.mFlags &= ~1;
		return;
	}
	switch (carry->mState) {
	case 0:
		break;
	case 1:
		lbl_8037E5C0[slot]->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		carry->mState++;
		break;
	case 2:
		if (carry->mObj != NULL) {
			CJObj joint;
			s32 path[8] = { 0, 0, 0, 0, 1, 1, 0, 0 };
			joint.Attach(FindJointByPath(carry->mObj->mJObj, path, 8));
			carry->mPos = GetJointPosition(joint.mJObj);
		}
		f32 end = GetAnimEndFrame(lbl_8037E5C0[slot]->mJObj);
		if (end - frames <= GetAnimFrame(lbl_8037E5C0[slot]->mJObj)) {
			lbl_8037E5C0[slot]->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			carry->mState++;
		}
		break;
	case 3:
		if (carry->mObj != NULL) {
			CJObj joint;
			s32 path[8] = { 0, 0, 0, 0, 1, 1, 0, 0 };
			joint.Attach(FindJointByPath(carry->mObj->mJObj, path, 8));
			carry->mPos = GetJointPosition(joint.mJObj);
		}
		f32 closeEnd = GetAnimEndFrame(lbl_8037E5C0[slot]->mJObj);
		if (closeEnd - frames <= GetAnimFrame(lbl_8037E5C0[slot]->mJObj)) {
			lbl_8037E5C0[slot]->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			carry->mState = 0;
			carry->mFlags.mFlags &= ~1;
			carry->mFlags.mFlags &= ~2;
		}
		break;
	}
}

/**
 * @note Address: 0x800EBE20
 * @note Size: 0xAF8
 */
// a test mode, the pad's direction sends it a step that way, up its thread and back
void XObjSpider::State9(u32 frames)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x20) {
		mPhase           = 0;
		mSpeed           = 0.0f;
		mMoveTranslate.x = mHome.x;
		mMoveTranslate.y = mHome.y;
		mMoveTranslate.z = mHome.z;
		SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		mFlags.mFlags &= ~0x100;
	}
	switch (mPhase) {
	case 0:
		if (pad.mPad->mTrigger & 0x200) {
			u32 button = pad.mPad->mButton;
			if (button & 8) {
				mTarget.x = mHome.x;
				mTarget.y = mHome.y;
				mTarget.z = mHome.z - 60.0f;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mPhase++;
				mSpeed = 0.3f;
				mDir   = PI;
			} else if (button & 4) {
				mTarget.x = mHome.x;
				mTarget.y = mHome.y;
				mTarget.z = 60.0f + mHome.z;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mPhase++;
				mSpeed = 0.3f;
				mDir   = PI;
			} else if (button & 1) {
				mTarget.x = mHome.x - 60.0f;
				mTarget.y = mHome.y;
				mTarget.z = mHome.z;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mPhase++;
				mSpeed = 0.3f;
				mDir   = PI;
			} else if (button & 2) {
				mTarget.x = 60.0f + mHome.x;
				mTarget.y = mHome.y;
				mTarget.z = mHome.z;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mPhase++;
				mSpeed = 0.3f;
				mDir   = PI;
			}
		}
		break;
	case 1:
		if (HeadTo(this, mTarget) < 20.0f) {
			mPhase++;
			SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		break;
	case 2:
		if (HeadTo(this, mTarget) < 1.0f) {
			mFlags.mFlags |= 0x100;
			mSpeed = 0.0f;
		}
		HSD_AObj* aobj = GetAObj(mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(mJObj);
		if (end - frames <= (aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f)) {
			mFlags.mFlags |= 0x100;
			mSpeed = 0.0f;
			mPhase++;
			SetAnim(5, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mSpeed = 0.3f;
		}
		break;
	case 3:
		if (mMoveTranslate.y >= 80.0f) {
			mPhase++;
			mSpeed = -0.3f;
		}
		break;
	case 4:
		if (mMoveTranslate.y <= 20.0f + mHome.y) {
			mPhase++;
			mMoveTranslate.y = 0.0f;
			mFlags.mFlags &= ~0x100;
			mTarget.x = mHome.x;
			mTarget.y = mHome.y;
			mTarget.z = mHome.z;
			SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mSpeed = 0.3f;
		}
		break;
	case 5: {
		f32 distance = HeadTo(this, mTarget);
		s32 done     = 0;
		if (distance < 1.0f) {
			mSpeed = 0.0f;
			done   = 1;
		}
		f32 backEnd = GetAnimEndFrame(mJObj);
		if (backEnd - frames <= GetAnimFrame(mJObj)) {
			done++;
		}
		if (done >= 2) {
			mPhase = 0;
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		break;
	}
	}
}

/**
 * @note Address: 0x800EAB58
 * @note Size: 0x3C0
 */
extern "C" void fn_800EAB58(CarryInfo* carry, u8 slot, bool sideways)
{
	if (slot != 255 && (carry->mFlags.mFlags & 1)) {
		XObj* model = lbl_8037E5C0[slot];
		if (model != NULL) {
			HSD_JObj* joint = model->mJObj;
			HSD_JObjSetTranslateX(joint, carry->mPos.x);
			HSD_JObjSetTranslateY(joint, carry->mPos.y);
			HSD_JObjSetTranslateZ(joint, carry->mPos.z);
			if (sideways) {
				HSD_JObjSetRotationX(joint, HALF_PI);
				HSD_JObjSetRotationZ(joint, HALF_PI + carry->mAngle);
			} else {
				HSD_JObjSetRotationY(joint, carry->mAngle);
			}
			HSD_JObjDispAll(joint, NULL, 7, 0);
		}
	}
}

/**
 * @note Address: 0x800EA99C
 * @note Size: 0x1BC
 */
extern "C" void fn_800EA99C(void* unused)
{
	lbl_8065B2C4 = 0;
	for (int i = 0; i < 11; i++) {
		lbl_8037E640[i].mCoin  = NULL;
		lbl_8037E640[i].mPhase = 0;
		lbl_8037E640[i].mCount = 0;
	}
	for (int i = 0; i < 96; i++) {
		XObj* obj = lbl_8025C400.mXObjs.mPools[7].Get(i);
		if (obj->GetKind() == 0x104) {
			((XObjCoin*)obj)->mBouncing = 0;
		}
	}
}

/**
 * @note Address: 0x800EA780
 * @note Size: 0x21C
 */
// NONMATCHING: regalloc
// moves the coins, shown, hopping, then falling to where they land
extern "C" void fn_800EA780(void* unused, u32 frames)
{
	f32 fall = 0.8f * (f32)frames;
	for (s32 i = 0; i < 11; ++i) {
		CoinBounce* entry = &lbl_8037E640[i];
		XObj* coin        = entry->mCoin;
		if (coin != NULL) {
			s32 phase   = entry->mPhase;
			s32 counter = entry->mCount;
			f32 targetY = entry->mTarget.y;
			f32 x       = entry->mPos.x;
			f32 y       = entry->mPos.y;
			f32 z       = entry->mPos.z;
			switch (phase) {
			case 0:
				coin->mStatus.mFlags |= 0x10000ULL;
				++phase;
				break;
			case 1: {
				++counter;
				f32 height = 10.0f * (f32)sin(DEG2RAD * (f32)((counter * 16) % 360)) + 0.2f;
				if (height < 0.2f) {
					coin->SetTranslateY(y + 0.2f, 0);
					++phase;
				} else {
					coin->SetTranslateY(y + height, 0);
				}
				break;
			}
			case 2:
				y -= fall;
				if (y <= targetY) {
					coin->mStatus.mFlags &= ~0x100000ULL;
					y = targetY;
					++phase;
				}
				coin->SetTranslateY(y + 0.2f, 0);
				break;
			case 3:
				break;
			}
			entry->mPhase = phase;
			entry->mCount = counter;
			entry->mPos.x = x;
			entry->mPos.y = y;
			entry->mPos.z = z;
		}
	}
}

/**
 * @note Address: 0x800EA554
 * @note Size: 0x22C
 */
// NONMATCHING: the pool loop steps another register
// puts one of the free coins at position and plays its anim
extern "C" void fn_800EA554(Vec* position, Vec* target, s32 anim)
{
	if (lbl_8065B2C4 >= 11)
		return;
	u32 stride = lbl_8025C400.mXObjs.mPools[7].mStride;
	u8* pool   = lbl_8025C400.mXObjs.mPools[7].mObjects;
	u32 offset = 0;
	for (s32 i = 0; i < 96; ++i, offset += stride) {
		XObjCoin* coin = (XObjCoin*)(pool + offset);
		if (coin->mStatus.mFlags & 1ULL) {
			if (coin->GetKind() == 0x104) {
				if ((coin->mStatus.mFlags & 1ULL) && !coin->mBouncing) {
					coin->mBouncing                   = 1;
					lbl_8037E640[lbl_8065B2C4].mPhase = 0;
					lbl_8037E640[lbl_8065B2C4].mCount += 2;
					lbl_8037E640[lbl_8065B2C4].mPos.x    = position->x;
					lbl_8037E640[lbl_8065B2C4].mPos.y    = position->y + 0.2f;
					lbl_8037E640[lbl_8065B2C4].mPos.z    = position->z;
					lbl_8037E640[lbl_8065B2C4].mTarget.x = target->x;
					lbl_8037E640[lbl_8065B2C4].mTarget.y = target->y;
					lbl_8037E640[lbl_8065B2C4].mTarget.z = target->z;
					lbl_8037E640[lbl_8065B2C4].mCoin     = coin;
					lbl_8037E640[lbl_8065B2C4].mCoin->SetTranslateX(position->x, 0);
					lbl_8037E640[lbl_8065B2C4].mCoin->SetTranslateY(position->y + 0.2f, 0);
					lbl_8037E640[lbl_8065B2C4].mCoin->SetTranslateZ(position->z, 0);
					lbl_8037E640[lbl_8065B2C4].mCoin->mStatus.mFlags |= 0x100000ULL;
					lbl_8037E640[lbl_8065B2C4].mCoin->SetAnim(anim, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					++lbl_8065B2C4;
					break;
				}
			}
		}
	}
}

/**
 * @note Address: 0x800EA40C
 * @note Size: 0x148
 */
// NONMATCHING: the jobj and the anim id swap registers
void XObjSpider::vf148()
{
	if (mStatus.mFlags & 1ULL) {
		if (mStatus.mFlags & 0x10000ULL) {
			HSD_JObj* joint = mJObj;
			s32 id          = mAnimId;
			HSD_AObj* aobj  = GetAObj(joint);
			f32 frame       = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
			mSfx.Update(id, frame, fn_8001DF00(&lbl_8025C400.mSound, &mMoveTranslate, 0));
		}
	}
}
