#include "game/xobjactor.h"

struct MamaJointPath {
	s32 indices[4];
};

extern "C" {
extern const float sMamaMinHeadPitch;
extern const float sMamaMaxHeadPitch;
extern const float sMamaMinHeadYaw;
extern const float sMamaMaxHeadYaw;

MamaJointPath sMamaHeadJointPath         = { 0, 4, 0, 0 };
MamaJointPath sMamaHeadJointPositionPath = { 0, 4, 0, 0 };
}

/**
 * @note Address: 0x80126510
 * @note Size: 0xAC
 */
HSD_JObj* XObjActorMama::GetHeadJoint()
{
	MamaJointPath path = sMamaHeadJointPath;
	return FindJointByPath(mJObj, path.indices, 4);
}

/**
 * @note Address: 0x801265BC
 * @note Size: 0x5C
 */
CVec XObjActorMama::GetHeadWorldPosition()
{
	MamaJointPath path = sMamaHeadJointPositionPath;
	return GetEventJointWorldPosition(this, 4, path.indices);
}

/**
 * @note Address: 0x80126618
 * @note Size: 0x8
 */
f32 XObjActorMama::GetMinHeadPitch()
{
	return sMamaMinHeadPitch;
}

/**
 * @note Address: 0x80126620
 * @note Size: 0x8
 */
f32 XObjActorMama::GetMaxHeadPitch()
{
	return sMamaMaxHeadPitch;
}

/**
 * @note Address: 0x80126628
 * @note Size: 0x8
 */
f32 XObjActorMama::GetMinHeadYaw()
{
	return sMamaMinHeadYaw;
}

/**
 * @note Address: 0x80126630
 * @note Size: 0x8
 */
f32 XObjActorMama::GetMaxHeadYaw()
{
	return sMamaMaxHeadYaw;
}
