#include "game/xobj.h"
#include "game/xobjchr.h"

static inline Vec FirstCoordinates(const XObjFloor* record)
{
	Vec value = lbl_8037827C;
	value.x   = record->_04[0] / 100.0f;
	value.y   = record->_04[1] / 100.0f;
	value.z   = record->_04[2] / 100.0f;
	return value;
}

static inline Vec SecondCoordinates(const XObjFloor* record)
{
	Vec value = lbl_80378288;
	value.x   = record->_14[0] / 100.0f;
	value.y   = record->_14[1] / 100.0f;
	value.z   = record->_14[2] / 100.0f;
	return value;
}

/**
 * @note Address: 0x800BD01C
 * @note Size: 0x160
 */
extern "C" f32 fn_800BD01C(const XObj* object)
{
	return object->mMoveScale.x * (FirstCoordinates(object->mFloors).y + SecondCoordinates(object->mFloors).y);
}
