#include "game/cgs.h"
#include "game/cgame.h"
#include "game/menu.h"

/**
 * @note Address: 0x80061ABC
 * @note Size: 0x2C
 */
// todo: weak copy
extern "C" void fn_80061ABC(CMenu* menu)
{
	menu->CMenu::Reset();
}

/**
 * @note Address: 0x80061AE8
 * @note Size: 0x18C
 */
// todo: weak copy
extern "C" BOOL fn_80061AE8(CMenu* menu)
{
#if defined(VERSION_JP)
	const s32 frame = 136;
#else
	const s32 frame = 138;
#endif
	CTpl* tpl                  = (CTpl*)lbl_8025C400.mTpl;
	TPLPalette* palette        = (TPLPalette*)tpl->mBuffer;
	menu->mWindow.mTex.mTex    = tpl->mTex.Init(&palette->descriptorArray[frame], 0);
	CTexObj* tex               = &menu->mWindow.mTex.mTex;
	menu->mWindow.mTex.mScaleX = menu->mWindow.mWidth / (f32)GXGetTexObjWidth(tex);
	menu->mWindow.mTex.mScaleY = menu->mWindow.mHeight / (f32)GXGetTexObjHeight(tex);
	PSMTXTrans(tex->mMtx[0], menu->mWindow.mTex.mX, menu->mWindow.mTex.mY, 0.0f);
	tex->mMtxDirty = TRUE;
	PSMTXScale(tex->mMtx[1], menu->mWindow.mTex.mScaleX, menu->mWindow.mTex.mScaleY, 1.0f);
	tex->mMtxDirty = TRUE;
	return TRUE;
}

/**
 * @note Address: 0x80061C74
 * @note Size: 0x5C
 */
// todo: weak copy
extern "C" void fn_80061C74(CMenu* menu)
{
	menu->CMenu::Init();
}
