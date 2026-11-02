#include "game/cgame.h"
#include "game/mic.h"
#include "game/cthread.h"
#include "game/mm.h"
#include <Dolphin/vi.h>
#include "game/cgameutil.h"

extern "C" {
int strncmp(const char* a, const char* b, unsigned long n);
}
extern "C" f32 lbl_8020FC10[4];
extern "C" f32 lbl_8020FC20[4];

/**
 * @note Address: 0x8000AD34
 * @note Size: 0x600
 */
// NONMATCHING: the loop's float registers are assigned in another order
// draws the stage name: a bubble per letter, then the letter with a shadow on four sides
extern "C" void fn_8000AD34(CGame* game)
{
	if (game->mStageName.mActive && !(game->mStatus.mFlags & 2)) {
		fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		CTexObj bubble = lbl_8025C400.mTpl.mTex.Init(&((TPLPalette*)lbl_8025C400.mTpl.mBuffer)->descriptorArray[138], 0);
		f32 zero       = 0.0f;
		u32 color      = game->mStageName.mColor & 0xFFFFFF00;
		for (s32 i = 0; i < game->mStageName.mCount; i++) {
			f32 scale = game->mStageName.mChars[i].mScale;
			if (scale <= zero || strncmp(game->mStageName.mChars[i].mText, " ", 1) == 0) {
				continue;
			}
			CFont font;
			s32 width;
			CTexObj letter = *fn_8019E8F4(&font, game->mStageName.mChars[i].mText, &width);
			f32 bob;
			f32 tilt;
			f32 swell;
			f32 pop;
			f32 x;
			f32 rot;
			f32 y;
			bob       = 4.0f * (f32)sin(game->mStageName.mChars[i].mBob);
			tilt      = sin(0.7f * game->mStageName.mChars[i].mTilt);
			swell     = sin(3.1415927f * scale);
			pop       = 0.2f * swell + (f32)sin(1.5707964f * scale);
			x         = 21.6f + (43.2f * i + (320.0f - 43.2f * game->mStageName.mCount / 2.0f));
			u32 alpha = 255.0f * scale;
			rot       = -(0.017453292f * (10.0f * tilt));
			y         = 150.0f + bob;
			fn_80197800(&lbl_8025C400.mGs, &bubble, alpha | 0xFFFFFF00, alpha | 0xFFFFFF00, alpha | 0xFFFFFF00, alpha | 0xFFFFFF00, x, y,
			            0.0f, 22.0f * (2.2f * pop), 22.0f * (2.2f * pop), zero, -zero, rot);
			f32 size   = 22.0f * (1.5f * pop);
			f32 shrink = width / 24.0f * size;
			f32 off    = size - shrink;
			for (s32 j = 0; j < 4; j++) {
				fn_80197800(&lbl_8025C400.mGs, &letter, alpha, alpha, alpha, alpha, off / 2.0f + (x + lbl_8020FC10[j]),
				            bob + (150.0f + lbl_8020FC20[j]), 0.0f, size, size, zero, -zero, rot);
			}
			fn_80197800(&lbl_8025C400.mGs, &letter, color | alpha, color | alpha, color | alpha, color | alpha, off / 2.0f + x, y, 0.0f,
			            size, size, zero, -zero, rot);
		}
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 7, 0);
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
	}
}

/**
 * @note Address: 0x8000BE38
 * @note Size: 0x560
 */
// draws the movie, centred at its own size unless _FF460 says otherwise. with alpha it's drawn
// three times over
extern "C" void fn_8000BE38(CGame* game)
{
	if (game->mMovie.mMovie.mState != 0) {
		f32 w   = fpclassify(game->mMovieRect[2]) == FP_NAN ? (f32)game->mMovie.mMovie.mVideoInfo.xSize : game->mMovieRect[2];
		f32 h   = fpclassify(game->mMovieRect[3]) == FP_NAN ? (f32)game->mMovie.mMovie.mVideoInfo.ySize : game->mMovieRect[3];
		f32 x   = fpclassify(game->mMovieRect[0]) == FP_NAN ? (640.0f - w) / 2 : game->mMovieRect[0];
		f32 y   = fpclassify(game->mMovieRect[1]) == FP_NAN ? (448.0f - h) / 2 : game->mMovieRect[1];
		CGs* gs = &lbl_8025C400.mGs;
		fn_8019B648(gs->mCurRenderMode, 1);
		fn_8019B5D0(gs->mCurRenderMode, 0);
		fn_8019B464(gs->mCurRenderMode, 1, 7, 0);
		if (game->mMovie.mMovie.mAlpha) {
			fn_8019B56C(gs->mCurRenderMode, 1, 4, 5, 15);
			fn_8019B504(gs->mCurRenderMode, 4, 0x20, 0, 3, 255);
			SetupAll(FALSE);
			if (game->mMovie.mMovie.BeginDraw() >= 0) {
				for (s32 i = 0; i < 3; i++) {
					game->mMovie.mMovie.Draw(x, y, w, h);
				}
				game->mMovie.mMovie.EndDraw();
			}
		} else {
			fn_8019B56C(gs->mCurRenderMode, 0, 4, 5, 15);
			fn_8019B504(gs->mCurRenderMode, 7, 0, 0, 7, 255);
			SetupAll(FALSE);
			game->mMovie.mMovie.DrawFrame(x, y, w, h);
		}
		fn_8019B504(gs->mCurRenderMode, 4, 0, 0, 3, 255);
		InvalidateAll();
	}
}

extern "C" {
s32 MICProbeEx(s32 chan);
s32 MICMountAsync(s32 chan, void* work, s32 size, void (*detach)(s32), void (*attach)(s32, s32));
s32 MICGetResultCode(s32 chan);
s32 MICGetDeviceID(s32 chan, u32* id);
s32 MICUnmount(s32 chan);
s32 M2SGetActiveChannel(void);
s32 M2SGetSamplesLeft(void);
void M2SGetSamples(s16** samples, s32 count);
void M2SAdvanceBuffer(s32 count);
void* memset(void* dst, int val, u32 size);
}

/**
 * @note Address: 0x8000A858
 * @note Size: 0x18
 */
extern "C" void fn_8000A858(s32 index)
{
	lbl_8020FC3C[index].mResult = -3;
}

/**
 * @note Address: 0x8000A6D8
 * @note Size: 0x180
 */
// NONMATCHING: the work area isn't read back for the mount
// mounts the mic in slot B and keeps it mounted
extern "C" void fn_8000A6D8(void)
{
	MicChannel* slot = &lbl_8020FC3C[1];
	s32 result;
	switch (slot->mState) {
	case 0:
		if (MICProbeEx(1) == 0) {
			slot->mState = 1;
		}
		break;
	case 1:
		slot->mBuffer = (s16*)OSAllocFromHeap(__OSCurrHeap, 0x3000);
		switch (result = MICMountAsync(1, slot->mBuffer, 0x3000, fn_8000A858, NULL)) {
		case 0:
			slot->mState = 2;
			break;
		case -1:
			break;
		default:
			goto failed;
		}
		break;
	case 2:
		switch (result = MICGetResultCode(1)) {
		case 0:
			MICGetDeviceID(1, &slot->mDeviceID);
			slot->mResult = 0;
			slot->mState  = 3;
			break;
		case -1:
			break;
		default:
			goto failed;
		}
		break;
	case 3:
		if (slot->mResult != 0) {
			MICUnmount(1);
			OSFreeToHeap(__OSCurrHeap, slot->mBuffer);
			slot->mBuffer = NULL;
			slot->mState  = 0;
		}
		break;
	}
	return;

failed:
	switch (result) {
	case -2:
		OSFreeToHeap(__OSCurrHeap, slot->mBuffer);
		slot->mBuffer = NULL;
		slot->mState  = 0;
		break;
	case -3:
		OSFreeToHeap(__OSCurrHeap, slot->mBuffer);
		slot->mBuffer = NULL;
		slot->mState  = 0;
		break;
	}
}

extern "C" s32 lbl_8020FC5C[3];
extern "C" s32 lbl_8020FC68[3];
extern "C" s32 lbl_8065B158;
extern "C" s32 lbl_8065B15C;
extern "C" s16* lbl_8065B160[2];
extern "C" u8 lbl_8065B168;
extern "C" u32 lbl_8065B16C[2];
extern "C" void (*lbl_8065B174)(void);

/**
 * @note Address: 0x8000A560
 * @note Size: 0x118
 */
// NONMATCHING: next and count swap registers
// pulls the next samples from the mic into the buffer, a faster step when more are waiting,
// and keeps the loudest ones
extern "C" void fn_8000A560(s16* buffer)
{
	memset(buffer, 0, 0x280);
	u32* pos = &lbl_8065B16C[M2SGetActiveChannel()];
	s32 left = M2SGetSamplesLeft();
	if (left < 0) {
		*pos = 0;
	} else if (left >= lbl_8020FC68[0]) {
		s32 step;
		if (left < lbl_8020FC68[1]) {
			step = lbl_8020FC5C[0];
		} else if (left < lbl_8020FC68[2]) {
			step = lbl_8020FC5C[1];
		} else {
			step = lbl_8020FC5C[2];
		}
		u32 next  = *pos + step * 159;
		s32 count = next >> 16;
		s16* samples;
		M2SGetSamples(&samples, count + 4);
		s16* sample = samples;
		for (s32 i = 0; i < count + 4; i++) {
			s32 value = *sample;
			if (value > lbl_8065B15C) {
				lbl_8065B15C = value;
			}
			if (value > 1000 && value > lbl_8065B158) {
				lbl_8065B158 = value;
			}
			sample++;
		}
		next += step;
		*pos = next & 0xFFFF;
		M2SAdvanceBuffer(next >> 16);
	}
}

extern "C" {
u32 CheckStageFlag(s32 stage, u32 mask);
BOOL M2SSetActiveChannel(s32 chan);
BOOL M2SSetMode(s32 mode);
BOOL M2SSetPrerecordSamples(s32 milliseconds);
void M2SOpen(void);
void M2SClose(void);
void M2SStart(void);
void M2SStop(void);
s32 MICSetGain(s32 chan, s32 gain);
s32 MICStart(s32 chan);
s32 MICGetButton(s32 chan, u32* button);
}
extern "C" BOOL lbl_8065B148;
extern "C" s64 lbl_8065B150;

// whether the stage takes the mic, which ended up true either way
static inline BOOL IsMicStage()
{
	if (CheckStageFlag(lbl_8025C400.mStage, 2)) {
		return TRUE;
	}
	return TRUE;
}

/**
 * @note Address: 0x8000A870
 * @note Size: 0x31C
 */
// the mic: opens it in whichever slot has one, records while its button is held and a little
// after, and hands the loudest sample to every object
extern "C" void fn_8000A870(u32 frames)
{
	if (lbl_8025C400.mStatus.mFlags & 1) {
		fn_8000A6D8();
		if (IsMicStage()) {
			if (lbl_80659ECC < 0) {
				if (lbl_8020FC3C[0].mResult == 0) {
					lbl_80659ECC = 0;
				} else if (lbl_8020FC3C[1].mResult == 0) {
					lbl_80659ECC = 1;
				}
				if (lbl_80659ECC >= 0 && lbl_8020FC3C[lbl_80659ECC].mDeviceID == 0) {
					M2SSetActiveChannel(lbl_80659ECC);
					M2SSetMode(3);
					M2SSetPrerecordSamples(400);
					M2SOpen();
					MICSetGain(lbl_80659ECC, 0);
					MICStart(lbl_80659ECC);
				}
			} else if (lbl_8020FC3C[lbl_80659ECC].mResult != 0) {
				M2SClose();
				lbl_80659ECC = -1;
			}
		}

		if (lbl_80659ECC >= 0 && lbl_8020FC3C[lbl_80659ECC].mDeviceID == 0) {
			u32 button;
			MICGetButton(lbl_80659ECC, &button);
			if (button & 0x10) {
				if (!lbl_8065B148 && lbl_8065B150 == 0) {
					M2SStart();
					lbl_8065B148 = TRUE;
				}
			} else if (lbl_8065B148 && lbl_8065B150 == 0) {
				lbl_8065B150 = OSGetTime();
			}
			if (OSGetTime() - lbl_8065B150 > OSMillisecondsToTicks(100) && lbl_8065B150 != 0 && lbl_8065B148) {
				M2SStop();
				lbl_8065B148 = FALSE;
				lbl_8065B150 = 0;
			}
		}

		if (lbl_8065B158 != 0) {
			if ((lbl_8025C400.mStatus.mFlags & 1) && !(lbl_8025C400.mStatus.mFlags & 2)) {
				for (s32 i = 0; i < lbl_8025C400.mDrawLists[0].mCount; i++) {
					if (lbl_8025C400.mDrawLists[0].mObjects[i]->mStatus.mFlags & 1) {
						lbl_8025C400.mDrawLists[0].mObjects[i]->vf144(-1, lbl_8065B158);
					}
				}
			}
			lbl_8065B158 = 0;
		}
	}
}

/**
 * @note Address: 0x8000A678
 * @note Size: 0x60
 */
extern "C" void fn_8000A678(void)
{
	lbl_8065B174();
	fn_8000A560(lbl_8065B160[lbl_8065B168]);
	DCFlushRange(lbl_8065B160[lbl_8065B168], 0x280);
	lbl_8065B168 ^= 1;
}

/**
 * @note Address: 0x8000AB8C
 * @note Size: 0x4
 */
extern "C" void fn_8000AB8C(void)
{
}

// the thread the movie plays on and its stack
extern "C" CThread lbl_80360AE8;
extern "C" u8 lbl_80360E10[0x1000];

/**
 * @note Address: 0x8000AC64
 * @note Size: 0xD0
 */
// NONMATCHING: count and result swap registers
// UpdateMovie, the movie thread: steps the movie once a retrace until it's done
extern "C" void* fn_8000AC64(void* arg)
{
	u32 count     = VIGetRetraceCount();
	CMovie* movie = &((CGame*)arg)->mMovie.mMovie;
	do {
		while (count == VIGetRetraceCount()) {
			VIWaitForRetrace();
		}
		count = VIGetRetraceCount();
		if (movie->mState != 0) {
			s32 result = movie->Update();
			if (result < 0) {
				movie->Stop();
				OSPanic("game.cpp", 3832, "%s%s", fn_801911B4("error on play movie! - %d\n", result), __FUNCTION__);
			}
		}
	} while (movie->mState != 0);
	return NULL;
}

/**
 * @note Address: 0x8000ABB8
 * @note Size: 0xAC
 */
// NONMATCHING: the flags are stored before the alpha bit is taken
// starts a movie and the thread that plays it, bit 0 of the flags plays it with alpha
extern "C" void fn_8000ABB8(CGame* game, const char* path, BOOL loop, u32 flags)
{
	game->mMovieFlags = flags;
	game->mMovie.mMovie.Start(path, loop, flags & 1, TRUE);
	game->mMovieRect[0] = game->mMovieRect[1] = game->mMovieRect[2] = game->mMovieRect[3] = NAN;
	lbl_80360AE8.Start(fn_8000AC64, game, lbl_80360E10, sizeof(lbl_80360E10));
}

extern "C" {
s32 fn_8006E7BC(void);
s32 fn_80087A40(void* card);
void fn_80061364(void* menu);
void fn_800821DC(CMsg* msg);
void CMsg_ResetPos(CMsg* msg);
void fn_8006F544(void);
void fn_801328C8(XOBJS* objs);
void psKillAllParticle(void);
void fn_8001E11C(XSound* sound, BOOL pause);
void fn_8001F8D8(XSound* sound);
void fn_80030CE8(void* title);
void fn_80031510(void* menu);
void fn_8006D5A4(void* info);
u8* fn_8006DFC4(s32 index);
int rand(void);
}
extern "C" GXColor lbl_80659EC0;

// a lead byte of a two byte shift jis character
static inline bool IsSJISByte(char c)
{
	return ((u8)c >= 0x81 && (u8)c <= 0x9F) || ((u8)c >= 0xE0 && (u8)c <= 0xFC);
}

static inline bool IsSJISLead(char c)
{
	return IsSJISByte(c) ? true : false;
}

// shuts a wipe off where it is
static inline void ClearWipe(CScrnWipe* wipe)
{
	fn_800236EC(wipe, 0, 0, 0, 0, TRUE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

/**
 * @note Address: 0x8000B394
 * @note Size: 0xAA4
 */
// NONMATCHING: the name loop's counter and letter pointer swap registers
// changes to another stage: 10000 is the title, 20000 the stage from the card, below 0 the
// next one. everything that belongs to the old stage is let go of, and the new stage's name
// is set up to pop in when showName is set
extern "C" void fn_8000B394(CGame* game, s32 stage, BOOL showName)
{
	fn_80023EE0(&game->mFade, 0, 0, 0, 255, 0, 0, 0, 255, 0.0f);
	if (stage == 10000) {
		s32 first = lbl_8020FBC0.mStage;
		if (first < 0 || (game->mStatus.mFlags & 0x8000000000000)) {
			game->SetStage(fn_8006E7BC());
		} else {
			game->SetStage(first);
		}
	} else if (stage == 20000) {
		game->SetStage(fn_80087A40(&game->mMemcard));
		game->mMemcardLoaded = TRUE;
	} else if (stage >= 0) {
		game->SetStage(stage);
	} else {
		game->mPrevStage = game->mStage;
		if (++game->mStage >= game->_344C) {
			game->mStage = 0;
		}
	}

	if (game->mMovie.mMovie.mState != 0) {
		game->mMovie.mMovie.Stop();
	}
	fn_80061364(game->mMenu.mMenus[0]);
	fn_8007F040(&game->mEvt, 1, 1);
	for (s32 i = 0; i < 2; i++) {
		fn_800821DC(&game->mCMsgs[i]);
		CMsg_ResetPos(&game->mCMsgs[i]);
		game->mCMsgs[i].SetColor(lbl_80659EC0);
	}
	for (s32 i = 0; i < 5; i++) {
		FbMirrorEx* mirror = &lbl_8035FA24[i];
		if (mirror->IsUsed()) {
			if (mirror->mJoint != NULL) {
				mirror->mJoint   = NULL;
				mirror->mVisible = FALSE;
				if (mirror->mActive && mirror->mTexture.mImage != NULL) {
					mm::Free(mirror->mTexture.mImage, 1);
					mirror->mTexture.mImage = NULL;
				}
			}
			mirror->mOwner = NULL;
		}
	}
	fn_8006F544();
	game->mLightList = game->mLights;
	fn_801328C8(&game->mXObjs);
	lbl_80659ECC = -1;
	game->mParticle.KillAll();
	psKillAllParticle();
	if (game->mStatus.mFlags & 4) {
		if (game->mStatus.mFlags & 4) {
			game->mStatus.mFlags &= ~4;
			fn_8001E11C(&lbl_8025C400.mSound, FALSE);
			fn_801CDC34();
		}
	}
	fn_8001E630(&lbl_8025C400.mSound, -1);
	fn_8001F8D8(&game->mSound);
	game->mStatus.mFlags &= ~0x20000000002F;
	game->mStatus.mFlags &= ~0xC00;
	game->mStatus.mFlags &= ~0x2000;
	game->mStatus.mFlags &= ~0x300;
	ClearWipe(&game->mRoboeyes);
	game->mStatus.mFlags |= 0x40000000000000;
	game->mStatus.mFlags |= 0x800000000000;
	fn_80066190(&game->mInfoScrn, 0, 0);

	if (stage == 10000) {
		game->mStatus.mFlags |= 0x20000000;
		game->mStatus.mFlags |= 0x1000000000000;
		game->mStatus.mFlags &= ~0x80;
		fn_80030CE8(&game->mTitle);
		fn_80031510(&game->mMenu);
		fn_8006D5A4(&game->mInfoScrn);
		game->mSubwin.Reset();
		fn_80023EE0(&game->mFade, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f);
		ClearWipe(&game->mWipeCircle);
		ClearWipe(&game->mWipeTex);
		ClearWipe(&game->mWipeCustom);
		ClearWipe(&game->mWipeTile0);
		fn_80023438(&game->mWipeTile1, 0, 0, 0, 0, 0, 0, 0, 320.0f, 224.0f, 640.0f, 448.0f, 640.0f, 448.0f, 0.0f, 0, TRUE);
		fn_800242A8(&game->mQuake, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		game->mBlur.mAlpha  = 0.0f;
		game->mBlur.mTarget = 0.0f;
		game->_3338         = 19;
	} else if (stage == 20000) {
		game->mSubwin.Reset();
		fn_80023EE0(&game->mFade, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f);
		ClearWipe(&game->mWipeCircle);
		ClearWipe(&game->mWipeTex);
		ClearWipe(&game->mWipeCustom);
		ClearWipe(&game->mWipeTile0);
		fn_80023438(&game->mWipeTile1, 0, 0, 0, 0, 0, 0, 0, 320.0f, 224.0f, 640.0f, 448.0f, 640.0f, 448.0f, 0.0f, 0, TRUE);
		fn_800242A8(&game->mQuake, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		game->mBlur.mAlpha  = 0.0f;
		game->mBlur.mTarget = 0.0f;
		game->_3338         = 20;
	} else {
		game->_3338 = 21;
	}

	if (showName) {
		game->mStageName.mActive = TRUE;
		game->mStageName.mCount  = 0;
		s32 cur                  = game->mStage;
		if (cur < GetStageNum()) {
			char* name = (char*)fn_8006DFC4(game->mStage);
			if (name != NULL) {
				game->mStageName.mColor = 0x80FFFF00;
				for (s32 i = 0; i < 20 && *name != '\0'; i++, name++) {
					game->mStageName.mChars[i].mText  = name;
					game->mStageName.mChars[i].mScale = -0.2f * i;
					game->mStageName.mChars[i].mWait  = 120.0f;
					game->mStageName.mChars[i].mBob   = DEG2RAD * (rand() % 360);
					game->mStageName.mChars[i].mTilt  = DEG2RAD * (rand() % 360);
					game->mStageName.mCount++;
#if defined(VERSION_JP)
					if (IsSJISLead(*name)) {
						name++;
					}
#else
					if (lbl_8025C400.mLanguage == 6 && IsSJISLead(*name)) {
						name++;
					}
#endif
				}
				for (s32 i = game->mStageName.mCount; i < 20; i++) {
					game->mStageName.mChars[i].mText  = NULL;
					game->mStageName.mChars[i].mScale = 0.0f;
					game->mStageName.mChars[i].mWait  = 0.0f;
				}
			}
		}
	}
}

/**
 * @note Address: 0x8000AB90
 * @note Size: 0x28
 */
extern "C" void fn_8000AB90(CGame* game)
{
	game->mMovie.mMovie.Stop();
}
