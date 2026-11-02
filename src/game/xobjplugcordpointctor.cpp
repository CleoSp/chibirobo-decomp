#include "game/xobjattachment.h"

inline void* operator new(size_t, void* storage)
{
	return storage;
}

// todo: weak copy
/**
 * @note Address: 0x800DBC80
 * @note Size: 0x18
 */
extern "C" XObjPlugCordPoint* __ct__17XObjPlugCordPointFv(XObjPlugCordPoint* point)
{
	return new (point) XObjPlugCordPoint;
}
