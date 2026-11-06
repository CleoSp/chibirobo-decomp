#include "game/cobj.h"
#include "game/cjobjinlineattach.h"

/**
 * @note Address: 0x800BB3D0
 * @note Size: 0x338
 */
#pragma section RX ".text.xobjchrjointrot"
extern "C" __declspec(section ".text.xobjchrjointrot") void SetJointYawByPath(CXObj* object, s32 count, s32* path, f32 angle)
{
	CJObj joint(GetJointByPath(object->mJObj, path, count));
	joint.SetRotationY(angle, 1);
}
