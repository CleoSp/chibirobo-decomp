#include "game/infopoint.h"
#include "game/cgame.h"

// the bottom row's height, the counters stack up from it
static inline f32 BaseHeight()
{
	f32 height = lbl_8025C400._417C;
	height     = height * 0.5f + lbl_8025C400._4174;
	return (height >= 418.0f ? 418.0f : height) - 64.0f;
}

/**
 * @note Address: 0x800650AC
 * @note Size: 0x214
 */
extern "C" void fn_800650AC(InfoPoint* point, s32 index)
{
	point->mIndex    = index;
	point->_18       = 0;
	point->_08       = 0;
	point->_0C       = 0;
	point->_10       = 0;
	point->_14       = 0.0f;
	point->mPos[0].x = -220.0f;
	point->mPos[1].x = -220.0f;
	point->mPos[2].x = -220.0f;
	point->mPos[3].x = -220.0f;
	point->mPos[4].x = -220.0f;
	point->mPos[5].x = -220.0f;
	point->_1C       = 0.0f;
	point->_20       = 0.0f;
	point->_24       = 0.0f;
	point->_28       = 0.0f;
	point->mVisible  = 0;
	point->_75       = 0;
	switch (point->mIndex) {
	case INFOPOINT_ID + 2:
		point->mResource = point->mIndex;
		point->mPos[0].y = BaseHeight() - 40.0f;
		point->mPos[1].y = BaseHeight() - 40.0f;
		break;
	case INFOPOINT_ID:
		point->mResource = point->mIndex;
		point->mPos[0].y = BaseHeight();
		point->mPos[1].y = BaseHeight();
		break;
	case INFOPOINT_ID + 1: {
		point->mResource = point->mIndex;
		f32 first        = BaseHeight();
		point->mPos[0].y = 40.0f + first;
		f32 second       = BaseHeight();
		point->mPos[1].y = 40.0f + second;
		break;
	}
	}
	point->mState = 0;
	point->_7C    = 0.0f;
	point->_80    = 0;
}
