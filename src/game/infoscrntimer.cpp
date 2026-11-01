#include "game/infoscrn.h"
#include "game/video.h"

#if defined(VERSION_PAL)
#define TIMER_RATE lbl_80660D4C
#else
#define TIMER_RATE 60
#endif
// 99:59.99, the most the timer shows
#define TIMER_MAX ((s32)((u32)TIMER_RATE * 6000U - 1U))

// frames to mmsscc
/**
 * @note Address: 0x8006697C
 * @note Size: 0x108
 */
extern "C" f32 fn_8006697C(f32 frames)
{
	f32 seconds    = frames / (f32)TIMER_RATE;
	s32 hundredths = ((s32)frames % TIMER_RATE) * 100 / TIMER_RATE;
	if (frames >= (f32)TIMER_MAX)
		hundredths = 99;
	return (f32)hundredths + (6000.0f * (s32)(seconds / 60.0f) + ((s32)seconds % 60) * 100);
}

/**
 * @note Address: 0x80066A84
 * @note Size: 0x118
 */
extern "C" s32 fn_80066A84(InfoScrn* scrn)
{
	return (s32)fn_8006697C(scrn->mTimer);
}

/**
 * @note Address: 0x80066B9C
 * @note Size: 0x54
 */
extern "C" void fn_80066B9C(InfoScrn* scrn, s32 hundredths)
{
	scrn->mTimer = (f32)TIMER_RATE * ((f32)hundredths / 100.0f);
	if (scrn->mTimer >= (f32)TIMER_MAX)
		scrn->mTimer = (f32)TIMER_MAX;
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
 * @note Address: 0x80066BFC
 * @note Size: 0xC
 */
extern "C" void fn_80066BFC(InfoScrn* scrn)
{
	scrn->mTimerShown = TRUE;
}
