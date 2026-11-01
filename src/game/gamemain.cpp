extern "C" {
#include <sysdolphin/controller.h>
}
#include "types.h"
#include "Version.h"
#include "game/cpad.h"
#include "game/cgame.h"
#include "game/video.h"

extern "C" {
void VIWaitForRetrace();
u32 VIGetRetraceCount();
void HSD_VISetBlack(s32 black);
void HSD_VICopyXFBAsync(s32 mode);
// CStopwatch's mWatch is the OSStopwatch
void OSResetStopwatch(void* stopwatch);
void OSStartStopwatch(void* stopwatch);
s64 OSCheckStopwatch(void* stopwatch);
void OSStopStopwatch(void* stopwatch);
}

/**
 * @note Address: 0x80015644
 * @note Size: 0x1C8
 */
int main(int argc, char** argv)
{
	// through CBase so the calls stay virtual
	((CBase*)&lbl_8025C400)->Initialize(argc, argv);
	((CBase*)&lbl_8025C400)->Start();
	HSD_VISetBlack(0);
	for (;;) {
		VIWaitForRetrace();
		lbl_8025C400.mRetraceStep = VIGetRetraceCount() - lbl_8025C400.mLastRetrace;
		lbl_8025C400.mLastRetrace = VIGetRetraceCount();
		if (lbl_8025C400.mRetraceStep > 4)
			lbl_8025C400.mRetraceStep = 4;
		OSResetStopwatch(&lbl_8025C400.mStopwatch[0]);
		OSStartStopwatch(&lbl_8025C400.mStopwatch[0]);
		OSResetStopwatch(&lbl_8025C400.mStopwatch[1]);
		OSStartStopwatch(&lbl_8025C400.mStopwatch[1]);
		OSResetStopwatch(&lbl_8025C400.mStopwatch[2]);
		OSStartStopwatch(&lbl_8025C400.mStopwatch[2]);
		HSD_VICopyXFBAsync(0);
		lbl_8025C400.mStopwatch[0].mElapsed = OSCheckStopwatch(&lbl_8025C400.mStopwatch[0]);
		OSStopStopwatch(&lbl_8025C400.mStopwatch[0]);
		((CPadEx*)lbl_8025C400.mPad)->Update(lbl_8025C400.mRetraceStep);
		((CBase*)&lbl_8025C400)->BeforeUpdate();
		((CBase*)&lbl_8025C400)->Update(lbl_8025C400.mRetraceStep);
		lbl_8025C400.mStopwatch[1].mElapsed = OSCheckStopwatch(&lbl_8025C400.mStopwatch[1]);
		OSStopStopwatch(&lbl_8025C400.mStopwatch[1]);
		((CBase*)&lbl_8025C400)->Draw();
		lbl_8025C400.mStopwatch[2].mElapsed = OSCheckStopwatch(&lbl_8025C400.mStopwatch[2]);
		OSStopStopwatch(&lbl_8025C400.mStopwatch[2]);
		s32 interval = ((CBase*)&lbl_8025C400)->FrameInterval() * 60;
#if defined(VERSION_PAL)
		while (VIGetRetraceCount() - lbl_8025C400.mLastRetrace < (u32)(interval / lbl_80660D4C - 1)) { }
#else
		while (VIGetRetraceCount() - lbl_8025C400.mLastRetrace < (u32)(interval / 60 - 1)) { }
#endif
	}
}
