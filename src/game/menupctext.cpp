#include "game/menupc.h"
#include "game/cgame.h"
#include "game/csound.h"

extern "C" void fn_80068C8C(f32* value, f32* target, f32 step);

/**
 * @note Address: 0x8004D584
 * @note Size: 0x2D8
 */
extern "C" void fn_8004D584(MenuPC* menu, u32 frames)
{
	switch (menu->mPhase) {
	case 0:
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x30008, 255);
		++menu->mPhase;
		break;
	case 1:
		break;
	}
	fn_80068C8C(&menu->mProgress, &menu->mTargetProgress, menu->mSpeed * frames);
	if (menu->mProgress >= 1.0f)
		menu->ResetText();
}
