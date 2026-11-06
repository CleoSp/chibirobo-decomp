#include <dolphin/types.h>

// Retail code also reaches these strings by offset from the first message.
#pragma push
#pragma force_active on
char sSynthHeapMessageTable[]         = "audio heap overflow. size = %u\n";
char sSynthBankOverflowMessage[]      = "Can't load SFX file; bank(id=%d) buffer overflow.\n";
char sSynthBankCapacityAssert[]       = "hsd_SynthSFXBankHead[bankID + 1] - hsd_SynthSFXBank[bankID] >= hsd_SynthSFXLoadBuf[1]";
char sSynthInvalidBankMessage[]       = "invalid bankID = %d; filename = %s\n";
char sSynthBankIdAssert[]             = "(bankID >= 0 && bankID < hsd_SynthSFXBankNum)";
char sSynthReaddressCountAssert[]     = "sfxGroupDataReaddressCounter > 0";
char sSynthRelocationFailureMessage[] = "Can't relocate SFX group; bank = %d; sfxgroup = %d\n";
char sSynthRelocationBoundsAssert[]   = "hsd_SynthSFXBank[bankID] + group->arsize <= hsd_SynthSFXBankHead[bankID + 1]";
char sPathSynthC[]                    = "synth.c";
char sSynthHeapPointerAssert[]        = "p";
#pragma section RW ".sdata" \
                   ".sbss"
__declspec(section ".sdata") s32 sSynthLoadRequestIds[2]          = { 0, 0 };
char sSynthFalseCondition[]                                       = "0";
s32 sSynthDecibelFloor                                            = -0x03880000;
s32 sSynthQueuedStreamPlaybackId                                  = -1;
char sSynthBeatMarkerAssert[]                                     = "bmb";
char sSynthNextBeatMarkerAssert[]                                 = "bmbnext";
__declspec(section ".sdata") u32 sSynthActiveChannelFaderMasks[2] = { 0, 0 };
f32 sSynthStreamVolume                                            = 1.0f;
char sSynthEntryAssert[]                                          = "entry";
#pragma pop

#pragma section R ".sdata2" \
                  ".sbss2"

const float sSynthMinimumAudibleVolume                   = 0.000030199517f;
const float sSynthUnitGain                               = 1.0f;
const double sSynthLogToDecibelScale                     = 5692384.633202305;
__declspec(section ".sdata2") const float sSynthZeroGain = 0.0f;
const double sSynthDecibelToLogScale                     = 1.75673301162265e-07;
const double sSynthSignedConversionBias                  = 4503599627370496.0 + 2147483648.0;
const float sSynthInversePanRange                        = 1.0f / 254.0f;
const float sSynthSampleRate                             = 32000.0f;
const float sSynthInverseVolumeRange                     = 1.0f / 255.0f;
const float sSynthMaxMixGain                             = 32767.0f;
const float sSynthFixedPointScale                        = 65536.0f;
const double sSynthInverseVolumeRangeDouble              = 1.0 / 255.0;
const float sSynthRearPanOffset                          = 60.0f;
const float sSynthRearPanScale                           = 0.527559042f;
const float sSynthSurroundAttenuation                    = 0.707167804f;
const double sSynthMaxMixGainDouble                      = 32767.0;
const double sSynthRearMixGain                           = 0.38;
const double sSynthSurroundMixGain                       = 0.49;
const float sSynthHalfGain                               = 0.5f;
const double sSynthSurroundMainGain                      = 0.87;
const float sSynthInverseSampleRate                      = 0.00003125f;
const double sSynthUnsignedConversionBias                = 4503599627370496.0;
const float sSynthMaxStreamVolume                        = 255.0f;
