#include "game/xobjspiderboss.h"
#include "game/video.h"

/**
 * @note Address: 0x8010660C
 * @note Size: 0x2C8
 */
extern "C" void fn_8010660C(XObj* object)
{
	XObjSpiderBoss* spider       = (XObjSpiderBoss*)object;
	spider->mHeading             = 0.0f;
	spider->mSpeed               = 0.0f;
	spider->mTargetRotY          = 0.0f;
	spider->mRotSpeed            = 0.1f;
	spider->mSpeedY              = 0.0f;
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
		Vec value   = { 0.0f, 0.0f, 0.0f };
		Vec& target = spider->mHitPos;
		target.x    = value.x;
		target.y    = value.y;
		target.z    = value.z;
	}
	{
		Vec value   = { 0.0f, 0.0f, 0.0f };
		Vec& target = spider->mBasePos;
		target.x    = value.x;
		target.y    = value.y;
		target.z    = value.z;
	}
	{
		Vec value   = { 0.0f, 0.0f, 0.0f };
		Vec& target = spider->_9F0;
		target.x    = value.x;
		target.y    = value.y;
		target.z    = value.z;
	}
	spider->mHitStrength = 0.0f;
	spider->_9EC         = 23;
	spider->mNextAction  = spider->_9EC;
	spider->_A2C         = 0;
	spider->_A30         = 0;
	spider->_A44         = 0;
	spider->_994         = 1.0f;
	spider->mStatus.mFlags |= 8ULL;
	spider->mRadius = 28.0f;
	{
		Vec value   = { 0.0f, 0.0f, 0.0f };
		Vec& target = spider->mAreaPos;
		target.x    = value.x;
		target.y    = value.y;
		target.z    = value.z;
	}
	spider->mAreaWidth = 0.0f;
	spider->mAreaDepth = 0.0f;
	{
		Vec value   = { 0.0f, 0.0f, 0.0f };
		Vec& target = spider->_A10;
		target.x    = value.x;
		target.y    = value.y;
		target.z    = value.z;
	}
	spider->_A1C = 0.0f;
	spider->vf12C(1, 0, 0, 0, 0.0f);
	spider->_A48 = 0.0f;
	spider->_A4C = 0.0f;
	spider->_A50 = 0;
	spider->_A54 = 0.0f;
	{
		Vec value   = { 0.0f, 0.0f, 0.0f };
		Vec& target = spider->_A58;
		target.x    = value.x;
		target.y    = value.y;
		target.z    = value.z;
	}
#if defined(VERSION_PAL)
	spider->_A64 = 3 * lbl_80660D4C; // 3 seconds
#else
	spider->_A64 = 180;
#endif
	spider->mAreaPos.x  = -200.0f;
	spider->mAreaPos.y  = 0.0f;
	spider->mAreaPos.z  = -200.0f;
	spider->mAreaWidth  = 400.0f;
	spider->mAreaDepth  = 400.0f;
	spider->_A68        = 0;
	spider->mBlinkTimer = 0;
	for (u32 i = 0; i < 8; ++i) {
		spider->mParts[i]._04 = 0;
		spider->mParts[i]._00 = 0;
	}
	if (spider->_258 == 0) {
		fn_800EA99C(&lbl_8065B2E8);
	}
	spider->_A20       = 0;
	spider->mAltAnimId = 0;
	spider->_A28       = 5;
	spider->mStatus.mFlags |= 4ULL;
	spider->_A95 = 0;
}
