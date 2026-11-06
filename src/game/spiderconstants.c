const float sSpiderZero = 0.0f;

// US and JP fill the unused bytes after these assertion strings with ASCII zeroes.
#if defined(VERSION_GGTP01)
const char sSpiderJointHeader[8]           = "jobj.h";
const char sSpiderJointCondition[8]        = "jobj";
const char sSpiderJointScaleCondition[8]   = "scale";
const char sSpiderJointRotationArgument[8] = "rotate";
const char sSpiderAnimHeader[8]            = "aobj.h";
const char sSpiderAnimCondition[8]         = "aobj";
#else
const char sSpiderJointHeader[8]           = "jobj.h\0"
                                             "0";
const char sSpiderJointCondition[8]        = "jobj\0"
                                             "000";
const char sSpiderJointScaleCondition[8]   = "scale\0"
                                             "00";
const char sSpiderJointRotationArgument[8] = "rotate\0"
                                             "0";
const char sSpiderAnimHeader[8]            = "aobj.h\0"
                                             "0";
const char sSpiderAnimCondition[8]         = "aobj\0"
                                             "000";
#endif

const float sSpiderCoinHeightOffset   = 0.2f;
const float sSpiderOne                = 1.0f;
const float sSpiderCoinBounceHeight   = 10.0f;
const float sSpiderCoinFallSpeed      = 0.8f;
const float sSpiderDegreesToRadians   = 0.0174532925f;
const double sSpiderUnsignedIntBias   = 4503599627370496.0;
const double sSpiderSignedIntBias     = 4503601774854144.0;
const float sSpiderHalfPi             = 1.5707964f;
const float sSpiderHalf               = 0.5f;
const double sSpiderSqrtHalf          = 0.5;
const double sSpiderSqrtThree         = 3.0;
const double sSpiderSqrtZero          = 0.0;
const float sSpiderNearbyDropDistance = 24.0f;

const float sSpiderShadowRingOffset        = 1.5f;
const float sSpiderTestStepDistance        = 60.0f;
const float sSpiderTestMoveSpeed           = 0.3f;
const float sSpiderPi                      = 3.1415927f;
const float sSpiderDefaultRadius           = 20.0f;
const float sSpiderDefaultAreaWidth        = 80.0f;
const float sSpiderTestDescentSpeed        = -0.3f;
const float sSpiderNegativeHalf            = -0.5f;
const float sSpiderFootstepFrame15         = 15.0f;
const float sSpiderTwo                     = 2.0f;
const float sSpiderEight                   = 8.0f;
const float sSpiderFour                    = 4.0f;
const float sSpiderRetreatSpeed            = -1.6f;
const float sSpiderApproachDistance        = 68.0f;
const float sSpiderRetreatDistance         = 48.0f;
const float sSpiderCombatDetectionDistance = 120.0f;
const float sSpiderFullTurn                = 6.2831855f;
const float sSpiderNegativePi              = -3.1415927f;
const float sSpiderHeadingStep             = 0.018f;
const float sSpiderInitialTurnSpeed        = 0.1f;
const float sSpiderCombatMoveSpeed         = 1.25f;
const float sSpiderShakeAmplitude          = 0.4f;
const float sSpiderCombatTurnAngle         = 0.104719758f;
const float sSpiderShotFrame30             = 30.0f;
#if !defined(VERSION_GGTP01)
const float sSpiderShotFrame31 = 31.0f;
const float sSpiderShotFrame32 = 32.0f;
#endif
const float sSpiderShakeStrength          = 3.0f;
const float sSpiderShotRange              = 72.0f;
const float sSpiderShotProgressStep       = 0.06f;
const float sSpiderAttachmentHeightOffset = 5.0f;
const char sSpiderMatrixCondition[]       = "mtx";
const float sSpiderHundred                = 100.0f;
const float sSpiderPlayerHitStrength      = 7.0f;
const float sSpiderThreadOpacity          = 90.0f;
const float sSpiderThreadTopHeight        = 300.0f;
const float sSpiderShadowDiscOpacity      = 160.0f;
const float sSpiderShadowRingOpacity      = 128.0f;
const float sSpiderShadowRingAngleStep    = 0.5235988f;
const float sSpiderNegativeHalfPi         = -1.5707964f;
const char sSpiderStage02[]               = "stage02";
const float sSpiderStage02AreaX           = -188.0f;
const float sSpiderStage02AreaZ           = 63.0f;
const float sSpiderStage02AreaWidth       = 221.0f;
const float sSpiderStage02AreaDepth       = 216.0f;
const float sSpiderStage02AreaScale       = 0.9f;
const float sSpiderSquareAreaOrigin       = -127.0f;
const float sSpiderSquareAreaExtent       = 254.0f;
const float sSpiderWallAreaX              = -52.0f;
const float sSpiderDefaultAreaX           = -50.0f;
const float sSpiderDefaultAreaZ           = -58.0f;
const float sSpiderDefaultAreaDepth       = 134.0f;

#pragma section R ".sdata2" \
                  ".sbss2"
__declspec(section ".sdata2") const int sSpiderHoldJointIndex;
__declspec(section ".sdata2") const int sSpiderThreadJointIndex;
