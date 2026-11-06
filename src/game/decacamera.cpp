#include "game/xobjactor.h"
#include "game/decacamerapath.h"

/**
 * @note Address: 0x8012F628
 * @note Size: 0x318
 */
#pragma section RX ".text.decacamera"
__declspec(section ".text.decacamera") Vec XObjActorDeca::GetCameraTarget()
{
	if (_C90.mFlags & 4) {
		CJObj jobj;
		DecaJointPath2 path = sDecaCameraJointPath;
		jobj.Attach(FindJointByPath(mJObj, path.indices, 2));
		Vec pos;
		pos = GetJointPosition(jobj.mJObj);
		Vec target;
		target = pos;
		target.y -= 0.5f;
		return target;
	}
	return GetCameraTarget__9XObjActorFv(this);
}
