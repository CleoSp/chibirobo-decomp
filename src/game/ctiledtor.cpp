#include "game/hsd_jobj.h"
#include "game/cprimitive.h"

extern "C" {
extern void* __vt__5CTile[];
extern void* __vt__7CCircle[];
}

/**
 * @note Address: 0x8001BB5C
 * @note Size: 0x5C
 */
// todo: weak copy
extern "C" CTile* __dt__5CTileFv(CTile* tile, s32 flags)
{
	if (tile != NULL) {
		*(void***)((u8*)tile + 0x164) = __vt__5CTile;
		tile->CPrimitive::~CPrimitive();
		if ((s16)flags > 0) {
			::operator delete(tile);
		}
	}
	return tile;
}

/**
 * @note Address: 0x8001BBB8
 * @note Size: 0x5C
 */
// todo: weak copy
extern "C" CCircle* __dt__7CCircleFv(CCircle* circle, s32 flags)
{
	if (circle != NULL) {
		*(void***)((u8*)circle + 0x164) = __vt__7CCircle;
		circle->CPrimitive::~CPrimitive();
		if ((s16)flags > 0) {
			::operator delete(circle);
		}
	}
	return circle;
}
