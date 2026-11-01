#include "game/xobjattachment.h"

// XObjPlug::Release
/**
 * @note Address: 0x800D93C8
 * @note Size: 0x4C
 */
extern "C" void fn_800D93C8(XObjPlug* object)
{
	object->mWire.Release();
	fn_800911C0(object);
	object->mLoadState = 0;
}
