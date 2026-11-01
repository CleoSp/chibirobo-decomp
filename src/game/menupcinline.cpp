#include "game/cgs.h"
#include "game/cgame.h"
#include "game/ccamera.h"
#include "game/globals.h"

// out of line copies of small inlines the menus use

/**
 * @note Address: 0x800490E4
 * @note Size: 0x8
 */
// todo: weak copy
extern "C" u8* fn_800490E4(CGame* game)
{
	// todo: FbSubwin member once mSubwin has a type
	return &game->mSubwin[0x42C8 - 0x42AC];
}

/**
 * @note Address: 0x800490EC
 * @note Size: 0x8
 */
// todo: weak copy
extern "C" void* fn_800490EC(void** object)
{
	return *object;
}

/**
 * @note Address: 0x800490F4
 * @note Size: 0x8
 */
// todo: weak copy
extern "C" void* fn_800490F4(void** object)
{
	return *object;
}

/**
 * @note Address: 0x800490FC
 * @note Size: 0x3C
 */
// todo: weak copy
extern "C" void fn_800490FC(CTexObj* tex, f32 x, f32 y)
{
	PSMTXTrans(tex->mMtx[0], x, y, 0.0f);
	tex->mMtxDirty = TRUE;
}

/**
 * @note Address: 0x80049138
 * @note Size: 0x3C
 */
// todo: weak copy
extern "C" void fn_80049138(CTexObj* tex, f32 x, f32 y)
{
	PSMTXScale(tex->mMtx[1], x, y, 1.0f);
	tex->mMtxDirty = TRUE;
}

/**
 * @note Address: 0x80049174
 * @note Size: 0x6C
 */
// todo: weak copy
extern "C" void fn_80049174(CGs* gs, Vec* pos, u32 color)
{
	fn_80198614(gs, pos[0].x, pos[0].y, pos[0].z, color, pos[1].x, pos[1].y, pos[1].z, color, pos[2].x, pos[2].y, pos[2].z, color, pos[3].x,
	            pos[3].y, pos[3].z, color);
}

/**
 * @note Address: 0x800491E0
 * @note Size: 0x24
 */
// todo: weak copy
extern "C" void fn_800491E0(CGs* gs, u8 width, s32 texOffs)
{
	fn_8019B34C(gs->mCurRenderMode, width, texOffs);
}

/**
 * @note Address: 0x80049204
 * @note Size: 0x24
 */
// todo: weak copy
extern "C" void fn_80049204(CGs* gs, s32 type, s32 src, s32 dst, s32 op)
{
	fn_8019B56C(gs->mCurRenderMode, type, src, dst, op);
}

#if !defined(VERSION_JP)
/**
 * @note Address: 0x80049228
 * @note Size: 0x8
 */
// todo: weak copy
extern "C" u8 fn_80049228(CGame* game)
{
	return game->mLanguage;
}
#endif

/**
 * @note Address: 0x80049230
 * @note Size: 0x2C
 */
// todo: weak copy
extern "C" void fn_80049230(CGs* gs, CTexObj* tex, u32 color, f32 x, f32 y, f32 z, f32 width, f32 height, f32 rx, f32 ry, f32 rz)
{
	fn_80197800(gs, tex, color, color, color, color, x, y, z, width, height, rx, ry, rz);
}

/**
 * @note Address: 0x8004925C
 * @note Size: 0x20
 */
// todo: weak copy
extern "C" u16 fn_8004925C(CTexObj* tex)
{
	return GXGetTexObjHeight(tex);
}

/**
 * @note Address: 0x8004927C
 * @note Size: 0x20
 */
// todo: weak copy
extern "C" u16 fn_8004927C(CTexObj* tex)
{
	return GXGetTexObjWidth(tex);
}

/**
 * @note Address: 0x8004929C
 * @note Size: 0x24
 */
// todo: weak copy
extern "C" void fn_8004929C(CGs* gs, s32 mode)
{
	fn_8019AFA0(gs->mCurTevStage, mode);
}

/**
 * @note Address: 0x800492C0
 * @note Size: 0xA0
 */
// todo: weak copy
extern "C" CTexObj& fn_800492C0(CTexObj* tex, const CTexObj* other)
{
	return *tex = *other;
}

// a frame of the shared tpl
/**
 * @note Address: 0x80049360
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" CTexObj& fn_80049360(CGame* game, s32 index)
{
	CTpl* tpl = (CTpl*)game->mTpl;
	return tpl->mTex.Init(&((TPLPalette*)tpl->mBuffer)->descriptorArray[index], 0);
}

/**
 * @note Address: 0x80049398
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_80049398(CGs* gs, Vec* pos, u32 color, u16 count)
{
	fn_80198D08(gs, GX_TRIANGLEFAN, gs->mCamera->mViewMtx, pos, color, count);
}

/**
 * @note Address: 0x800493D0
 * @note Size: 0x24
 */
// todo: weak copy
extern "C" void fn_800493D0(CGs* gs, u8 enable, s32 func, u8 update)
{
	fn_8019B464(gs->mCurRenderMode, enable, func, update);
}

/**
 * @note Address: 0x800493F4
 * @note Size: 0x4
 */
// todo: weak copy
extern "C" void fn_800493F4()
{
}

/**
 * @note Address: 0x800493F8
 * @note Size: 0x20
 */
// todo: weak copy
extern "C" const char* fn_800493F8(s32 index)
{
	return fn_8006DBC8(index);
}
