#include "game/xobjspidersmall.h"
#include "game/cgame.h"
#include "game/xchr.h"

/**
 * @note Address: 0x800F57C0
 * @note Size: 0x174
 */
// is the player inside the spider's area, and close enough in height
extern "C" BOOL fn_800F57C0(XObjSpiderSmall* spider)
{
	CVec pos  = lbl_8025C400.mPlayer->mMoveTranslate;
	Vec point = pos;
	f32 y     = point.y;
	f32 z     = point.z;
	if (y < HSD_JObjGetTranslationY(spider->mJObj) - 20.0f || y > 20.0f + HSD_JObjGetTranslationY(spider->mJObj)) {
		return FALSE;
	}
	f32 left  = spider->mAreaPos.x + spider->mRadius;
	f32 right = spider->mAreaPos.x + spider->mAreaWidth - spider->mRadius;
	f32 near  = spider->mAreaPos.z + spider->mRadius;
	f32 far   = spider->mAreaPos.z + spider->mAreaDepth - spider->mRadius;
	if (left > point.x || right < point.x || near > z || far < z) {
		return FALSE;
	}
	return TRUE;
}
