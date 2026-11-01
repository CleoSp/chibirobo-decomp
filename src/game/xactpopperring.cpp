#include "game/xact.h"
#include "game/cgame.h"
#include "game/cgs.h"

extern "C" void fn_80015300(void* game, const Vec* pos, f32* x, f32* y, f32* z, CCamera* camera);

/**
 * @note Address: 0x800BFE18
 * @note Size: 0x22C
 */
// draws the popper's rings
extern "C" void fn_800BFE18(XActPopper* act)
{
	XActPopperShot* shot = act->mShot;
	for (s32 i = 0; i < 10; ++i) {
		for (s32 j = 0; j < 2; ++j) {
			if (shot->mRing[j].mAmount > 0.0f) {
				f32 y, x;
				fn_80015300(&lbl_8025C400, &shot->mRing[j].mPos, &x, &y, NULL, NULL);
				f32 inner = shot->mRing[j].mRadius * shot->mRing[j].mAmount;
				f32 outer = shot->mRing[j].mRadius * (1.0f - shot->mRing[j].mAmount);
				u32 clear = shot->mRing[j].mColor & 0xFFFFFF00;
				u32 color = (u32)(s32)((f32)(shot->mRing[j].mColor & 0xFF) * shot->mRing[j].mAmount);
				color &= 0xFF;
				color |= shot->mRing[j].mColor & 0xFFFFFF00;
				fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 2, 4, 15);
				{
					f32 glow      = 2.0f * inner;
					u32 glowColor = (u32)(255.0f * shot->mRing[j].mAmount) | 0xFFFFFF00;
					fn_80198258((CGs*)lbl_8025C400.mGs, 16, glowColor, 0xFFFFFF00, x, y, 0.0f, glow, glow, 0.0f, 0.0f, 0.0f);
				}
				{
					f32 glow      = 1.5f * inner;
					u32 glowColor = (u32)(128.0f * shot->mRing[j].mAmount) | 0xFFFFFF00;
					fn_80198258((CGs*)lbl_8025C400.mGs, 16, glowColor, 0xFFFFFF00, x, y, 0.0f, glow, glow, 0.0f, 0.0f, 0.0f);
				}
				fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 4, 5, 15);
				fn_80198258((CGs*)lbl_8025C400.mGs, 16, clear, color, x, y, 0.0f, outer, outer, 0.0f, 0.0f, 0.0f);
			}
		}
		++shot;
	}
}

/**
 * @note Address: 0x800C0044
 * @note Size: 0x4C
 */
extern "C" void fn_800C0044(XActPopper* act)
{
	XActPopperShot* shot = act->mShot;
	for (s32 i = 0; i < 10; ++i) {
		fn_80024CA0(&shot->mPtcl);
		++shot;
	}
}
