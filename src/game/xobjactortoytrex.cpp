#include "game/xobjs.h"

// a joint's transform, the 0x1FC toy saves a few
struct JointPose {
	HSD_JObj* mJObj;    // _00
	Vec mTranslate;     // _04
	u32 _10;            // _10
	Quaternion mRotate; // _14
	Vec mScale;         // _24
};
STATIC_ASSERT(sizeof(JointPose) == 0x30);

/**
 * @note Address: 0x8011B6C8
 * @note Size: 0x124
 */
extern "C" void fn_8011B6C8(JointPose* pose, HSD_JObj* joint)
{
	pose->mJObj = joint;
	HSD_JObjGetTranslation(pose->mJObj, &pose->mTranslate);
	HSD_JObjGetRotation(pose->mJObj, &pose->mRotate);
	HSD_JObjGetScale(pose->mJObj, &pose->mScale);
}
