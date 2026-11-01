#include "game/xchr.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cvec.h"
#include <math.h>
#include "game/gmath.h"
#include "game/evt.h"

struct XObjChrJointPath {
	s32 mSteps[2]; // _00
};

extern "C" {
extern const XObjChrJointPath lbl_80660084;
extern const XObjChrJointPath lbl_8066007C;
}

// XObjChr::SetMoveDir, stops whatever push is running and keeps the heading he walks towards
/**
 * @note Address: 0x800A0694
 * @note Size: 0x1BC
 */
extern "C" void fn_800A0694(XObjChr* chr, const Vec* direction, f32 speed)
{
	chr->_988.Set(0.0f, 0.0f, 0.0f);
	chr->_988.mTarget.Set(0.0f, 0.0f, 0.0f);
	chr->_988.mSpeed.Set(0.0f, 0.0f, 0.0f);
	chr->mMoveRotate.Set(chr->mMoveRotate.mTarget);
	if (chr->mPush.mState != 0) {
		fn_800C5A00(&chr->mPush);
		chr->mPush.mState = 0;
		chr->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		chr->_A2C = 3;
	}
	chr->CXObj::SetMoveDir(direction, speed);
	CVec horizontal = chr->mMoveTranslate.mSpeed;
	horizontal.y    = 0.0f;
	if (horizontal.Length() > 0.0f) {
		const CVec& delta = chr->mMoveTranslate.mTarget - (Vec)chr->mMoveTranslate;
		f32 angle         = (f32)atan2(delta.x, delta.z);
		angle             = WrapAngle(angle);
		chr->_12DF8       = angle;
	}
}

/**
 * @note Address: 0x800A0850
 * @note Size: 0x44
 */
extern "C" CVec fn_800A0850(CJObj* object)
{
	XObjChrJointPath path = lbl_80660084;
	return fn_800A0894(object, 2, path.mSteps);
}

// a joint's local translation
/**
 * @note Address: 0x800A0894
 * @note Size: 0xE4
 */
extern "C" CVec fn_800A0894(CJObj* object, s32 depth, s32* path)
{
	HSD_JObj* jobj = GetJointByPath(object->mJObj, path, depth);
	JOBJ_H_ASSERT(996, jobj);
	return CVec(jobj->translate.x, jobj->translate.y, jobj->translate.z);
}

/**
 * @note Address: 0x800A0978
 * @note Size: 0x44
 */
extern "C" CVec fn_800A0978(CJObj* object)
{
	XObjChrJointPath path = lbl_8066007C;
	return fn_8007E7F4(object, 2, path.mSteps);
}
