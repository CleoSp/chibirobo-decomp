#include "game/xact.h"
#include "game/hsd_cobj.h"

// todo: weak copy
/**
 * @note Address: 0x800A2C70
 * @note Size: 0x14
 */
extern "C" bool fn_800A2C70(const XAction* action, s32 state)
{
	return (u8)(action->mState == state);
}

// todo: weak copy
/**
 * @note Address: 0x800A2C84
 * @note Size: 0x14
 */
extern "C" bool fn_800A2C84(const XAction* action, s32 state)
{
	return (u8)(action->mState == state);
}

// todo: weak copy
/**
 * @note Address: 0x800A2C98
 * @note Size: 0x1C
 */
extern "C" void fn_800A2C98(Vec* dst, const Vec* src)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

// todo: weak copy
/**
 * @note Address: 0x800A2CB4
 * @note Size: 0x14
 */
extern "C" bool fn_800A2CB4(const XAction* action, s32 state)
{
	return (u8)(action->mState == state);
}

// todo: weak copy
/**
 * @note Address: 0x800A2CC8
 * @note Size: 0x1C
 */
extern "C" void fn_800A2CC8(Vec* dst, const Vec* src)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

// todo: weak copy
/**
 * @note Address: 0x800A2CE4
 * @note Size: 0xA4
 */
extern "C" f32 fn_800A2CE4(CJObj* object)
{
	HSD_AObj* aobj = GetAObj(object->mJObj);
	return aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
}

// todo: weak copy
/**
 * @note Address: 0x800A2D88
 * @note Size: 0xA4
 */
extern "C" f32 fn_800A2D88(CJObj* object)
{
	HSD_AObj* aobj = GetAObj(object->mJObj);
	return aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
}
