#include "game/infoscrn.h"
#include "game/infopoint.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/ctpl.h"
#include "game/xchr.h"
#include "game/stg.h"
#include "game/video.h"
#include <string.h>
#include <math.h>

// the info screen, the hud drawn over the game: the clock, the battery, the happy points and
// the three counters that pop up in the corner. CGame keeps it in mInfoScrn

// the timer counts frames, europe at the video mode's rate
#if defined(VERSION_PAL)
#define TIMER_RATE lbl_80660D4C
#else
#define TIMER_RATE 60
#endif
// 99:59.99, the most the timer shows
#define TIMER_MAX ((s32)((u32)TIMER_RATE * 6000U - 1U))

extern "C" {
u16 GXGetTexObjWidth(const GXTexObj* obj);
u16 GXGetTexObjHeight(const GXTexObj* obj);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void fn_8006389C(InfoPoint* point);
void fn_80066714(InfoScrn* scrn);
void fn_80066C08(InfoScrn* scrn);
void fn_80067418(InfoScrn* scrn);
void fn_80067928(InfoScrn* scrn);
void fn_80068D10(InfoScrn* scrn);
void fn_8006AB00(InfoScrn* scrn);
void fn_8006AF34(InfoScrn* scrn, u32 frames);
bool fn_80051C60();
const char* fn_8006DBF0(s32 id);
}

// the right edge the corner counters line up on
static inline f32 GetRight()
{
	return (lbl_8025C400.mWipeTile1.mHeight / 2 + lbl_8025C400.mWipeTile1.mY >= 418.0f)
	         ? 418.0f
	         : lbl_8025C400.mWipeTile1.mHeight / 2 + lbl_8025C400.mWipeTile1.mY;
}

// the tu's bss from lbl_80366E58. what comes before the counters isn't known yet, the
// compiler puts each counter's destructor link in front of it
#if defined(VERSION_JP)
static u8 sBss[0x134];
#else
static u8 sBss[0x140];
#endif
static InfoPoint sPoint0; // lbl_80366FA4, the moolah
static InfoPoint sPoint1; // lbl_80367038, the happy points
static InfoPoint sPoint2; // lbl_803670CC, the scrap
static f32 sHistory[10];

// how bright the hud is, the counters and gauges multiply their alpha by it
static f32 sScale = 1.0f;

// moves cur toward target by step, not while the game is paused
static inline void Approach(f32* cur, f32* target, f32 step)
{
	if (lbl_8025C400.mStatus.mFlags & 0x200000000000ULL) {
		return;
	}
	if (*cur > *target && (*cur -= step) < *target) {
		*cur = *target;
	}
	if (*cur < *target && (*cur += step) > *target) {
		*cur = *target;
	}
}

// a quarter of the way there each frame, at least 1
static inline f32 ApproachStep(f32 cur, f32 target)
{
	f32 step = (target - cur) / 4;
	if (step < 0.0f) {
		step = -step;
	}
	if (step <= 1.0f) {
		step = 1.0f;
	}
	return step;
}

// the chibi-pc menu is open, the counters stay hidden under it. japan has more menus and
// no empty slots
static inline BOOL IsPCOpen()
{
#if defined(VERSION_JP)
	return (lbl_8025C400.mMenu.mMenus[8])->IsOpen();
#else
	CMenu* menu = lbl_8025C400.mMenu.mMenus[3];
	return menu ? menu->IsOpen() : FALSE;
#endif
}

// frames to a clock reading, minutes * 6000 + seconds * 100 + hundredths
static inline f32 FramesToClock(f32 frames)
{
	f32 seconds    = frames / (f32)TIMER_RATE;
	s32 hundredths = ((s32)frames % TIMER_RATE) * 100 / TIMER_RATE;
	if (frames >= (f32)TIMER_MAX) {
		hundredths = 99;
	}
	f32 minutes = seconds / 60.0f;
	return hundredths + (6000.0f * (s32)minutes + ((s32)seconds % 60) * 100);
}

// 10 to the n
static inline f32 Pow10(int n)
{
	f32 p = 1.0f;
	for (int i = 0; i < n; i++) {
		p *= 10.0f;
	}
	return p;
}

// how many items chibi robo can carry, by the upgrade he holds
static inline s32 ItemMax()
{
	XObjChr* player = lbl_8025C400.GetPlayer();
	for (int i = 0; i < 256; i++) {
		s16 id    = player->mParams.mInventory.mItems[i].mId;
		s16 count = player->mParams.mInventory.mItems[i].mCount;
		if (id >= 0 && count > 0) {
			switch (id) {
			case 0x57:
				return 5;
			case 0x59:
				return 10;
			case 0x5A:
				return 15;
			}
		}
	}
	return 5;
}

extern "C" {
void fn_8002390C(CScrnColor* color, CGs* gs);
void fn_80023B38(CScrnColor* color, u32 frames);
}

// an event is running that keeps the hud up
static inline bool IsEventHud()
{
	bool on = false;
	if ((lbl_8025C400.mDvdStatus != 0 || lbl_8025C400.mPrevDvdStatus != 0) && lbl_8025C400.mDiskOk == 0) {
		on = true;
	}
	return on;
}

// the hud is drawn in the field, outside of events
static inline bool IsHudOn()
{
	bool on = false;
	if ((lbl_8025C400.mStatus.mFlags & 1) && !(lbl_8025C400.mStatus.mFlags & 0x22) && !IsEventHud()) {
		on = true;
	}
	return on;
}

/**
 * @note Address: 0x8006D850
 * @note Size: 0x2A4
 */
// NONMATCHING: the zero vectors sit 0x48 later in .rodata
extern "C" void fn_8006D850(InfoScrn* scrn)
{
	scrn->mFade.Reset();
	scrn->mWipe.Reset();
	scrn->_A8  = 0.0f;
	scrn->_AC  = 0.0f;
	scrn->_B0  = 0.0f;
	scrn->_140 = 0.0f;
	scrn->_144 = 0.0f;
	scrn->_98  = 0;
	scrn->_9C  = 0;
	scrn->_A0  = 0;
	scrn->_A4  = 0;
	scrn->_B4  = 0;
	scrn->_1D4 = 0.0f;
	scrn->_B8  = 0;
	scrn->_17C = 0;
	scrn->_180 = 0;
	fn_800650AC(&sPoint1, INFOPOINT_ID + 2);
	fn_800650AC(&sPoint0, INFOPOINT_ID);
	fn_800650AC(&sPoint2, INFOPOINT_ID + 1);
	sPoint1.mVisible     = FALSE;
	sPoint0.mVisible     = FALSE;
	sPoint2.mVisible     = FALSE;
	scrn->_BC            = 0;
	scrn->_C0            = 0.0f;
	scrn->_C4            = 0.0f;
	scrn->_C8            = 0.0f;
	const Vec v0         = { 0.0f, 0.0f, 0.0f };
	scrn->_128.x         = v0.x;
	scrn->_128.y         = v0.y;
	scrn->_128.z         = v0.z;
	scrn->_148           = 0;
	scrn->_14C           = 0.0f;
	scrn->_150           = 0;
	scrn->_154           = 0;
	scrn->_158           = 0;
	const Vec v1         = { 0.0f, 0.0f, 0.0f };
	scrn->_170.x         = v1.x;
	scrn->_170.y         = v1.y;
	scrn->_170.z         = v1.z;
	scrn->_16C           = 1.0f;
	scrn->_188           = 0;
	scrn->_18C           = 0.0f;
	const Vec v2         = { 0.0f, 0.0f, 0.0f };
	scrn->_190.x         = v2.x;
	scrn->_190.y         = v2.y;
	scrn->_190.z         = v2.z;
	scrn->_19C           = NULL;
	scrn->_1A0           = 0;
	scrn->_1A4           = 0.0f;
	const Vec v3         = { 0.0f, 0.0f, 0.0f };
	scrn->_1A8.x         = v3.x;
	scrn->_1A8.y         = v3.y;
	scrn->_1A8.z         = v3.z;
	scrn->_1B4           = 0;
	scrn->_1B8           = 0.0f;
	scrn->mTimer         = 0.0f;
	scrn->mTimerShown    = 0;
	scrn->_1C4           = 0;
	scrn->mScanlineAlpha = 0.0f;
	scrn->_1CC           = 0.0f;
	scrn->_184           = 0;
	scrn->_1D0           = 0;
	scrn->_CC            = 0.0f;
	scrn->_D0            = 0.0f;
	scrn->_D4            = 0;
	scrn->_D8            = 0;
	scrn->_DC            = 1.4f;
	f32 d                = scrn->_CC - scrn->_D0;
	for (int i = 0; i < 4; i++) {
		scrn->_E0[i]  = 0.0f;
		scrn->_F0[i]  = 0;
		scrn->_100[i] = 0;
		scrn->_104[i] = d;
		scrn->_114[i] = 0.0f;
	}
	scrn->_124     = 0;
	scrn->_134     = 0;
	scrn->_138     = 0.0f;
	scrn->_13C     = 0.0f;
	sScale         = 1.0f;
	scrn->mItemMax = 5;
}

/**
 * @note Address: 0x8006D824
 * @note Size: 0x2C
 */
extern "C" BOOL fn_8006D824(InfoScrn* scrn)
{
	sPoint1.mVisible = FALSE;
	sPoint0.mVisible = FALSE;
	sPoint2.mVisible = FALSE;
	return TRUE;
}

/**
 * @note Address: 0x8006D5A4
 * @note Size: 0x280
 */
// NONMATCHING: the zero vectors sit 0x48 later in .rodata
extern "C" void fn_8006D5A4(InfoScrn* scrn)
{
	scrn->_A8  = 0.0f;
	scrn->_AC  = 0.0f;
	scrn->_B0  = 0.0f;
	scrn->_140 = 0.0f;
	scrn->_144 = 0.0f;
	scrn->_98  = 0;
	scrn->_9C  = 0;
	scrn->_A0  = 0;
	scrn->_A4  = 0;
	scrn->_B4  = 0;
	scrn->_1D4 = 0.0f;
	scrn->_B8  = 0;
	scrn->_17C = 0;
	scrn->_180 = 0;
	fn_800650AC(&sPoint1, INFOPOINT_ID + 2);
	fn_800650AC(&sPoint0, INFOPOINT_ID);
	fn_800650AC(&sPoint2, INFOPOINT_ID + 1);
	sPoint1.mVisible     = FALSE;
	sPoint0.mVisible     = FALSE;
	sPoint2.mVisible     = FALSE;
	scrn->_BC            = 0;
	scrn->_C0            = 0.0f;
	scrn->_C4            = 0.0f;
	scrn->_C8            = 0.0f;
	const Vec v0         = { 0.0f, 0.0f, 0.0f };
	scrn->_128.x         = v0.x;
	scrn->_128.y         = v0.y;
	scrn->_128.z         = v0.z;
	scrn->_148           = 0;
	scrn->_14C           = 0.0f;
	scrn->_150           = 0;
	scrn->_154           = 0;
	scrn->_158           = 0;
	const Vec v1         = { 0.0f, 0.0f, 0.0f };
	scrn->_170.x         = v1.x;
	scrn->_170.y         = v1.y;
	scrn->_170.z         = v1.z;
	scrn->_16C           = 1.0f;
	scrn->_188           = 0;
	scrn->_18C           = 0.0f;
	const Vec v2         = { 0.0f, 0.0f, 0.0f };
	scrn->_190.x         = v2.x;
	scrn->_190.y         = v2.y;
	scrn->_190.z         = v2.z;
	scrn->_19C           = NULL;
	scrn->_1A0           = 0;
	scrn->_1A4           = 0.0f;
	const Vec v3         = { 0.0f, 0.0f, 0.0f };
	scrn->_1A8.x         = v3.x;
	scrn->_1A8.y         = v3.y;
	scrn->_1A8.z         = v3.z;
	scrn->_1B4           = 0;
	scrn->_1B8           = 0.0f;
	scrn->mTimer         = 0.0f;
	scrn->mTimerShown    = 0;
	scrn->_1C4           = 0;
	scrn->mScanlineAlpha = 0.0f;
	scrn->_1CC           = 0.0f;
	scrn->_184           = 0;
	scrn->_1D0           = 0;
	scrn->_CC            = 0.0f;
	scrn->_D0            = 0.0f;
	scrn->_D4            = 0;
	scrn->_D8            = 0;
	scrn->_DC            = 1.4f;
	f32 d                = scrn->_CC - scrn->_D0;
	for (int i = 0; i < 4; i++) {
		scrn->_E0[i]  = 0.0f;
		scrn->_F0[i]  = 0;
		scrn->_100[i] = 0;
		scrn->_104[i] = d;
		scrn->_114[i] = 0.0f;
	}
	scrn->_124     = 0;
	scrn->_134     = 0;
	scrn->_138     = 0.0f;
	scrn->_13C     = 0.0f;
	sScale         = 1.0f;
	scrn->mItemMax = 5;
}

/**
 * @note Address: 0x8006D548
 * @note Size: 0x5C
 */
extern "C" void fn_8006D548(InfoScrn* scrn, u32 frames)
{
	fn_80023B38(&scrn->mFade, frames);
	scrn->mWipe.Update(frames);
	fn_8006AF34(scrn, frames);
}

/**
 * @note Address: 0x8006D498
 * @note Size: 0xB0
 */
extern "C" void fn_8006D498(InfoScrn* scrn)
{
	fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
	fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 3, 0);
	fn_8002390C(&scrn->mFade, &lbl_8025C400.mGs);
	scrn->mWipe.Draw(&lbl_8025C400.mGs, 0);
	fn_8006AB00(scrn);
}

// the loading bar, the robot walks along it
static const u8 sWalkAnim[]  = { 3, 2, 1, 0, 3, 1, 2, 0xFF };
static const u8 sStandAnim[] = { 0, 1, 2, 3, 0xFF };
static struct {
	f32 mX;          // _00
	s32 mFrame;      // _04
	const u8* mAnim; // _08
} sLoading = { 0.0f, 0, sWalkAnim };

/**
 * @note Address: 0x8006CB68
 * @note Size: 0x930
 */
// NONMATCHING: the robot's x reads the struct through a register, scratch registers
extern "C" void fn_8006CB68(InfoScrn* scrn)
{
	if (lbl_8025C400.mStatus.mFlags & 0x40) {
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 3, 0);
		if (lbl_8025C400.mXObjs.mObjectCount > 0) {
			f32 fill;
			f32 max;
			f32 y;
			f32 left;
			f32 half;
			f32 tw;
			f32 th;
			f32 scale = 1.0f / lbl_8025C400.mXObjs.mObjectCount;
			max       = lbl_8025C400.mXObjs.mObjectCount;
			fill      = max * (lbl_8025C400.mXObjs._11D0 * scale);
			CTexObj tex;
			if (sLoading.mAnim == sWalkAnim) {
				if (sWalkAnim[sLoading.mFrame] == 0xFF) {
					sLoading.mFrame = 0;
				}
				tex = (&lbl_8025C400.mTpl)
				          ->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[0x82 + sWalkAnim[sLoading.mFrame]], 0);
			} else {
				if (sStandAnim[sLoading.mFrame / 2] == 0xFF) {
					sLoading.mFrame = 0;
				}
				tex = (&lbl_8025C400.mTpl)
				          ->mTex.Init(
				              &((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[0x86 + sStandAnim[sLoading.mFrame / 2]], 0);
			}
			tw = GXGetTexObjWidth(&tex);
			th = GXGetTexObjHeight(&tex);
			fn_8019B360(lbl_8025C400.mGs.mCurRenderMode, 1);
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 2);
			half = max / 2;
			y    = 406.0f - th / 2;
			left = 320.0f - half;
			fn_80197800(&lbl_8025C400.mGs, &tex, 0x808080C8, 0x808080C8, 0x808080C8, 0x808080C8, sLoading.mX + left, y, 0.0f, 0.9f * tw,
			            0.9f * th, 0.0f, 3.1415927f, 0.0f);
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
			fn_80197800(&lbl_8025C400.mGs, &tex, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, sLoading.mX + left, y, 0.0f, 0.8f * tw,
			            0.8f * th, 0.0f, 3.1415927f, 0.0f);
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
			fn_8019B360(lbl_8025C400.mGs.mCurRenderMode, 2);

			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
			tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[117], 0);
			PSMTXScale(tex.mMtx[1], 0.25f, 1.0f, 1.0f);
			tex.mMtxDirty = TRUE;
			PSMTXTrans(tex.mMtx[0], 0.25f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(&lbl_8025C400.mGs, &tex, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 322.0f, 408.0f, 0.0f, max, 14.0f, 0.0f,
			            0.0f, 3.1415927f);
			PSMTXTrans(tex.mMtx[0], 0.5f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(&lbl_8025C400.mGs, &tex, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 322.0f - half - 4.0f, 408.0f, 0.0f, 8.0f,
			            14.0f, 0.0f, 0.0f, 3.1415927f);
			PSMTXTrans(tex.mMtx[0], 0.0f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(&lbl_8025C400.mGs, &tex, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 4.0f + (322.0f + half), 408.0f, 0.0f, 8.0f,
			            14.0f, 0.0f, 0.0f, 3.1415927f);

			tex      = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[118], 0);
			u16 barW = GXGetTexObjWidth(&tex);
			PSMTXScale(tex.mMtx[1], fill / barW, 1.0f, 1.0f);
			tex.mMtxDirty      = TRUE;
			static f32 sScroll = 0.0f;
			sScroll -= 0.1f;
			PSMTXTrans(tex.mMtx[0], sScroll, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(&lbl_8025C400.mGs, &tex, 0x9FFA1CC8, 0x9FFA1CC8, 0x9FFA1CC8, 0x9FFA1CC8, 321.0f - (max - fill) / 2, 408.0f, 0.0f,
			            fill, 10.0f, 0.0f, 0.0f, 0.0f);
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);

			sLoading.mFrame++;
			if ((sLoading.mX += 6.0f) >= max) {
				sLoading.mX = max;
				if (sLoading.mAnim == sWalkAnim) {
					sLoading.mAnim  = sStandAnim;
					sLoading.mFrame = 0;
				}
			}
		} else {
			sLoading.mX     = 0.0f;
			sLoading.mFrame = 0;
			sLoading.mAnim  = sWalkAnim;
		}
		const char* text = fn_8006DBF0(0x59);
		f32 w            = fn_801968BC(&lbl_8025C400.mGs, text, 1, 16.0f);
		fn_80196610(&lbl_8025C400, -1, text, 320.0f - w / 2, 386.0f, 0.0f, 16.0f, 16.0f);
	}
}

/**
 * @note Address: 0x8006CA90
 * @note Size: 0xD8
 */
extern "C" void fn_8006CA90(InfoScrn* scrn)
{
	if (lbl_8025C400.mStatus.mFlags & 0x200000000000ULL) {
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 3, 0);
		const char* text = "\\B00000080 PAUSE ";
		f32 w            = fn_801968BC(&lbl_8025C400.mGs, text, 1, 24.0f);
		fn_80196610(&lbl_8025C400, -1, text, 320.0f - w / 2, 200.0f, 0.0f, 24.0f, 24.0f);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 3, 0);
	}
}

/**
 * @note Address: 0x8006AB00
 * @note Size: 0x434
 */
// NONMATCHING: the bss address and the zero swap r6 and r7
extern "C" void fn_8006AB00(InfoScrn* scrn)
{
	if (IsHudOn() || (lbl_8025C400.mStatus.mFlags & 0x200000000000ULL)) {
		fn_8006389C(&sPoint1);
		fn_8006389C(&sPoint0);
		fn_8006389C(&sPoint2);
		fn_80068D10(scrn);
		fn_80067928(scrn);
		f32 alpha = scrn->_18C * sScale;
		if (alpha > 0.0f) {
			CTexObj tex;
			if (scrn->_B4 % 32 >= 8) {
				tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[150], 0);
			} else {
				tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[149], 0);
			}
			f32 h;
			f32 w;
			f32 y;
			f32 x;
			u16 tw = GXGetTexObjWidth(&tex);
			w      = 0.8f * (1.1f * tw);
			u16 th = GXGetTexObjHeight(&tex);
			h      = 0.8f * th;
			x      = 6.0f + (w / 2 + scrn->_190.x);
			y      = scrn->_190.y - h / 2 - 6.0f;
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
			u32 color = (u32)(255.0f * alpha) | 0xC8C8C800;
			fn_80197800(&lbl_8025C400.mGs, &tex, color, color, color, color, x, y, 0.0f, w, h, 0.0f, 0.0f, 0.0f);
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		}
		fn_80067418(scrn);
		fn_80066C08(scrn);
		fn_80066714(scrn);
	}
}

/**
 * @note Address: 0x80068C8C
 * @note Size: 0x84
 */
extern "C" void fn_80068C8C(f32* cur, f32* target, f32 step)
{
	Approach(cur, target, step);
}

/**
 * @note Address: 0x80068C24
 * @note Size: 0x68
 */
extern "C" void fn_80068C24(InfoScrn* scrn, f32* y, f32* x)
{
	f32 right = GetRight() - 64.0f - 40.0f - 9.0f;
	if (y) {
		*y = 60.0f;
	}
	if (x) {
		*x = right;
	}
}

/**
 * @note Address: 0x80068C10
 * @note Size: 0x14
 */
extern "C" void fn_80068C10(InfoScrn* scrn)
{
	sPoint1.mVisible = TRUE;
}

/**
 * @note Address: 0x80068BFC
 * @note Size: 0x14
 */
extern "C" void fn_80068BFC(InfoScrn* scrn)
{
	sPoint1.mVisible = FALSE;
}

/**
 * @note Address: 0x80068BE8
 * @note Size: 0x14
 */
extern "C" void fn_80068BE8(InfoScrn* scrn)
{
	sPoint0.mVisible = TRUE;
}

/**
 * @note Address: 0x80068BD4
 * @note Size: 0x14
 */
extern "C" void fn_80068BD4(InfoScrn* scrn)
{
	sPoint0.mVisible = FALSE;
}

/**
 * @note Address: 0x80068BC0
 * @note Size: 0x14
 */
extern "C" void fn_80068BC0(InfoScrn* scrn)
{
	sPoint2.mVisible = TRUE;
}

/**
 * @note Address: 0x80068BAC
 * @note Size: 0x14
 */
extern "C" void fn_80068BAC(InfoScrn* scrn)
{
	sPoint2.mVisible = FALSE;
}

/**
 * @note Address: 0x80068B98
 * @note Size: 0x14
 */
extern "C" void fn_80068B98(InfoScrn* scrn)
{
	sPoint2.mState = 2;
}

/**
 * @note Address: 0x80068B70
 * @note Size: 0x28
 */
extern "C" void fn_80068B70(InfoScrn* scrn)
{
	sPoint1._75 = TRUE;
	sPoint0._75 = TRUE;
	sPoint2._75 = TRUE;
}

/**
 * @note Address: 0x80068B48
 * @note Size: 0x28
 */
extern "C" void fn_80068B48(InfoScrn* scrn)
{
	sPoint1._75 = FALSE;
	sPoint0._75 = FALSE;
	sPoint2._75 = FALSE;
}

/**
 * @note Address: 0x80067418
 * @note Size: 0x510
 */
extern "C" void fn_80067418(InfoScrn* scrn)
{
	f32 alpha = scrn->_1A4 * sScale;
	if (alpha > 0.0f) {
		CTexObj tex;
		s32 frame = scrn->_B4 % 8;
		if (frame < 2) {
			tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[149], 0);
		} else if (frame < 4) {
			tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[150], 0);
		} else if (frame < 6) {
			tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[149], 0);
		} else {
			tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[151], 0);
		}
		f32 x;
		f32 y;
		f32 w;
		f32 h;
		u16 tw  = GXGetTexObjWidth(&tex);
		w       = 0.8f * (1.1f * tw);
		u16 th  = GXGetTexObjHeight(&tex);
		h       = 0.8f * th;
		Vec ofs = { 0.0f, 10.0f, 0.0f };
		f32 sx;
		f32 sy;
		lbl_8025C400.GetPlayer()->vf10C(&ofs, &sx, &sy, NULL, 0);
		x = 6.0f + (w / 2 + sx);
		y = sy - h / 2 - 6.0f;
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		u32 color = (u32)(255.0f * alpha) | 0xC8C8C800;
		fn_80197800(&lbl_8025C400.mGs, &tex, color, color, color, color, x, y, 0.0f, w, h, 0.0f, 0.0f, 0.0f);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
	}
}

/**
 * @note Address: 0x80066C08
 * @note Size: 0x810
 */
extern "C" void fn_80066C08(InfoScrn* scrn)
{
	f32 alpha = scrn->_1B8 * sScale;
	CTexObj tex;
	if (alpha > 0.0f) {
		u32 a      = 255.0f * alpha;
		u32 top    = a | 0xEFEBC500;
		u32 bottom = a | 0xEDA83600;
		f32 clock  = FramesToClock(scrn->mTimer);
		f32 step;
		f32 w;
		f32 h;
		f32 smallW;
		f32 smallH;
		f32 value = 10000.0f * (s32)(clock / 6000.0f) + (s32)clock % 6000;
		tex       = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[125], 0);
		u16 tw    = GXGetTexObjWidth(&tex);
		w         = 1.8f * tw;
		u16 th    = GXGetTexObjHeight(&tex);
		h         = 1.8f * (th / 10);
		tw        = GXGetTexObjWidth(&tex);
		smallW    = 0.7f * (1.8f * tw);
		th        = GXGetTexObjHeight(&tex);
		smallH    = 0.7f * (1.8f * (th / 10));
		PSMTXScale(tex.mMtx[1], 1.0f, 0.1f, 1.0f);
		tex.mMtxDirty = TRUE;
		step          = 7.0f * w;
		s32 digit;
		for (s32 i = 0; i < 6; i++) {
			f32 div = 1.0f;
			if (i > 0) {
				for (s32 j = 0; j < i; j++) {
					div *= 10.0f;
				}
			}
			digit = (s32)(value / div);
			digit %= 10;
			PSMTXTrans(tex.mMtx[0], 0.0f, 0.1f * digit, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
			f32 ofs = 0.0f;
			if (i == 0) {
				ofs = -7.2f;
			} else if (i == 1) {
				ofs = -1.8f;
			} else if (i == 4 || i == 5) {
				ofs = -9.0f;
			}
			if (i == 5 && digit == 0) {
				continue;
			}
			if (i < 2) {
				fn_80197800(&lbl_8025C400.mGs, &tex, top, top, bottom, bottom, ofs + (step + (372.0f - i * (w - 3.6f)) - 14.4f), 75.6f,
				            0.0f, smallW, smallH, 0.0f, 0.0f, 0.0f);
			} else {
				fn_80197800(&lbl_8025C400.mGs, &tex, top, top, bottom, bottom, ofs + (step + (372.0f - i * (w - 3.6f)) - 14.4f), 72.0f,
				            0.0f, w, h, 0.0f, 0.0f, 0.0f);
			}
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		}
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		tex        = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[129], 0);
		u16 colonW = GXGetTexObjWidth(&tex);
		u16 colonH = GXGetTexObjHeight(&tex);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		fn_80197800(&lbl_8025C400.mGs, &tex, top, top, bottom, bottom, 472.8f, 72.9f, 0.0f, 0.6f * (1.8f * colonW), 0.6f * (1.8f * colonH),
		            0.0f, 0.0f, 0.0f);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
	}
}

/**
 * @note Address: 0x80066BFC
 * @note Size: 0xC
 */
extern "C" void fn_80066BFC(InfoScrn* scrn)
{
	scrn->mTimerShown = TRUE;
}

/**
 * @note Address: 0x80066BF0
 * @note Size: 0xC
 */
extern "C" void fn_80066BF0(InfoScrn* scrn)
{
	scrn->mTimerShown = FALSE;
}

/**
 * @note Address: 0x80066B9C
 * @note Size: 0x54
 */
extern "C" void fn_80066B9C(InfoScrn* scrn, s32 hundredths)
{
	scrn->mTimer = (f32)TIMER_RATE * (hundredths / 100.0f);
	if (scrn->mTimer >= (f32)TIMER_MAX) {
		scrn->mTimer = (f32)TIMER_MAX;
	}
}

/**
 * @note Address: 0x80066A84
 * @note Size: 0x118
 */
extern "C" s32 fn_80066A84(InfoScrn* scrn)
{
	return FramesToClock(scrn->mTimer);
}

/**
 * @note Address: 0x8006697C
 * @note Size: 0x108
 */
// NONMATCHING: the three int to float conversions land in other registers
extern "C" f32 fn_8006697C(f32 frames)
{
	f32 seconds    = frames / (f32)TIMER_RATE;
	s32 hundredths = ((s32)frames % TIMER_RATE) * 100 / TIMER_RATE;
	if (frames >= (f32)TIMER_MAX) {
		hundredths = 99;
	}
	f32 minutes = seconds / 60.0f;
	f32 clock   = hundredths;
	clock += 6000.0f * (s32)minutes + ((s32)seconds % 60) * 100;
	return clock;
}

/**
 * @note Address: 0x80066714
 * @note Size: 0x268
 */
extern "C" void fn_80066714(InfoScrn* scrn)
{
	f32 alpha = scrn->mScanlineAlpha;
	if (alpha > 0.0f) {
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
		fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 1);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 0);
		u32 a = 255.0f * alpha;

		static f32 sScroll = 0.0f;
		if (!lbl_8025C400.mMenu.mActive) {
			sScroll += 1.0f;
			if (sScroll >= 4.0f) {
				sScroll = 0.0f;
			}
		}

		fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 3, 4, 5, 15);
		fn_8019B34C(lbl_8025C400.mGs.mCurRenderMode, 12, 5);
		u32 color = a | 0x30303000;
		for (s32 y = 0; y < 448; y += 4.0f) {
			fn_801992BC(&lbl_8025C400.mGs, NULL, 0.0f, y + sScroll, 0.0f, color, 640.0f, y + sScroll, 0.0f, color);
		}
		fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
		fn_8019B34C(lbl_8025C400.mGs.mCurRenderMode, 12, 5);
		for (int i = 9; i > 0; i--) {
			sHistory[i] = sHistory[i - 1];
		}
		sHistory[0] = 0.0f;
		fn_8019B34C(lbl_8025C400.mGs.mCurRenderMode, 12, 5);
	}
}

static const char* sAscii = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}";
static const char* sWide  = "\x81\x40\x81\x49\x81\x68\x81\x94\x81\x90\x81\x93\x81\x95\x81\x66"
                            "\x81\x69\x81\x6A\x81\x96\x81\x7B\x81\x43\x81\x5B\x81\x44\x81\x5E"
                            "\x82\x4F\x82\x50\x82\x51\x82\x52\x82\x53\x82\x54\x82\x55\x82\x56"
                            "\x82\x57\x82\x58\x81\x46\x81\x47\x81\x83\x81\x81\x81\x84\x81\x48"
                            "\x81\x97\x82\x60\x82\x61\x82\x62\x82\x63\x82\x64\x82\x65\x82\x66"
                            "\x82\x67\x82\x68\x82\x69\x82\x6A\x82\x6B\x82\x6C\x82\x6D\x82\x6E"
                            "\x82\x6F\x82\x70\x82\x71\x82\x72\x82\x73\x82\x74\x82\x75\x82\x76"
                            "\x82\x77\x82\x78\x82\x79\x81\x6D\x81\x8F\x81\x6E\x81\x4F\x81\x51"
                            "\x81\x65\x82\x81\x82\x82\x82\x83\x82\x84\x82\x85\x82\x86\x82\x87"
                            "\x82\x88\x82\x89\x82\x8A\x82\x8B\x82\x8C\x82\x8D\x82\x8E\x82\x8F"
                            "\x82\x90\x82\x91\x82\x92\x82\x93\x82\x94\x82\x95\x82\x96\x82\x97"
                            "\x82\x98\x82\x99\x82\x9A\x81\x6F\x81\x62\x81\x70";

/**
 * @note Address: 0x80066630
 * @note Size: 0xE4
 */
extern "C" s32 fn_80066630(char* text)
{
	char buf[0x200];
	char* out = buf;
	char* dst;
	char* p;
	s32 count = 0;
	for (p = text, dst = out; *p != '\0'; p++) {
		BOOL found = FALSE;
		for (s32 i = 0; sAscii[i] != '\0'; i++) {
			if (*p == sAscii[i]) {
				strncpy(dst, &sWide[i * 2], 2)[2] = '\0';
				found                             = TRUE;
				count++;
				dst += 2;
				break;
			}
		}
		if (!found) {
			strncpy(dst, p, 1)[1] = '\0';
			dst++;
		}
	}
	strcpy(text, out);
	return count;
}

/**
 * @note Address: 0x800665FC
 * @note Size: 0x34
 */
extern "C" BOOL fn_800665FC(InfoScrn* scrn)
{
	return scrn->_1D4 == 10.0f * lbl_8025C400.GetPlayer()->mParams.mMaxBattery;
}

/**
 * @note Address: 0x800665E0
 * @note Size: 0x1C
 */
extern "C" BOOL fn_800665E0(InfoScrn* scrn)
{
	return scrn->_1D4 <= 0.0f;
}

/**
 * @note Address: 0x8006656C
 * @note Size: 0x74
 */
extern "C" void fn_8006656C(InfoScrn* scrn, f32 x)
{
	for (int i = 0; i < 5; i++) {
		scrn->_E0[i]  = 1.0f;
		scrn->_100[i] = 0;
		scrn->_104[i] = 0.0f;
		scrn->_114[i] = 0.0f;
	}
	scrn->_D0 = 10.0f * x;
	scrn->_CC = scrn->_D0;
}

/**
 * @note Address: 0x800664C4
 * @note Size: 0xA8
 */
extern "C" void fn_800664C4(InfoScrn* scrn, u8 show, s32 mode)
{
	switch (mode) {
	case 0:
		if (show) {
			scrn->_BC = 1;
		} else {
			scrn->_BC = 2;
		}
		break;
	case 1:
		if (show) {
			scrn->_BC = 3;
		} else {
			scrn->_BC = 4;
		}
		break;
	case 2:
		if (show) {
			scrn->_C8 = -52.0f;
		} else {
			scrn->_C8 = 0.0f;
		}
		break;
	case 3:
		if (show) {
			scrn->_BC = 5;
		} else {
			scrn->_BC = 6;
		}
		break;
	}
}

/**
 * @note Address: 0x80066250
 * @note Size: 0x274
 */
// NONMATCHING: regalloc in the item loop
extern "C" void fn_80066250(InfoScrn* scrn, u8 show, s32 mode, s32 flag)
{
	switch (mode) {
	case 0:
		if (show) {
			scrn->_148 = 1;
		} else {
			scrn->_148 = 2;
		}
		break;
	case 1:
		if (show) {
			scrn->_148 = 3;
		} else {
			scrn->_148 = 4;
		}
		break;
	case 2:
		scrn->mItemMax = ItemMax();
		break;
	case 3:
		scrn->_148         = 5;
		scrn->mNextItemMax = ItemMax();
		break;
	case 4:
		if (show) {
			scrn->_148 = 7;
		} else {
			scrn->_148 = 8;
		}
		break;
	}
}

/**
 * @note Address: 0x800661E4
 * @note Size: 0x6C
 */
extern "C" void fn_800661E4(InfoScrn* scrn, XObj* obj, u8 show, s32 mode)
{
	scrn->_19C = NULL;
	switch (mode) {
	case 0:
		if (show) {
			scrn->_188 = 1;
			scrn->_19C = obj;
		} else {
			scrn->_188 = 2;
			scrn->_19C = NULL;
		}
		break;
	case 1:
		if (show) {
			scrn->_188 = 3;
			scrn->_19C = obj;
		} else {
			scrn->_188 = 4;
			scrn->_19C = NULL;
		}
		break;
	}
}

/**
 * @note Address: 0x80066190
 * @note Size: 0x54
 */
extern "C" void fn_80066190(InfoScrn* scrn, u8 show, s32 mode)
{
	switch (mode) {
	case 0:
		if (show) {
			scrn->_1A0 = 1;
		} else {
			scrn->_1A0 = 2;
		}
		break;
	case 1:
		if (show) {
			scrn->_1A0 = 3;
		} else {
			scrn->_1A0 = 4;
		}
		break;
	}
}

/**
 * @note Address: 0x8006613C
 * @note Size: 0x54
 */
extern "C" void fn_8006613C(InfoScrn* scrn, u8 show, s32 mode)
{
	switch (mode) {
	case 0:
		if (show) {
			scrn->_1B4 = 1;
		} else {
			scrn->_1B4 = 2;
		}
		break;
	case 1:
		if (show) {
			scrn->_1B4 = 3;
		} else {
			scrn->_1B4 = 4;
		}
		break;
	}
}

/**
 * @note Address: 0x800660E8
 * @note Size: 0x54
 */
extern "C" void fn_800660E8(InfoScrn* scrn, u8 show, s32 mode)
{
	switch (mode) {
	case 0:
		if (show) {
			scrn->_1C4 = 1;
		} else {
			scrn->_1C4 = 2;
		}
		break;
	case 1:
		if (show) {
			scrn->_1C4 = 3;
		} else {
			scrn->_1C4 = 4;
		}
		break;
	}
}

/**
 * @note Address: 0x800660E4
 * @note Size: 0x4
 */
extern "C" void fn_800660E4(InfoScrn* scrn, s32 delta)
{
}

/**
 * @note Address: 0x800660E0
 * @note Size: 0x4
 */
extern "C" void fn_800660E0(InfoScrn* scrn, s32 level)
{
}

/**
 * @note Address: 0x80065E44
 * @note Size: 0x29C
 */
extern "C" void fn_80065E44(u32 color, f32 x, f32 y, f32 w, f32 h, f32 rx, f32 ry)
{
	Vec pos[65];
	f32 right;
	f32 bottom;
	f32 sx;
	f32 sy;
	sx     = 9.0f * rx;
	sy     = 9.0f * ry;
	right  = x + w;
	bottom = y + h;
	for (int i = 0; i < 65; i++) {
		f32 cx;
		f32 cy;
		f32 s;
		f32 angle = 0.09817477f * i - 1.5707964f;
		s         = sin(angle);
		f32 c     = -(f32)cos(angle);
		if ((i >= 0 && i < 16) || i == 64) {
			cx = x;
			cy = y;
		} else if ((i >= 16 && i < 32) || i == 64) {
			cx = right;
			cy = y;
		} else if ((i >= 32 && i < 48) || i == 64) {
			cx = right;
			cy = bottom;
		} else {
			cx = x;
			cy = bottom;
		}
		pos[i].x = s * sx + cx - w / 2;
		pos[i].y = c * sy + cy - h / 2;
		pos[i].z = 0.0f;
	}
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 1);
	fn_80198D08(&lbl_8025C400.mGs, (GXPrimitive)0xA0, lbl_8025C400.mGs.mCamera->mViewMtx, pos, color, 65);
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 0);
}

/**
 * @note Address: 0x800652C0
 * @note Size: 0xB84
 */
// NONMATCHING: vertex temporaries and floating-point register allocation
extern "C" void fn_800652C0(u32 color, u32 textureColor, f32 x, f32 y, f32 width, f32 height, f32 alpha, f32 scaleX, f32 scaleY)
{
	f32 angle  = 4.712389f * (1.0f - alpha);
	f32 sx     = 4.0f * (f32)sin(angle);
	f32 sy     = 4.0f * (f32)sin(angle);
	f32 left   = x - width / 2.0f - 40.0f - sx;
	f32 right  = sx + (40.0f + (x + width / 2.0f));
	f32 top    = y - 8.0f - sy;
	f32 bottom = sy + (8.0f + (y + height));
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 1);
	fn_80198614(&lbl_8025C400.mGs, left - 8.0f, top - 8.0f, -1.0f, 0xFF000000, 9.0f + (8.0f + right), top - 8.0f, -1.0f, 0xFF000000,
	            9.0f + (8.0f + right), 9.0f + (8.0f + bottom), -1.0f, 0xFF000000, left - 8.0f, 9.0f + (8.0f + bottom), -1.0f, 0xFF000000);
	Vec pos[10] = {
		(Vec) { x, y, 0.0f },
		(Vec) { (1.0f + left), (8.0f + (1.0f + top)), 0.0f },
		(Vec) { (8.0f + (1.0f + left)), (1.0f + top), 0.0f },
		(Vec) { ((right - 1.0f) - 8.0f), (1.0f + top), 0.0f },
		(Vec) { (right - 1.0f), (8.0f + (1.0f + top)), 0.0f },
		(Vec) { (right - 1.0f), ((bottom - 1.0f) - 8.0f), 0.0f },
		(Vec) { ((right - 1.0f) - 8.0f), (bottom - 1.0f), 0.0f },
		(Vec) { (8.0f + (1.0f + left)), (bottom - 1.0f), 0.0f },
		(Vec) { (1.0f + left), ((bottom - 1.0f) - 8.0f), 0.0f },
		(Vec) { (1.0f + left), (8.0f + (1.0f + top)), 0.0f },
	};
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 1);
	fn_80198D08(&lbl_8025C400.mGs, (GXPrimitive)0xA0, lbl_8025C400.mGs.mCamera->mViewMtx, pos, color, 10);
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 1, 1);
	fn_80198258(&lbl_8025C400.mGs, 16, color, color, (8.0f + (1.0f + left)), (8.0f + (1.0f + top)), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f);
	fn_80198258(&lbl_8025C400.mGs, 16, color, color, ((right - 1.0f) - 8.0f), (8.0f + (1.0f + top)), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f);
	fn_80198258(&lbl_8025C400.mGs, 16, color, color, (8.0f + (1.0f + left)), ((bottom - 1.0f) - 8.0f), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f,
	            0.0f);
	fn_80198258(&lbl_8025C400.mGs, 16, color, color, ((right - 1.0f) - 8.0f), ((bottom - 1.0f) - 8.0f), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f,
	            0.0f);
	u32 shadow = (u32)(64.0f * alpha);
	for (int i = 0; i < 10; i++) {
		pos[i].x += 9.0f;
		pos[i].y += 9.0f;
	}
	fn_80198D08(&lbl_8025C400.mGs, (GXPrimitive)0xA0, lbl_8025C400.mGs.mCamera->mViewMtx, pos, shadow, 10);
	fn_80198258(&lbl_8025C400.mGs, 16, shadow, shadow, (8.0f + (1.0f + (9.0f + left))), (8.0f + (1.0f + (9.0f + top))), 0.0f, 16.0f, 16.0f,
	            0.0f, 0.0f, 0.0f);
	fn_80198258(&lbl_8025C400.mGs, 16, shadow, shadow, ((9.0f + right) - 1.0f - 8.0f), (8.0f + (1.0f + (9.0f + top))), 0.0f, 16.0f, 16.0f,
	            0.0f, 0.0f, 0.0f);
	fn_80198258(&lbl_8025C400.mGs, 16, shadow, shadow, (8.0f + (1.0f + (9.0f + left))), ((9.0f + bottom) - 1.0f - 8.0f), 0.0f, 16.0f, 16.0f,
	            0.0f, 0.0f, 0.0f);
	fn_80198258(&lbl_8025C400.mGs, 16, shadow, shadow, ((9.0f + right) - 1.0f - 8.0f), ((9.0f + bottom) - 1.0f - 8.0f), 0.0f, 16.0f, 16.0f,
	            0.0f, 0.0f, 0.0f);
	CTexObj tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[114], 0);
	f32 tw      = GXGetTexObjWidth(&tex);
	f32 th      = GXGetTexObjHeight(&tex);
	f32 w       = right - left - 128.0f;
	if (w < 0.0f)
		w = 0.0f;
	f32 h = bottom - top - 64.0f;
	if (h < 0.0f)
		h = 0.0f;
	f32 tiles[8][8] = {
		{ 0.0f, 0.0f, .5f, .5f, (32.0f + left - 3.0f), (8.0f + ((16.0f + top) - 10.0f)), 64.0f, 32.0f },
		{ .5f, 0.0f, 1.0f / tw, .5f, x, (8.0f + ((16.0f + top) - 10.0f)), (6.0f + w), 32.0f },
		{ .5f, 0.0f, .5f, .5f, (3.0f + (right - 32.0f)), (8.0f + ((16.0f + top) - 10.0f)), 64.0f, 32.0f },
		{ 0.0f, .5f, .5f, 1.0f / th, (32.0f + left - 3.0f), (4.0f + (height * 0.5f + y)), 64.0f, ((20.0f + h) - 8.0f) },
		{ .5f, .5f, .5f, 1.0f / th, (3.0f + (right - 32.0f)), (4.0f + (height * 0.5f + y)), 64.0f, ((20.0f + h) - 8.0f) },
		{ 0.0f, .5f, .5f, .5f, (32.0f + left - 3.0f), (10.0f + (bottom - 16.0f)), 64.0f, 32.0f },
		{ .5f, .5f, 1.0f / tw, .5f, x, (10.0f + (bottom - 16.0f)), (6.0f + w), 32.0f },
		{ .5f, .5f, .5f, .5f, (3.0f + (right - 32.0f)), (10.0f + (bottom - 16.0f)), 64.0f, 32.0f },
	};
	fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 1, 1);
	for (int i = 0; i < 8; i++) {
		PSMTXTrans(tex.mMtx[0], tiles[i][0], tiles[i][1], 0.0f);
		tex.mMtxDirty = TRUE;
		PSMTXScale(tex.mMtx[1], tiles[i][2], tiles[i][3], 1.0f);
		tex.mMtxDirty = TRUE;
		fn_80197800(&lbl_8025C400.mGs, &tex, textureColor, textureColor, textureColor, textureColor, tiles[i][4], tiles[i][5], 1.0f,
		            tiles[i][6], tiles[i][7], 0.0f, 0.0f, 0.0f);
	}
	fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 1);
	fn_80198614(&lbl_8025C400.mGs, left - 8.0f, top - 8.0f, -1.0f, 0xFF000000, 9.0f + (8.0f + right), top - 8.0f, -1.0f, 0xFF000000,
	            9.0f + (8.0f + right), 9.0f + (8.0f + bottom), -1.0f, 0xFF000000, left - 8.0f, 9.0f + (8.0f + bottom), -1.0f, 0xFF000000);
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 3, 0);
}

/**
 * @note Address: 0x800650AC
 * @note Size: 0x214
 */
extern "C" void fn_800650AC(InfoPoint* point, s32 id)
{
	point->mIndex = id;
	point->_18    = 0;
	point->_08    = 0;
	point->_0C    = 0;
	point->_10    = 0;
	point->_14    = 0.0f;
	for (int i = 0; i < 6; i++) {
		point->mPos[i].x = -220.0f;
	}
	for (int i = 0; i < 4; i++) {
		point->mValue[i] = 0.0f;
	}
	point->mVisible = 0;
	point->_75      = 0;
	switch (point->mIndex) {
	case INFOPOINT_ID + 2:
		point->mResource = point->mIndex;
		point->mPos[0].y = GetRight() - 64.0f - 40.0f;
		point->mPos[1].y = GetRight() - 64.0f - 40.0f;
		break;
	case INFOPOINT_ID:
		point->mResource = point->mIndex;
		point->mPos[0].y = GetRight() - 64.0f;
		point->mPos[1].y = GetRight() - 64.0f;
		break;
	case INFOPOINT_ID + 1:
		point->mResource = point->mIndex;
		point->mPos[0].y = 40.0f + (GetRight() - 64.0f);
		point->mPos[1].y = 40.0f + (GetRight() - 64.0f);
		break;
	}
	point->mState = 0;
	point->_7C    = 0.0f;
	point->_80    = 0;
}

/**
 * @note Address: 0x800643A4
 * @note Size: 0xD08
 */
extern "C" void fn_800643A4(InfoPoint* point, u32 frames)
{
	switch (point->mIndex) {
	case INFOPOINT_ID + 2:
		point->mValue[1] = lbl_8025C400.GetPlayer()->mParams.mExperience;
		break;
	case INFOPOINT_ID:
		point->mValue[1] = lbl_8025C400.GetPlayer()->mParams.mMoney;
		break;
	case INFOPOINT_ID + 1:
		point->mValue[1] = lbl_8025C400.GetPlayer()->mParams.mScrap;
		break;
	}

	bool changed = false;
	if (point->mValue[1] - point->mValue[0]) {
		changed = true;
	}
	if (IsPCOpen()) {
		point->mValue[0] = point->mValue[1];
		point->mValue[2] = point->mValue[3];
		point->mPos[1].x = -220.0f;
		point->mPos[0].x = -220.0f;
		point->mPos[3].x = -220.0f;
		point->mPos[2].x = -220.0f;
		point->mPos[5].x = -220.0f;
		point->mPos[4].x = -220.0f;
		point->_08       = 0;
		changed          = false;
	} else {
		f32 step = (point->mValue[1] - point->mValue[0]) / 4;
		if (step < 0.0f) {
			step = -step;
		}
		f32 speed = step;
		if (step <= 1.0f) {
			speed = 1.0f;
		}
		Approach(&point->mValue[0], &point->mValue[1], speed);
		Approach(&point->mValue[2], &point->mValue[3], 1.0f);
	}
	Approach(&point->mPos[0].x, &point->mPos[1].x, ApproachStep(point->mPos[0].x, point->mPos[1].x));
	Approach(&point->mPos[2].x, &point->mPos[3].x, ApproachStep(point->mPos[2].x, point->mPos[3].x));
	Approach(&point->mPos[4].x, &point->mPos[5].x, ApproachStep(point->mPos[4].x, point->mPos[5].x));

	bool empty = false;
	if (0.0f == lbl_8025C400.GetPlayer()->_A30) {
		empty = true;
	}
	if (point->_75) {
		empty = false;
	}
	switch (point->mIndex) {
	case INFOPOINT_ID + 2:
		point->mPos[0].y = GetRight() - 64.0f - 40.0f;
		point->mPos[1].y = GetRight() - 64.0f - 40.0f;
		break;
	case INFOPOINT_ID:
		point->mPos[0].y = GetRight() - 64.0f;
		point->mPos[1].y = GetRight() - 64.0f;
		break;
	case INFOPOINT_ID + 1:
		point->mPos[0].y = 40.0f + (GetRight() - 64.0f);
		point->mPos[1].y = 40.0f + (GetRight() - 64.0f);
		break;
	}

	switch (point->_08) {
	case 0:
		if (IsPCOpen()) {
			break;
		}
		if (empty) {
			point->mPos[1].x = 60.0f;
			point->_10       = 0;
			point->_08++;
		} else if (changed) {
			point->mPos[1].x = 60.0f;
			point->_10       = 0;
			point->_08       = 10;
		}
		break;
	case 1:
		point->_10++;
		if (point->_10 > 6) {
			point->mPos[3].x = 60.0f;
			point->_10       = 0;
			point->_08++;
		}
		break;
	case 2:
		point->_10++;
		if (point->_10 > 6) {
			point->mPos[5].x = 60.0f;
			point->_08++;
		}
		break;
	case 3:
		if (!empty) {
			point->mPos[1].x = -220.0f;
			point->_10       = 0;
			point->_08++;
		}
		break;
	case 4:
		if (changed) {
			point->mPos[1].x = 60.0f;
			point->_10       = 0;
			point->_08       = 10;
		} else {
			point->_10++;
			if (point->_10 > 6) {
				point->mPos[5].x = -220.0f;
				point->_10       = 0;
				point->_08++;
			}
		}
		break;
	case 5:
		if (changed) {
			point->mPos[1].x = 60.0f;
			point->_10       = 0;
			point->_08       = 10;
		} else {
			point->_10++;
			if (point->_10 > 6) {
				point->mPos[3].x = -220.0f;
				point->_08       = 0;
			}
		}
		break;
	case 10:
		point->_10++;
		if (point->_10 > 3) {
			point->mPos[3].x = 60.0f;
			point->_10       = 0;
			point->_08++;
		}
		break;
	case 11:
		point->_10++;
		if (point->_10 > 3) {
			point->mPos[5].x = 60.0f;
			point->_10       = 0;
			point->_08++;
		}
		break;
	case 12:
		if (empty) {
			point->_08 = 3;
		} else {
			point->_10++;
			if (changed) {
				point->_10 = 0;
			}
			if (point->_10 > 90) {
				point->mPos[1].x = -220.0f;
				point->_10       = 0;
				point->_08++;
			}
		}
		break;
	case 13:
		if (changed) {
			point->mPos[1].x = 60.0f;
			point->_10       = 0;
			point->_08       = 10;
		} else {
			point->_10++;
			if (point->_10 > 6) {
				point->mPos[5].x = -220.0f;
				point->_10       = 0;
				point->_08++;
			}
		}
		break;
	case 14:
		if (changed) {
			point->mPos[1].x = 60.0f;
			point->_10       = 0;
			point->_08       = 10;
		} else {
			point->_10++;
			if (point->_10 > 6) {
				point->mPos[3].x = -220.0f;
				point->_08       = 0;
			}
		}
		break;
	}

	if (point->mVisible) {
		point->mPos[1].x = 60.0f;
		point->mPos[3].x = 60.0f;
		point->mPos[5].x = 60.0f;
		point->_10       = 0;
		point->_08       = 3;
	}

	switch (point->_0C) {
	case 0:
		point->_14 = 1.0f;
		break;
	case 1:
		point->_14 = 1.0f;
		break;
	case 2:
		point->_14 = 0.0f;
		break;
	case 3:
		point->_14 += 0.08f * frames;
		if (point->_14 >= 1.0f) {
			point->_14 = 1.0f;
		}
		if (lbl_8025C400.mEvt.mData.mScript) {
			point->_0C = 4;
		}
		break;
	case 4:
		point->_14 -= 0.08f * frames;
		if (point->_14 < 0.0f) {
			point->_14 = 0.0f;
		}
		if (!lbl_8025C400.mEvt.mData.mScript) {
			point->_0C = 3;
		}
		break;
	}

	point->_7C = 1.0f;
	switch (point->mIndex) {
	case INFOPOINT_ID + 1:
		if (fn_80051C60()) {
			point->mState = 0;
			point->_18    = 0;
			point->_7C    = 1.0f;
			break;
		}
		switch (point->mState) {
		case 0:
			if (point->mValue[1] >= 300.0f) {
				point->mState++;
				point->_18 = 0;
			}
			break;
		case 1:
			point->_18 += frames;
			point->_7C = sin(0.017453292f * ((point->_18 * 6) % 360));
			if (point->_7C > 0.0f) {
				fn_8001E904(&lbl_8025C400.mSound, 0x183, 0xFF);
				point->mState++;
			}
			break;
		case 2:
			point->_18 += frames;
			point->_7C = sin(0.017453292f * ((point->_18 * 6) % 360));
			if (point->mValue[1] < 300.0f) {
				point->mState = 0;
			}
			break;
		case 3:
			if (point->mValue[1] < 300.0f) {
				point->mState = 0;
			}
			break;
		}
		break;
	}
}

/**
 * @note Address: 0x8006389C
 * @note Size: 0xB08
 */
// NONMATCHING: float registers
extern "C" void fn_8006389C(InfoPoint* point)
{
	f32 alpha = point->_14 * sScale;
	if (alpha > 0.0f) {
		CTexObj tex;
		s32 len = 1;
		f32 x0  = point->mPos[0].x;
		f32 y0  = point->mPos[0].y;
		f32 x2  = point->mPos[2].x;
		f32 x4  = point->mPos[4].x;
		while (true) {
			if ((s32)(point->mValue[0] / Pow10(len)) == 0) {
				break;
			}
			len++;
		}

		Vec pos[65];
		f32 cy = y0 - 7.2f;
		for (int i = 0; i < 65; i++) {
			f32 s;
			f32 angle = 0.09817477f * i - 1.5707964f;
			s         = sin(angle);
			f32 c     = -(f32)cos(angle);
			pos[i].x  = x0 + s * 14.4f;
			pos[i].y  = cy + c * 14.4f;
			pos[i].z  = 0.0f;
		}
		f32 a255 = 255.0f * alpha;
		u32 a    = a255;
		fn_8019B34C(lbl_8025C400.mGs.mCurRenderMode, 0x12, 5);
		fn_801990EC(&lbl_8025C400.mGs, pos, a, 65);
		fn_8019B34C(lbl_8025C400.mGs.mCurRenderMode, 12, 5);

		tex              = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[125], 0);
		u16 tw           = GXGetTexObjWidth(&tex);
		f32 lineY        = cy + 14.4f;
		point->mValue[3] = len * (1.2f * tw - 3.6f) + 34.0f;
		pos[0].x         = x0;
		pos[0].y         = lineY;
		pos[1].x         = x0 + point->mValue[2];
		pos[1].y         = lineY;
		fn_8019B34C(lbl_8025C400.mGs.mCurRenderMode, 0x12, 5);
		fn_801990EC(&lbl_8025C400.mGs, pos, a, 2);
		fn_8019B34C(lbl_8025C400.mGs.mCurRenderMode, 12, 5);

		tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[155], 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		u16 ew = GXGetTexObjWidth(&tex);
		u16 eh = GXGetTexObjHeight(&tex);
		u32 c  = a | 0xC8C8C800;
		fn_80197800(&lbl_8025C400.mGs, &tex, c, c, c, c, pos[1].x, pos[1].y, 0.0f, 1.2f * ew, 1.2f * eh, 0.0f, 0.0f, 0.0f);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);

		tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[point->mResource], 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		f32 iconW = 0.8f * GXGetTexObjWidth(&tex);
		f32 iconY = y0 - 9.0f;
		f32 iconH = 0.8f * GXGetTexObjHeight(&tex);
		if (point->mIndex == INFOPOINT_ID + 2) {
			u32 ic = (u32)(a255 * point->_7C) | 0xC8C8C800;
			fn_80197800(&lbl_8025C400.mGs, &tex, ic, ic, ic, ic, x4, 4.0f + iconY, 0.0f, iconW, iconH, 0.0f, 0.0f, 0.0f);
		} else {
			u32 ic = (u32)(a255 * point->_7C) | 0xC8C8C800;
			fn_80197800(&lbl_8025C400.mGs, &tex, ic, ic, ic, ic, x4, iconY, 0.0f, iconW, iconH, 0.0f, 0.0f, 0.0f);
		}

		tex    = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[125], 0);
		f32 dx = x2 - 12.0f;
		f32 dy = y0 - 4.0f;
		f32 w  = 1.2f * GXGetTexObjWidth(&tex);
		f32 h  = 1.2f * (GXGetTexObjHeight(&tex) / 10);
		PSMTXScale(tex.mMtx[1], 1.0f, 0.1f, 1.0f);
		u8 da         = (s32)a255;
		f32 step      = 7.0f * w;
		f32 base      = len * (w - 3.6f) - 117.6f;
		u32 blueT     = da | 0xCECEFF00;
		u32 blueB     = da | 0xE037EE00;
		u32 goldT     = da | 0xEFEBC500;
		u32 goldB     = da | 0xEDA83600;
		u32 pinkB     = da | 0x5959FF00;
		tex.mMtxDirty = TRUE;
		for (int i = 0; i < 7; i++) {
			s32 v = (s32)(point->mValue[0] / Pow10(i));
			if (i != 0 && v == 0) {
				continue;
			}
			PSMTXTrans(tex.mMtx[0], 0.0f, 0.1f * (v % 10), 0.0f);
			tex.mMtxDirty = TRUE;
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
			u32 top;
			u32 bottom;
			switch (point->mIndex) {
			case INFOPOINT_ID + 2:
				top    = blueT;
				bottom = blueB;
				break;
			case INFOPOINT_ID:
				top    = goldT;
				bottom = goldB;
				break;
			case INFOPOINT_ID + 1:
				top    = blueT;
				bottom = pinkB;
				break;
			}
			fn_80197800(&lbl_8025C400.mGs, &tex, top, top, bottom, bottom, base + (step + (dx - i * (w - 3.6f))), dy - 2.0f, 0.0f, w, h,
			            0.0f, 0.0f, 0.0f);
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		}
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
	}
}
