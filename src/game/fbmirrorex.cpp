#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/progressive.h"
#include "game/cpad.h"
#include "game/video.h"
#include "game/globals.h"
#include "game/ut.h"

extern "C" {
#include <sysdolphin/video.h>
}

/**
 * @note Address: 0x8001BC34
 * @note Size: 0xC8
 */
// sets the mirror's camera up for its 256 square texture
extern "C" void Init__10FbMirrorExFv(FbMirrorEx* mirror)
{
	mirror->mJoint = NULL;
	mirror->mCamera.Init(0.1f, 32768.0f, 0.5f, -0.5f, -0.5f, 0.5f, &gGame.mGs.mScreen);
	mirror->mCamera.SetViewport(0.0f, 256.0f, 0.0f, 256.0f);
	mirror->mCamera.SetScissor(0, 0x100, 0, 0x100);
	mirror->Reset();
	mirror->mOwner = NULL;
}

static inline s32 DrawPromptLines(const char* const* lines, u32 color)
{
	s32 count = 0;
	for (const char* const* line = lines; *line; ++line)
		++count;
	f32 top = 224.0f - (24.0f * count) * 0.5f;
	for (s32 i = 0; i < count; ++i) {
		f32 width = MeasureGameTextWidth(&gGame.mGs, lines[i], TRUE, 24.0f);
		DrawGsProportionalTextFormat(&gGame, color, lines[i], 320.0f - width * 0.5f, 24.0f * i + top, 0.0f, 24.0f, 24.0f);
	}
	return count;
}

template <bool alternate>
static inline void DrawScanModeConfirmation(u32 color)
{
#if defined(VERSION_PAL)
	const s32 first     = alternate ? 0x112 : 0x10f;
	const char* lines[] = { GetLocalizedInterfaceText(first), GetLocalizedInterfaceText(first + 1), NULL };
#elif defined(VERSION_JP)
	const char* lines[]
	    = { GetLocalizedInterfaceText(3), GetLocalizedInterfaceText(alternate ? 5 : 4), GetLocalizedInterfaceText(6), NULL };
#else
	const s32 first = alternate ? 6 : 3;
	const char* lines[]
	    = { GetLocalizedInterfaceText(first), GetLocalizedInterfaceText(first + 1), GetLocalizedInterfaceText(first + 2), NULL };
#endif
	DrawPromptLines(lines, color);
}

/**
 * @note Address: 0x8001BCFC
 * @note Size: 0x5C8
 */
extern "C" void DrawScanModePrompt()
{
	if (!lbl_80210730._00)
		return;
	CGs& gs = gGame.mGs;
	SetGsChannelEnabled(&gs, 0, 0);
	SetGsColorUpdate(gs.mCurRenderMode, TRUE);
	SetGsAlphaUpdate(gs.mCurRenderMode, FALSE);
	SetGsAlphaCompare(gs.mCurRenderMode, 4, 0, 0, 3, 255);
	SetGsBlendMode(gs.mCurRenderMode, 1, 4, 5, 15);
	SetGsDepthMode(gs.mCurRenderMode, 0, 7, 0);
	SetGsTevMode(gs.mCurTevStage, 0);
	s32 alpha = (s32)(255.0f * lbl_80210730._0C);
	u32 color = 0xffffff00 | (u8)alpha;
	switch (lbl_80210730._00) {
	case 1:
	case 10: {
#if defined(VERSION_PAL)
		const char* lines[]  = { GetLocalizedInterfaceText(0x107),
			                     GetLocalizedInterfaceText(0x108),
			                     GetLocalizedInterfaceText(0x109),
			                     "",
			                     GetLocalizedInterfaceText(0x10a),
			                     GetLocalizedInterfaceText(0x10b),
			                     GetLocalizedInterfaceText(0x10c),
			                     GetLocalizedInterfaceText(0x10d),
			                     "",
			                     "",
			                     NULL };
		const s32 choiceText = 0x10e;
#else
		const char* lines[]  = { GetLocalizedInterfaceText(0), GetLocalizedInterfaceText(1), "", "", NULL };
		const s32 choiceText = 2;
#endif
		s32 count           = DrawPromptLines(lines, color);
		const char* format  = GetLocalizedInterfaceText(choiceText);
		const char* yes     = lbl_80210730._14 == 0 ? "\\B404040" : "\\B000000";
		const char* no      = lbl_80210730._14 == 1 ? "\\B404040" : "\\B000000";
		const char* choices = FormatToStaticBuffer(format, yes, (u8)(lbl_80210730._14 == 0 ? alpha : 0), "\\B00000000", no,
		                                           (u8)(lbl_80210730._14 == 1 ? alpha : 0), "\\B00000000");
		f32 width           = MeasureGameTextWidth(&gs, choices, TRUE, 24.0f);
		DrawGsProportionalTextFormat(&gGame, color, choices, 320.0f - width * 0.5f, 24.0f * (count - 1) + (224.0f - (24.0f * count) * 0.5f),
		                             0.0f, 24.0f, 24.0f, yes, no);
		break;
	}
	case 4:
	case 40: {
		if (lbl_80210730._14 == 0)
			DrawScanModeConfirmation<false>(color);
		else
			DrawScanModeConfirmation<true>(color);
		break;
	}
	}
	SetGsTevMode(gs.mCurTevStage, 4);
}

// the pad as the progressive question reads it
struct ResetPad {
	u8 _00[0x4];  // _00
	u32 mButton;  // _04
	u8 _08[0xC];  // _08
	u32 mTrigger; // _14
	u32 mRepeat;  // _18
};

extern "C" {
u32 VIGetDTVStatus(void);
u32 OSGetResetCode(void);
u32 OSGetProgressiveMode(void);
void OSSetProgressiveMode(u32 mode);
u32 OSGetEuRgb60Mode(void);
void OSSetEuRgb60Mode(u32 mode);
}
extern "C" s32* lbl_8065B178;

#if defined(VERSION_PAL)
#define PROGRESSIVE_FADE_STEP (1.0f / gProgressiveFadeFrames)
#define VIDEO_FPS             lbl_80660D4C
#else
#define PROGRESSIVE_FADE_STEP 0.11111111f
#define VIDEO_FPS             60
#endif

/**
 * @note Address: 0x8001C2C4
 * @note Size: 0x590
 */
// the progressive scan question: asked when the cable allows it and the console is already
// in progressive mode or B is held, the answer goes to the scan mode and the saved setting
extern "C" BOOL UpdateProgressiveScanPrompt(u32 frames)
{
	CPad pad(0);

	switch (lbl_80210730._00) {
	case 0:
#if !defined(VERSION_JP)
		lbl_80210730._00 = 100;
		break;
	case 100:
#endif
#if defined(VERSION_PAL)
		if ((OSGetResetCode() >> 31) && lbl_8065B178[0]) {
			lbl_80210730._00 = 10;
			lbl_80210730._10 = lbl_8065B178[0];
			lbl_80210730._14 = lbl_8065B178[1];
			break;
		}
		lbl_80210730._00 = 1;
		lbl_80210730._08 = 10 * VIDEO_FPS;
		lbl_80210730._0C = 0.0f;
		if (OSGetEuRgb60Mode() != 1) {
			lbl_80210730._14 = TRUE;
		}
#else
		if (VIGetDTVStatus() != 0) {
			if ((OSGetResetCode() >> 31) && lbl_8065B178[0]) {
				lbl_80210730._00 = 10;
				lbl_80210730._10 = lbl_8065B178[0];
				lbl_80210730._14 = lbl_8065B178[1];
				break;
			}
			BOOL ask = FALSE;
			if (OSGetProgressiveMode() == 1) {
				ask = TRUE;
			} else if (((ResetPad*)pad.mPad)->mButton & 0x200) {
				ask = TRUE;
			}
			if (ask) {
				lbl_80210730._08 = 600.0f;
				lbl_80210730._00 = 1;
				lbl_80210730._0C = 0.0f;
			} else {
#if !defined(VERSION_JP)
				lbl_80210730._00 = 0;
#endif
				OSSetProgressiveMode(0);
				lbl_8065B178[0] = TRUE;
				lbl_8065B178[1] = TRUE;
			}
		} else {
#if !defined(VERSION_JP)
			lbl_80210730._00 = 0;
#endif
			OSSetProgressiveMode(0);
			lbl_8065B178[0] = TRUE;
			lbl_8065B178[1] = TRUE;
		}
#endif
		break;
	case 1: {
		if (lbl_80210730._0C < 1.0f && (lbl_80210730._0C += PROGRESSIVE_FADE_STEP * frames) > 1.0f) {
			lbl_80210730._0C = 1.0f;
		}
		BOOL done = FALSE;
		if ((lbl_80210730._08 -= frames) <= 0.0f) {
			done = TRUE;
		} else if (((ResetPad*)pad.mPad)->mTrigger & 0x100) {
			done = TRUE;
		} else if (((ResetPad*)pad.mPad)->mRepeat & 1) {
			lbl_80210730._14 = FALSE;
		} else if (((ResetPad*)pad.mPad)->mRepeat & 2) {
			lbl_80210730._14 = TRUE;
		}
		if (done) {
			lbl_80210730._00 = 10;
		}
		break;
	}
	case 10:
		if (lbl_80210730._0C > 0.0f && (lbl_80210730._0C -= PROGRESSIVE_FADE_STEP * frames) < 0.0f) {
			lbl_80210730._0C = 0.0f;
		}
		if (lbl_80210730._0C == 0.0f) {
			if (!lbl_80210730._14) {
				lbl_80210730._00 = 2;
				lbl_80210730._08 = 2.0f;
				HSD_VISetBlack(TRUE);
			} else {
				lbl_80210730._00 = 4;
				lbl_80210730._04 = lbl_80210730._14 ? FALSE : TRUE;
				lbl_80210730._08 = 10 * VIDEO_FPS;
			}
		}
		break;
	case 2:
		if ((lbl_80210730._08 -= frames) <= 0.0f) {
			lbl_80210730._00 = 3;
			lbl_80210730._04 = lbl_80210730._14 ? FALSE : TRUE;
			lbl_80210730._08 = 2 * VIDEO_FPS;
		}
		break;
	case 3:
		if ((lbl_80210730._08 -= frames) <= 0.0f) {
			lbl_80210730._00 = 4;
			lbl_80210730._08 = 10 * VIDEO_FPS;
			HSD_VISetBlack(FALSE);
		}
		break;
	case 4:
		if ((OSGetResetCode() >> 31) && lbl_80210730._10) {
			lbl_80210730._00 = 40;
		} else {
			if (lbl_80210730._0C < 1.0f && (lbl_80210730._0C += PROGRESSIVE_FADE_STEP * frames) > 1.0f) {
				lbl_80210730._0C = 1.0f;
			}
			BOOL done = FALSE;
			if ((lbl_80210730._08 -= frames) <= 0.0f) {
				done = TRUE;
			} else if (((ResetPad*)pad.mPad)->mTrigger & 0x100) {
				done = TRUE;
			}
			if (done) {
				lbl_80210730._00 = 40;
			}
		}
		break;
	case 40:
		if (lbl_80210730._0C > 0.0f && (lbl_80210730._0C -= PROGRESSIVE_FADE_STEP * frames) < 0.0f) {
			lbl_80210730._0C = 0.0f;
		}
		if (lbl_80210730._0C == 0.0f) {
			lbl_80210730._00 = 0;
		}
		break;
	}

	if (lbl_80210730._04 >= 0) {
		if (lbl_80210730._04 != 0) {
#if defined(VERSION_PAL)
			OSSetEuRgb60Mode(1);
			CGsScreen* screen = &gGame.mGs.mScreen;
			ConfigureGsScreenMode(screen, 28, 0, 18);
			screen->_40     = screen->xfbHeight / 444.0f;
			lbl_80660D4C    = 60;
			CCamera* first  = &gGame.mCameras[0];
			CCamera* second = &gGame.mCameras[1];
			first->SetViewport(0.0f, 0.0f, screen);
			second->SetViewport(0.0f, 0.0f, screen);
#else
			OSSetProgressiveMode(1);
			ConfigureGsScreenMode(&gGame.mGs.mScreen, 8, 0, 16);
#endif
		} else {
#if defined(VERSION_PAL)
			OSSetEuRgb60Mode(0);
#else
			OSSetProgressiveMode(0);
#endif
		}
		lbl_80210730._04 = -1;
		lbl_8065B178[0]  = TRUE;
		lbl_8065B178[1]  = lbl_80210730._14;
	}
	return !lbl_80210730._00;
}
