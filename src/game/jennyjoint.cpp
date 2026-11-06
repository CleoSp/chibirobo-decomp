#include "game/xobjactor.h"

struct JennyJointPath {
	s32 indices[3];
};

extern "C" {
extern const float sJennyRaisedMinHeadPitch;
extern const float sJennyDefaultMinHeadPitch;
extern const float sJennyRaisedMaxHeadPitch;
extern const float sJennyDefaultMaxHeadPitch;

JennyJointPath sJennyHeadJointPath         = { 0, 3, 0 };
JennyJointPath sJennyHeadJointPositionPath = { 0, 3, 0 };
}

/**
 * @note Address: 0x80122F14
 * @note Size: 0xA4
 */
HSD_JObj* XObjActorJenny::GetHeadJoint()
{
	JennyJointPath path = sJennyHeadJointPath;
	return FindJointByPath(mJObj, path.indices, 3);
}

/**
 * @note Address: 0x80122FB8
 * @note Size: 0x54
 */
CVec XObjActorJenny::GetHeadWorldPosition()
{
	JennyJointPath path = sJennyHeadJointPositionPath;
	return GetEventJointWorldPosition(this, 3, path.indices);
}

/**
 * @note Address: 0x8012300C
 * @note Size: 0x28
 */
f32 XObjActorJenny::GetMinHeadPitch()
{
	switch (mAnimId) {
	case 0x412:
	case 0x413:
	case 0x414:
		return sJennyRaisedMinHeadPitch;
	default:
		return sJennyDefaultMinHeadPitch;
	}
}

/**
 * @note Address: 0x80123034
 * @note Size: 0x28
 */
f32 XObjActorJenny::GetMaxHeadPitch()
{
	switch (mAnimId) {
	case 0x412:
	case 0x413:
	case 0x414:
		return sJennyRaisedMaxHeadPitch;
	default:
		return sJennyDefaultMaxHeadPitch;
	}
}
