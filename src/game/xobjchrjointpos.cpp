#include "game/cjobjinlineattach.h"
#include "game/cjobjset.h"

/**
 * @note Address: 0x800C7810
 * @note Size: 0x2B4
 */
extern "C" void fn_800C7810(CJObj* object, s32 depth, s32* path, const Vec& position)
{
	CJObj joint(GetJointByPath(object->mJObj, path, depth));
	joint.CJObj::SetTranslate(position.x, position.y, position.z, TRUE);
}
