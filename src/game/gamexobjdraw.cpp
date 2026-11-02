#include "game/xobj.h"

/**
 * @note Address: 0x8000DA50
 * @note Size: 0x2C
 */
// XObj's vfB4, the reflection pass draws it like any other view
extern "C" void vfB4__4XObjFPA4_f(XObj* object, Mtx view)
{
	object->vfB0(view);
}
