#include "game/xobjattachment.h"

// XObjPlug::Release
/**
 * @note Address: 0x800D93C8
 * @note Size: 0x4C
 */
extern "C" void Release__8XObjPlugFv(XObjPlug* object)
{
	object->mWire.Release();
	Release__4XObjFv(object);
	object->mLoadState = 0;
}
