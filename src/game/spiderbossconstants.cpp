#include <dolphin/mtx/mtxtypes.h>

extern "C" {
extern const Vec sSpiderBossActionVectorTemplates[] = { { -1.5707964f, 0, 0 }, {}, { 1, 1, 1 }, {} };
extern const s32 sSpiderBossJointPathIndices[27]    = { 0, 2, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 1, 0, 0, 0, 2, 0, 1, 1, 0, 0 };

extern const float sSpiderBossZero             = 0.0f;
extern const char sSpiderBossJObjFile[]        = "jobj.h";
extern const char sSpiderBossJObjCondition[]   = "jobj";
extern const char sSpiderBossScaleCondition[]  = "scale";
extern const char sSpiderBossRotateCondition[] = "rotate";
extern const char sSpiderBossAObjFile[]        = "aobj.h";
extern const char sSpiderBossAObjCondition[]   = "aobj";

extern const float sSpiderBossAnimRate         = 1.0f;
extern const float sSpiderBossAnimBlendRate    = 10.0f;
extern const float sSpiderBossFullTurn         = 6.2831855f;
extern const float sSpiderBossHalfTurn         = 3.1415927f;
extern const float sSpiderBossNegativeHalfTurn = -3.1415927f;
extern const float sSpiderBossTurnRate         = 0.12f;
}
