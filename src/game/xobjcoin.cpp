#include "game/xobjflags.h"
#include "game/hsd_cobj.h"
#include <math.h>
#include <stdlib.h>
#include "game/gmath.h"
#include "game/stagedata.h"

#pragma section RX ".text.coinplacement"

/**
 * @note Address: 0x800E1E60
 * @note Size: 0xD0
 */
__declspec(section ".text.coinplacement") BOOL XObjCoin::LoadFromPlacement(const CoordInfo& info)
{
	_260 = info._00->_2C;
	if (IsFlagSet())
		return TRUE;
	if (XObj::LoadFromPlacement(info)) {
		mStatus.mFlags |= 0x10000000ULL;
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x800E1F30
 * @note Size: 0x1C8
 */
extern "C" __declspec(section ".text.coinplacement") void ResetObjectState__8XObjCoinFv(XObjCoin* coin)
{
	ResetObjectState__4XObjFv(coin);
	s32 sign       = rand() & 1 ? 1 : -1;
	f32 angle      = DEG2RAD * (rand() % 360);
	coin->mSpin    = angle * sign;
	HSD_AObj* aobj = GetAObj(coin->mJObj);
	f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
	if (end > 0.0f) {
		f32 frame              = fmod((f32)rand(), end);
		UnkStageObj* placement = coin->mCoord._00;
		coin->SetAnim(placement->_30, TRUE, NULL, NULL, NULL, frame, 1.0f, 1.0f, 10.0f);
	}
	coin->_98C      = 0.0f;
	coin->mState    = 0;
	coin->mParam0   = 0;
	coin->mBouncing = 0;
}

/**
 * @note Address: 0x800E20F8
 * @note Size: 0x44
 */
extern "C" __declspec(section ".text.coinplacement") s32 GetFlagIndex__8XObjCoinFv(XObjCoin* coin)
{
	return coin->GetPlacementFlagIndex(1);
}

/**
 * @note Address: 0x800E213C
 * @note Size: 0xC0
 */
#pragma push
#pragma optimization_level 2
#pragma opt_propagation off
__declspec(section ".text.coinplacement") BOOL XObjItem::LoadFromPlacement(const CoordInfo& info)
{
	XObjItem* item = this;
	BOOL result    = item->XObj::LoadFromPlacement(info);
	if (result && item->IsFlagSet())
		item->mStatus.mFlags &= ~0x10000ULL;
	return result;
}
#pragma pop

/**
 * @note Address: 0x800E21FC
 * @note Size: 0x44
 */
extern "C" __declspec(section ".text.coinplacement") s32 GetFlagIndex__8XObjItemFv(XObjItem* item)
{
	return item->GetPlacementFlagIndex(0);
}
