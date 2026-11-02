#ifndef _GAME_CGAMEUTIL_H
#define _GAME_CGAMEUTIL_H

#include "types.h"
#include "game/cgame.h"
#include "game/stagedata.h"

// the inlines the cgame units share

extern "C" {
void fn_8019B278(CGsRenderMode* mode, BOOL force);
void fn_8019AED0(CGsTevStage* stage, BOOL force);
void fn_8019AE94(CGsChannel* chan, BOOL force);
void HSD_PadRumbleOffN(u8 no);
void HSD_PadRumblePauseAll(void);
s32 DVDGetDriveStatus(void);
BOOL DVDCheckDisk(void);
void HSD_PadRumbleActiveAll(void);
}

// sends all of the current render, tev and channel state
static inline void SetupAll(BOOL force)
{
	CGs* gs = &lbl_8025C400.mGs;
	if (gs->mCurRenderMode != NULL) {
		fn_8019B278(gs->mCurRenderMode, force);
	}
	if (gs->mCurTevStage != NULL) {
		fn_8019AED0(gs->mCurTevStage, force);
	}
	if (gs->mCurChannel != NULL) {
		fn_8019AE94(gs->mCurChannel, force);
	}
}

// forgets all of the state after drawing behind its back
static inline void InvalidateAll()
{
	CGs* gs = &lbl_8025C400.mGs;
	HSD_StateInvalidate(HSD_STATE_ALL);
	if (gs->mCurRenderMode != NULL) {
		gs->mCurRenderMode->Invalidate();
	}
	if (gs->mCurTevStage != NULL) {
		gs->mCurTevStage->Invalidate();
	}
	if (gs->mCurChannel != NULL) {
		if (!gs->mCurChannel->mDirty) {
			gs->mCurChannel->mDirty = TRUE;
		}
	}
}

// a mirror is off while its owner is gone or hidden
static inline bool IsMirrorOff(FbMirrorEx* mirror)
{
	bool off = false;
	if (mirror->mOwner == NULL || (mirror->mOwner->mStatus.mFlags & 0x1000000000000)) {
		off = true;
	}
	return off;
}

// the player, only the objects it points at
struct UnkPlayer : public XObj {
	u8 _988[0x9E0 - 0x988]; // _988
	XObj* mShadowObj;       // _9E0, the one its shadow height comes from
	u8 _9E4[0xA48 - 0x9E4]; // _9E4
	s32 _A48;               // _A48, the action, 0 and 4 are idle
	u8 _A4C[0xA64 - 0xA4C]; // _A4C
	XObj* _A64;             // _A64
	u8 _A68[0xA94 - 0xA68]; // _A68
	XObj* _A94;             // _A94
	u8 _A98[0xAC4 - 0xA98]; // _A98
	s32 _AC4;               // _AC4
	u8 _AC8[0xB14 - 0xAC8]; // _AC8
	s32 _B14;               // _B14
	u8 _B18[0xB68 - 0xB18]; // _B18
	s32 _B68;               // _B68
	u8 _B6C[0xB84 - 0xB6C]; // _B6C
	XObj* _B84;             // _B84
	u8 _B88[0xBAC - 0xB88]; // _B88
	s32 _BAC;               // _BAC
	u8 _BB0[0xBC8 - 0xBB0]; // _BB0
	XObj* _BC8;             // _BC8
	u8 _BCC[0xBEC - 0xBCC]; // _BCC
	s32 _BEC;               // _BEC
	u8 _BF0[0xC08 - 0xBF0]; // _BF0
	XObj* _C08;             // _C08
	u8 _C0C[0xC44 - 0xC0C]; // _C0C
	s32 _C44;               // _C44

	bool IsIdle() const
	{
		bool idle = false;
		if ((_A48 == 0 || _A48 == 4) && _AC4 == 0 && _B14 == 0 && _B68 == 0 && _BAC == 0 && _BEC == 0 && _C44 == 0) {
			idle = true;
		}
		return idle;
	}

	XObj* GetShadowObj()
	{
		if (mShadowObj) {
			return mShadowObj;
		}
		return NULL;
	}
};

// the current stage's settings
struct UnkStageInfo {
	u8 _00[0x6];        // _00
	s16 mCameraMode;    // _06
	u8 mMirrorFade;     // _08
	u8 _09[0x10 - 0x9]; // _09
	u8 mFlags;          // _10, bit 0 keeps the shadows at full strength
};
STATIC_ASSERT(sizeof(UnkStageInfo) == 0x12);

// the loaded stage's EnvInfo starts with a pointer to them, see game/stagedata.h
static inline UnkStageInfo* GetStageInfo()
{
	return *(UnkStageInfo**)fn_8006F4B8();
}

// CBase::Update, keeps the rumble off while the disc can't be read and watches for the reset
// button or B X start held for a second. CGame::Update has it inlined
static inline void BaseUpdate(CBase* base, u32 frames)
{
	base->mPrevDvdStatus = base->mDvdStatus;
	base->mDvdStatus     = DVDGetDriveStatus();
	base->mDiskOk        = DVDCheckDisk();
	switch (base->mDvdStatus) {
	case -1:
	case 4:
	case 5:
	case 6:
	case 11:
		if (base->mDvdStatus != base->mPrevDvdStatus) {
			CPadEx* pad     = &base->mPad;
			base->mDvdError = TRUE;
			for (s32 i = 0; i < 4; i++) {
				if (pad->mPad[i].mConnected) {
					HSD_PadRumbleOffN(pad->mPad[i].mPort);
				}
			}
			HSD_PadRumblePauseAll();
		}
		break;
	case 0:
		if (base->mPrevDvdStatus != 0 && base->mDvdError) {
			base->mDvdError = FALSE;
			HSD_PadRumbleActiveAll();
		}
		break;
	}

	if (!base->mReset) {
		if (HSD_PadGetResetSwitch()) {
			base->mResetHold = 0;
			base->mReset     = TRUE;
		} else if ((base->mPad.mPad[0].mButton & 0xFFFF) == 0x1600) {
#if defined(VERSION_PAL)
			if ((base->mResetHold += base->mRetraceStep) > lbl_80660D4C) {
#else
			if ((base->mResetHold += base->mRetraceStep) > 60) {
#endif
				base->mResetHold = 0;
				base->mReset     = TRUE;
			}
		} else if (!base->mReset) {
			base->mResetHold = 0;
		}
	}
}

// what the command line can set
struct CmdLineArgs {
	s32 mStage;   // _00
	s32 mSlot;    // _04, the card slot, 0 for A
	u8 _08[0x18]; // _08
};
STATIC_ASSERT(sizeof(CmdLineArgs) == 0x20);

extern "C" CmdLineArgs lbl_8020FBC0;

#endif
