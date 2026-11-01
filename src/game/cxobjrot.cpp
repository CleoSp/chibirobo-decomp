#include "game/cobj.h"
#include "game/cjobjset.h"

/**
 * @note Address: 0x800266F8
 * @note Size: 0x240
 */
// CXObj's SetRot(x, y, z)
extern "C" void fn_800266F8(CXObj* object, f32 x, f32 y, f32 z, s32 force)
{
	object->mMoveRotate.x         = x;
	object->mMoveRotate.y         = y;
	object->mMoveRotate.z         = z;
	object->mMoveRotate.mTarget.x = x;
	object->mMoveRotate.mTarget.y = y;
	object->mMoveRotate.mTarget.z = z;
	if (object->mJObj != NULL) {
		object->CJObj::SetRotation(x, y, z, force);
	}
}

/**
 * @note Address: 0x80026938
 * @note Size: 0x22C
 */
// CXObj's SetRot(rot)
extern "C" void fn_80026938(CXObj* object, const Vec& rotation, s32 force)
{
	object->mMoveRotate.x         = rotation.x;
	object->mMoveRotate.y         = rotation.y;
	object->mMoveRotate.z         = rotation.z;
	object->mMoveRotate.mTarget.x = rotation.x;
	object->mMoveRotate.mTarget.y = rotation.y;
	object->mMoveRotate.mTarget.z = rotation.z;
	if (object->mJObj != NULL) {
		object->CJObj::SetRotation(rotation, force);
	}
}
