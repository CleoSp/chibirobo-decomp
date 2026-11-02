#include "types.h"
#include "Version.h"
#include "game/hsd_jobj.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cjobjset.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/cptcl.h"
#include "game/cscrn.h"
#include "game/csound.h"
#include "game/globals.h"
#include "game/gmath.h"
#include "game/logo.h"
#include "game/memcard.h"
#include "game/menusave.h"
#include "game/title.h"
#include "game/video.h"
#include "game/xobjdemo.h"
#include "game/xobjs.h"
#include "Dolphin/OS/OSUtil.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// game functions 0x8001F8D8-0x80031004: the end of the sound object, the screen wipes and
// fades, the dust particles, the logos and the title screen. most names are lost, so they
// are extern "C" under their dol address

extern "C" {
void InitAudio(void* owner, u32 heapSize, u32 voices, u32 groups);
s32 HSD_SynthSFXNewBank(s32 size);
s32 HSD_SynthSFXBankClear(s32 bank);
s32 HSD_SynthSFXGetPendingLoadCount();
s32 HSD_SynthSFXLoad(const char* name, s32 bank, void (*callback)(s32, s32), s32 arg);
BOOL fn_801E8C18(void* data);
void HSD_JObjSetDPtclCallback(void (*callback)(int, int, int, HSD_JObj*));
void fn_8001FCBC(int sfx, int low, int high, HSD_JObj* joint);
extern const char* stageSfxTable[];
void OSReport(const char* format, ...);
void OSPanic(const char* file, int line, const char* format, ...);
void fn_8001E00C(XSound* sound, BOOL mono);
void HSD_PadRumbleOn(u8 no);
void HSD_PadRumbleOffN(u8 no);
void HSD_PadRumbleRemove(u8 no);
// a window frame, see the menus
void fn_800652C0(u32 color0, u32 color1, f32 x, f32 y, f32 w, f32 h, f32 sx, f32 sy, f32 sz);
void fn_80068B98(void* info);
s32 fn_80035184(OSTime first, OSTime second, OSTime third);
// the shadow offsets of the title's name letters
extern f32 lbl_8021195C[4];
extern f32 lbl_8021196C[4];

// Title's mode steps, the PMF table at lbl_802116D8 picks one by mMode
void State20__5TitleFUl(Title* title);
void fn_80030CE8(Title* title);
}

// drops every sfx name waiting to load and empties the stage bank
static inline void ClearStageBank(XSound* sound)
{
	for (u32 i = 0; i < 16; ++i) {
		SfxLoadRequest* request = &sound->mLoadRequests[i];
		request->mName          = NULL;
		request->mResult        = -1;
		request->mLoaded        = 0;
	}
	sound->mLoadRequestCount = 0;
	HSD_SynthSFXBankClear(sound->mStageBank);
}

// queues a name for the stage bank unless it is already there, sound.cpp has the same
static inline void RegisterName(XSound* sound, const char* name)
{
	s32 bank = sound->mStageBank;
	if (name == NULL) {
		return;
	}
	for (u32 i = 0; i < 16; ++i) {
		if (sound->mLoadRequests[i].mName != NULL && strcmp(sound->mLoadRequests[i].mName, name) == 0) {
			return;
		}
	}
	for (u32 i = 0; i < 16; ++i) {
		if (sound->mLoadRequests[i].mName == NULL) {
			++sound->mLoadRequestCount;
			sound->mLoadRequests[i].mName = name;
			sound->mLoadRequests[i].mBank = bank;
#if !defined(VERSION_PAL)
			OSReport("Register:%s\n", name);
#endif
			return;
		}
	}
#if !defined(VERSION_PAL)
	OSReport("Ssm::Load xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
#endif
}

/**
 * @note Address: 0x8001F8D8
 * @note Size: 0xA4
 */
// NONMATCHING: the requests are reached through an object at _2D0
extern "C" void fn_8001F8D8(XSound* sound)
{
	ClearStageBank(sound);
}

/**
 * @note Address: 0x8001F97C
 * @note Size: 0x2D8
 */
// NONMATCHING: the strings are in sound.cpp's rodata
// a negative stage loads the shared samples a step per call, TRUE once they're in
extern "C" BOOL fn_8001F97C(XSound* sound, s32 stage)
{
	if (stage < 0) {
		switch (sound->mLoadState) {
		case 0:
			sound->mSampleReady = 0;
			if (sound->mFile.ReadAsync("sfx_sample.sem", 0, 0, FALSE) >= 0) {
				++sound->mLoadState;
			} else {
#if defined(VERSION_JP)
				OSPanic("sound.cpp", 0xED, "%s%s", "", "Read");
#else
				OSPanic("sound.cpp", 0xF1, "%s%s", "", "Read");
#endif
			}
			break;
		case 1: {
			s32 result = sound->mFile.GetResult();
			if (result == 0) {
				++sound->mLoadState;
			} else if (result < 0) {
#if defined(VERSION_JP)
				OSPanic("sound.cpp", 0xF5, "%s%s", "", "Read");
#else
				OSPanic("sound.cpp", 0xF9, "%s%s", "", "Read");
#endif
			}
			break;
		}
		case 2:
			fn_801E8C18(sound->mFile.mBuffer);
			HSD_SynthSFXLoad("sfx_sample.ssm", sound->mSampleBank, NULL, 0);
			++sound->mLoadState;
			break;
		case 3:
			if (!(bool)HSD_SynthSFXGetPendingLoadCount()) {
				sound->mLoadState   = 0;
				sound->mSampleReady = 1;
				return TRUE;
			}
			break;
		default:
			sound->mLoadState = 0;
			break;
		}
		return FALSE;
	}
	sound->mDefaultTrack = 0;
	ClearStageBank(sound);
#if !defined(VERSION_PAL)
	OSReport(">>>%s\n", stageSfxTable[stage]);
#endif
	RegisterName(sound, stageSfxTable[stage]);
	return TRUE;
}

/**
 * @note Address: 0x8001FC54
 * @note Size: 0x68
 */
extern "C" void fn_8001FC54(XSound* sound)
{
	InitAudio(sound, 0x700000, 0x60, 0x20);
	sound->mSampleBank  = HSD_SynthSFXNewBank(0x480000);
	sound->mStageBank   = HSD_SynthSFXNewBank(0x280000);
	sound->mLoadState   = 0;
	sound->mSampleReady = 0;
	HSD_JObjSetDPtclCallback(fn_8001FCBC);
}

/**
 * @note Address: 0x8001FCBC
 * @note Size: 0x38
 */
// particle sounds from joint anims
extern "C" void fn_8001FCBC(int sfx, int low, int high, HSD_JObj* joint)
{
	fn_8001E904(&lbl_8025C400.mSound, sfx, 255);
}

/**
 * @note Address: 0x80021474
 * @note Size: 0x8C
 */
extern "C" void Update__13CScrnRoboeyesFUl(CScrnRoboeyes* eyes, u32 frames)
{
	Update__9CScrnWipeFUl(eyes, frames);
	if (eyes->mActive) {
		eyes->mAngle += 0.02617994f * frames;
		eyes->mPhase += 0.10471976f * frames;
	}
}

/**
 * @note Address: 0x80021500
 * @note Size: 0x70
 */
extern "C" void Reset__13CScrnRoboeyesFv(CScrnRoboeyes* eyes)
{
	eyes->mActive = FALSE;
	eyes->mX = eyes->mY = 0.0f;
	eyes->mWidth = eyes->mHeight = 0.0f;
	eyes->mTargetWidth = eyes->mTargetHeight = 0.0f;
	eyes->mSpeedWidth = eyes->mSpeedHeight = 0.0f;
	eyes->mColor.r                         = 0.0f;
	eyes->mColor.g                         = 0.0f;
	eyes->mColor.b                         = 0.0f;
	eyes->mColor.a                         = 0.0f;
	eyes->mTarget.r                        = 0.0f;
	eyes->mTarget.g                        = 0.0f;
	eyes->mTarget.b                        = 0.0f;
	eyes->mTarget.a                        = 0.0f;
	eyes->mSpeed.r                         = 0.0f;
	eyes->mSpeed.g                         = 0.0f;
	eyes->mSpeed.b                         = 0.0f;
	eyes->mSpeed.a                         = 0.0f;
	eyes->_5C = eyes->_60 = 0.0f;
	eyes->mAngle          = 0.0f;
	eyes->mPhase          = 0.0f;
}

// steps a value towards its target, it stops there
static inline void StepChannel(f32& current, f32& target, f32& speed, u32 frames)
{
	if (current != target) {
		current += speed * frames;
		if (speed > 0.0f && current >= target) {
			current = target;
			speed   = 0.0f;
		}
		if (speed < 0.0f && current <= target) {
			current = target;
			speed   = 0.0f;
		}
	}
}

static inline bool SizeStopped(CScrnWipe* wipe)
{
	return 0.0f == wipe->mSpeedWidth && 0.0f == wipe->mSpeedHeight;
}

/**
 * @note Address: 0x80023030
 * @note Size: 0x408
 */
// a wipe's update: it turns, sizes its hole and fades its color, and goes off once the size
// stops when it was asked to
extern "C" void Update__9CScrnWipeFUl(CScrnWipe* wipe, u32 frames)
{
	if (!wipe->mActive) {
		return;
	}
	if (0.0f != wipe->_60) {
		wipe->_5C = fmod(wipe->_60 * frames + wipe->_5C, TWO_PI);
	}
	StepChannel(wipe->mWidth, wipe->mTargetWidth, wipe->mSpeedWidth, frames);
	StepChannel(wipe->mHeight, wipe->mTargetHeight, wipe->mSpeedHeight, frames);
	StepChannel(wipe->mColor.r, wipe->mTarget.r, wipe->mSpeed.r, frames);
	StepChannel(wipe->mColor.g, wipe->mTarget.g, wipe->mSpeed.g, frames);
	StepChannel(wipe->mColor.b, wipe->mTarget.b, wipe->mSpeed.b, frames);
	StepChannel(wipe->mColor.a, wipe->mTarget.a, wipe->mSpeed.a, frames);
	if (wipe->mHideWhenDone && SizeStopped(wipe)) {
		wipe->mActive = FALSE;
	}
}

/**
 * @note Address: 0x80023438
 * @note Size: 0x2B4
 */
// starts a wipe, the color and the hole size both go from the first values to the second
extern "C" void fn_80023438(CScrnWipe* wipe, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, f32 x, f32 y, f32 width0, f32 height0,
                            f32 width1, f32 height1, f32 time, u8 a1, BOOL hideWhenDone)
{
	wipe->mActive       = TRUE;
	wipe->mHideWhenDone = hideWhenDone;
	wipe->mX            = x;
	wipe->mY            = y;
	wipe->mWidth        = width0;
	wipe->mHeight       = height0;
	wipe->mTargetWidth  = width1;
	wipe->mTargetHeight = height1;
	if (0.0f == time) {
		wipe->mWidth  = width1;
		wipe->mHeight = height1;
	} else {
		wipe->mSpeedWidth  = (width1 - width0) / (FRAME_RATE * time);
		wipe->mSpeedHeight = (height1 - height0) / (FRAME_RATE * time);
	}
	wipe->mColor.r  = r0;
	wipe->mColor.g  = g0;
	wipe->mColor.b  = b0;
	wipe->mColor.a  = a0;
	wipe->mTarget.r = r1;
	wipe->mTarget.g = g1;
	wipe->mTarget.b = b1;
	wipe->mTarget.a = a1;
	if (0.0f == time) {
		wipe->mColor.r = r1;
		wipe->mColor.g = g1;
		wipe->mColor.b = b1;
		wipe->mColor.a = a1;
		wipe->mSpeed.r = 0.0f;
		wipe->mSpeed.g = 0.0f;
		wipe->mSpeed.b = 0.0f;
		wipe->mSpeed.a = 0.0f;
	} else {
		time           = FRAME_RATE * time;
		wipe->mSpeed.r = ((f32)r1 - (f32)r0) / time;
		wipe->mSpeed.g = ((f32)g1 - (f32)g0) / time;
		wipe->mSpeed.b = ((f32)b1 - (f32)b0) / time;
		wipe->mSpeed.a = ((f32)a1 - (f32)a0) / time;
	}
	if (wipe->mHideWhenDone && SizeStopped(wipe))
		wipe->mActive = FALSE;
}

/**
 * @note Address: 0x800236EC
 * @note Size: 0x1B8
 */
// the same with one color
extern "C" void fn_800236EC(CScrnWipe* wipe, u8 r, u8 g, u8 b, u8 a, BOOL hideWhenDone, f32 x, f32 y, f32 width0, f32 height0, f32 width1,
                            f32 height1, f32 time)
{
	wipe->mActive       = TRUE;
	wipe->mHideWhenDone = hideWhenDone;
	wipe->mX            = x;
	wipe->mY            = y;
	wipe->mWidth        = width0;
	wipe->mHeight       = height0;
	wipe->mTargetWidth  = width1;
	wipe->mTargetHeight = height1;
	if (0.0f == time) {
		wipe->mWidth      = width1;
		wipe->mHeight     = height1;
		wipe->mSpeedWidth = wipe->mSpeedHeight = 0.0f;
	} else {
		wipe->mSpeedWidth  = (width1 - width0) / (FRAME_RATE * time);
		wipe->mSpeedHeight = (height1 - height0) / (FRAME_RATE * time);
	}
	wipe->mColor.r  = r;
	wipe->mColor.g  = g;
	wipe->mColor.b  = b;
	wipe->mColor.a  = a;
	wipe->mTarget.r = r;
	wipe->mTarget.g = g;
	wipe->mTarget.b = b;
	wipe->mTarget.a = a;
	wipe->mSpeed.r  = r;
	wipe->mSpeed.g  = g;
	wipe->mSpeed.b  = b;
	wipe->mSpeed.a  = a;
	if (wipe->mHideWhenDone && SizeStopped(wipe))
		wipe->mActive = FALSE;
}

/**
 * @note Address: 0x800238A4
 * @note Size: 0x68
 */
extern "C" void Reset__9CScrnWipeFv(CScrnWipe* wipe)
{
	wipe->mActive = FALSE;
	wipe->mX = wipe->mY = 0.0f;
	wipe->mWidth = wipe->mHeight = 0.0f;
	wipe->mTargetWidth = wipe->mTargetHeight = 0.0f;
	wipe->mSpeedWidth = wipe->mSpeedHeight = 0.0f;
	wipe->mColor.r                         = 0.0f;
	wipe->mColor.g                         = 0.0f;
	wipe->mColor.b                         = 0.0f;
	wipe->mColor.a                         = 0.0f;
	wipe->mTarget.r                        = 0.0f;
	wipe->mTarget.g                        = 0.0f;
	wipe->mTarget.b                        = 0.0f;
	wipe->mTarget.a                        = 0.0f;
	wipe->mSpeed.r                         = 0.0f;
	wipe->mSpeed.g                         = 0.0f;
	wipe->mSpeed.b                         = 0.0f;
	wipe->mSpeed.a                         = 0.0f;
	wipe->_5C = wipe->_60 = 0.0f;
}

static inline bool HasColor(CScrnColor* color)
{
	return 0.0f != color->mColor.r || 0.0f != color->mColor.g || 0.0f != color->mColor.b || 0.0f != color->mColor.a;
}

/**
 * @note Address: 0x8002390C
 * @note Size: 0x22C
 */
// NONMATCHING: the quad template's address is loaded first
extern "C" void fn_8002390C(CScrnColor* color, CGs* gs)
{
	if (HasColor(color)) {
		Vec quad[4] = { { 0.0f, 0.0f, 0.0f }, { 640.0f, 0.0f, 0.0f }, { 640.0f, 448.0f, 0.0f }, { 0.0f, 448.0f, 0.0f } };
		u32 packed  = ((u32)color->mColor.r << 24) | ((u32)color->mColor.g << 16) | ((u32)color->mColor.b << 8) | (u32)color->mColor.a;
		fn_80199AB0(gs, 0, 0);
		fn_8019B56C(gs->mCurRenderMode, 1, 4, 5, 15);
		fn_8019B464(gs->mCurRenderMode, 1, 1, 0);
		fn_80198614(gs, quad[0].x, quad[0].y, quad[0].z, packed, quad[1].x, quad[1].y, quad[1].z, packed, quad[2].x, quad[2].y, quad[2].z,
		            packed, quad[3].x, quad[3].y, quad[3].z, packed);
	}
}

/**
 * @note Address: 0x80023B38
 * @note Size: 0x23C
 */
extern "C" void fn_80023B38(CScrnColor* color, u32 frames)
{
	StepChannel(color->mColor.r, color->mTarget.r, color->mSpeed.r, frames);
	StepChannel(color->mColor.g, color->mTarget.g, color->mSpeed.g, frames);
	StepChannel(color->mColor.b, color->mTarget.b, color->mSpeed.b, frames);
	StepChannel(color->mColor.a, color->mTarget.a, color->mSpeed.a, frames);
}

/**
 * @note Address: 0x80023D74
 * @note Size: 0x16C
 */
extern "C" void fn_80023D74(CScrnColor* color, u8 r, u8 g, u8 b, u8 a, f32 time)
{
	color->mTarget.r = r;
	color->mTarget.g = g;
	color->mTarget.b = b;
	color->mTarget.a = a;
	if (0.0f == time) {
		color->mColor.r = r;
		color->mColor.g = g;
		color->mColor.b = b;
		color->mColor.a = a;
		color->mSpeed.r = 0.0f;
		color->mSpeed.g = 0.0f;
		color->mSpeed.b = 0.0f;
		color->mSpeed.a = 0.0f;
	} else {
		time            = FRAME_RATE * time;
		f32 sr          = ((f32)r - color->mColor.r) / time;
		f32 sg          = ((f32)g - color->mColor.g) / time;
		f32 sb          = ((f32)b - color->mColor.b) / time;
		f32 sa          = ((f32)a - color->mColor.a) / time;
		color->mSpeed.r = sr;
		color->mSpeed.g = sg;
		color->mSpeed.b = sb;
		color->mSpeed.a = sa;
	}
}

/**
 * @note Address: 0x80023EE0
 * @note Size: 0x20C
 */
extern "C" void fn_80023EE0(CScrnColor* color, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, u8 a1, f32 time)
{
	color->mColor.r  = r0;
	color->mColor.g  = g0;
	color->mColor.b  = b0;
	color->mColor.a  = a0;
	color->mTarget.r = r1;
	color->mTarget.g = g1;
	color->mTarget.b = b1;
	color->mTarget.a = a1;
	if (0.0f == time) {
		color->mColor.r = r1;
		color->mColor.g = g1;
		color->mColor.b = b1;
		color->mColor.a = a1;
		color->mSpeed.r = 0.0f;
		color->mSpeed.g = 0.0f;
		color->mSpeed.b = 0.0f;
		color->mSpeed.a = 0.0f;
	} else {
		time            = FRAME_RATE * time;
		color->mSpeed.r = ((f32)r1 - (f32)r0) / time;
		color->mSpeed.g = ((f32)g1 - (f32)g0) / time;
		color->mSpeed.b = ((f32)b1 - (f32)b0) / time;
		color->mSpeed.a = ((f32)a1 - (f32)a0) / time;
	}
}

/**
 * @note Address: 0x800240EC
 * @note Size: 0x1BC
 */
extern "C" void fn_800240EC(CScrnQuake* quake, u32 frames)
{
	StepChannel(quake->mShake[0].x, quake->mShake[1].x, quake->mShake[2].x, frames);
	StepChannel(quake->mShake[0].y, quake->mShake[1].y, quake->mShake[2].y, frames);
	StepChannel(quake->mShake[0].z, quake->mShake[1].z, quake->mShake[2].z, frames);
	quake->mSign = -quake->mSign;
}

static inline void CopyVector(CVec& destination, const Vec& source)
{
	destination.x = source.x;
	destination.y = source.y;
	destination.z = source.z;
}

/**
 * @note Address: 0x800242A8
 * @note Size: 0x184
 */
extern "C" void fn_800242A8(CScrnQuake* quake, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 time)
{
	quake->mShake[0].Set(x0, y0, z0);
	quake->mShake[0].x = Abs(quake->mShake[0].x);
	quake->mShake[0].y = Abs(quake->mShake[0].y);
	quake->mShake[0].z = Abs(quake->mShake[0].z);
	quake->mShake[1].Set(x1, y1, z1);
	quake->mShake[1].x = Abs(quake->mShake[1].x);
	quake->mShake[1].y = Abs(quake->mShake[1].y);
	quake->mShake[1].z = Abs(quake->mShake[1].z);
	if (time == 0.0f) {
		CopyVector(quake->mShake[0], quake->mShake[1]);
		quake->mShake[2].Set(0.0f, 0.0f, 0.0f);
	} else {
		CopyVector(quake->mShake[2], (quake->mShake[1] - quake->mShake[0]) / (FRAME_RATE * time));
	}
	quake->mSign = 1;
}

/**
 * @note Address: 0x8002442C
 * @note Size: 0x74
 */
CVec CVec::operator/(f32 divisor) const
{
	CVec result = *this;
	result.x /= divisor;
	result.y /= divisor;
	result.z /= divisor;
	return result;
}

/**
 * @note Address: 0x800244A0
 * @note Size: 0x84
 */
CVec CVec::operator-(Vec other) const
{
	CVec result = *this;
	PSVECSubtract(&result, &other, &result);
	return result;
}

/**
 * @note Address: 0x80024524
 * @note Size: 0x34
 */
extern "C" void fn_80024524(CScrnQuake* quake)
{
	quake->mShake[0].Set(0.0f, 0.0f, 0.0f);
	quake->mShake[1].Set(0.0f, 0.0f, 0.0f);
	quake->mShake[2].Set(0.0f, 0.0f, 0.0f);
	quake->mSign = 1;
}

/**
 * @note Address: 0x800245F0
 * @note Size: 0x10
 */
void CPtclOne::Init()
{
	mActive = FALSE;
	mTex    = NULL;
}

/**
 * @note Address: 0x80024600
 * @note Size: 0x1D8
 */
void CPtclDust3::Draw(CGs* gs)
{
	if (!mActive) {
		return;
	}
	f32 width = mSize.x * (f32)sin(0.8f * (PI * mAlpha));
	width *= 2.0f;
	f32 height = mSize.y * (f32)sin(0.8f * (PI * mAlpha));
	height *= 2.0f;
	u32 edgeColor = mColor & 0xFFFFFF00;
	u32 color     = (mColor & 0xFFFFFF00) | ((u32)(s32)((f32)(mColor & 0xFF) * mAlpha) & 0xFF);
	if (mTex != NULL) {
		fn_8019AFA0(gs->mCurTevStage, 0);
		Vec rot = { 0.0f, 0.0f, 0.0f };
		rot.z   = mAngle;
		fn_80197800(gs, mTex, color, color, color, color, mPos.x, mPos.y, mPos.z, width, height, rot.x, rot.y, rot.z);
		fn_8019AFA0(gs->mCurTevStage, 4);
	} else {
		Vec rot = { 0.0f, 0.0f, 0.0f };
		rot.z   = mAngle;
		fn_80198258(gs, 8, color, edgeColor, mPos.x, mPos.y, mPos.z, width, height, rot.x, rot.y, rot.z);
	}
}

// moves a position by an offset passed by value
static inline void Move(CVec& pos, Vec offset)
{
	PSVECAdd(&pos, &offset, &pos);
}

/**
 * @note Address: 0x800247D8
 * @note Size: 0x15C
 */
void CPtclDust3::Update(u32 frames)
{
	if (!mActive) {
		return;
	}
	if (mAlphaStep < 0.0f) {
		if ((mAlpha -= mAlphaStep * (f32)frames) >= 1.0f) {
			mAlphaStep = -mAlphaStep;
		}
	}
	if (mAlphaStep > 0.0f) {
		if ((mAlpha -= mAlphaStep * (f32)frames) <= 0.0f) {
			mActive = FALSE;
			mAlpha  = 0.0f;
			return;
		}
	}
	Move(mPos, (mVel * 1.0f) * (f32)frames);
	mAngle += mAngleSpeed;
}

/**
 * @note Address: 0x80024934
 * @note Size: 0x1E8
 */
void CPtclDust2::Draw(CGs* gs)
{
	if (!mActive) {
		return;
	}
	u32 color  = (mColor & 0xFFFFFF00) | ((u32)(s32)((f32)(mColor & 0xFF) * mAlpha) & 0xFF);
	f32 radius = 1.5f * (mSize.x > mSize.y ? mSize.x : mSize.y);
	if (mTex != NULL) {
		fn_8019AFA0(gs->mCurTevStage, 0);
		Vec rot = { 0.0f, 0.0f, 0.0f };
		rot.z   = mAngle;
		fn_80197800(gs, mTex, color, color, color, color, mPos.x, mPos.y, mPos.z, radius, radius, rot.x, rot.y, rot.z);
		fn_8019AFA0(gs->mCurTevStage, 4);
	} else {
		// two triangles make a star
		Vec first = { 0.0f, 0.0f, 0.0f };
		first.z   = mAngle;
		fn_80198258(gs, 3, color, color, mPos.x, mPos.y, mPos.z, radius, radius, first.x, first.y, first.z);
		Vec second = { 0.0f, 0.0f, 0.0f };
		second.z   = THIRD_PI + mAngle;
		fn_80198258(gs, 3, color, color, mPos.x, mPos.y, mPos.z, radius, radius, second.x, second.y, second.z);
	}
}

/**
 * @note Address: 0x80024B1C
 * @note Size: 0x184
 */
void CPtclDust2::Update(u32 frames)
{
	if (!mActive) {
		return;
	}
	if (mAlphaStep < 0.0f) {
		if ((mAlpha -= mAlphaStep * (f32)frames) >= 1.0f) {
			mAlphaStep = -mAlphaStep;
		}
	}
	if (mAlphaStep > 0.0f) {
		if ((mAlpha -= mAlphaStep * (f32)frames) <= 0.0f) {
			mActive = FALSE;
			mAlpha  = 0.0f;
			return;
		}
	}
	mVel.y -= 0.05f * (f32)frames;
	Move(mPos, (mVel * 1.0f) * (f32)frames);
	mAngle += mAngleSpeed;
}

/**
 * @note Address: 0x80024CA0
 * @note Size: 0xE0
 */
extern "C" void fn_80024CA0(CPtcl* ptcl)
{
	CGs* gs = &lbl_8025C400.mGs;
	fn_80199AB0(gs, 0, 0);
	fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 3, 0);
	lbl_8025C400.mGs._29C = 1;
	for (s32 i = 0; i < ptcl->mCount; ++i) {
		CPtclOne* one = ptcl->Get(i);
		if (one->mActive != FALSE) {
			one->Draw(&lbl_8025C400.mGs);
		}
	}
	lbl_8025C400.mGs._29C = 0;
}

/**
 * @note Address: 0x80024D80
 * @note Size: 0x4
 */
void CPtclOne::Draw(CGs* gs)
{
}

/**
 * @note Address: 0x80024D84
 * @note Size: 0x84
 */
extern "C" void fn_80024D84(CPtcl* ptcl, u32 frames)
{
	for (s32 i = 0; i < ptcl->mCount; ++i) {
		CPtclOne* one = ptcl->Get(i);
		if (one->mActive != FALSE) {
			one->Update(frames);
		}
	}
}

/**
 * @note Address: 0x80024E08
 * @note Size: 0x4
 */
void CPtclOne::Update(u32 frames)
{
}

/**
 * @note Address: 0x80024E0C
 * @note Size: 0x44
 */
extern "C" void fn_80024E0C(CPtcl* ptcl)
{
	for (s32 i = 0; i < ptcl->mCount; ++i) {
		CPtclOne* one = ptcl->Get(i);
		if (one->mActive != FALSE) {
			one->mActive = FALSE;
			one->mTex    = NULL;
		}
	}
}

/**
 * @note Address: 0x80024E50
 * @note Size: 0x15C
 */
extern "C" CPtclOne* fn_80024E50(CPtcl* ptcl, const Vec* pos, const Vec* size, const Vec* vel, u32 color)
{
	for (s32 i = 0; i < ptcl->mCount; ++i) {
		CPtclOne* one = ptcl->Get(i);
		if (one->mActive == FALSE) {
			one->mActive     = TRUE;
			one->mPos        = CVec(*pos);
			one->mVel        = CVec(*vel);
			one->mSize       = CVec(*size);
			one->mAngle      = (f32)fmod((f32)rand(), PI);
			one->mAngleSpeed = 0.0f;
			one->mColor      = color;
			one->mAlpha      = 1.0f;
			one->mAlphaStep  = 0.02f;
			one->mTex        = NULL;
			return one;
		}
	}
	return NULL;
}

/**
 * @note Address: 0x80024FAC
 * @note Size: 0x84
 */
extern "C" void fn_80024FAC(CPtcl* ptcl, void* ptcls, s32 stride, s32 count)
{
	ptcl->mPtcls   = (u8*)ptcls;
	ptcl->mStride  = stride;
	ptcl->mCount   = count;
	ptcl->mCounter = 0;
	for (s32 i = 0; i < count; ++i) {
		ptcl->Get(i)->Init();
	}
}

/**
 * @note Address: 0x80025150
 * @note Size: 0xD8
 */
// Cursor's SetScaleY
extern "C" void SetScaleY__6CursorFfl(Cursor* cursor, f32 y, s32 force)
{
	y                            = -y;
	cursor->mMoveScale.y         = y;
	cursor->mMoveScale.mTarget.y = y;
	if (cursor->mJObj != NULL) {
		cursor->CJObj::SetScaleY(y, force);
	}
}

/**
 * @note Address: 0x80025228
 * @note Size: 0x190
 */
// Cursor's SetScale(scale)
extern "C" void SetScale__6CursorFfl(Cursor* cursor, f32 scale, s32 force)
{
	f32 x = scale;
	f32 y = -scale;
	f32 z = scale;

	cursor->mMoveScale.x         = x;
	cursor->mMoveScale.y         = y;
	cursor->mMoveScale.z         = z;
	cursor->mMoveScale.mTarget.x = x;
	cursor->mMoveScale.mTarget.y = y;
	cursor->mMoveScale.mTarget.z = z;
	if (cursor->mJObj) {
		if (force == 0) {
			Vec v = { x, y, z };
			if (!(cursor->mScale != v)) {
				return;
			}
		}
		cursor->mScale.x = x;
		cursor->mScale.y = y;
		cursor->mScale.z = z;
		HSD_JObjSetScale(cursor->mJObj, &cursor->mScale);
	}
}

/**
 * @note Address: 0x800253B8
 * @note Size: 0x190
 */
// Cursor's SetScale(x, y, z)
extern "C" void SetScale__6CursorFfffl(Cursor* cursor, f32 x, f32 y, f32 z, s32 force)
{
	y = -y;

	cursor->mMoveScale.x         = x;
	cursor->mMoveScale.y         = y;
	cursor->mMoveScale.z         = z;
	cursor->mMoveScale.mTarget.x = x;
	cursor->mMoveScale.mTarget.y = y;
	cursor->mMoveScale.mTarget.z = z;
	if (cursor->mJObj) {
		if (force == 0) {
			Vec v = { x, y, z };
			if (!(cursor->mScale != v)) {
				return;
			}
		}
		cursor->mScale.x = x;
		cursor->mScale.y = y;
		cursor->mScale.z = z;
		HSD_JObjSetScale(cursor->mJObj, &cursor->mScale);
	}
}

/**
 * @note Address: 0x80025548
 * @note Size: 0x19C
 */
// Cursor's SetScale(v)
extern "C" void SetScale__6CursorFRC3Vecl(Cursor* cursor, const Vec& scale, s32 force)
{
	f32 x = scale.x;
	f32 y = -scale.y;
	f32 z = scale.z;

	cursor->mMoveScale.x         = x;
	cursor->mMoveScale.y         = y;
	cursor->mMoveScale.z         = z;
	cursor->mMoveScale.mTarget.x = x;
	cursor->mMoveScale.mTarget.y = y;
	cursor->mMoveScale.mTarget.z = z;
	if (cursor->mJObj) {
		if (force == 0) {
			Vec v = { x, y, z };
			if (!(cursor->mScale != v)) {
				return;
			}
		}
		cursor->mScale.x = x;
		cursor->mScale.y = y;
		cursor->mScale.z = z;
		HSD_JObjSetScale(cursor->mJObj, &cursor->mScale);
	}
}

// animation ids from 1000 up are in the object's own anim archive once that is read, the
// rest are in its model archive. the last three digits pick the animation
static inline s32 AnimationGroup(XObj* object, s32 id)
{
	bool extra = id >= 1000;
	if (extra) {
		extra = object->mAnim.mArchive != NULL;
	}
	return extra ? id / 1000 : 0;
}

/**
 * @note Address: 0x800256E4
 * @note Size: 0x38
 */
// Cursor's SetAnim, keeps the classical scale
extern "C" void SetAnim__6CursorFliPvPvPvffff(Cursor* object, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim,
                                              f32 frame, f32 rate, f32 blend, f32 blendRate)
{
	SetAnim__4XObjFliPvPvPvffff(object, id, force, jointAnim, matAnim, shapeAnim, frame, rate, blend, blendRate);
	HSD_JObjSetFlags(object->mJObj, CLASSICAL_SCALE);
}

/**
 * @note Address: 0x8002571C
 * @note Size: 0x4
 */
// XObj's vf94
extern "C" void vf94__4XObjFv()
{
}

/**
 * @note Address: 0x80025720
 * @note Size: 0x24
 */
// XObj's vfA0
extern "C" BOOL vfA0__4XObjFPCc(XObj* object, const char* path)
{
	return fn_8008A38C(&object->mAnim, path);
}

/**
 * @note Address: 0x80025744
 * @note Size: 0x4
 */
// XObj's vfCC
extern "C" void vfCC__4XObjFv()
{
}

/**
 * @note Address: 0x80025748
 * @note Size: 0x1C
 */
// XObj's GetCameraTarget
extern "C" Vec GetCameraTarget__4XObjFv(CXObj* object)
{
	return object->mMoveTranslate;
}

/**
 * @note Address: 0x80025764
 * @note Size: 0xF4
 */
// XObj's vfF0
extern "C" HSD_AnimJoint* vfF0__4XObjFl(XObj* object, s32 id)
{
	if (AnimationGroup(object, id)) {
		HSD_AnimJoint** animations = object->mAnim.mAnimJoint;
		return animations != NULL ? animations[(id % 1000) % 1000] : NULL;
	}
	HSD_AnimJoint** animations = object->mArchive.mAnimJoint;
	return animations != NULL ? animations[id % 1000] : NULL;
}

/**
 * @note Address: 0x80025858
 * @note Size: 0xF4
 */
// XObj's vfF4
extern "C" HSD_MatAnimJoint* vfF4__4XObjFl(XObj* object, s32 id)
{
	if (AnimationGroup(object, id)) {
		HSD_MatAnimJoint** animations = object->mAnim.mMatAnimJoint;
		return animations != NULL ? animations[(id % 1000) % 1000] : NULL;
	}
	HSD_MatAnimJoint** animations = object->mArchive.mMatAnimJoint;
	return animations != NULL ? animations[id % 1000] : NULL;
}

/**
 * @note Address: 0x8002594C
 * @note Size: 0xF4
 */
// XObj's vfF8
extern "C" HSD_ShapeAnimJoint* vfF8__4XObjFl(XObj* object, s32 id)
{
	if (AnimationGroup(object, id)) {
		HSD_ShapeAnimJoint** animations = object->mAnim.mShapeAnimJoint;
		return animations != NULL ? animations[(id % 1000) % 1000] : NULL;
	}
	HSD_ShapeAnimJoint** animations = object->mArchive.mShapeAnimJoint;
	return animations != NULL ? animations[id % 1000] : NULL;
}

/**
 * @note Address: 0x80025A40
 * @note Size: 0x8
 */
// XObj's vf100
extern "C" s32 vf100__4XObjFll(void* object, s32 unused, s32 value)
{
	return value;
}

/**
 * @note Address: 0x80025A48
 * @note Size: 0x50
 */
// NONMATCHING: the loop test is placed differently
// XObj's vf124
extern "C" s32 vf124__4XObjFlPl(void* object, s32 count)
{
	for (s32 i = 0; i < count; ++i) { }
	return 0;
}

/**
 * @note Address: 0x80025A98
 * @note Size: 0x8
 */
// XObj's vf128
extern "C" s32 vf128__4XObjFv(XObj* object)
{
	return object->_80C;
}

/**
 * @note Address: 0x80025AA0
 * @note Size: 0x4
 */
// XObj's vf130
extern "C" void vf130__4XObjFv()
{
}

/**
 * @note Address: 0x80025AA4
 * @note Size: 0xD8
 */
// CXObj's SetTranslateX
extern "C" void SetTranslateX__5CXObjFfl(CXObj* object, f32 x, s32 force)
{
	object->mMoveTranslate.x         = x;
	object->mMoveTranslate.mTarget.x = x;
	if (object->mJObj != NULL) {
		object->CJObj::SetTranslateX(x, force);
	}
}

/**
 * @note Address: 0x80025B7C
 * @note Size: 0xD8
 */
// CXObj's SetTranslateY
extern "C" void SetTranslateY__5CXObjFfl(CXObj* object, f32 y, s32 force)
{
	object->mMoveTranslate.y         = y;
	object->mMoveTranslate.mTarget.y = y;
	if (object->mJObj != NULL) {
		object->CJObj::SetTranslateY(y, force);
	}
}

/**
 * @note Address: 0x80025C54
 * @note Size: 0xD8
 */
// CXObj's SetTranslateZ
extern "C" void SetTranslateZ__5CXObjFfl(CXObj* object, f32 z, s32 force)
{
	object->mMoveTranslate.z         = z;
	object->mMoveTranslate.mTarget.z = z;
	if (object->mJObj != NULL) {
		object->CJObj::SetTranslateZ(z, force);
	}
}

/**
 * @note Address: 0x80025D2C
 * @note Size: 0xD8
 */
// CXObj's SetScaleX
extern "C" void SetScaleX__5CXObjFfl(CXObj* object, f32 x, s32 force)
{
	object->mMoveScale.x         = x;
	object->mMoveScale.mTarget.x = x;
	if (object->mJObj != NULL) {
		object->CJObj::SetScaleX(x, force);
	}
}

/**
 * @note Address: 0x80025E04
 * @note Size: 0xD8
 */
// CXObj's SetScaleZ
extern "C" void SetScaleZ__5CXObjFfl(CXObj* object, f32 z, s32 force)
{
	object->mMoveScale.z         = z;
	object->mMoveScale.mTarget.z = z;
	if (object->mJObj != NULL) {
		object->CJObj::SetScaleZ(z, force);
	}
}

/**
 * @note Address: 0x80025EDC
 * @note Size: 0x1B0
 */
// CXObj's SetRotationX
extern "C" void SetRotationX__5CXObjFfl(CXObj* object, f32 x, s32 force)
{
	object->mMoveRotate.x         = x;
	object->mMoveRotate.mTarget.x = x;
	if (object->mJObj != NULL) {
		object->CJObj::SetRotationX(x, force);
	}
}

/**
 * @note Address: 0x8002608C
 * @note Size: 0x1B0
 */
// CXObj's SetRotationY
extern "C" void SetRotationY__5CXObjFfl(CXObj* object, f32 y, s32 force)
{
	object->mMoveRotate.y         = y;
	object->mMoveRotate.mTarget.y = y;
	if (object->mJObj != NULL) {
		object->CJObj::SetRotationY(y, force);
	}
}

/**
 * @note Address: 0x8002623C
 * @note Size: 0x1B0
 */
// CXObj's SetRotationZ
extern "C" void SetRotationZ__5CXObjFfl(CXObj* object, f32 z, s32 force)
{
	object->mMoveRotate.z         = z;
	object->mMoveRotate.mTarget.z = z;
	if (object->mJObj != NULL) {
		object->CJObj::SetRotationZ(z, force);
	}
}

/**
 * @note Address: 0x800263EC
 * @note Size: 0x190
 */
// CXObj's SetPos(x, y, z)
extern "C" void SetPos__5CXObjFfffl(CXObj* obj, f32 x, f32 y, f32 z, s32 force)
{
	obj->mMoveTranslate.x         = x;
	obj->mMoveTranslate.y         = y;
	obj->mMoveTranslate.z         = z;
	obj->mMoveTranslate.mTarget.x = x;
	obj->mMoveTranslate.mTarget.y = y;
	obj->mMoveTranslate.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v = { x, y, z };
			if (!(obj->mTranslate != v)) {
				return;
			}
		}
		obj->mTranslate.x = x;
		obj->mTranslate.y = y;
		obj->mTranslate.z = z;
		HSD_JObjSetTranslate(obj->mJObj, &obj->mTranslate);
	}
}

/**
 * @note Address: 0x8002657C
 * @note Size: 0x17C
 */
// CXObj's SetPos(pos)
extern "C" void SetPos__5CXObjFRC3Vecl(CXObj* obj, const Vec& v, s32 force)
{
	obj->mMoveTranslate.Set(v);
	obj->mMoveTranslate.mTarget.Set(v);
	if (obj->mJObj) {
		obj->CJObj::SetTranslate(v, force);
	}
}

/**
 * @note Address: 0x800266F8
 * @note Size: 0x240
 */
// CXObj's SetRot(x, y, z)
extern "C" void SetRot__5CXObjFfffl(CXObj* obj, f32 x, f32 y, f32 z, s32 force)
{
	obj->mMoveRotate.x         = x;
	obj->mMoveRotate.y         = y;
	obj->mMoveRotate.z         = z;
	obj->mMoveRotate.mTarget.x = x;
	obj->mMoveRotate.mTarget.y = y;
	obj->mMoveRotate.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v = { x, y, z };
			// tests the cached translation, as CJObj's does
			if (!(obj->mTranslate != v)) {
				return;
			}
		}
		obj->mRotate.x = x;
		obj->mRotate.y = y;
		obj->mRotate.z = z;
		obj->UpdateRotation();
	}
}

/**
 * @note Address: 0x80026938
 * @note Size: 0x22C
 */
// CXObj's SetRot(rot)
extern "C" void SetRot__5CXObjFRC3Vecl(CXObj* obj, const Vec& v, s32 force)
{
	obj->mMoveRotate.Set(v);
	obj->mMoveRotate.mTarget.Set(v);
	if (obj->mJObj) {
		obj->CJObj::SetRotation(v, force);
	}
}

/**
 * @note Address: 0x80026B64
 * @note Size: 0x38
 */
// Cursor's vfAC
extern "C" void vfAC__6CursorFv(Cursor* cursor)
{
	if (cursor->mJObj != NULL) {
		HSD_JObjDispAll(cursor->mJObj, NULL, 7, 0);
	}
}

/**
 * @note Address: 0x80026B9C
 * @note Size: 0x90
 */
// Cursor's update, spins it
extern "C" void vfA4__6CursorFUl(Cursor* cursor, u32 frames)
{
	cursor->mAngle = fmod(cursor->mAngleSpeed * (f32)frames + cursor->mAngle, TWO_PI);
	cursor->SetRotationZ(cursor->mAngle, 0);
	vfA4__4XObjFUl(cursor, frames);
}

/**
 * @note Address: 0x80026C2C
 * @note Size: 0x4C
 */
extern "C" BOOL vf9C__6CursorFRC9CoordInfo(Cursor* cursor, CoordInfo* info)
{
	if (vf9C__4XObjFRC9CoordInfo(cursor, info)) {
		HSD_JObjSetFlags(cursor->mJObj, CLASSICAL_SCALE);
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80026C78
 * @note Size: 0x38
 */
extern "C" void vf90__6CursorFUl(Cursor* cursor, u32 id)
{
	vf90__4XObjFUl(cursor, id);
	cursor->mAngleSpeed = 0.0f;
	cursor->mAngle      = 0.0f;
}

/**
 * @note Address: 0x80026CB0
 * @note Size: 0x878
 */
// NONMATCHING: float regalloc
// Logo::Draw, the current logo centred on the screen at its alpha. the first one has a shine
// that comes and goes with the angle
extern "C" void fn_80026CB0(Logo* logo)
{
	if (logo->IsActive() && logo->mAlpha > 0.0f) {
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
		fn_8019B648(lbl_8025C400.mGs.mCurRenderMode, 1);
		fn_8019B5D0(lbl_8025C400.mGs.mCurRenderMode, 0);
		fn_8019B504(lbl_8025C400.mGs.mCurRenderMode, 4, 0, 0, 3, 255);
		fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 7, 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		CTexObj tex;
		if (logo->mState == 3 || logo->mState == 4 || logo->mState == 5) {
			tex       = logo->mTpl.mTex.Init(logo->GetDesc(3), 0);
			u16 w     = GXGetTexObjWidth(&tex);
			f32 h     = GXGetTexObjHeight(&tex);
			u32 color = (u32)(255.0f * logo->mAlpha) | 0xFFFFFF00;
			fn_80197800(&lbl_8025C400.mGs, &tex, color, color, color, color, 320.0f, 224.0f, 0.0f, w, h, 0.0f, 0.0f, 0.0f);

			tex        = logo->mTpl.mTex.Init(logo->GetDesc(4), 0);
			f32 shineW = GXGetTexObjWidth(&tex);
			f32 shineH = GXGetTexObjHeight(&tex);
			f32 y      = shineH / 2.0f + 359.0f;
			f32 shine  = sin(logo->mAngle);
			if (shine < 0.0f) {
				shine = 0.0f;
			}
			color = (u32)(255.0f * logo->mAlpha * shine) | 0xFFFFFF00;
			fn_80197800(&lbl_8025C400.mGs, &tex, color, color, color, color, 320.0f, y, 0.0f, shineW, shineH, 0.0f, 0.0f, 0.0f);
		}
		if (logo->mState == 6 || logo->mState == 7 || logo->mState == 8) {
			tex       = logo->mTpl.mTex.Init(logo->GetDesc(0), 0);
			u16 w     = GXGetTexObjWidth(&tex);
			f32 h     = GXGetTexObjHeight(&tex);
			u32 color = (u32)(255.0f * logo->mAlpha) | 0xDC000000;
			fn_80197800(&lbl_8025C400.mGs, &tex, color, color, color, color, 320.0f, h / 2.0f + 154.0f, 0.0f, w, h, 0.0f, 0.0f, 0.0f);
		}
		if (logo->mState == 9 || logo->mState == 10 || logo->mState == 11) {
			tex       = logo->mTpl.mTex.Init(logo->GetDesc(1), 0);
			u16 w     = GXGetTexObjWidth(&tex);
			u16 h     = GXGetTexObjHeight(&tex);
			u32 color = (u32)(255.0f * logo->mAlpha) | 0xFFFFFF00;
			fn_80197800(&lbl_8025C400.mGs, &tex, color, color, color, color, 320.0f, 224.0f, 0.0f, w, h, 0.0f, 0.0f, 0.0f);
		}
		if (logo->mState == 12 || logo->mState == 13 || logo->mState == 14) {
			tex       = logo->mTpl.mTex.Init(logo->GetDesc(2), 0);
			u16 w     = GXGetTexObjWidth(&tex);
			u16 h     = GXGetTexObjHeight(&tex);
			u32 color = (u32)(255.0f * logo->mAlpha) | 0xFFFFFF00;
			fn_80197800(&lbl_8025C400.mGs, &tex, color, color, color, color, 320.0f, 224.0f, 0.0f, w, h, 0.0f, 0.0f, 0.0f);
		}
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
	}
}

/**
 * @note Address: 0x80027528
 * @note Size: 0x554
 */
// NONMATCHING: sprintf's format and buffer are set up in another order
// Logo::Update, reads the logos in, then fades each of them in and out
extern "C" void fn_80027528(Logo* logo, u32 frames)
{
	char path[64];
	switch (logo->mState) {
	case 1:
		logo->mState = 2;
	case 2:
		sprintf(path, "tpl/logo%s.tpl", "_us");
		if (!logo->mTpl.ReadEx(path, 0, 0)) {
			break;
		}
		logo->mTpl.CloseHandle();
		if ((s32)((TPLPalette*)logo->mTpl.mBuffer)->numDescriptors > 3) {
			logo->mState = 3;
		} else {
			logo->mState = 6;
		}
		logo->ResetFade();
	case 3:
		if (logo->mAlpha < 1.0f && (logo->mAlpha += 0.033333335f * frames) > 1.0f) {
			logo->mAlpha = 1.0f;
		}
		if (logo->mAlpha != 1.0f) {
			break;
		}
		logo->mState = 4;
	case 4:
		logo->mTimer += frames;
		if (logo->mTimer >= 90.0f) {
			if ((logo->mAngle += 0.05235988f * frames) >= 3.1415927f) {
				logo->mAngle = 0.0f;
			}
			BOOL done = logo->mTimer >= 3600.0f ? TRUE : (&lbl_8025C400.mPad.mPad[0])->mTrigger != 0;
			if (!done) {
				break;
			}
		} else {
			break;
		}
		logo->mState = 5;
	case 5:
		if (logo->mAlpha > 0.0f && (logo->mAlpha -= 0.033333335f * frames) < 0.0f) {
			logo->mAlpha = 0.0f;
		}
		if (logo->mAlpha == 0.0f) {
			logo->mState = 6;
			logo->ResetFade();
		}
		break;
	case 6:
		if (logo->mAlpha < 1.0f && (logo->mAlpha += 0.033333335f * frames) > 1.0f) {
			logo->mAlpha = 1.0f;
		}
		if (logo->mAlpha != 1.0f) {
			break;
		}
		logo->mState = 7;
	case 7:
		if ((logo->mTimer += frames) >= 60.0f) {
			logo->mState = 8;
		} else {
			break;
		}
	case 8:
		if (logo->mAlpha > 0.0f && (logo->mAlpha -= 0.033333335f * frames) < 0.0f) {
			logo->mAlpha = 0.0f;
		}
		if (logo->mAlpha == 0.0f) {
			logo->mState = 9;
			logo->ResetFade();
		}
		break;
	case 9:
		if (logo->mAlpha < 1.0f && (logo->mAlpha += 0.033333335f * frames) > 1.0f) {
			logo->mAlpha = 1.0f;
		}
		if (logo->mAlpha != 1.0f) {
			break;
		}
		logo->mState = 10;
	case 10:
		if ((logo->mTimer += frames) >= 60.0f) {
			logo->mState = 11;
		} else {
			break;
		}
	case 11:
		if (logo->mAlpha > 0.0f && (logo->mAlpha -= 0.033333335f * frames) < 0.0f) {
			logo->mAlpha = 0.0f;
		}
		if (logo->mAlpha == 0.0f) {
			logo->mState = 12;
			logo->ResetFade();
		}
		break;
	case 12:
		if (logo->mAlpha < 1.0f && (logo->mAlpha += 0.033333335f * frames) > 1.0f) {
			logo->mAlpha = 1.0f;
		}
		if (logo->mAlpha != 1.0f) {
			break;
		}
		logo->mState = 13;
	case 13:
		if ((logo->mTimer += frames) >= 60.0f) {
			logo->mState = 14;
		} else {
			break;
		}
	case 14:
		if (logo->mAlpha > 0.0f && (logo->mAlpha -= 0.033333335f * frames) < 0.0f) {
			logo->mAlpha = 0.0f;
		}
		if (logo->mAlpha == 0.0f) {
			logo->mState = 15;
			logo->ResetFade();
			logo->mTpl.Close();
		}
		break;
	case 15:
		break;
	}
}

/**
 * @note Address: 0x80027A7C
 * @note Size: 0x1C
 */
// Logo's init
extern "C" void fn_80027A7C(Logo* logo)
{
	logo->mState = 0;
	logo->ResetFade();
}

/**
 * @note Address: 0x80027A98
 * @note Size: 0xD8
 */
// CXObj's SetScaleY
extern "C" void SetScaleY__5CXObjFfl(CXObj* object, f32 value, s32 force)
{
	object->mMoveScale.y         = value;
	object->mMoveScale.mTarget.y = value;
	if (object->mJObj != NULL) {
		object->CJObj::SetScaleY(value, force);
	}
}

/**
 * @note Address: 0x80027B70
 * @note Size: 0x18C
 */
// CXObj's SetScale(scale)
extern "C" void SetScale__5CXObjFfl(CXObj* obj, f32 scale, s32 force)
{
	f32 x = scale;
	f32 y = scale;
	f32 z = scale;

	obj->mMoveScale.x         = x;
	obj->mMoveScale.y         = y;
	obj->mMoveScale.z         = z;
	obj->mMoveScale.mTarget.x = x;
	obj->mMoveScale.mTarget.y = y;
	obj->mMoveScale.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v = { x, y, z };
			if (!(obj->mScale != v)) {
				return;
			}
		}
		obj->mScale.x = x;
		obj->mScale.y = y;
		obj->mScale.z = z;
		HSD_JObjSetScale(obj->mJObj, &obj->mScale);
	}
}

/**
 * @note Address: 0x80027CFC
 * @note Size: 0x18C
 */
// CXObj's SetScale(x, y, z)
extern "C" void SetScale__5CXObjFfffl(CXObj* obj, f32 x, f32 y, f32 z, s32 force)
{
	obj->mMoveScale.x         = x;
	obj->mMoveScale.y         = y;
	obj->mMoveScale.z         = z;
	obj->mMoveScale.mTarget.x = x;
	obj->mMoveScale.mTarget.y = y;
	obj->mMoveScale.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v = { x, y, z };
			if (!(obj->mScale != v)) {
				return;
			}
		}
		obj->mScale.x = x;
		obj->mScale.y = y;
		obj->mScale.z = z;
		HSD_JObjSetScale(obj->mJObj, &obj->mScale);
	}
}

/**
 * @note Address: 0x80027E88
 * @note Size: 0x178
 */
// CXObj's SetScale(v)
extern "C" void SetScale__5CXObjFRC3Vecl(CXObj* obj, const Vec& scale, s32 force)
{
	obj->mMoveScale.Set(scale);
	obj->mMoveScale.mTarget.Set(scale);
	if (obj->mJObj) {
		obj->CJObj::SetScale(scale, force);
	}
}

/**
 * @note Address: 0x80028000
 * @note Size: 0xB4
 */
// XObjDemo's vfAC, draws through its own camera and lights when it has them
extern "C" void vfAC__8XObjDemoFv(XObjDemo* demo)
{
	if (demo->mState != 0) {
		CCamera* camera = demo->mArchive.mDesc[0] != NULL ? &demo->mCamera : &lbl_8025C400.mCameras[0];
		CLight* light   = demo->mArchive.mDesc[1] != NULL ? &demo->mLights[0] : &lbl_8025C400.mLights[0];
		light->SetCurrent(camera);
		if (camera->SetCurrent() != NULL) {
			demo->vfB0(NULL);
			camera->EndCurrent();
		}
	}
}

static inline bool FadeDone()
{
	return 0.0f == lbl_8025C400.mFade.mSpeed.r && 0.0f == lbl_8025C400.mFade.mSpeed.g && 0.0f == lbl_8025C400.mFade.mSpeed.b
	    && 0.0f == lbl_8025C400.mFade.mSpeed.a;
}

/**
 * @note Address: 0x800280B4
 * @note Size: 0x244
 */
// XObjDemo's update, steps the camera and light anims with the model
extern "C" void vfA4__8XObjDemoFUl(XObjDemo* demo, u32 frames)
{
	if (demo->mState == 0) {
		return;
	}
	if (demo->mState == 2 && FadeDone()) {
		demo->mState = 0;
		return;
	}
	vfA4__4XObjFUl(demo, frames);
	if (demo->mArchive.mDesc[0] != NULL && demo->mCameraAnim >= 0) {
		demo->mCamera.SetAnimRate((f32)frames);
		demo->mCamera.Anim();
	}
	for (s32 i = 0; i < demo->mLightCount; ++i) {
		if (demo->mArchive.mDesc[i + 1] != NULL && demo->mLightAnims[i] >= 0) {
			demo->mLights[i].SetAnimRate((f32)frames);
			demo->mLights[i].Anim();
		}
	}
	if (demo->mLoop) {
		if (GetAObj(demo->mJObj)->flags & 0x44000000) {
			demo->mState = 1;
			demo->SetPos(demo->mMoveTranslate, 0);
			demo->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
	}
}

/**
 * @note Address: 0x80028368
 * @note Size: 0xA4
 */
// XObjDemo's Release
extern "C" void Release__8XObjDemoFv(XObjDemo* demo)
{
	if (demo->mArchive.mDesc[0] != NULL) {
		demo->mCamera.Release();
	}
	for (s32 i = 0; i < demo->mLightCount; ++i) {
		if (demo->mArchive.mDesc[i + 1] != NULL) {
			demo->mLights[i].Release();
		}
	}
	Release__4XObjFv(demo);
}

/**
 * @note Address: 0x8002840C
 * @note Size: 0xD4
 */
// XObjDemo's vf9C, builds the camera and the light list from the archive
extern "C" BOOL vf9C__8XObjDemoFRC9CoordInfo(XObjDemo* demo, CoordInfo* info)
{
	if (vf9C__4XObjFRC9CoordInfo(demo, info)) {
		if (demo->mArchive.mDesc[0] != NULL) {
			demo->mCamera.Init((HSD_CObjDesc*)demo->mArchive.mDesc[0]->_00);
#if defined(VERSION_PAL)
			demo->mCamera.SetViewport(0.0f, 0.0f, &lbl_8025C400.mGs.mScreen);
#endif
		}
		HSD_LObj* prev = NULL;
		for (s32 i = 0; i < 8; ++i) {
			if (demo->mArchive.mDesc[i + 1] != NULL) {
				demo->mLights[i].Init((HSD_LightDesc*)demo->mArchive.mDesc[i + 1]->_00, FALSE);
				demo->mLightCount++;
				if (prev != NULL) {
					prev->next = &demo->mLights[i];
				}
				prev = &demo->mLights[i];
			}
		}
		if (prev != NULL) {
			prev->next = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x800284E0
 * @note Size: 0x64
 */
// XObjDemo's vf98
extern "C" void vf98__8XObjDemoFv(XObjDemo* demo)
{
	vf98__4XObjFv(demo);
	demo->mState      = 0;
	demo->mLoop       = FALSE;
	demo->mCameraAnim = -1;
	demo->mLightCount = 0;
	for (s32 i = 0; i < 8; ++i) {
		demo->mLightAnims[i] = -1;
	}
}

// the name's letters shrink away
static inline void HideName(Title* title)
{
	for (u32 i = 0; i < 20; i++) {
		title->mName.mChars[i].mWait = 0.0f;
	}
}

// the card was taken out: back to the card check with the save files cleared
static inline void OnCardRemoved(Title* title)
{
	title->mResult = -3;
	title->SetMode(9);
	title->mOffset = -32.0f;
	for (s32 i = 0; i < 3; i++) {
		fn_8003D7DC(&lbl_80361E8C[i]);
	}
}

// the card a new file was made on went away: back to the start
static inline void OnFormattedCardGone(Title* title)
{
	title->SetMode(25);
	title->mState = 11;
	fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
	title->_37BC   = -200.0f;
	title->mOffset = 0.0f;
	title->mAlpha  = 0.0f;
	HideName(title);
	title->_37A8 = 0;
	title->_3809 = 0;
	title->_3808 = 0;
}

// moves the yes or no choice with left and right
static inline void MoveYesNo(Title* title, CPad& pad)
{
	if (pad.mPad->mRepeat & 0x40001) {
		if (title->mChoice > 0) {
			title->mChoice--;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		}
	} else if (pad.mPad->mRepeat & 0x80002) {
		if (title->mChoice < 1) {
			title->mChoice++;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		}
	}
}

/**
 * @note Address: 0x80028BAC
 * @note Size: 0x5BC
 */
// NONMATCHING: the loop's float registers are assigned in another order
// draws the title's name: a bubble per letter, then the letter with a shadow on four sides
extern "C" void fn_80028BAC(Title* title)
{
	if (title->mName.mActive) {
		fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		CTexObj bubble = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[108], 0);
		f32 zero       = 0.0f;
		u32 color      = title->mName.mColor & 0xFFFFFF00;
		for (s32 i = 0; i < title->mName.mCount; i++) {
			f32 scale = title->mName.mChars[i].mScale;
			if (scale <= zero || strncmp(title->mName.mChars[i].mText, " ", 1) == 0) {
				continue;
			}
			CFont font;
			s32 width;
			CTexObj letter = *fn_8019E8F4(&font, title->mName.mChars[i].mText, &width);
			f32 bob;
			f32 tilt;
			f32 swell;
			f32 pop;
			f32 x;
			f32 rot;
			f32 y;
			bob       = 4.0f * (f32)sin(title->mName.mChars[i].mBob);
			tilt      = sin(0.7f * title->mName.mChars[i].mTilt);
			swell     = sin(3.1415927f * scale);
			pop       = 0.2f * swell + (f32)sin(1.5707964f * scale);
			x         = 24.0f + (48.0f * i + (320.0f - 48.0f * title->mName.mCount / 2.0f));
			u32 alpha = 255.0f * scale;
			rot       = -(0.017453292f * (10.0f * tilt));
			y         = 64.0f + bob;
			fn_80197800(&lbl_8025C400.mGs, &bubble, alpha | 0xFFFFFF00, alpha | 0xFFFFFF00, alpha | 0xFFFFFF00, alpha | 0xFFFFFF00, x, y,
			            0.0f, 20.0f * (2.0f * pop), 20.0f * (2.0f * pop), zero, -zero, rot);
			f32 size   = 20.0f * (1.7f * pop);
			f32 shrink = width / 24.0f * size;
			f32 off    = size - shrink;
			for (s32 j = 0; j < 4; j++) {
				fn_80197800(&lbl_8025C400.mGs, &letter, alpha, alpha, alpha, alpha, off / 2.0f + (x + lbl_8021195C[j]),
				            bob + (64.0f + lbl_8021196C[j]), 0.0f, size, size, zero, -zero, rot);
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
 * @note Address: 0x80029168
 * @note Size: 0x5F8
 */
// the options: up and down pick rumble, sound or back, A changes the line, B goes back. the
// rumble test runs the motor for 24 frames
extern "C" void fn_80029168(Title* title, u32 frames)
{
	CPad pad(0);

	title->mCursor.vfA4(frames);
	title->_1C += frames;
	if (title->_1C >= 60.0f) {
		title->_1C = 60;
		title->_18 = fmod(title->_18 + frames * 9, 360.0);
	}
	if (!pad.mPad->mConnected && title->_3818) {
		title->_3818 = FALSE;
		lbl_8025C400.mStatus.mFlags &= ~0x800000000ULL;
	}

	switch (title->_08) {
	case 0:
		if (pad.mPad->mRepeat & 0x10008) {
			if (title->_380C > 0) {
				title->_380C--;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
				title->_18 = 45.0f;
			}
		} else if (pad.mPad->mRepeat & 0x20004) {
			if (title->_380C < 2) {
				title->_380C++;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
				title->_18 = 45.0f;
			}
		}
		if (pad.mPad->mTrigger & 0x100) {
			switch (title->_380C) {
			case 0:
				if (pad.mPad->mConnected) {
					if (title->_3818) {
						title->_3818 = FALSE;
						lbl_8025C400.mStatus.mFlags &= ~0x800000000ULL;
					} else {
						title->_3818 = TRUE;
						lbl_8025C400.mStatus.mFlags |= 0x800000000ULL;
						if (pad.mPad->mConnected) {
							HSD_PadRumbleOn(pad.mPad->mPort);
						}
						title->_08   = 1;
						title->_3820 = 24;
					}
					fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				} else {
					fn_8001E904(&lbl_8025C400.mSound, 0x85, 0xFF);
				}
				break;
			case 1:
				if (title->_3819) {
					title->_3819 = FALSE;
					lbl_8025C400.mStatus.mFlags &= ~0x1000000000ULL;
					fn_8001E00C(&lbl_8025C400.mSound, FALSE);
				} else {
					title->_3819 = TRUE;
					lbl_8025C400.mStatus.mFlags |= 0x1000000000ULL;
					fn_8001E00C(&lbl_8025C400.mSound, TRUE);
				}
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				break;
			case 2:
				title->_08 = 2;
				title->mCursor.SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				title->_18 = 90.0f;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				break;
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			title->_08 = 2;
			title->mCursor.SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			title->_18 = 90.0f;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		}
		break;
	case 1:
		if (--title->_3820 < 0) {
			title->_08 = 0;
			if (pad.mPad->mConnected) {
				HSD_PadRumbleOffN(pad.mPad->mPort);
			}
			if (pad.mPad->mConnected) {
				HSD_PadRumbleRemove(pad.mPad->mPort);
			}
		}
		break;
	case 2:
		HSD_AObj* aobj = GetAObj(title->mCursor.mJObj);
		if ((aobj->flags & 0x44000000) || title->mCursor.mAnimId == 0 || title->mCursor.mArchive.mNum[0] < 2) {
			title->mCursor.SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			title->_18    = 0.0f;
			title->mState = 11;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			title->_3814   = 0.0f;
			title->_08     = 0;
			title->_380C   = 0;
			title->_37BC   = -200.0f;
			title->mOffset = 0.0f;
			title->mAlpha  = 0.0f;
			HideName(title);
		}
		break;
	}
}

// how far the cursor's first animation is through, from 0 to 1
static inline f32 GetAnimProgress(Title* title)
{
	HSD_AObj* aobj = GetAObj(title->mCursor.mJObj);
	f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
	aobj           = GetAObj(title->mCursor.mJObj);
	return frame / (aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f);
}

// the options line is lit once the window is fully open
static inline bool IsLineLit(Title* title, s32 line)
{
	bool lit = false;
	if (title->_380C == line && title->_1C >= 60.0f) {
		lit = true;
	}
	return lit;
}

/**
 * @note Address: 0x80029760
 * @note Size: 0x69C
 */
// NONMATCHING: float regalloc
// draws the options: a window, its title and the three lines, the picked one lit once the
// window is open. the back line bobs while the cursor plays its anim
extern "C" void fn_80029760(Title* title)
{
	f32 scale = title->_3810;
	if (scale > 0.0f) {
		f32 alpha  = 255.0f * scale;
		f32 h      = 224.0f * scale;
		s32 a      = alpha;
		u32 window = (u32)alpha | 0xFFFFFF00;
		f32 top;
		fn_800652C0((u32)(160.0f * scale), window, 320.0f, top = 224.0f - h / 2.0f, 320.0f * scale, h, 1.0f, 1.0f, 1.0f);

		u32 lit    = (u8)a | 0xFFFFFF00;
		u32 normal = (u8)alpha | 0xFFFFFF00;
		fn_8006DBF0(0x60);
		f32 titleSize    = 24.0f * title->_3810;
		const char* text = fn_8006DBF0(0x60);
		fn_80196610(&lbl_8025C400, normal, text, 320.0f - fn_801968BC(&lbl_8025C400.mGs, text, 1, titleSize) / 2.0f, titleSize + top, 0.0f,
		            titleSize, titleSize);

		f32 size  = 24.0f * title->_3810;
		u32 color = normal;
		if (IsLineLit(title, 0)) {
			color = lit;
		}
		text  = fn_8006DBF0(0x10);
		f32 x = 320.0f - fn_801968BC(&lbl_8025C400.mGs, text, 1, size) / 2.0f - 8.0f;
		f32 y;
		fn_80196610(&lbl_8025C400, color, text, x - fn_801968BC(&lbl_8025C400.mGs, text, 1, size) / 2.0f,
		            y = size + (4.0f * (size / 2.0f) + top), 0.0f, size, size);
		text = title->_3818 ? fn_8006DBF0(0x11) : fn_8006DBF0(0x12);
		x    = 8.0f + (fn_801968BC(&lbl_8025C400.mGs, text, 1, size) / 2.0f + 320.0f);
		fn_80196610(&lbl_8025C400, normal, text, x - fn_801968BC(&lbl_8025C400.mGs, text, 1, size) / 2.0f, y, 0.0f, size, size);

		color = normal;
		if (IsLineLit(title, 1)) {
			color = lit;
		}
		text = fn_8006DBF0(0x13);
		x    = 320.0f - fn_801968BC(&lbl_8025C400.mGs, text, 1, size) / 2.0f - 8.0f;
		fn_80196610(&lbl_8025C400, color, text, x - fn_801968BC(&lbl_8025C400.mGs, text, 1, size) / 2.0f,
		            y = size + (4.0f * (2.0f * (size / 2.0f)) + top), 0.0f, size, size);
		text = title->_3819 ? fn_8006DBF0(0x14) : fn_8006DBF0(0x15);
		x    = 8.0f + (fn_801968BC(&lbl_8025C400.mGs, text, 1, size) / 2.0f + 320.0f);
		fn_80196610(&lbl_8025C400, normal, text, x - fn_801968BC(&lbl_8025C400.mGs, text, 1, size) / 2.0f, y, 0.0f, size, size);

		f32 t        = title->mCursor.mAnimId == 1 ? GetAnimProgress(title) : 0.0f;
		f32 bob      = title->_380C == 2 ? 2.0f * (f32)sin(4.712389f * t) : 0.0f;
		f32 backSize = 24.0f + bob;
		if (IsLineLit(title, 2)) {
			normal = lit;
		}
		text = fn_8006DBF0(0x16);
		fn_80196610(&lbl_8025C400, normal, text, 320.0f - fn_801968BC(&lbl_8025C400.mGs, text, 1, size) / 2.0f,
		            (backSize - size) / 2.0f + ((2.0f * size + (4.0f * (3.0f * (size / 2.0f)) + top)) - backSize), 0.0f, size, size);
	}
}

/**
 * @note Address: 0x8002BB7C
 * @note Size: 0x4
 */
extern "C" void State25__5TitleFUl(Title* title)
{
}

/**
 * @note Address: 0x8002BB80
 * @note Size: 0x148
 */
// waits out the delay, then fades the labels and leaves for the game
extern "C" void State24__5TitleFUl(Title* title, u32 frames)
{
	switch (title->mPhase) {
	case 0:
		title->mDelay -= frames;
		if (title->mDelay < 0) {
			title->mDelay = 0;
			++title->mPhase;
		}
		break;
	case 1:
		HideName(title);
		title->_37BC   = -200.0f;
		title->mOffset = 0.0f;
		title->mAlpha  = 0.0f;
		title->mChoice = 0;
		title->mState  = 13;
		title->SetMode(25);
		XSound* sound = &lbl_8025C400.mSound;
		fn_8001E904(sound, 0x88, 255);
		fn_8001E660(sound, (s32)0xFFFF0003);
		lbl_8025C400.mInfoScrn._1D0 = title->mFile;
		break;
	}
}

/**
 * @note Address: 0x8002BCC8
 * @note Size: 0x4
 */
extern "C" void State23__5TitleFUl(Title* title)
{
}

/**
 * @note Address: 0x8002BCCC
 * @note Size: 0x20
 */
extern "C" void State22__5TitleFUl(Title* title)
{
	State20__5TitleFUl(title);
}

/**
 * @note Address: 0x8002BCEC
 * @note Size: 0x20
 */
extern "C" void State21__5TitleFUl(Title* title)
{
	State20__5TitleFUl(title);
}

/**
 * @note Address: 0x8002BD0C
 * @note Size: 0x4AC
 */
// the card check's message, it waits for A or for the card to change
extern "C" void State20__5TitleFUl(Title* title)
{
	CPad pad(0);
	s32 error = title->mResult;

	if (error == -3) {
		if (title->_37A8 == 1) {
			if (title->mReady) {
				OnFormattedCardGone(title);
				return;
			}
		} else if (title->mReady == 1) {
			for (s32 i = 0; i < 3; i++) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard.mCard, title->mChannel, 1);
			title->SetMode(1);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			return;
		}
	} else if (title->_37A8 == 1) {
		if (!title->mReady || !title->_3809) {
			OnFormattedCardGone(title);
			return;
		}
	} else if (!title->mReady) {
		OnCardRemoved(title);
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		if (title->_37A8 == 1) {
			OnFormattedCardGone(title);
			return;
		}
		if (error == -3) {
			OnCardRemoved(title);
		} else if (error == -5) {
			title->SetMode(19);
			title->mShowMessage = 1;
		} else {
			title->SetMode(5);
			title->mOffset = -32.0f;
			title->mChoice = 1;
		}
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
	}
}

static inline void HandleUnavailableCard(Title* title)
{
	CPad pad(0);
	if (title->mReady == 0) {
		OnCardRemoved(title);
#if !defined(VERSION_JP)
		title->_3C44 = 0;
#endif
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		title->SetMode(5);
		title->mOffset = -32.0f;
		title->mChoice = 1;
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 255);
#if !defined(VERSION_JP)
		title->_3C44 = 0;
#endif
	}
}

/**
 * @note Address: 0x8002C1B8
 * @note Size: 0x118
 */
extern "C" void State19__5TitleFUl(Title* title)
{
	HandleUnavailableCard(title);
}

/**
 * @note Address: 0x8002C2D0
 * @note Size: 0x118
 */
extern "C" void State18__5TitleFUl(Title* title)
{
	HandleUnavailableCard(title);
}

/**
 * @note Address: 0x8002C3E8
 * @note Size: 0x4
 */
extern "C" void State17__5TitleFUl(Title* title)
{
}

/**
 * @note Address: 0x8002C3EC
 * @note Size: 0x4
 */
extern "C" void State16__5TitleFUl(Title* title)
{
}

/**
 * @note Address: 0x8002C3F0
 * @note Size: 0x348
 */
// NONMATCHING: regalloc
// waits for the card to read the file picked, then shows what is in it
extern "C" void State15__5TitleFUl(Title* title, u32 frames)
{
	Memcard* card = &lbl_8025C400.mMemcard.mCard;
	s32 step      = card->mState;
	SavePreviewRecord* file;
	s32 result;

	title->mDelay -= frames;
	if (title->mDelay < 0) {
		title->mDelay = 0;
	}
	switch (step) {
	case 0:
		title->_37D0 = lbl_8025C400.mMemcard.mCard._16A04;
		title->_37D4 = lbl_8025C400.mMemcard.mCard._16A00;
		file         = &card->mSaves[0][title->mFile];
		if (file->mState != 0) {
			lbl_80361E8C[title->mFile].mUsed       = 1;
			lbl_80361E8C[title->mFile].mLevel      = file->mHeader.mLevel;
			lbl_80361E8C[title->mFile].mBattery    = file->mHeader.mBattery;
			lbl_80361E8C[title->mFile].mMaxBattery = file->mHeader.mMaxBattery;
			lbl_80361E8C[title->mFile].mMoney      = file->mHeader.mMoney;
			lbl_80361E8C[title->mFile].mExp        = file->mHeader.mExp;
			lbl_80361E8C[title->mFile].mScrap      = file->mHeader.mScrap;
			lbl_80361E8C[title->mFile].mRank       = file->mHeader.mRank;
			lbl_80361E8C[title->mFile].mHours      = file->mHeader.mFrames / 216000;
			lbl_80361E8C[title->mFile].mMinutes    = (file->mHeader.mFrames / 3600) % 60;
			lbl_80361E8C[title->mFile].mStage      = file->mHeader.mStage;
			lbl_80361E8C[title->mFile].mTimeMode   = file->mHeader.mTimeMode;
			lbl_80361E8C[title->mFile].mState      = 0;
			if (file->mHeader._34 > 0) {
				lbl_80361E8C[title->mFile].mActive = 1;
			} else {
				lbl_80361E8C[title->mFile].mActive = 0;
			}
		}
		title->SetMode(24);
		break;
	case 4:
		break;
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		result         = lbl_8025C400.mMemcard.mCard.mCard.mResult;
		title->mResult = result;
		switch (result) {
		case -128:
		case -14:
		case -13:
		case -12:
		case -11:
		case -10:
		case -9:
		case -8:
		case -7:
		case -6:
		case -5:
		case -4:
		case -3:
		case -2:
		case -1:
		case 0:
			if (step == 2 && (result == -6 || result == -13)) {
				title->SetMode(11);
				title->mChoice      = 1;
				title->mShowMessage = 1;
			} else if (result == -3 || result == -5) {
				title->SetMode(20);
				title->mShowMessage = 1;
			} else {
				title->SetMode(14);
				title->mOffset = -32.0f;
			}
			title->mAlpha = 1.0f;
			break;
		default:
			title->SetMode(14);
			title->mAlpha  = 1.0f;
			title->mOffset = -32.0f;
			break;
		}
		break;
	}
}

/**
 * @note Address: 0x8002C738
 * @note Size: 0x408
 */
// shows what is wrong with the card until A is pressed or the card changes
extern "C" void State14__5TitleFUl(Title* title)
{
	CPad pad(0);
	s32 error = title->mResult;

	switch (error) {
	case -3:
		if (title->_37E2 == 1) {
			for (s32 i = 0; i < 3; i++) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard.mCard, title->mChannel, 1);
			title->SetMode(1);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			title->mOffset = 0.0f;
			return;
		}
		if (title->mReady == 1) {
			for (s32 i = 0; i < 3; i++) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard.mCard, title->mChannel, 1);
			title->SetMode(1);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			title->mOffset = 0.0f;
			return;
		}
		break;
	case -2:
		if (!title->_37E2) {
			OnCardRemoved(title);
			return;
		}
		break;
	case -128:
		if ((!title->mReady || !title->_3809) && title->_37A8 == 1) {
			OnFormattedCardGone(title);
			return;
		}
		break;
	default:
		if (!title->mReady) {
			OnCardRemoved(title);
			return;
		}
		break;
	}
	if (pad.mPad->mTrigger & 0x100) {
		switch (error) {
		case -9:
			title->SetMode(18);
			title->mOffset = 0.0f;
			break;
		case -128:
			title->SetMode(6);
			title->mOffset = -32.0f;
			break;
		default:
			title->SetMode(5);
			title->mOffset = -32.0f;
			title->mChoice = 1;
			break;
		}
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
	}
}

/**
 * @note Address: 0x8002CB40
 * @note Size: 0x1B4
 */
extern "C" void State13__5TitleFUl(Title* title, u32 frames)
{
	CPad pad(0);

	if (!title->mReady) {
		OnCardRemoved(title);
		return;
	}
	switch (title->mPhase) {
	case 0:
		title->mDelay -= frames;
		if (title->mDelay < 0) {
			title->mDelay = 0;
			title->mPhase++;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		}
		break;
	case 1:
		if (pad.mPad->mTrigger & 0x100) {
			for (s32 i = 0; i < 3; i++) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard.mCard, title->mChannel, 1);
			title->SetMode(1);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			return;
		}
		break;
	}
}

/**
 * @note Address: 0x8002CCF4
 * @note Size: 0x14C
 */
extern "C" void State12__5TitleFUl(Title* title, u32 frames)
{
	title->mDelay -= frames;
	if (title->mDelay < 0) {
		title->mDelay = 0;
	}
	switch (lbl_8025C400.mMemcard.mCard.mState) {
	case 0:
		title->_37D0 = lbl_8025C400.mMemcard.mCard._16A04;
		title->_37D4 = lbl_8025C400.mMemcard.mCard._16A00;
		title->SetMode(13);
		title->mAlpha = 1.0f;
		break;
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15: {
		s32 result     = lbl_8025C400.mMemcard.mCard.mCard.mResult;
		title->mResult = result;
		switch (result) {
		case -128:
		case -14:
		case -13:
		case -12:
		case -11:
		case -10:
		case -9:
		case -8:
		case -7:
		case -6:
		case -5:
		case -4:
		case -3:
		case -2:
		case -1:
		case 0:
			if (result == -6 || result == -13) {
				title->SetMode(19);
				title->mShowMessage = 1;
			} else {
				title->SetMode(21);
				title->mShowMessage = 1;
			}
			title->mAlpha = 1.0f;
			break;
		default:
			title->SetMode(14);
			title->mAlpha  = 1.0f;
			title->mOffset = -32.0f;
			break;
		}
		break;
	}
	}
}

/**
 * @note Address: 0x8002CE40
 * @note Size: 0x348
 */
// asked twice whether to make a new save file
extern "C" void State11__5TitleFUl(Title* title)
{
	CPad pad(0);

	if (!title->mReady) {
		OnCardRemoved(title);
		return;
	}
	switch (title->mPhase) {
	case 0:
		MoveYesNo(title, pad);
		if (pad.mPad->mTrigger & 0x100) {
			if (title->mChoice == 0) {
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				title->mChoice = 1;
				title->mPhase++;
			} else {
				title->SetMode(19);
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
#if !defined(VERSION_JP)
				title->_3C44 = 1;
#endif
			}
			return;
		}
		break;
	case 1:
		MoveYesNo(title, pad);
		if (pad.mPad->mTrigger & 0x100) {
			if (title->mChoice == 0) {
				fn_80088E78(&lbl_8025C400.mMemcard.mCard, title->mChannel, 1);
				title->SetMode(12);
				title->mDelay = 90;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			} else {
				title->SetMode(19);
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
#if !defined(VERSION_JP)
				title->_3C44 = 1;
#endif
			}
			return;
		}
		break;
	}
}

/**
 * @note Address: 0x8002D188
 * @note Size: 0x4
 */
extern "C" void State10__5TitleFUl(Title* title)
{
}

/**
 * @note Address: 0x8002D18C
 * @note Size: 0x148
 */
extern "C" void State9__5TitleFUl(Title* title)
{
	CPad pad(0);
	if (title->_37E2 == 1 || title->mReady == 1) {
		for (s32 i = 0; i < 3; ++i) {
			fn_8003D7DC(&lbl_80361E8C[i]);
		}
		fn_80089890(&lbl_8025C400.mMemcard.mCard, title->mChannel, TRUE);
		title->SetMode(1);
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 255);
		title->mOffset = 0.0f;
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		title->SetMode(5);
		title->mOffset = -32.0f;
		title->mChoice = 1;
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 255);
	}
}

/**
 * @note Address: 0x8002D2D4
 * @note Size: 0x218
 */
extern "C" void State8__5TitleFUl(Title* title)
{
	s32 cardState = lbl_8025C400.mMemcard.mCard.mState;
	switch (cardState) {
	case 0:
		HideName(title);
		fn_8001E660(&lbl_8025C400.mSound, (s32)0xFFFF0003);
		title->_37BC                = -200.0f;
		title->mAlpha               = 0.0f;
		lbl_8025C400.mInfoScrn._1D0 = title->mFile;
		fn_80068B98(&lbl_8025C400.mInfoScrn);
		title->SetMode(25);
		title->mState   = 13;
		title->mTimeout = 20000;
		break;
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15: {
		s32 result     = lbl_8025C400.mMemcard.mCard.mCard.mResult;
		title->mResult = result;
		switch (result) {
		case -128:
		case -14:
		case -13:
		case -12:
		case -11:
		case -10:
		case -9:
		case -8:
		case -7:
		case -6:
		case -5:
		case -4:
		case -3:
		case -2:
		case -1:
		case 0:
			if (cardState == 2 && (result == -6 || result == -13)) {
				title->SetMode(11);
				title->mChoice      = 1;
				title->mShowMessage = 1;
			} else if (result == -3 || result == -5) {
				title->SetMode(22);
				title->mShowMessage = 1;
			} else {
				title->SetMode(14);
				title->mOffset = -32.0f;
			}
			title->mAlpha = 1.0f;
			break;
		default:
			title->SetMode(14);
			title->mAlpha  = 1.0f;
			title->mOffset = -32.0f;
			break;
		}
		break;
	}
	}
}

/**
 * @note Address: 0x8002D4EC
 * @note Size: 0x4
 */
extern "C" void State7__5TitleFUl(Title* title)
{
}

/**
 * @note Address: 0x8002D4F0
 * @note Size: 0x304
 */
// asked whether to save over the file, unless the card went away under it
extern "C" void State6__5TitleFUl(Title* title)
{
	CPad pad(0);

	if ((!title->mReady || !title->_3809) && title->_37A8 == 1) {
		title->SetMode(25);
		title->mState = 11;
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		title->_37BC   = -200.0f;
		title->mOffset = 0.0f;
		title->mAlpha  = 0.0f;
		HideName(title);
		title->_37A8 = 0;
		title->_3809 = 0;
		title->_3808 = 0;
		return;
	}
	if (!title->mReady) {
		OnCardRemoved(title);
		return;
	}
	if (pad.mPad->mRepeat & 0x40001) {
		if (title->mChoice > 0) {
			title->mChoice--;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			return;
		}
	} else if (pad.mPad->mRepeat & 0x80002) {
		if (title->mChoice < 1) {
			title->mChoice++;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			return;
		}
	}
	if (pad.mPad->mTrigger & 0x100) {
		switch (title->mChoice) {
		case 0:
			fn_8008A1F4(&lbl_8025C400.mMemcard.mCard, title->mChannel, title->mFile, 1);
			title->SetMode(15);
			title->mDelay = 90;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			title->mOffset = -32.0f;
			break;
		case 1:
			title->SetMode(2);
			title->mChoice = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
			break;
		}
	}
}

/**
 * @note Address: 0x8002D7F4
 * @note Size: 0x504
 */
// asked whether to go on without saving
extern "C" void State5__5TitleFUl(Title* title)
{
	CPad pad(0);

	switch (title->mResult) {
	case -3:
		if (title->mReady == 1) {
			for (s32 i = 0; i < 3; i++) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard.mCard, title->mChannel, 1);
			title->mOffset = 0.0f;
			title->SetMode(1);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			return;
		}
		break;
	case -2:
		if (!title->_37E2) {
			OnCardRemoved(title);
			return;
		}
		break;
	default:
		if (!title->mReady) {
			OnCardRemoved(title);
			return;
		}
		break;
	}
	MoveYesNo(title, pad);
	if (pad.mPad->mTrigger & 0x100) {
		if (title->mChoice == 0 && (pad.mPad->mTrigger & 0x100)) {
			HideName(title);
			title->_37BC   = -200.0f;
			title->mOffset = 0.0f;
			title->mAlpha  = 0.0f;
			title->mChoice = 0;
			title->mState  = 13;
			title->SetMode(25);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			fn_8001E660(&lbl_8025C400.mSound, (s32)0xFFFF0003);
			lbl_8025C400.mInfoScrn._1D0 = title->mFile;
			return;
		}
		if (title->mChoice == 1 && (pad.mPad->mTrigger & 0x100)) {
			if (title->mShowMessage == 1) {
				title->SetMode(19);
				title->mOffset = 0.0f;
			} else if (title->mResult == 0) {
				title->SetMode(2);
				title->mChoice = 0;
			} else if (title->mResult == -3) {
				title->SetMode(25);
				title->mState = 11;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				title->_37BC   = -200.0f;
				title->mAlpha  = 0.0f;
				title->mChoice = 0;
				title->mOffset = 0.0f;
				HideName(title);
			} else {
				for (s32 i = 0; i < 3; i++) {
					fn_8003D7DC(&lbl_80361E8C[i]);
				}
				fn_80089890(&lbl_8025C400.mMemcard.mCard, title->mChannel, 1);
				title->SetMode(1);
				title->mOffset = 0.0f;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			return;
		}
	}
}

/**
 * @note Address: 0x8002DCF8
 * @note Size: 0x4
 */
extern "C" void State4__5TitleFUl(Title* title)
{
}

/**
 * @note Address: 0x8002DCFC
 * @note Size: 0x240
 */
// the yes or no question, left and right pick, A takes it
extern "C" void State3__5TitleFUl(Title* title)
{
	CPad pad(0);
	CPadOne* state;

	if (!title->mReady) {
		OnCardRemoved(title);
		return;
	}
	state = pad.mPad;
	if (state->mRepeat & 0x40001) {
		if (title->mChoice > 0) {
			title->mChoice--;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			return;
		}
	} else if (state->mRepeat & 0x80002) {
		if (title->mChoice < 1) {
			title->mChoice++;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			return;
		}
	}
	if (state->mTrigger & 0x100) {
		switch (title->mChoice) {
		case 0:
			fn_8008A1F4(&lbl_8025C400.mMemcard.mCard, title->mChannel, title->mFile, 1);
			title->SetMode(15);
			title->mDelay = 90;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			title->mOffset = -32.0f;
			return;
		case 1:
			title->SetMode(2);
			title->mChoice = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
			return;
		}
	}
}

/**
 * @note Address: 0x8002DF3C
 * @note Size: 0x638
 */
// picks one of the three save files, A starts or loads it, B goes back
extern "C" void State2__5TitleFUl(Title* title)
{
	CPad pad(0);

	if ((!title->mReady || !title->_3809) && title->_37A8 == 1) {
		OnFormattedCardGone(title);
		return;
	}
	if (!title->mReady) {
		OnCardRemoved(title);
		return;
	}
	title->mOffset = 0.0f;
	if (pad.mPad->mRepeat & 0x40001) {
		title->mFile--;
		if (title->mFile < 0) {
			title->mFile = 2;
		}
		fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
	} else if (pad.mPad->mRepeat & 0x80002) {
		title->mFile++;
		if (title->mFile > 2) {
			title->mFile = 0;
		}
		fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
	}
	if (pad.mPad->mTrigger & 0x100) {
		switch (title->_37A8) {
		case 0:
			if (lbl_80361E8C[title->mFile].mUsed == 1) {
				title->SetMode(3);
				title->mChoice = 1;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			} else if (title->_37D0 >= 6 && title->_37D4 > 0) {
				fn_8008A1F4(&lbl_8025C400.mMemcard.mCard, title->mChannel, title->mFile, 1);
				title->SetMode(15);
				title->mDelay  = 90;
				title->mOffset = -32.0f;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			} else {
				title->mResult = -9;
				title->SetMode(14);
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				title->mOffset = -32.0f;
			}
			return;
		case 1:
			if (lbl_80361E8C[title->mFile].mUsed == 1) {
				if (!fn_8003D7C4(&lbl_80361E8C[title->mFile])) {
					title->mResult = -128;
					title->SetMode(14);
					fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
					title->mOffset = 0.0f;
					return;
				}
				if (lbl_80361E8C[title->mFile].mStage == 0) {
					HideName(title);
					title->_37BC   = -200.0f;
					title->mAlpha  = 0.0f;
					title->mChoice = 0;
					title->mState  = 13;
					title->SetMode(25);
					fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
					fn_8001E660(&lbl_8025C400.mSound, (s32)0xFFFF0003);
					lbl_8025C400.mInfoScrn._1D0 = title->mFile;
					return;
				}
				fn_80089C90(&lbl_8025C400.mMemcard.mCard, title->mChannel, title->mFile, 1);
				title->SetMode(8);
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			} else {
				fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
			}
			return;
		}
	} else if (pad.mPad->mTrigger & 0x200) {
		title->SetMode(25);
		title->mState = 11;
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		title->_37BC   = -200.0f;
		title->mOffset = 0.0f;
		title->mAlpha  = 0.0f;
		HideName(title);
	}
}

/**
 * @note Address: 0x8002E574
 * @note Size: 0x358
 */
extern "C" void State1__5TitleFUl(Title* title, u32 frames)
{
	Memcard* card = &lbl_8025C400.mMemcard.mCard;
	s32 state     = card->mState;
	title->mDelay -= frames;
	if (title->mDelay < 0)
		title->mDelay = 0;
	switch (state) {
	case 0: {
		OSTime timestamps[3];
		for (s32 i = 0; i < 3; ++i) {
			SavePreviewRecord* save = &card->mSaves[0][i];
			if (save->mState) {
				OSCalendarTime calendar;
				OSTicksToCalendarTime(OSSecondsToTicks((OSTime)save->mStat.time), &calendar);
				s32 frames                  = save->mHeader.mFrames;
				s32 active                  = save->mHeader._34;
				lbl_80361E8C[i].mUsed       = 1;
				lbl_80361E8C[i].mLevel      = save->mHeader.mLevel;
				lbl_80361E8C[i].mBattery    = save->mHeader.mBattery;
				lbl_80361E8C[i].mMaxBattery = save->mHeader.mMaxBattery;
				lbl_80361E8C[i].mMoney      = save->mHeader.mMoney;
				lbl_80361E8C[i].mExp        = save->mHeader.mExp;
				lbl_80361E8C[i].mScrap      = save->mHeader.mScrap;
				lbl_80361E8C[i].mRank       = save->mHeader.mRank;
				lbl_80361E8C[i].mHours      = frames / 216000;
				lbl_80361E8C[i].mMinutes    = (frames / 3600) % 60;
				lbl_80361E8C[i].mStage      = save->mHeader.mStage;
				lbl_80361E8C[i].mTimeMode   = save->mHeader.mTimeMode;
				if (active > 0)
					lbl_80361E8C[i].mActive = 1;
				else
					lbl_80361E8C[i].mActive = 0;
				timestamps[i] = OSCalendarTimeToTicks(&calendar);
				if (save->mStatus != 0 || !fn_80086F24(save))
					lbl_80361E8C[i].mState = 1;
				else
					lbl_80361E8C[i].mState = 0;
			}
		}
		title->_37D0 = lbl_8025C400.mMemcard.mCard._16A04;
		title->_37D4 = lbl_8025C400.mMemcard.mCard._16A00;
		title->SetMode(2);
		if (title->_37A8 == 0)
			title->mFile = 0;
		else
			title->mFile = fn_80035184(timestamps[0], timestamps[1], timestamps[2]);
		title->mAlpha = 1.0f;
		break;
	}
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15: {
		s32 result     = lbl_8025C400.mMemcard.mCard.mCard.mResult;
		title->mResult = result;
		switch (result) {
		case -128:
		case -14:
		case -13:
		case -12:
		case -11:
		case -10:
		case -9:
		case -8:
		case -7:
		case -6:
		case -5:
		case -4:
		case -3:
		case -2:
		case -1:
		case 0:
			if (result == -6 || result == -13) {
				title->SetMode(11);
				title->mChoice = 1;
			} else if (result == -5) {
				title->SetMode(19);
				title->mShowMessage = 1;
			} else {
				title->SetMode(14);
				title->mOffset = -32.0f;
			}
			title->mAlpha = 1.0f;
			break;
		default:
			title->SetMode(14);
			title->mAlpha  = 1.0f;
			title->mOffset = -32.0f;
			break;
		}
		break;
	}
	}
}

/**
 * @note Address: 0x8002E8CC
 * @note Size: 0x4
 */
extern "C" void State0__5TitleFUl(Title* title)
{
}

/**
 * @note Address: 0x80030CE8
 * @note Size: 0x1FC
 */
extern "C" void fn_80030CE8(Title* title)
{
	title->mState   = 0;
	title->mMode    = 0;
	title->mTimeout = -1;
	title->_10      = 0.0f;
	title->_14      = 0.0f;
	title->_18      = 90.0f;
	title->_1C      = 0;
	title->_20      = 0;
	title->mCursor.vf90(0);
	title->mCursor.mAngle      = 0.0f;
	title->mCursor.mAngleSpeed = 0.0f;
	title->_37A8               = 0;
	title->mFile               = 0;
	title->mChoice             = 0;
	title->_37E4               = 0.0f;
	title->mAlpha              = 0.0f;
	title->mPrevMode           = 0;
	title->mPhase              = 0;
	title->mDelay              = 0;
	title->_37B0               = -10.0f;
	title->_37B4               = -200.0f;
	title->_37B8               = -10.0f;
	title->_37BC               = -200.0f;
	title->_37C0               = 0.0f;
	title->_37C4               = 0.0f;
	title->_37C8               = 0.0f;
	title->mOffset             = 0.0f;
	title->_380C               = 0;
	title->_08                 = 0;
	title->_3810               = 0.0f;
	title->_3814               = 0.0f;
	CPad pad(0);
	if (pad.mPad->mConnected != 0) {
		if (lbl_8025C400.mStatus.mFlags & 0x800000000ULL)
			title->_3818 = 1;
		else
			title->_3818 = 0;
	} else {
		title->_3818 = 0;
	}
	if (lbl_8025C400.mStatus.mFlags & 0x1000000000ULL)
		title->_3819 = 1;
	else
		title->_3819 = 0;
	title->mChannel = 0;
	for (s32 i = 0; i < 3; ++i)
		fn_8003D7DC(&lbl_80361E8C[i]);
	title->_37AC = 0.0f;
	title->_3809 = 0;
	title->_3808 = 0;
	fn_8001E630(&lbl_8025C400.mSound, 1);
#if !defined(VERSION_JP)
	title->_3C44 = 0;
#endif
}

/**
 * @note Address: 0x80030EE4
 * @note Size: 0x120
 */
extern "C" void fn_80030EE4(Title* title)
{
	fn_80030CE8(title);
	title->mDemo.vf90(0);
	for (s32 i = 0; i < 16; ++i) {
		title->mDust[i].mState  = 0;
		title->mDust[i].mTimer  = 0;
		title->mDust[i].mX      = rand() % 640;
		title->mDust[i].mY      = rand() % 480;
		title->mDust[i].mSpeedX = 0.0f;
		title->mDust[i].mSpeedY = 0.0f;
	}
#if !defined(VERSION_JP)
	title->_3C44 = 0;
#endif
}
