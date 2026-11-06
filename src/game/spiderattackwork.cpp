#include "game/xobj.h"

extern "C" {
extern const Vec sSmallSpiderCircleRotation     = { -1.5707964f, 0.0f, 0.0f };
extern const Vec sSmallSpiderInitialHitPosition = { 0.0f, 0.0f, 0.0f };
extern XObjFloor sSmallSpiderLowFloor[];
extern XObjFloor sSmallSpiderHighFloor[];
XObjFloor* sSmallSpiderFloors[2] = { sSmallSpiderLowFloor, sSmallSpiderHighFloor };

u8 gSpiderCoinBounceUpdateContext[4];
s32 lbl_8065B2BC;
u8 lbl_8065B2C0;
s32 gSpiderCoinBounceCount;
u8 gSpiderCoinBounceManager[4];
s32 gSpiderDrainFlashTimers[2];
u8 gSpiderDrainFlashClaimed;
s32 gAttachedSmallSpiderCount;
XObj* lbl_8065B2DC;
u8 lbl_8065B2E0[8];
u32 gSpiderBossCoinBounceContext;
f32 gSpiderBossSavedTargetYaw;
}
