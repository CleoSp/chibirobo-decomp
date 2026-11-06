#include "game/xobjactor.h"

struct DecaJointPath {
	s32 indices[3];
};

extern "C" {
DecaJointPath sDecaHeadJointPath         = { 0, 3, 0 };
DecaJointPath sDecaHeadJointPositionPath = { 0, 3, 0 };
}

/**
 * @note Address: 0x8012F530
 * @note Size: 0xA4
 */
HSD_JObj* XObjActorDeca::GetHeadJoint()
{
	DecaJointPath path = sDecaHeadJointPath;
	return FindJointByPath(mJObj, path.indices, 3);
}

/**
 * @note Address: 0x8012F5D4
 * @note Size: 0x54
 */
CVec XObjActorDeca::GetHeadWorldPosition()
{
	DecaJointPath path = sDecaHeadJointPositionPath;
	return GetEventJointWorldPosition(this, 3, path.indices);
}
