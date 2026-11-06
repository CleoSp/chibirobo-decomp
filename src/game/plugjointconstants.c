#include "types.h"

const char sPlugEulerRotationCondition[] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sPlugTranslationCondition[]   = "translate";
const f32 sPlugJointZero                 = 0.0f;
const char sPlugJointHeader[]            = "jobj.h";
const char sPlugJointCondition[]         = "jobj";
const char sPlugScaleCondition[]         = "scale";
const char sPlugRotateCondition[]        = "rotate";

#pragma section R ".sdata2" \
                  ".sbss2"
__declspec(section ".sdata2") const s32 sPlugRootJointIndex;

const f32 sPlugWireAnimBlendRate   = 10.0f;
const f32 sPlugInitialCordSpeed    = 0.5f;
const f32 sPlugCordOne             = 1.0f;
const f32 sPlugCordShadowWidth     = 0.15f;
const f32 sPlugInitialCordWidth    = 0.2f;
const f32 sPlugCordJointTipOffsetZ = 5.0f;
