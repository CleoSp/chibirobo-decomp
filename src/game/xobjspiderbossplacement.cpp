#include "game/xobjspiderboss.h"
#include "game/spiderplacement.h"

extern "C" {
u8 gSpiderBossPlacementWork[0xD4];
Vec gSpiderBossPosition, gSpiderBossScale, gSpiderBossRotation;
#if defined(VERSION_JP)
u8 gSpiderBossWorkEnd[0x138 - 0xF8];
#else
u8 gSpiderBossWorkEnd[0x140 - 0xF8];
#endif
}

#pragma push
#pragma inline_max_size(4096)
/**
 * @note Address: 0x80106088
 * @note Size: 0x584
 */
extern "C" void ApplySpiderBossPlacement(XObjSpiderBoss* spider)
{
	spider->vf12C(1, 0, 0, 0, 0.0f);
	ReadSpiderPlacement(spider, spider->mBasePos, spider->mHeading, spider->mRadius, 28.0f, gSpiderBossPosition, gSpiderBossRotation,
	                    gSpiderBossScale);
	spider->mStep      = 0;
	spider->mTimer     = 0;
	spider->mPrevState = spider->mState;
	spider->mState     = 0;
	spider->mSpiderStatus.mFlags |= 0x8000;
	spider->mSpiderStatus.mFlags |= 0x02000000;
}
#pragma pop
