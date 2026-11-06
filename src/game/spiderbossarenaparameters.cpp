#include <dolphin/mtx/mtxtypes.h>

extern "C" {
extern const Vec sSpiderBossPatrolEffectVectors[4] = { { 1, 1, 1 }, {}, { 1, 1, 1 }, {} };

extern const float sSpiderBossAimTurnRate             = 0.018f;
extern const float sSpiderBossAttackRange             = 220.0f;
extern const float sSpiderBossShakeAmplitude          = 3.0f;
extern const float sSpiderBossShakeDuration           = 0.4f;
extern const float sSpiderBossGrabRange               = 70.0f;
extern const float sSpiderBossGrabEscapeThreshold     = 450.0f;
extern const float sSpiderBossGrabEscapeInputStrength = 8.0f;
extern const float sSpiderBossArenaMin                = -200.0f;
extern const float sSpiderBossArenaExtent             = 400.0f;
extern const float sSpiderBossHeldPlayerHeightOffset  = 6.0f;
extern const float sSpiderBossHeldPlayerRadiusOffset  = 0.5f;

extern const char sSpiderBossMatrixCondition[] = "mtx";

extern const char sTypeNameXObjSpiderBoss[16]      = "XObjSpiderBoss";
extern const char sSpiderBossTypeNameCXObjIntp[12] = "CXObjIntp";
}
