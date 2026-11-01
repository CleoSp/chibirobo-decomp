#include "game/infoscrn_shared.h"

/**
 * @note Address: 0x8006D850
 * @note Size: 0x2A4
 */
extern "C" void fn_8006D850(InfoScrn* self)
{
	self->mFade.Reset();
	self->mWipe.Reset();
	self->_A8  = 0.0f;
	self->_AC  = 0.0f;
	self->_B0  = 0.0f;
	self->_140 = 0.0f;
	self->_144 = 0.0f;
	self->_98  = 0;
	self->_9C  = 0;
	self->_A0  = 0;
	self->_A4  = 0;
	self->_B4  = 0;
	self->_1D4 = 0.0f;
	self->_B8  = 0;
	self->_17C = 0;
	self->_180 = 0;
	fn_800650AC(&sPoint0, 0x9A);
	fn_800650AC(&sPoint1, 0x98);
	fn_800650AC(&sPoint2, 0x99);
	sPoint0._74  = FALSE;
	sPoint1._74  = FALSE;
	sPoint2._74  = FALSE;
	self->_BC    = 0;
	self->_C0    = 0.0f;
	self->_C4    = 0.0f;
	self->_C8    = 0.0f;
	const Vec v0 = { 0.0f, 0.0f, 0.0f };
	self->_128.x = v0.x;
	self->_128.y = v0.y;
	self->_128.z = v0.z;
	self->_148   = 0;
	self->_14C   = 0.0f;
	self->_150   = 0;
	self->_154   = 0;
	self->_158   = 0;
	const Vec v1 = { 0.0f, 0.0f, 0.0f };
	self->_170.x = v1.x;
	self->_170.y = v1.y;
	self->_170.z = v1.z;
	self->_16C   = 1.0f;
	self->_188   = 0;
	self->_18C   = 0.0f;
	const Vec v2 = { 0.0f, 0.0f, 0.0f };
	self->_190.x = v2.x;
	self->_190.y = v2.y;
	self->_190.z = v2.z;
	self->_19C   = NULL;
	self->_1A0   = 0;
	self->_1A4   = 0.0f;
	const Vec v3 = { 0.0f, 0.0f, 0.0f };
	self->_1A8.x = v3.x;
	self->_1A8.y = v3.y;
	self->_1A8.z = v3.z;
	self->_1B4   = 0;
	self->_1B8   = 0.0f;
	self->_1BC   = 0.0f;
	self->_1C0   = 0;
	self->_1C4   = 0;
	self->_1C8   = 0.0f;
	self->_1CC   = 0.0f;
	self->_184   = 0;
	self->_1D0   = 0;
	self->_CC    = 0.0f;
	self->_D0    = 0.0f;
	self->_D4    = 0;
	self->_D8    = 0;
	self->_DC    = 1.4f;
	f32 d        = self->_CC - self->_D0;
	for (int i = 0; i < 4; i++) {
		self->_E0[i]  = 0.0f;
		self->_F0[i]  = 0;
		self->_100[i] = 0;
		self->_104[i] = d;
		self->_114[i] = 0.0f;
	}
	self->_124 = 0;
	self->_134 = 0;
	self->_138 = 0.0f;
	self->_13C = 0.0f;
	sScale     = 1.0f;
	self->_164 = 5;
}

/**
 * @note Address: 0x8006D824
 * @note Size: 0x2C
 */
extern "C" BOOL fn_8006D824(InfoScrn* self)
{
	sPoint0._74 = FALSE;
	sPoint1._74 = FALSE;
	sPoint2._74 = FALSE;
	return TRUE;
}

/**
 * @note Address: 0x8006D5A4
 * @note Size: 0x280
 */
extern "C" void fn_8006D5A4(InfoScrn* self)
{
	self->_A8  = 0.0f;
	self->_AC  = 0.0f;
	self->_B0  = 0.0f;
	self->_140 = 0.0f;
	self->_144 = 0.0f;
	self->_98  = 0;
	self->_9C  = 0;
	self->_A0  = 0;
	self->_A4  = 0;
	self->_B4  = 0;
	self->_1D4 = 0.0f;
	self->_B8  = 0;
	self->_17C = 0;
	self->_180 = 0;
	fn_800650AC(&sPoint0, 0x9A);
	fn_800650AC(&sPoint1, 0x98);
	fn_800650AC(&sPoint2, 0x99);
	sPoint0._74  = FALSE;
	sPoint1._74  = FALSE;
	sPoint2._74  = FALSE;
	self->_BC    = 0;
	self->_C0    = 0.0f;
	self->_C4    = 0.0f;
	self->_C8    = 0.0f;
	const Vec v0 = { 0.0f, 0.0f, 0.0f };
	self->_128.x = v0.x;
	self->_128.y = v0.y;
	self->_128.z = v0.z;
	self->_148   = 0;
	self->_14C   = 0.0f;
	self->_150   = 0;
	self->_154   = 0;
	self->_158   = 0;
	const Vec v1 = { 0.0f, 0.0f, 0.0f };
	self->_170.x = v1.x;
	self->_170.y = v1.y;
	self->_170.z = v1.z;
	self->_16C   = 1.0f;
	self->_188   = 0;
	self->_18C   = 0.0f;
	const Vec v2 = { 0.0f, 0.0f, 0.0f };
	self->_190.x = v2.x;
	self->_190.y = v2.y;
	self->_190.z = v2.z;
	self->_19C   = NULL;
	self->_1A0   = 0;
	self->_1A4   = 0.0f;
	const Vec v3 = { 0.0f, 0.0f, 0.0f };
	self->_1A8.x = v3.x;
	self->_1A8.y = v3.y;
	self->_1A8.z = v3.z;
	self->_1B4   = 0;
	self->_1B8   = 0.0f;
	self->_1BC   = 0.0f;
	self->_1C0   = 0;
	self->_1C4   = 0;
	self->_1C8   = 0.0f;
	self->_1CC   = 0.0f;
	self->_184   = 0;
	self->_1D0   = 0;
	self->_CC    = 0.0f;
	self->_D0    = 0.0f;
	self->_D4    = 0;
	self->_D8    = 0;
	self->_DC    = 1.4f;
	f32 d        = self->_CC - self->_D0;
	for (int i = 0; i < 4; i++) {
		self->_E0[i]  = 0.0f;
		self->_F0[i]  = 0;
		self->_100[i] = 0;
		self->_104[i] = d;
		self->_114[i] = 0.0f;
	}
	self->_124 = 0;
	self->_134 = 0;
	self->_138 = 0.0f;
	self->_13C = 0.0f;
	sScale     = 1.0f;
	self->_164 = 5;
}

/**
 * @note Address: 0x8006D548
 * @note Size: 0x5C
 */
extern "C" void fn_8006D548(InfoScrn* self, u32 frames)
{
	fn_80023B38(&self->mFade, frames);
	self->mWipe.Update(frames);
	fn_8006AF34(self, frames);
}

/**
 * @note Address: 0x8006D498
 * @note Size: 0xB0
 */
extern "C" void fn_8006D498(InfoScrn* self)
{
	fn_80199AB0(lbl_8025C400, 0, 0);
	fn_8019B56C(GS_RENDER, 1, 4, 5, 15);
	fn_8019B464(GS_RENDER, 0, 3, 0);
	fn_8002390C(&self->mFade, lbl_8025C400);
	self->mWipe.Draw(lbl_8025C400, FALSE);
	fn_8006AB00(self);
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
extern "C" void fn_8006CB68(InfoScrn* self)
{
	if (GAME_FLAGS & 0x40) {
		fn_8019B464(GS_RENDER, 1, 3, 0);
		if (GAME_LOAD_MAX > 0) {
			f32 fill;
			f32 max;
			f32 y;
			f32 left;
			f32 half;
			f32 tw;
			f32 th;
			f32 scale = 1.0f / GAME_LOAD_MAX;
			max       = GAME_LOAD_MAX;
			fill      = max * (GAME_LOAD_CUR * scale);
			CTexObj tex;
			if (sLoading.mAnim == sWalkAnim) {
				if (sWalkAnim[sLoading.mFrame] == 0xFF) {
					sLoading.mFrame = 0;
				}
				tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[0x82 + sWalkAnim[sLoading.mFrame]], 0);
			} else {
				if (sStandAnim[sLoading.mFrame / 2] == 0xFF) {
					sLoading.mFrame = 0;
				}
				tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[0x86 + sStandAnim[sLoading.mFrame / 2]], 0);
			}
			tw = GXGetTexObjWidth(&tex);
			th = GXGetTexObjHeight(&tex);
			fn_8019B360(GS_RENDER, 1);
			fn_8019AFA0(GS_TEV, 2);
			half = max / 2;
			y    = 406.0f - th / 2;
			left = 320.0f - half;
			fn_80197800(lbl_8025C400, &tex, 0x808080C8, 0x808080C8, 0x808080C8, 0x808080C8, sLoading.mX + left, y, 0.0f, 0.9f * tw,
			            0.9f * th, 0.0f, 3.1415927f, 0.0f);
			fn_8019AFA0(GS_TEV, 0);
			fn_80197800(lbl_8025C400, &tex, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, sLoading.mX + left, y, 0.0f, 0.8f * tw,
			            0.8f * th, 0.0f, 3.1415927f, 0.0f);
			fn_8019AFA0(GS_TEV, 4);
			fn_8019B360(GS_RENDER, 2);

			fn_8019AFA0(GS_TEV, 0);
			tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[117], 0);
			PSMTXScale(tex.mMtx[1], 0.25f, 1.0f, 1.0f);
			tex.mMtxDirty = TRUE;
			PSMTXTrans(tex.mMtx[0], 0.25f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(lbl_8025C400, &tex, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 322.0f, 408.0f, 0.0f, max, 14.0f, 0.0f, 0.0f,
			            3.1415927f);
			PSMTXTrans(tex.mMtx[0], 0.5f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(lbl_8025C400, &tex, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 322.0f - half - 4.0f, 408.0f, 0.0f, 8.0f, 14.0f,
			            0.0f, 0.0f, 3.1415927f);
			PSMTXTrans(tex.mMtx[0], 0.0f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(lbl_8025C400, &tex, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 4.0f + (322.0f + half), 408.0f, 0.0f, 8.0f,
			            14.0f, 0.0f, 0.0f, 3.1415927f);

			tex      = GAME_TEX->Init(&GAME_TPL->descriptorArray[118], 0);
			u16 barW = GXGetTexObjWidth(&tex);
			PSMTXScale(tex.mMtx[1], fill / barW, 1.0f, 1.0f);
			tex.mMtxDirty      = TRUE;
			static f32 sScroll = 0.0f;
			sScroll -= 0.1f;
			PSMTXTrans(tex.mMtx[0], sScroll, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(lbl_8025C400, &tex, 0x9FFA1CC8, 0x9FFA1CC8, 0x9FFA1CC8, 0x9FFA1CC8, 321.0f - (max - fill) / 2, 408.0f, 0.0f, fill,
			            10.0f, 0.0f, 0.0f, 0.0f);
			fn_8019AFA0(GS_TEV, 4);

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
		f32 w            = fn_801968BC(lbl_8025C400, text, 1, 16.0f);
		fn_80196610(lbl_8025C400, -1, text, 320.0f - w / 2, 386.0f, 0.0f, 16.0f, 16.0f);
	}
}

/**
 * @note Address: 0x8006CA90
 * @note Size: 0xD8
 */
extern "C" void fn_8006CA90(InfoScrn* self)
{
	if (GAME_FLAGS & 0x200000000000ULL) {
		fn_8019B464(GS_RENDER, 0, 3, 0);
		const char* text = "\\B00000080 PAUSE ";
		f32 w            = fn_801968BC(lbl_8025C400, text, 1, 24.0f);
		fn_80196610(lbl_8025C400, -1, text, 320.0f - w / 2, 200.0f, 0.0f, 24.0f, 24.0f);
		fn_8019B464(GS_RENDER, 1, 3, 0);
	}
}

/**
 * @note Address: 0x8006AB00
 * @note Size: 0x434
 */
// NONMATCHING: the bss address and the zero swap r6 and r7
extern "C" void fn_8006AB00(InfoScrn* self)
{
	if (IsHudOn() || (GAME_FLAGS & 0x200000000000ULL)) {
		fn_8006389C(&sPoint0);
		fn_8006389C(&sPoint1);
		fn_8006389C(&sPoint2);
		fn_80068D10(self);
		fn_80067928(self);
		f32 alpha = self->_18C * sScale;
		if (alpha > 0.0f) {
			CTexObj tex;
			if (self->_B4 % 32 >= 8) {
				tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[150], 0);
			} else {
				tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[149], 0);
			}
			f32 h;
			f32 w;
			f32 y;
			f32 x;
			u16 tw = GXGetTexObjWidth(&tex);
			w      = 0.8f * (1.1f * tw);
			u16 th = GXGetTexObjHeight(&tex);
			h      = 0.8f * th;
			x      = 6.0f + (w / 2 + self->_190.x);
			y      = self->_190.y - h / 2 - 6.0f;
			fn_8019AFA0(GS_TEV, 0);
			u32 color = (u32)(255.0f * alpha) | 0xC8C8C800;
			fn_80197800(lbl_8025C400, &tex, color, color, color, color, x, y, 0.0f, w, h, 0.0f, 0.0f, 0.0f);
			fn_8019AFA0(GS_TEV, 4);
		}
		fn_80067418(self);
		fn_80066C08(self);
		fn_80066714(self);
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
extern "C" void fn_80068C24(InfoScrn* self, f32* y, f32* x)
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
extern "C" void fn_80068C10(InfoScrn* self)
{
	sPoint0._74 = TRUE;
}

/**
 * @note Address: 0x80068BFC
 * @note Size: 0x14
 */
extern "C" void fn_80068BFC(InfoScrn* self)
{
	sPoint0._74 = FALSE;
}

/**
 * @note Address: 0x80068BE8
 * @note Size: 0x14
 */
extern "C" void fn_80068BE8(InfoScrn* self)
{
	sPoint1._74 = TRUE;
}

/**
 * @note Address: 0x80068BD4
 * @note Size: 0x14
 */
extern "C" void fn_80068BD4(InfoScrn* self)
{
	sPoint1._74 = FALSE;
}

/**
 * @note Address: 0x80068BC0
 * @note Size: 0x14
 */
extern "C" void fn_80068BC0(InfoScrn* self)
{
	sPoint2._74 = TRUE;
}

/**
 * @note Address: 0x80068BAC
 * @note Size: 0x14
 */
extern "C" void fn_80068BAC(InfoScrn* self)
{
	sPoint2._74 = FALSE;
}

/**
 * @note Address: 0x80068B98
 * @note Size: 0x14
 */
extern "C" void fn_80068B98(InfoScrn* self)
{
	sPoint2._78 = 2;
}

/**
 * @note Address: 0x80068B70
 * @note Size: 0x28
 */
extern "C" void fn_80068B70(InfoScrn* self)
{
	sPoint0._75 = TRUE;
	sPoint1._75 = TRUE;
	sPoint2._75 = TRUE;
}

/**
 * @note Address: 0x80068B48
 * @note Size: 0x28
 */
extern "C" void fn_80068B48(InfoScrn* self)
{
	sPoint0._75 = FALSE;
	sPoint1._75 = FALSE;
	sPoint2._75 = FALSE;
}

/**
 * @note Address: 0x80067418
 * @note Size: 0x510
 */
extern "C" void fn_80067418(InfoScrn* self)
{
	f32 alpha = self->_1A4 * sScale;
	if (alpha > 0.0f) {
		CTexObj tex;
		s32 frame = self->_B4 % 8;
		if (frame < 2) {
			tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[149], 0);
		} else if (frame < 4) {
			tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[150], 0);
		} else if (frame < 6) {
			tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[149], 0);
		} else {
			tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[151], 0);
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
		GAME_ROBO->vf10C(&ofs, &sx, &sy, NULL, 0);
		x = 6.0f + (w / 2 + sx);
		y = sy - h / 2 - 6.0f;
		fn_8019AFA0(GS_TEV, 0);
		u32 color = (u32)(255.0f * alpha) | 0xC8C8C800;
		fn_80197800(lbl_8025C400, &tex, color, color, color, color, x, y, 0.0f, w, h, 0.0f, 0.0f, 0.0f);
		fn_8019AFA0(GS_TEV, 4);
	}
}

/**
 * @note Address: 0x80066C08
 * @note Size: 0x810
 */
extern "C" void fn_80066C08(InfoScrn* self)
{
	f32 alpha = self->_1B8 * sScale;
	CTexObj tex;
	if (alpha > 0.0f) {
		u32 a      = 255.0f * alpha;
		u32 top    = a | 0xEFEBC500;
		u32 bottom = a | 0xEDA83600;
		f32 clock  = FramesToClock(self->_1BC);
		f32 step;
		f32 w;
		f32 h;
		f32 smallW;
		f32 smallH;
		f32 value = 10000.0f * (s32)(clock / 6000.0f) + (s32)clock % 6000;
		tex       = GAME_TEX->Init(&GAME_TPL->descriptorArray[125], 0);
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
			fn_8019AFA0(GS_TEV, 0);
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
				fn_80197800(lbl_8025C400, &tex, top, top, bottom, bottom, ofs + (step + (372.0f - i * (w - 3.6f)) - 14.4f), 75.6f, 0.0f,
				            smallW, smallH, 0.0f, 0.0f, 0.0f);
			} else {
				fn_80197800(lbl_8025C400, &tex, top, top, bottom, bottom, ofs + (step + (372.0f - i * (w - 3.6f)) - 14.4f), 72.0f, 0.0f, w,
				            h, 0.0f, 0.0f, 0.0f);
			}
			fn_8019AFA0(GS_TEV, 4);
		}
		fn_8019AFA0(GS_TEV, 4);
		tex        = GAME_TEX->Init(&GAME_TPL->descriptorArray[129], 0);
		u16 colonW = GXGetTexObjWidth(&tex);
		u16 colonH = GXGetTexObjHeight(&tex);
		fn_8019AFA0(GS_TEV, 0);
		fn_80197800(lbl_8025C400, &tex, top, top, bottom, bottom, 472.8f, 72.9f, 0.0f, 0.6f * (1.8f * colonW), 0.6f * (1.8f * colonH), 0.0f,
		            0.0f, 0.0f);
		fn_8019AFA0(GS_TEV, 4);
	}
}
extern "C" void fn_80066714(InfoScrn* self);
static const char* sWide = "\x81\x40\x81\x49\x81\x68\x81\x94\x81\x90\x81\x93\x81\x95\x81\x66"
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
extern "C" {
void fn_80198614(void* gs, u32 c0, u32 c1, u32 c2, u32 c3, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3,
                 f32 y3, f32 z3);
void fn_80198258(void* gs, s32 mode, u32 c0, u32 c1, f32 x, f32 y, f32 z, f32 w, f32 h, f32 a, f32 b, f32 c);
}
extern "C" void fn_800650AC(InfoPoint* self, s32 id);
extern "C" void fn_8006389C(InfoPoint* self);
