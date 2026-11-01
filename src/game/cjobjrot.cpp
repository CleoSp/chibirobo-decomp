#include "game/cjobj.h"
#include "game/cjobjset.h"

// todo: weak copy, CJObj setters out of cjobj.h

/**
 * @note Address: 0x80018118
 * @note Size: 0x21C
 */
extern "C" void fn_80018118(CJObj* object, f32 x, f32 y, f32 z, s32 force)
{
	object->CJObj::SetRotation(x, y, z, force);
}

/**
 * @note Address: 0x80018334
 * @note Size: 0x1FC
 */
extern "C" void fn_80018334(CJObj* object, const Vec& rotation, s32 force)
{
	object->CJObj::SetRotation(rotation, force);
}

/**
 * @note Address: 0x80018530
 * @note Size: 0x80
 */
extern "C" void fn_80018530(CJObj* object, f32 rate, u32 mask)
{
	object->CJObj::SetAnimRate(rate, mask);
}
