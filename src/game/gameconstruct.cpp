#define DEFER_CSCRNCOLOR_CONSTRUCTOR
#include "game/hsd_jobj.h"
#include "game/cgame.h"

extern "C" const f32 gGameFloatZero;

inline CScrnColor::CScrnColor()
{
	ResetChannels(gGameFloatZero);
}

// CGame's constructor and the constructors and destructors the compiler writes for the
// objects it owns

/**
 * @note Address: 0x800197A4
 * @note Size: 0x6B8
 */
#pragma section RX ".text.gameconstruct"
__declspec(section ".text.gameconstruct") CGame::CGame()
{
}

/**
 * @note Address: 0x80019E5C
 * @note Size: 0x488
 */
__declspec(section ".text.gameconstruct") CCameraActions::CCameraActions()
{
}
