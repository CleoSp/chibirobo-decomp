#include "types.h"
#include <dolphin/mtx/mtxtypes.h>

#if defined(VERSION_GGTP01)
#define PAD '\0'
#else
#define PAD '0'
#endif

extern "C" {
extern const s32 sToy6FHideJointPath[]                    = { 0, 3, 2, 0, 0, 0 };
extern const s32 sToy6FShowJointPath[]                    = { 0, 3, 2, 0, 0, 0 };
extern const s32 sToy6FSpinJointPath[]                    = { 0, 3, 0, 0 };
extern const s32 sToy117ShowJointPath[]                   = { 0, 0, 3 };
extern const s32 sToy117HideJointPath[]                   = { 0, 0, 3 };
extern const s32 sToy1FBScaleFirstJointPath[]             = { 0, 0, 0, 0, 0, 0, 0 };
extern const s32 sToy1FBScaleSecondJointPath[]            = { 0, 0, 1, 0, 0, 0, 0 };
extern const s32 sToy1FCExtensionCaptureFirstJointPath[]  = { 0, 2, 0, 1 };
extern const s32 sToy1FCExtensionCaptureSecondJointPath[] = { 0, 2, 0, 0, 0 };
extern const s32 sToy1FCExtensionExtendFirstJointPath[]   = { 0, 2, 0, 1 };
extern const s32 sToy1FCExtensionExtendSecondJointPath[]  = { 0, 2, 0, 0, 0 };
extern const s32 sToy1FCExtensionRetractFirstJointPath[]  = { 0, 2, 0, 1 };
extern const s32 sToy1FCExtensionRetractSecondJointPath[] = { 0, 2, 0, 0, 0 };
extern const s32 sToy1FCExtensionRestoreFirstJointPath[]  = { 0, 2, 0, 1 };
extern const s32 sToy1FCExtensionRestoreSecondJointPath[] = { 0, 2, 0, 0, 0 };
extern const s32 sToy1FCHideFirstJointPath[]              = { 0, 0, 0, 1 };
extern const s32 sToy1FCHideSecondJointPath[]             = { 0, 0, 0, 0, 0 };
extern const s32 sToy1FCHideThirdJointPath[]              = { 0, 0, 0, 0, 0, 0 };
extern const s32 sToy1FCHideFourthJointPath[]             = { 0, 0, 0, 0, 0, 0, 0 };
extern const s32 sToy1FCShowFirstJointPath[]              = { 0, 0, 0, 1 };
extern const s32 sToy1FCShowSecondJointPath[]             = { 0, 0, 0, 0, 0 };
extern const s32 sToy1FCShowThirdJointPath[]              = { 0, 0, 0, 0, 0, 0 };
extern const s32 sToy1FCShowFourthJointPath[]             = { 0, 0, 0, 0, 0, 0, 0 };
extern const s32 sToyVerticalOffsetJointPath[]            = { 0, 2, 4, 0 };
extern const s32 sToy0FCarryJointPath[]                   = { 0, 3, 0, 0, 0, 0, 0, 5, 0 };
extern const s32 sToy156CarryJointPath[]                  = { 0, 2, 4, 0 };
extern const s32 sToy0FHitRecoveryJointPath[]             = { 0, 3, 0, 0, 0, 0, 0, 5, 0 };
extern const s32 sToy156HitRecoveryJointPath[]            = { 0, 2, 4, 0 };
extern const Vec sToyHitRecoveryZeroVector                = {};
extern const s32 sToy0FPickUpJointPath[]                  = { 0, 3, 0, 0, 0, 0, 0, 5, 0 };
extern const s32 sToy156PickUpJointPath[]                 = { 0, 2, 4, 0 };
extern const s32 sToyHeldPropJointPath[]                  = { 0, 2, 4, 0 };
extern const s32 sToyTextureAnimJointPath[]               = { 0, 2, 0 };
extern const char sTypeNameXObjActorToy[]                 = "XObjActorToy";
extern const char sActorToyIntpTypeName[]                 = "CXObjIntp";
extern const char sActorToyActorTypeName[]                = "XObjActor";
extern const f32 sActorToyZero                            = 0.0f;
extern const char sActorToyJObjFile[8]                    = { 'j', 'o', 'b', 'j', '.', 'h', '\0', PAD };
extern const char sActorToyJObjCondition[8]               = { 'j', 'o', 'b', 'j', '\0', PAD, PAD, PAD };
extern const char sActorToyScaleCondition[8]              = { 's', 'c', 'a', 'l', 'e', '\0', PAD, PAD };
extern const char sActorToyRotateCondition[8]             = { 'r', 'o', 't', 'a', 't', 'e', '\0', PAD };
extern const char sActorToyAObjFile[8]                    = { 'a', 'o', 'b', 'j', '.', 'h', '\0', PAD };
extern const char sActorToyAObjCondition[8]               = { 'a', 'o', 'b', 'j', '\0', PAD, PAD, PAD };
extern const f32 sActorToyOne                             = 1.0f;
extern const f32 sActorToyAnimBlendRate                   = 10.0f;
extern const f32 sToyHeldPropOffset                       = 2.0f;
extern const char sActorToyMatrixCondition[]              = "mtx";
}
