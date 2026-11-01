#include "game/cgs.h"
#include "game/cgame.h"

// todo: use XObjChr's Params from game/xchr.h
struct PlayerParams {
	u8 _00[0x4]; // _00
	u32 _04;     // _04
};

/**
 * @note Address: 0x80048EB0
 * @note Size: 0x8
 */
// todo: weak copy
extern "C" u32 fn_80048EB0(PlayerParams* params)
{
	return params->_04;
}

// pool 2's object index's mParams, pool 2 only holds chibi robo
/**
 * @note Address: 0x80048EB8
 * @note Size: 0x20
 */
// todo: weak copy
extern "C" PlayerParams* fn_80048EB8(CGame* game, u32 index)
{
#if defined(VERSION_JP)
	return (PlayerParams*)((u8*)game->mPlayer + (index * game->_96CC + 0x963C));
#else
	return (PlayerParams*)((u8*)game->mPlayer + (index * game->_96CC + 0x964C));
#endif
}

/**
 * @note Address: 0x80048ED8
 * @note Size: 0x2C
 */
// todo: weak copy
extern "C" void fn_80048ED8(CGs* gs, u32 color, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1)
{
	fn_801992BC(gs, NULL, x0, y0, z0, color, x1, y1, z1, color);
}

#if !defined(VERSION_JP)
// the a button icon
/**
 * @note Address: 0x80048F04
 * @note Size: 0x1E0
 */
extern "C" void fn_80048F04(f32 x, f32 y, f32 alpha)
{
	CTpl* tpl   = (CTpl*)lbl_8025C400.mTpl;
	CTexObj tex = tpl->mTex.Init(&((TPLPalette*)tpl->mBuffer)->descriptorArray[119], 0);
	PSMTXScale(tex.mMtx[1], 1.0f / 17.0f, 1.0f, 1.0f);
	tex.mMtxDirty = TRUE;
	PSMTXTrans(tex.mMtx[0], 2.0f / 17.0f, 0.0f, 0.0f);
	tex.mMtxDirty = TRUE;
	fn_8019AFA0(((CGs*)lbl_8025C400.mGs)->mCurTevStage, 0);
	u32 color = 0xFFFFFF00 | (u32)(255.0f * alpha);
	fn_80197800((CGs*)lbl_8025C400.mGs, &tex, color, color, color, color, x, y, 0.0f, 18.0f, 18.0f, 0.0f, 0.0f, 0.0f);
	fn_8019AFA0(((CGs*)lbl_8025C400.mGs)->mCurTevStage, 4);
}
#endif
