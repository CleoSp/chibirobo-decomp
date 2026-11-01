#include "game/hsd_jobj.h"
#include "game/menu.h"

extern "C" {
// todo: the menus are the real tu's statics, the compiler reaches them off this one
extern u8 lbl_80361F68[];
}

/**
 * @note Address: 0x8003164C
 * @note Size: 0xF4
 */
extern "C" void fn_8003164C(MenuEx* menus)
{
	menus->mActive = FALSE;
#if defined(VERSION_JP)
	menus->mMenus[0]  = (CMenu*)(lbl_80361F68 + 0x140);
	menus->mMenus[1]  = (CMenu*)(lbl_80361F68 + 0x364);
	menus->mMenus[2]  = (CMenu*)(lbl_80361F68 + 0xC98);
	menus->mMenus[3]  = (CMenu*)(lbl_80361F68 + 0x10E8);
	menus->mMenus[4]  = (CMenu*)(lbl_80361F68 + 0x17D4);
	menus->mMenus[5]  = (CMenu*)(lbl_80361F68 + 0x1634);
	menus->mMenus[6]  = (CMenu*)(lbl_80361F68 + 0x1938);
	menus->mMenus[7]  = (CMenu*)(lbl_80361F68 + 0x1ABC);
	menus->mMenus[8]  = (CMenu*)(lbl_80361F68 + 0x36B4);
	menus->mMenus[9]  = (CMenu*)(lbl_80361F68 + 0x3B10);
	menus->mMenus[10] = (CMenu*)(lbl_80361F68 + 0x3D04);
	menus->mMenus[11] = (CMenu*)(lbl_80361F68 + 0x41A0);
	menus->mMenus[12] = (CMenu*)(lbl_80361F68 + 0x4324);
#else
	for (s32 i = 0; i < (s32)ARRAY_SIZE(menus->mMenus); ++i)
		menus->mMenus[i] = NULL;
#if defined(VERSION_PAL)
	menus->mMenus[0] = (CMenu*)(lbl_80361F68 + 0x44C);
	menus->mMenus[1] = (CMenu*)(lbl_80361F68 + 0x648);
	menus->mMenus[2] = (CMenu*)(lbl_80361F68 + 0xF94);
	menus->mMenus[3] = (CMenu*)(lbl_80361F68 + 0x13FC);
	menus->mMenus[4] = (CMenu*)(lbl_80361F68 + 0x1858);
	menus->mMenus[5] = (CMenu*)(lbl_80361F68 + 0x1DA0);
	menus->mMenus[6] = (CMenu*)(lbl_80361F68 + 0x3998);
	menus->mMenus[7] = (CMenu*)(lbl_80361F68 + 0x3E34);
#else
	menus->mMenus[0] = (CMenu*)(lbl_80361F68 + 0x34C);
	menus->mMenus[1] = (CMenu*)(lbl_80361F68 + 0x548);
	menus->mMenus[2] = (CMenu*)(lbl_80361F68 + 0xE94);
	menus->mMenus[3] = (CMenu*)(lbl_80361F68 + 0x12E4);
	menus->mMenus[4] = (CMenu*)(lbl_80361F68 + 0x1740);
	menus->mMenus[5] = (CMenu*)(lbl_80361F68 + 0x1C88);
	menus->mMenus[6] = (CMenu*)(lbl_80361F68 + 0x3880);
	menus->mMenus[7] = (CMenu*)(lbl_80361F68 + 0x3D1C);
#endif
#endif
	for (s32 i = 0; i < (s32)ARRAY_SIZE(menus->mMenus); ++i) {
#if !defined(VERSION_JP)
		if (menus->mMenus[i] != NULL)
#endif
			menus->mMenus[i]->Init();
	}
	menus->mCursor.vf90(0);
}
