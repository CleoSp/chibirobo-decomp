#include "game/xchr.h"
#include "game/cgame.h"
#include "game/infoscrn.h"
#include <stdlib.h>
extern "C" {
#include <sysdolphin/mtx.h>
#include "game/gmath.h"
#include "game/csound.h"
#include "game/evt.h"
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

static inline u8 IsStopped(XObjChr* chr)
{
	return (u8)(chr->mParams.mBattery <= 0.0f && fn_800665E0((InfoScrn*)lbl_8025C400.mInfoScrn));
}

static inline u8 IsDown(XObjChr* chr)
{
	return (u8)(IsStopped(chr) && chr->_158A4 != 0);
}

static inline u8 IsEventHeld(XObjChr* chr)
{
	return (u8)((lbl_8025C400.mStatus.mFlags & 0x40000000ULL) && IsDown(chr));
}

static inline u8 IsTumbling(XObjChr* chr)
{
	u8 result = 0;
	if (chr->mTumble.mState == 4 || chr->mTumble.mState == 5 || chr->mTumble.mState == 6)
		result = 1;
	return result;
}

static inline u8 IsEventBlocked(CGame* game)
{
	XObjChr* chr = game->mPlayer;
	u8 blocked   = 0;
	if (IsEventHeld(chr) && IsTumbling(chr))
		blocked = 1;
	return blocked;
}

/**
 * @note Address: 0x800B504C
 * @note Size: 0x130
 */
extern "C" void fn_800B504C(CGame* game, const void* script, void* context, u32 first, u32 second)
{
	if (!IsEventBlocked(game)) {
		fn_8007EEA4((CEvt*)&game->_5D07C, (u8*)script, (u32)context, first, second);
	}
}

static inline u8 IsDiskBusy(CGame* game)
{
	return (u8)((game->mDvdStatus || game->mPrevDvdStatus) && game->mDiskOk == 0);
}

/**
 * @note Address: 0x800B517C
 * @note Size: 0x80
 */
extern "C" u8 fn_800B517C(CGame* game)
{
	return (u8)((game->mStatus.mFlags & 1) && !(game->mStatus.mFlags & 0x22) && !IsDiskBusy(game));
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
		for (s32 i = 0; i < 2; ++i) {
			chr->mOXMenu.mMark[i].mSpin  = DEG2RAD * (rand() % 360);
			chr->mOXMenu.mMark[i].mTilt  = DEG2RAD * (rand() % 360);
			chr->mOXMenu.mMark[i].mScale = 0.5f;
		}
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
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
	if (dir == 0) {
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
		if (chr->mMenu.mAtc._280 != 3)
			chr->mMenu.mSel = chr->mMenu.mPrevSel;
	} else if (dir > 0) {
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
	} else if (dir < 0) {
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
		if (chr->mMenu.mAtc._280 != 3)
			chr->mMenu.mSel = chr->mMenu.mPrevSel;
	}
	chr->mMenu.mAtc.Select(dir);
	chr->mMenu.mDrs.Select(dir);
}

static inline u8 IsMenuUp(XObjChr* chr)
{
	return (u8)(chr->mMenu.mAtc.mOpen > 0 || chr->mMenu.mDrs.mOpen > 0);
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
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
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
