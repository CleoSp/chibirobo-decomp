const float sArmyCloseRange = 40.0f;
const float sArmyZero       = 0.0f;
#if defined(VERSION_GGTP01)
const char sArmyJointHeader[]    = "jobj.h";
const char sArmyJointCondition[] = "jobj";
#else
const char sArmyJointHeader[8]    = "jobj.h\0"
                                    "0";
const char sArmyJointCondition[8] = "jobj\0"
                                    "000";
#endif
const double sArmySqrtHalf  = 0.5;
const double sArmySqrtThree = 3.0;
const double sArmySqrtZero  = 0.0;
const float sArmyFullTurn   = 6.2831855f;
#if defined(VERSION_GGTP01)
const char sArmyAObjHeader[]    = "aobj.h";
const char sArmyAObjCondition[] = "aobj";
#else
const char sArmyAObjHeader[8]    = "aobj.h\0"
                                   "0";
const char sArmyAObjCondition[8] = "aobj\0"
                                   "000";
#endif
const float sArmyShotOriginHeight     = 6.0f;
const float sArmyShotInitialSpeed     = 3.0f;
const float sArmyShotTallTargetHeight = 28.0f;
const float sArmyShotPlayerHeight     = 10.0f;
const float sArmyShotHitRadius        = 5.0f;
const float sArmyShotReverseVelocity  = -1.0f;
const float sArmyShotBounceSpeed      = 0.5f;
const float sArmyShotGravity          = -0.15f;
const float sArmyShotGroundHeight     = 1.5f;
const float sArmyShotGroundSpeed      = 0.3f;
const float sArmyPercentScale         = 100.0f;
const float sArmyFrame20              = 20.0f;
#if !defined(VERSION_GGTP01)
const char sArmyBiasPadding[4] = "0000";
#endif
const double sArmySignedIntBias        = 4503601774854144.0;
const float sArmyOne                   = 1.0f;
const float sArmyGuardEndFrameLead     = 2.0f;
const float sArmyPi                    = 3.1415927f;
const float sArmyKnockMoveSpeed        = 0.1f;
const float sArmyPatrolMoveSpeed       = 0.4f;
const float sArmyPlayerDetectionRange  = 80.0f;
const double sArmyFacingConeRadians    = 0.3490658402442932;
const float sArmyForwardDetectionRange = 60.0f;
const float sArmyHalfPi                = 1.5707964f;
const float sArmyFrame50               = 50.0f;
const float sArmyMarchSpacing          = 7.5f;
const double sArmyAimAngle20           = 20.0;
const float sArmyAimTurnSpeed          = 0.05f;
#if !defined(VERSION_GGTP01)
const char sArmyAnglePadding[4] = "0000";
#endif
const double sArmyAimAngle45 = 45.0;
const float sArmyFrame15     = 15.0f;
