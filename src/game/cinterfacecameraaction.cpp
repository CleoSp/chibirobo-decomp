#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

// todo: weak copy, the inline virtuals from ccameraaction.h
extern "C" {
/**
 * @note Address: 0x80015DD8
 * @note Size: 0x4
 */
void PreviousUpdate__22CInterfaceCameraActionFv(CInterfaceCameraAction*)
{
}

/**
 * @note Address: 0x80015DDC
 * @note Size: 0x4
 */
void SetCamera__22CInterfaceCameraActionFP7CCamera(CInterfaceCameraAction*, CCamera*)
{
}

/**
 * @note Address: 0x80015DE0
 * @note Size: 0x4
 */
void Init__22CInterfaceCameraActionFv(CInterfaceCameraAction*)
{
}

/**
 * @note Address: 0x80015DE4
 * @note Size: 0x4
 */
void vf20__22CInterfaceCameraActionFv(CInterfaceCameraAction*)
{
}

/**
 * @note Address: 0x80015DE8
 * @note Size: 0x4
 */
void vf24__22CInterfaceCameraActionFv(CInterfaceCameraAction*)
{
}

/**
 * @note Address: 0x80015DEC
 * @note Size: 0x4
 */
void vf28__22CInterfaceCameraActionFv(CInterfaceCameraAction*)
{
}

/**
 * @note Address: 0x80015DF0
 * @note Size: 0x38
 */
void SetEye__22CInterfaceCameraActionFRC3Vec(CInterfaceCameraAction* object, const Vec* value)
{
	object->SetEye(value->x, value->y, value->z);
}

/**
 * @note Address: 0x80015E28
 * @note Size: 0x10
 */
void SetEye__22CInterfaceCameraActionFfff(CInterfaceCameraAction* object, f32 x, f32 y, f32 z)
{
	object->mEye.x = x;
	object->mEye.y = y;
	object->mEye.z = z;
}

/**
 * @note Address: 0x80015E38
 * @note Size: 0x38
 */
void SetInterest__22CInterfaceCameraActionFRC3Vec(CInterfaceCameraAction* object, const Vec* value)
{
	object->SetInterest(value->x, value->y, value->z);
}

/**
 * @note Address: 0x80015E70
 * @note Size: 0x10
 */
void SetInterest__22CInterfaceCameraActionFfff(CInterfaceCameraAction* object, f32 x, f32 y, f32 z)
{
	object->mInterest.x = x;
	object->mInterest.y = y;
	object->mInterest.z = z;
}

/**
 * @note Address: 0x80015E80
 * @note Size: 0x38
 */
void SetAngle__22CInterfaceCameraActionFRC3Vec(CInterfaceCameraAction* object, const Vec* value)
{
	object->SetAngle(value->x, value->y, value->z);
}

/**
 * @note Address: 0x80015EB8
 * @note Size: 0x10
 */
void SetAngle__22CInterfaceCameraActionFfff(CInterfaceCameraAction* object, f32 x, f32 y, f32 z)
{
	object->mAngle.x = x;
	object->mAngle.y = y;
	object->mAngle.z = z;
}

/**
 * @note Address: 0x80015EC8
 * @note Size: 0x8
 */
void SetDistance__22CInterfaceCameraActionFf(CInterfaceCameraAction* object, f32 distance)
{
	object->mDistance = distance;
}

/**
 * @note Address: 0x80015ED0
 * @note Size: 0x8
 */
void SetFov__22CInterfaceCameraActionFf(CInterfaceCameraAction* object, f32 fov)
{
	object->mFov = fov;
}

/**
 * @note Address: 0x80015ED8
 * @note Size: 0x8
 */
void SetAngleZ__22CInterfaceCameraActionFf(CInterfaceCameraAction* object, f32 z)
{
	object->mAngle.z = z;
}

/**
 * @note Address: 0x80015EE0
 * @note Size: 0x8
 */
u32 vf50__22CInterfaceCameraActionFv(CInterfaceCameraAction*)
{
	return 0;
}

/**
 * @note Address: 0x80015EE8
 * @note Size: 0x8
 */
u32 Get_38__22CInterfaceCameraActionFv(CInterfaceCameraAction* object)
{
	return object->_38;
}

/**
 * @note Address: 0x80015EF0
 * @note Size: 0x8
 */
f32 vf58__22CInterfaceCameraActionFv(CInterfaceCameraAction*)
{
	return 12.0f;
}

/**
 * @note Address: 0x80015EF8
 * @note Size: 0x8
 */
u32 vf5C__22CInterfaceCameraActionFv(CInterfaceCameraAction*)
{
	return 0;
}

/**
 * @note Address: 0x80015F00
 * @note Size: 0x8
 */
XObj* GetTarget__22CInterfaceCameraActionFv(CInterfaceCameraAction*)
{
	return NULL;
}
}
