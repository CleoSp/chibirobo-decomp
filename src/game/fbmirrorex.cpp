#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/progressive.h"
#include "game/cpad.h"

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
	mirror->mCamera.Init(0.1f, 32768.0f, 0.5f, -0.5f, -0.5f, 0.5f, &lbl_8025C400.mGs.mScreen);
	mirror->mCamera.SetViewport(0.0f, 256.0f, 0.0f, 256.0f);
	mirror->mCamera.SetScissor(0, 0x100, 0, 0x100);
	mirror->Reset();
	mirror->mOwner = NULL;
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
}
extern "C" s32* lbl_8065B178;

/**
 * @note Address: 0x8001C2C4
 * @note Size: 0x590
 */
// the progressive scan question: asked when the cable allows it and the console is already
// in progressive mode or B is held, the answer goes to the scan mode and the saved setting
extern "C" BOOL fn_8001C2C4(u32 frames)
{
	CPad pad(0);

	switch (lbl_80210730._00) {
	case 0:
		lbl_80210730._00 = 100;
		break;
	case 100:
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
				lbl_80210730._00 = 0;
				OSSetProgressiveMode(0);
				lbl_8065B178[0] = TRUE;
				lbl_8065B178[1] = TRUE;
			}
		} else {
			lbl_80210730._00 = 0;
			OSSetProgressiveMode(0);
			lbl_8065B178[0] = TRUE;
			lbl_8065B178[1] = TRUE;
		}
		break;
	case 1: {
		if (lbl_80210730._0C < 1.0f && (lbl_80210730._0C += 0.11111111f * frames) > 1.0f) {
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
		if (lbl_80210730._0C > 0.0f && (lbl_80210730._0C -= 0.11111111f * frames) < 0.0f) {
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
				lbl_80210730._08 = 600.0f;
			}
		}
		break;
	case 2:
		if ((lbl_80210730._08 -= frames) <= 0.0f) {
			lbl_80210730._00 = 3;
			lbl_80210730._04 = lbl_80210730._14 ? FALSE : TRUE;
			lbl_80210730._08 = 120.0f;
		}
		break;
	case 3:
		if ((lbl_80210730._08 -= frames) <= 0.0f) {
			lbl_80210730._00 = 4;
			lbl_80210730._08 = 600.0f;
			HSD_VISetBlack(FALSE);
		}
		break;
	case 4:
		if ((OSGetResetCode() >> 31) && lbl_80210730._10) {
			lbl_80210730._00 = 40;
		} else {
			if (lbl_80210730._0C < 1.0f && (lbl_80210730._0C += 0.11111111f * frames) > 1.0f) {
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
		if (lbl_80210730._0C > 0.0f && (lbl_80210730._0C -= 0.11111111f * frames) < 0.0f) {
			lbl_80210730._0C = 0.0f;
		}
		if (lbl_80210730._0C == 0.0f) {
			lbl_80210730._00 = 0;
		}
		break;
	}

	if (lbl_80210730._04 >= 0) {
		if (lbl_80210730._04 != 0) {
			OSSetProgressiveMode(1);
			fn_8019B850(&lbl_8025C400.mGs.mScreen, 8, 0, 16);
		} else {
			OSSetProgressiveMode(0);
		}
		lbl_80210730._04 = -1;
		lbl_8065B178[0]  = TRUE;
		lbl_8065B178[1]  = lbl_80210730._14;
	}
	return !lbl_80210730._00;
}
