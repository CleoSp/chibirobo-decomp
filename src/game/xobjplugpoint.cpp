#include "game/xobjattachment.h"

/**
 * @note Address: 0x800D9068
 * @note Size: 0x34
 */
extern "C" CVec fn_800D9068(const XObjPlugCord* cord, s32 index)
{
	return cord->mPoints[index].mPos;
}
