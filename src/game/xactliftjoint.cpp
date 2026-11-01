#include "game/xobjs.h"

extern "C" {
#include <dolphin/mtx/mtx.h>
}

extern "C" Vec lbl_8037C340;

/**
 * @note Address: 0x800D0D3C
 * @note Size: 0x90
 */
extern "C" void fn_800D0D3C(HSD_JObj* jobj)
{
	HSD_JObjClearMtxFlags(jobj);
}

// todo: cleanup
static inline Vec FloorScale(XObj* object)
{
	XObjFloor* floor = object->mFloors;
	// retail copies this before filling it in
	Vec scale = lbl_8037C340;
	scale.x   = floor->_14[0] / 100.0f;
	scale.y   = floor->_14[1] / 100.0f;
	scale.z   = floor->_14[2] / 100.0f;
	return scale;
}

/**
 * @note Address: 0x800D0DCC
 * @note Size: 0xC0
 */
extern "C" f32 fn_800D0DCC(XObj* object)
{
	Vec scale = FloorScale(object);
	return object->mMoveScale.x * scale.x;
}

/**
 * @note Address: 0x800D0E8C
 * @note Size: 0xC0
 */
extern "C" void fn_800D0E8C(HSD_JObj* jobj, MtxPtr mtx)
{
	HSD_JObjCopyMtx(jobj, mtx);
}
