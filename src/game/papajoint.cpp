#include "game/xobjactor.h"

struct PapaJointPath {
	s32 indices[3];
};

extern "C" {
PapaJointPath sPapaHeadJointPath         = { 0, 2, 0 };
PapaJointPath sPapaHeadJointPositionPath = { 0, 2, 0 };
}

/**
 * @note Address: 0x80129410
 * @note Size: 0xA4
 */
HSD_JObj* XObjActorPapa::GetHeadJoint()
{
	PapaJointPath path = sPapaHeadJointPath;
	return FindJointByPath(mJObj, path.indices, 3);
}

/**
 * @note Address: 0x801294B4
 * @note Size: 0x54
 */
CVec XObjActorPapa::GetHeadWorldPosition()
{
	PapaJointPath path = sPapaHeadJointPositionPath;
	return GetEventJointWorldPosition(this, 3, path.indices);
}

/**
 * @note Address: 0x80129508
 * @note Size: 0x44
 */
bool XObjActorPapa::IsFree()
{
	if ((mStatus.mFlags & 1) == 0) {
		return true;
	}
	return !(_998.mFlags & 0x2000);
}

/**
 * @note Address: 0x8012954C
 * @note Size: 0x8
 */
f32 XObjActorPapa::GetMinHeadPitch()
{
	return -0.17453292f;
}

/**
 * @note Address: 0x80129554
 * @note Size: 0x8
 */
f32 XObjActorPapa::GetMaxHeadPitch()
{
	return 0.17453292f;
}
