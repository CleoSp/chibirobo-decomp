#include <dolphin/types.h>

#pragma section R ".sdata2" \
                  ".sbss2"

const float sAXVolumeMax                                 = 255.0f;
const float sAXInverseVolumeMax                          = 1.0f / 255.0f;
const double sAXSignedIntBias                            = 4503599627370496.0 + 2147483648.0;
const float sAXSquaredVolumeMax                          = 255.0f * 255.0f;
__declspec(section ".sdata2") const float sAXFloatZero   = 0.0f;
const double sAXDoubleHalf                               = 0.5;
const double sAXDoubleThree                              = 3.0;
__declspec(section ".sdata2") const double sAXDoubleZero = 0.0;
const float sAXFloatOne                                  = 1.0f;
const double sAXPitchBase                                = 2.0;
const float sAXCentsPerOctave                            = 1200.0f;
const double sAXUnsignedIntBias                          = 4503599627370496.0;

u32 sAXAuxConfig            = 2;
s32 sAXFrontRightAngle      = 30;
s32 sAXRearRightAngle       = 120;
s32 sAXRearLeftAngle        = 240;
s32 sAXFrontLeftAngle       = 330;
char sAXFalseCondition[]    = "0";
char sAXPositivePidAssert[] = "pid > 0";
s32 sAXStreamSlotCount      = 1;

// Some callers address these messages by offset from the driver's pooled data.
#pragma push
#pragma force_active on
char sAXVoicePidAssert[]         = "v && v->pid == pid";
char sAXVoiceIdOverflowMessage[] = "vidhigh exceeds the max value\n";
char sAXVoiceActiveAssert[]      = "(v->flags&SMSTATE_MASK) == SMSTATE_ACTIVE";
char sAXAuxChannelAssert[]       = "channel == 0 || channel == 1";
char sAXAuxTypeAssert[]          = "0 <= type && type <= 4";
char sAXAuxParamAssert[]         = "type == 0 || param != NULL";
#pragma pop
s32 sStreamVoiceIds[5] = { 0, -1, -1, -1, -1 };
