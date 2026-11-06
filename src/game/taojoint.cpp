#include "game/xobjactor.h"

struct TaoJointPath {
	s32 indices[4];
};

extern "C" {
TaoJointPath sTaoHeadJointPath;
TaoJointPath sTaoHeadJointPositionPath;
}

/**
 * @note Address: 0x8012C94C
 * @note Size: 0xAC
 */
HSD_JObj* XObjActorTao::GetHeadJoint()
{
	TaoJointPath path = sTaoHeadJointPath;
	return FindJointByPath(mJObj, path.indices, 4);
}

/**
 * @note Address: 0x8012C9F8
 * @note Size: 0x5C
 */
CVec XObjActorTao::GetHeadWorldPosition()
{
	TaoJointPath path = sTaoHeadJointPositionPath;
	return GetEventJointWorldPosition(this, 4, path.indices);
}

/**
 * @note Address: 0x8012CA54
 * @note Size: 0x44
 */
bool XObjActorTao::IsFree()
{
	if ((mStatus.mFlags & 1) == 0) {
		return true;
	}
	return !(_998.mFlags & 0x2000);
}
