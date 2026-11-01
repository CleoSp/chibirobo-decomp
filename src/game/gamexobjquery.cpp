#include "game/xobj.h"
#include "game/cgame.h"
#include <math.h>

/**
 * @note Address: 0x8000D02C
 * @note Size: 0x30
 */
// XObj's vfAC, draws without a view
extern "C" void fn_8000D02C(XObj* object)
{
	object->vfB0(NULL);
}

/**
 * @note Address: 0x8000D05C
 * @note Size: 0x2C
 */
// CBase's GetCamera
extern "C" CCamera* fn_8000D05C(CBase* base)
{
	return base->vf30();
}

/**
 * @note Address: 0x8000D088
 * @note Size: 0x88
 */
// XObj's vf114, how far _77C is above the object, 0 while it's unset
extern "C" f32 fn_8000D088(XObj* object)
{
	if (__fpclassifyf(object->_77C) == FP_NAN)
		return 0.0f;
	return object->_77C - object->mMoveTranslate.y;
}

/**
 * @note Address: 0x8000D110
 * @note Size: 0x8
 */
// XObj's vf110
extern "C" f32 fn_8000D110(XObj* object)
{
	return object->_6F8;
}
