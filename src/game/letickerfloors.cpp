#include "game/xobj.h"

extern "C" {
static XObjFloor sLetickerLiftFloor0[] = {
	{ 2, 0, { 0, 0, 0 }, 0, { 1200, 5000, 1200 } },
	{ -1 },
};

static XObjFloor sLetickerLiftFloor1[] = {
	{ 2, 0, { 0, 517, 0 }, 0, { 1216, 878, 1216 } },
	{ -1 },
};

static XObjFloor sLetickerLevel10Floors[] = {
	{ 2, 0, { 0, 1000, -4650 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -13800 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -22750 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -28600 }, 0, { 600, 150, 2650 } },
	{ -1 },
};

static XObjFloor sLetickerLevel9Floors[] = {
	{ 2, 0, { 0, 1000, -4650 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -13800 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -22750 }, 0, { 600, 150, 7650 } },
	{ -1 },
};

static XObjFloor sLetickerLevel8Floors[] = {
	{ 2, 0, { 0, 1000, -4650 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -13800 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -21300 }, 0, { 600, 150, 5000 } },
	{ -1 },
};

static XObjFloor sLetickerLevel7Floors[] = {
	{ 2, 0, { 0, 1000, -4650 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -13800 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -19600 }, 0, { 600, 150, 2650 } },
	{ -1 },
};

static XObjFloor sLetickerLevel6Floors[] = {
	{ 2, 0, { 0, 1000, -4650 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -13800 }, 0, { 600, 150, 7650 } },
	{ -1 },
};

static XObjFloor sLetickerLevel5Floors[] = {
	{ 2, 0, { 0, 1000, -4650 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -12350 }, 0, { 600, 150, 5000 } },
	{ -1 },
};

static XObjFloor sLetickerLevel4Floors[] = {
	{ 2, 0, { 0, 1000, -4650 }, 0, { 600, 150, 7650 } },
	{ 2, 0, { 0, 1000, -10650 }, 0, { 600, 150, 2650 } },
	{ -1 },
};

static XObjFloor sLetickerLevel3Floors[] = {
	{ 2, 0, { 0, 1000, -4650 }, 0, { 600, 150, 7650 } },
	{ -1 },
};

static XObjFloor sLetickerLevel2Floors[] = {
	{ 2, 0, { 0, 1000, -3200 }, 0, { 600, 150, 5000 } },
	{ -1 },
};

static XObjFloor sLetickerLevel1Floors[] = {
	{ 2, 0, { 0, 1000, -1500 }, 0, { 600, 150, 2650 } },
	{ -1 },
};

static XObjFloor sLetickerLevel0Floors[] = {
	{ 2, 0, { 0, 2564, 6 }, 0, { 604, 2827, 121 } },
	{ -1 },
};

XObjFloor* sLetickerFloorsByLevel[] = { sLetickerLevel0Floors, sLetickerLevel1Floors, sLetickerLevel2Floors, sLetickerLevel3Floors,
	                                    sLetickerLevel4Floors, sLetickerLevel5Floors, sLetickerLevel6Floors, sLetickerLevel7Floors,
	                                    sLetickerLevel8Floors, sLetickerLevel9Floors, sLetickerLevel10Floors };
XObjFloor* sLetickerLiftFloors[]    = { sLetickerLiftFloor0, sLetickerLiftFloor1 };
}
