#ifndef _GAME_CMDLINE_H
#define _GAME_CMDLINE_H
#include "types.h"

struct CmdLineArgs {
	s32 mStage;
	s32 mSlot;
	u8 mMenuMode : 2;
	u8 mPick1 : 2;
	u8 mPick2 : 2;
	u8 mPick3 : 2;
	u8 _09[3];
	u32 mPlayerFlags;
	s16 mPlayerSetting0;
	s16 mPlayerSetting1;
	s32 mCrosshairX, mCrosshairY;
	s32 mTitleCursor;
};
STATIC_ASSERT(sizeof(CmdLineArgs) == 0x20);
extern "C" CmdLineArgs lbl_8020FBC0;
#endif
