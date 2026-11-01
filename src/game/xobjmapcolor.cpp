#include "game/xobjmap.h"

// todo: cleanup
static inline u32 FloorColorChannel(s32 value)
{
	if (value >= 256)
		value = 255;
	return value;
}

// todo: cleanup
static inline u32 FloorColor(s32 red, s32 green, s32 blue)
{
	u32 r = FloorColorChannel(red);
	u32 g = FloorColorChannel(green);
	u32 b = FloorColorChannel(blue);
	return (r << 16) | (g << 8) | b;
}

/**
 * @note Address: 0x80093210
 * @note Size: 0x48C
 */
extern "C" s32 fn_80093210(XObjMap* map, s32 trigon, s32 tri)
{
	static u32 sColors[13] = {
		FloorColor(0, 0, 0),       FloorColor(256, 256, 128), FloorColor(256, 0, 0), FloorColor(0, 0, 0), FloorColor(256, 256, 0),
		FloorColor(256, 256, 256), FloorColor(0, 0, 0),       FloorColor(0, 0, 0),   FloorColor(0, 0, 0), FloorColor(0, 256, 256),
		FloorColor(256, 0, 256),   FloorColor(256, 0, 128),   FloorColor(0, 0, 0),
	};
	u32 color = map->mTrigon[trigon].mTri[tri].mColor;
	for (s32 i = 1; i < 13; ++i) {
		if (color == sColors[i])
			return i;
	}
	return 0;
}
