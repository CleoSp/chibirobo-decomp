#include "game/xobjhbox.h"
#include "game/stagedata.h"

/**
 * @note Address: 0x800E0F40
 * @note Size: 0x4C
 */
extern "C" void fn_800E0F40(XObjHBox* box)
{
	box->mPart.mObj.Release();
	fn_800911C0(box);
	box->mLoadState = 0;
}

/**
 * @note Address: 0x800E0F8C
 * @note Size: 0x14C
 */
extern "C" BOOL fn_800E0F8C(XObjHBox* box, CoordInfo* info)
{
	switch (box->mLoadState) {
	case 0:
		if (!fn_80091C80(box, info))
			break;
		box->_80C = 0;
		++box->mLoadState;
		// fall through, the part starts reading right away
	case 1:
		if (!box->mPart.mObj.vf9C(CoordInfo(GetSpawnData(36))))
			break;
		box->mPart.mObj.mStatus.mFlags &= ~0x10000ULL;
		box->mPart.mObj.SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		box->mPart.mObj.SetPos(box->mMoveTranslate, 0);
		++box->mLoadState;
		break;
	}
	if (box->mLoadState > 1) {
		box->mLoadState = 0;
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x800E10D8
 * @note Size: 0x60
 */
extern "C" void fn_800E10D8(XObjHBox* box)
{
	fn_800928E8(box);
	box->mLoadState = 0;
	box->mRing      = 0.0f;
	box->mPart.mObj.vf98();
	box->mPart.mHit   = 0.0f;
	box->mPart.mRange = 5.0f;
}

/**
 * @note Address: 0x800E1138
 * @note Size: 0x30
 */
extern "C" void fn_800E1138(XObjHBox* box)
{
	box->mPart.mObj.vf94();
}

/**
 * @note Address: 0x800E1168
 * @note Size: 0x50
 */
extern "C" void fn_800E1168(XObjHBox* box, u32 id)
{
	fn_800929C4(box, id);
	box->mPart.mObj.vf90(id);
}
