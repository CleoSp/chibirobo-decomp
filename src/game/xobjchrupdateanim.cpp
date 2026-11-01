#include "game/xchr.h"

// XObjChr::UpdateAnim
/**
 * @note Address: 0x800A54FC
 * @note Size: 0x20
 */
extern "C" void fn_800A54FC(XObjChr* chr, u32 frames)
{
	chr->CXObjIntp::UpdateAnim(frames);
}
