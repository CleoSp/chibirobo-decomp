#include "game/cgame.h"

// todo: cleanup
static inline u8 LogoActive(CGame* game)
{
	s32 state = game->_472C;
	return (u8)(state >= 1 && state < 15);
}

// todo: cleanup
static inline u8 TitleActive(CGame* game)
{
	s32 state = game->_4930;
	return (u8)(state >= 1 && state < 23);
}

// todo: cleanup
static inline u8 IntroActive(CGame* game)
{
	u8 active = 0;
	if (LogoActive(game) || TitleActive(game))
		active = 1;
	return active;
}

/**
 * @note Address: 0x8000A4B8
 * @note Size: 0x68
 */
// CGame's FrameInterval, the logo and title run at 60, the game at 30
extern "C" s32 fn_8000A4B8(CGame* game)
{
	return IntroActive(game) ? 1 : 2;
}

/**
 * @note Address: 0x8000A520
 * @note Size: 0xC
 */
// CGame's GetCamera
extern "C" void* fn_8000A520(CGame* game)
{
	return game->mCamera;
}

/**
 * @note Address: 0x8000A52C
 * @note Size: 0xC
 */
extern "C" CLight* fn_8000A52C(CGame* game)
{
	return game->_E7E20;
}

/**
 * @note Address: 0x8000A538
 * @note Size: 0x8
 */
extern "C" u32 fn_8000A538(CBase* base)
{
	return base->mRetraceStep;
}

/**
 * @note Address: 0x8000A540
 * @note Size: 0x8
 */
extern "C" void* fn_8000A540(CBase* base)
{
	return base->mCameras[0];
}

/**
 * @note Address: 0x8000A548
 * @note Size: 0x8
 */
extern "C" void* fn_8000A548(CBase* base)
{
	return base->mCameras[1];
}

/**
 * @note Address: 0x8000A550
 * @note Size: 0x8
 */
extern "C" void* fn_8000A550(CBase* base)
{
	return base->mLights[0];
}

/**
 * @note Address: 0x8000A558
 * @note Size: 0x8
 */
extern "C" void* fn_8000A558(CBase* base)
{
	return base->mLights[4];
}
