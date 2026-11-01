#include "game/cobj.h"
#include "game/cjobjset.h"

/**
 * @note Address: 0x80025EDC
 * @note Size: 0x1B0
 */
// CXObj's SetRotationX
extern "C" void fn_80025EDC(CXObj* object, f32 x, s32 force)
{
	object->mMoveRotate.x         = x;
	object->mMoveRotate.mTarget.x = x;
	if (object->mJObj != NULL) {
		object->CJObj::SetRotationX(x, force);
	}
}

/**
 * @note Address: 0x8002608C
 * @note Size: 0x1B0
 */
// CXObj's SetRotationY
extern "C" void fn_8002608C(CXObj* object, f32 y, s32 force)
{
	object->mMoveRotate.y         = y;
	object->mMoveRotate.mTarget.y = y;
	if (object->mJObj != NULL) {
		object->CJObj::SetRotationY(y, force);
	}
}

/**
 * @note Address: 0x8002623C
 * @note Size: 0x1B0
 */
// CXObj's SetRotationZ
extern "C" void fn_8002623C(CXObj* object, f32 z, s32 force)
{
	object->mMoveRotate.z         = z;
	object->mMoveRotate.mTarget.z = z;
	if (object->mJObj != NULL) {
		object->CJObj::SetRotationZ(z, force);
	}
}

/**
 * @note Address: 0x800263EC
 * @note Size: 0x190
 */
// CXObj's SetPos(x, y, z)
extern "C" void fn_800263EC(CXObj* object, f32 x, f32 y, f32 z, s32 force)
{
	object->mMoveTranslate.x         = x;
	object->mMoveTranslate.y         = y;
	object->mMoveTranslate.z         = z;
	object->mMoveTranslate.mTarget.x = x;
	object->mMoveTranslate.mTarget.y = y;
	object->mMoveTranslate.mTarget.z = z;
	if (object->mJObj != NULL) {
		object->CJObj::SetTranslate(x, y, z, force);
	}
}

/**
 * @note Address: 0x8002657C
 * @note Size: 0x17C
 */
// CXObj's SetPos(pos)
extern "C" void fn_8002657C(CXObj* object, const Vec& position, s32 force)
{
	object->mMoveTranslate.x         = position.x;
	object->mMoveTranslate.y         = position.y;
	object->mMoveTranslate.z         = position.z;
	object->mMoveTranslate.mTarget.x = position.x;
	object->mMoveTranslate.mTarget.y = position.y;
	object->mMoveTranslate.mTarget.z = position.z;
	if (object->mJObj != NULL) {
		object->CJObj::SetTranslate(position, force);
	}
}
