#include "game/ccameraaction.h"

// CCameraActionAreaExM's _68 to _74, they all go to the shared area state

/**
 * @note Address: 0x80015C54
 * @note Size: 0x10
 */
extern "C" f32 fn_80015C54(CCameraActionAreaExM* action)
{
	return lbl_805A054C._10;
}

/**
 * @note Address: 0x80015C64
 * @note Size: 0x10
 */
extern "C" s32 fn_80015C64(CCameraActionAreaExM* action)
{
	return lbl_805A054C._14;
}

/**
 * @note Address: 0x80015C74
 * @note Size: 0x10
 */
extern "C" void fn_80015C74(CCameraActionAreaExM* action, s32 mode)
{
	lbl_805A054C._14 = mode;
}

/**
 * @note Address: 0x80015C84
 * @note Size: 0x44
 */
extern "C" s32 fn_80015C84(CCameraActionAreaExM* action, const Vec& pos)
{
	return fn_801374F4(&lbl_805A054C, pos);
}
