#include "game/xchr.h"

// how many of an attachment chibi robo has
/**
 * @note Address: 0x800991FC
 * @note Size: 0x28
 */
extern "C" s32 fn_800991FC(XObjChr* chr, s32 type)
{
	return fn_8009C1BC(&chr->mAttachments, type);
}
