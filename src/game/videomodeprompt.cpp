#include "game/progressive.h"

extern "C" {
VideoModePrompt gVideoModePrompt = { 0, -1, 0.0f, 0.0f, 0, 0 };
}

/**
 * @note Address: 0x8001C89C
 * @note Size: 0x28
 */
extern "C" void ResetProgressiveScanPrompt(void)
{
	gVideoModePrompt.step              = 0;
	gVideoModePrompt.pendingMode       = -1;
	gVideoModePrompt.framesRemaining   = 0.0f;
	gVideoModePrompt.restoredSelection = 0;
	gVideoModePrompt.useStandardMode   = 0;
}
