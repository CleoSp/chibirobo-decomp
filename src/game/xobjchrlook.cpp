#include "game/xchr.h"
#include <math.h>
#include "game/video.h"

// 0.3 seconds
#if defined(VERSION_PAL)
#define LOOK_FADE_STEP (1.0f / (0.3f * lbl_80660D4C))
#else
#define LOOK_FADE_STEP (1.0f / 18.0f)
#endif

// steps the marker on what chibi robo looks at
/**
 * @note Address: 0x8009D600
 * @note Size: 0x178
 */
extern "C" void fn_8009D600(XObjChrLook* look, u32 frames)
{
	if (look->mActive) {
		if (look->mFade < 1.0f) {
			f32 fade    = look->mFade + LOOK_FADE_STEP * frames;
			look->mFade = fade;
			if (fade >= 1.0f)
				look->mFade = 1.0f;
			look->mPulse = look->mFade;
		}
	} else if (look->mFade > 0.0f) {
		f32 fade    = look->mFade - LOOK_FADE_STEP * frames;
		look->mFade = fade;
		if (fade <= 0.0f)
			look->mFade = 0.0f;
	}
	if (look->mFade > 0.0f) {
		look->mPhase = fmod(look->mPhase + (f32)(frames * 6), 360.0);
		if (look->mFade >= 1.0f) {
			f32 pulse    = look->mPulse + LOOK_FADE_STEP * frames;
			look->mPulse = pulse;
			if (pulse >= 2.0f)
				look->mPulse = fmod(look->mPulse, 2.0);
		}
	}
}
