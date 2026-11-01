#include "game/menu.h"
#include "game/cgame.h"
#include "game/xchr.h"
#include "game/globals.h"

extern "C" {
void GXSetScissor(u32 left, u32 top, u32 width, u32 height);
void fn_80054314(f32 alpha);
void fn_80056608(MenuStatus* menu);
void fn_8005D148(s32 item, f32 x, f32 y, f32 alpha, f32 scale, f32 value);
void fn_8005CD3C(s32 item, f32 x, f32 y, f32 alpha, f32 value);
void fn_8004EE0C(MenuStatus* menu);
void fn_800559C4(MenuStatus* menu);
void fn_800554A8(MenuStatus* menu);
void fn_8004F930(MenuStatus* menu);
void fn_80053714(MenuStatus* menu);
void fn_80053D58(MenuStatus* menu);
}

static inline void SetScissor(u32 top, u32 height)
{
#if defined(VERSION_PAL)
	f32 scale     = ((CGs*)lbl_8025C400.mGs)->mScreen._40;
	u32 scaledH   = (u32)((f32)height * scale);
	u32 scaledTop = (u32)((f32)top * scale);
	GXSetScissor(0, scaledTop, 640, scaledH);
#else
	GXSetScissor(0, top, 640, height);
#endif
}

/**
 * @note Address: 0x8005773C
 * @note Size: 0x1F8
 */
extern "C" BOOL fn_8005773C(MenuStatus* menu)
{
	if (menu->CMenu::Draw()) {
		fn_80054314(menu->mAlpha);
		fn_80056608(menu);
		if (menu->mMode == 2 || menu->mMode == 4) {
			SetScissor(104, 248);
			f32 x    = (menu->mX - 42.0f) + menu->mOffsetX;
			f32 y    = (180.0f + menu->mY) + menu->mOffsetY;
			s32 item = menu->mPhase == 100 ? -1 : menu->mItem;
			fn_8005D148(item, x, y, menu->mAlpha, 1.0f, menu->_694);
			fn_8005CD3C(lbl_80365E08.mItems[menu->mItem].mItem, 320.0f, 224.0f, menu->mAlpha, menu->_66C);
			for (s32 i = 255; i >= 0; --i) {
				s16 id = lbl_8025C400.mPlayer->mParams.mInventory.mItems[i].mId;
				fn_8006E44C(id, 4);
			}
			fn_8004EE0C(menu);
			SetScissor(0, 448);
		}
		fn_800559C4(menu);
		fn_800554A8(menu);
		fn_8004F930(menu);
		fn_80053714(menu);
		fn_80053D58(menu);
		return TRUE;
	}
	return FALSE;
}
