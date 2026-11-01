#include "game/hsd_jobj.h"
#include "game/menu.h"

/**
 * @note Address: 0x800313D8
 * @note Size: 0x88
 */
extern "C" void fn_800313D8(MenuEx* menus)
{
	if (menus->mActive != 0) {
		for (s32 i = 0; i < (s32)ARRAY_SIZE(menus->mMenus); ++i) {
#if !defined(VERSION_JP)
			if (menus->mMenus[i] != NULL)
#endif
			{
				if (menus->mMenus[i]->IsOpen()) {
					menus->mMenus[i]->Draw();
				}
			}
		}
	}
}

/**
 * @note Address: 0x80031460
 * @note Size: 0xB0
 */
extern "C" void fn_80031460(MenuEx* menus, u32 frames)
{
	menus->mActive = FALSE;
	for (s32 i = 0; i < (s32)ARRAY_SIZE(menus->mMenus); ++i) {
#if !defined(VERSION_JP)
		if (menus->mMenus[i] != NULL)
#endif
		{
			if (menus->mMenus[i]->IsOpen()) {
				if (menus->mMenus[i]->Update(frames))
					menus->mActive = TRUE;
			}
		}
	}
	menus->mCursor.vfA4(frames);
}

/**
 * @note Address: 0x80031510
 * @note Size: 0x74
 */
extern "C" void fn_80031510(MenuEx* menus)
{
	menus->mCursor.vf98();
	for (s32 i = 0; i < (s32)ARRAY_SIZE(menus->mMenus); ++i) {
#if !defined(VERSION_JP)
		if (menus->mMenus[i] != NULL)
#endif
		{
			menus->mMenus[i]->Reset();
		}
	}
}
