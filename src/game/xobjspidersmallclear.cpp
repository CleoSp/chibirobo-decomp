#include "game/xobjspidersmall.h"
#include "game/cgame.h"

extern "C" {
extern u32 lbl_8065B2C8;
extern u32 lbl_8065B2CC[2];
}

/**
 * @note Address: 0x800FFB00
 * @note Size: 0x2E0
 */
extern "C" void fn_800FFB00(XObjSpiderSmall* spider)
{
	spider->_980                 = 0.0f;
	spider->_984                 = 0.0f;
	spider->_988                 = 0.0f;
	spider->_98C                 = 0.1f;
	spider->_99C                 = 0.0f;
	spider->_9A0                 = 0.0f;
	spider->_9A4                 = 0.0f;
	spider->mState               = 0;
	spider->mPrevState           = 0;
	spider->mStep                = 0;
	spider->mTimer               = 0;
	spider->mSpiderStatus.mFlags = 0;
	spider->_9C0                 = 0;
	spider->_9C4                 = 1.0f;
	spider->_9C8                 = 1.0f;
	{
		Vec zero          = { 0.0f, 0.0f, 0.0f };
		spider->mHitPos.x = zero.x;
		spider->mHitPos.y = zero.y;
		spider->mHitPos.z = zero.z;
	}
	{
		Vec zero       = { 0.0f, 0.0f, 0.0f };
		spider->_9D8.x = zero.x;
		spider->_9D8.y = zero.y;
		spider->_9D8.z = zero.z;
	}
	{
		Vec zero       = { 0.0f, 0.0f, 0.0f };
		spider->_9F0.x = zero.x;
		spider->_9F0.y = zero.y;
		spider->_9F0.z = zero.z;
	}
	{
		Vec zero       = { 0.0f, 0.0f, 0.0f };
		spider->_9FC.x = zero.x;
		spider->_9FC.y = zero.y;
		spider->_9FC.z = zero.z;
	}
	spider->mHitStrength = 0.0f;
	spider->_9EC         = 3;
	spider->_9E8         = spider->_9EC;
	spider->mEvtArgs[0]  = 0;
	spider->mEvtArgs[1]  = 0;
	spider->_A48         = 0;
	spider->_994         = 1.0f;
	spider->mStatus.mFlags |= 8ULL;
	spider->mStatus.mFlags |= 0x10000000ULL;
	spider->mFx.mPos.x         = 0.0f;
	spider->mFx.mPos.y         = 0.0f;
	spider->mFx.mPos.z         = 0.0f;
	spider->mFx._10.x          = 0.0f;
	spider->mFx._10.y          = 0.0f;
	spider->mFx._10.z          = 0.0f;
	spider->mFx.mRotY          = 0.0f;
	spider->mFx._24            = 0.0f;
	spider->mFx.mState         = 0;
	spider->mFx._2C            = 0;
	spider->mFx.mStatus.mFlags = 0;
	spider->mRadius            = 16.0f;
	if (spider->_258 == 0) {
		fn_800EA99C(&lbl_8065B2C8);
		lbl_8065B2CC[0] = 0;
		lbl_8065B2CC[1] = 0;
		lbl_8065B2D4    = 0;
		lbl_8065B2D8    = 0;
	}
	spider->_A84     = 0;
	spider->mFxIndex = 255;
	spider->_A8C     = 0.0f;
	spider->_A90     = 0.0f;
	spider->_A94     = 0;
	spider->_A98     = 0.0f;
	{
		Vec zero       = { 0.0f, 0.0f, 0.0f };
		spider->_A9C.x = zero.x;
		spider->_A9C.y = zero.y;
		spider->_A9C.z = zero.z;
	}
	{
		Vec zero           = { 0.0f, 0.0f, 0.0f };
		spider->mAreaPos.x = zero.x;
		spider->mAreaPos.y = zero.y;
		spider->mAreaPos.z = zero.z;
	}
	spider->mAreaWidth = 0.0f;
	spider->mAreaDepth = 0.0f;
	{
		Vec zero             = { 0.0f, 0.0f, 0.0f };
		spider->mTargetPos.x = zero.x;
		spider->mTargetPos.y = zero.y;
		spider->mTargetPos.z = zero.z;
	}
	spider->mTargetRadius = 0.0f;
	spider->vf12C(1, 0, 0, 0, 0.0f);
	spider->_ABC = 0;
	spider->_AC0 = 0;
	spider->_AC4 = 0;
	spider->_AA8 = 0;
}

/**
 * @note Address: 0x800FFDE0
 * @note Size: 0xDC
 */
extern "C" void fn_800FFDE0(XObjSpiderSmall* spider)
{
	spider->mStage = -1;
	fn_800FFB00(spider);
	spider->mStep      = 0;
	spider->mTimer     = 0;
	spider->mPrevState = spider->mState;
	spider->mState     = 0;
	spider->mSpiderStatus.mFlags |= 0x04000000;
	spider->_AAC = -1;
	spider->_AB4 = -1;
	if (spider->_258 == 0) {
		for (s32 i = 0; i < 39; ++i) {
			for (s32 j = 0; j < 16; ++j) {
				lbl_8037E97C[i][j].mFlags = 0;
			}
		}
	}
	spider->mTimeMode = lbl_8025C400.mTimeState.mMode;
}

/**
 * @note Address: 0x800FFEBC
 * @note Size: 0xE4
 */
extern "C" void fn_800FFEBC(XObjSpiderSmall* spider, u32 id)
{
	fn_800929C4(spider, id);
	spider->mStage = -1;
	fn_800FFB00(spider);
	spider->mStep      = 0;
	spider->mTimer     = 0;
	spider->mPrevState = spider->mState;
	spider->mState     = 0;
	spider->mSpiderStatus.mFlags |= 0x04000000;
	spider->_AAC = -1;
	spider->_AB4 = -1;
	if (spider->_258 == 0) {
		for (s32 i = 0; i < 39; ++i) {
			for (s32 j = 0; j < 16; ++j) {
				lbl_8037E97C[i][j].mFlags = 0;
			}
		}
	}
	spider->mTimeMode = lbl_8025C400.mTimeState.mMode;
}
