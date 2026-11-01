#include "game/title.h"
#include "game/cgame.h"
#include "game/csound.h"

// Title's mode steps, the PMF table at lbl_802116D8 picks one by mMode

extern "C" void fn_8002BD0C(Title* title);

/**
 * @note Address: 0x8002BB7C
 * @note Size: 0x4
 */
extern "C" void fn_8002BB7C(Title* title)
{
}

/**
 * @note Address: 0x8002BB80
 * @note Size: 0x148
 */
// waits out the delay, then fades the labels and leaves for the game
extern "C" void fn_8002BB80(Title* title, u32 frames)
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
		title->mLabels[0].mAlpha  = 0.0f;
		title->mLabels[1].mAlpha  = 0.0f;
		title->mLabels[2].mAlpha  = 0.0f;
		title->mLabels[3].mAlpha  = 0.0f;
		title->mLabels[4].mAlpha  = 0.0f;
		title->mLabels[5].mAlpha  = 0.0f;
		title->mLabels[6].mAlpha  = 0.0f;
		title->mLabels[7].mAlpha  = 0.0f;
		title->mLabels[8].mAlpha  = 0.0f;
		title->mLabels[9].mAlpha  = 0.0f;
		title->mLabels[10].mAlpha = 0.0f;
		title->mLabels[11].mAlpha = 0.0f;
		title->mLabels[12].mAlpha = 0.0f;
		title->mLabels[13].mAlpha = 0.0f;
		title->mLabels[14].mAlpha = 0.0f;
		title->mLabels[15].mAlpha = 0.0f;
		title->mLabels[16].mAlpha = 0.0f;
		title->mLabels[17].mAlpha = 0.0f;
		title->mLabels[18].mAlpha = 0.0f;
		title->mLabels[19].mAlpha = 0.0f;
		title->_37BC              = -200.0f;
		title->mOffset            = 0.0f;
		title->mAlpha             = 0.0f;
		title->mChoice            = 0;
		title->mState             = 13;
		title->SetMode(25);
		CSound* sound = (CSound*)&lbl_8025C400.mSound;
		fn_8001E904(sound, 0x88, 255);
		fn_8001E660(sound, (s32)0xFFFF0003);
		lbl_8025C400._9108 = title->mFile;
		break;
	}
}

/**
 * @note Address: 0x8002BCC8
 * @note Size: 0x4
 */
extern "C" void fn_8002BCC8(Title* title)
{
}

/**
 * @note Address: 0x8002BCCC
 * @note Size: 0x20
 */
extern "C" void fn_8002BCCC(Title* title)
{
	fn_8002BD0C(title);
}

/**
 * @note Address: 0x8002BCEC
 * @note Size: 0x20
 */
extern "C" void fn_8002BCEC(Title* title)
{
	fn_8002BD0C(title);
}
