#include "game/hsd_jobj.h"
#include "game/stagedata.h"
#include "game/menu.h"

// stage.cpp's, stage.h can't come in next to the hsd headers

/**
 * @note Address: 0x80031584
 * @note Size: 0xC8
 */
extern "C" BOOL fn_80031584(MenuEx* menus)
{
	if (menus->mCursor.vf9C(CoordInfo(GetSpawnData(12)))) {
		for (s32 i = 0; i < (s32)ARRAY_SIZE(menus->mMenus); ++i) {
#if !defined(VERSION_JP)
			if (menus->mMenus[i] != NULL)
#endif
			{
				menus->mMenus[i]->LoadTexture();
				menus->mMenus[i]->mCursor = &menus->mCursor;
			}
		}
		return TRUE;
	}
	return FALSE;
}
