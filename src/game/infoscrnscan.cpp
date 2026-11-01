#include "game/infoscrn.h"
#include "game/cgame.h"

extern "C" {
extern f32 lbl_80367154[11];
}

// scrolling scanlines over the whole screen
/**
 * @note Address: 0x80066714
 * @note Size: 0x268
 */
extern "C" void fn_80066714(InfoScrn* scrn)
{
	f32 alpha = scrn->mScanlineAlpha;
	if (!(alpha > 0.0f))
		return;
	fn_80199AB0((CGs*)lbl_8025C400.mGs, 0, 0);
	fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 4, 5, 15);
	fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 7, 1);
	fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 7, 0);
	u32 a             = (u32)(255.0f * alpha);
	static f32 scroll = 0.0f;
	if (lbl_8025C400._8580 == 0) {
		if ((scroll = 1.0f + scroll) >= 4.0f)
			scroll = 0.0f;
	}
	fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 3, 4, 5, 15);
	fn_8019B34C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 12, 5);
	u32 color = a | 0x30303000;
	for (s32 row = 0; row < 448; row = (s32)(row + 4.0f)) {
		f32 y = row + scroll;
		fn_801992BC((CGs*)lbl_8025C400.mGs, NULL, 0.0f, y, 0.0f, color, 640.0f, y, 0.0f, color);
	}
	fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 4, 5, 15);
	fn_8019B34C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 12, 5);
	for (s32 i = 9; i > 0; --i)
		lbl_80367154[i] = lbl_80367154[i - 1];
	lbl_80367154[0] = 0.0f;
	fn_8019B34C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 12, 5);
}
