#include "game/cobj.h"
#include "game/xobj.h"

inline void* operator new(size_t, void* storage)
{
	return storage;
}

// a bare CXObj around a joint that's already loaded
/**
 * @note Address: 0x800D3F14
 * @note Size: 0x1D8
 */
extern "C" CXObj* fn_800D3F14(CXObj* object, HSD_JObj* joint)
{
	new (object) CXObj;
	object->mJObj = joint;
	HSD_JObjGetTranslation(object->mJObj, &object->mTranslate);
	HSD_JObjGetRotation(object->mJObj, (Quaternion*)&object->mRotate);
	HSD_JObjGetScale(object->mJObj, &object->mScale);
	fn_800D40EC(object);
	return object;
}
