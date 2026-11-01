#include "game/cjobjinlineattach.h"
#include "game/cjobjset.h"

/**
 * @note Address: 0x8009F4F8
 * @note Size: 0x2D8
 */
extern "C" void fn_8009F4F8(CJObj* object, s32 depth, s32* path, f32 x, f32 y, f32 z)
{
	CJObj joint(GetJointByPath(object->mJObj, path, depth));
	joint.CJObj::SetTranslate(x, y, z, TRUE);
}
