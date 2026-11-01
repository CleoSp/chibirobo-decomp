#include "game/xobjspiderboss.h"

/**
 * @note Address: 0x80105CDC
 * @note Size: 0x4
 */
extern "C" void fn_80105CDC(XObjSpiderBoss* spider)
{
}

/**
 * @note Address: 0x80105CE0
 * @note Size: 0x4
 */
extern "C" void fn_80105CE0(XObjSpiderBoss* spider)
{
}

/**
 * @note Address: 0x80105CE4
 * @note Size: 0x4
 */
extern "C" void fn_80105CE4(XObjSpiderBoss* spider)
{
}

/**
 * @note Address: 0x80105CE8
 * @note Size: 0x20
 */
extern "C" void fn_80105CE8(XObjSpiderBoss* spider, const Vec* origin, f32 opacity)
{
	fn_8009056C(spider, origin, opacity);
}

/**
 * @note Address: 0x80105D08
 * @note Size: 0x20
 */
extern "C" void fn_80105D08(XObjSpiderBoss* spider)
{
	fn_80090864(spider);
}

/**
 * @note Address: 0x80105D28
 * @note Size: 0x30
 */
extern "C" void fn_80105D28(XObjSpiderBoss* spider)
{
	spider->vfB0(NULL);
}
