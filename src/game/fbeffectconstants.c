#include "Dolphin/GX/GXTypes.h"

const float sOutlineInitialOffsets[4][2] = { { -3, -3 }, { 3, -3 }, { 3, 3 }, { -3, 3 } };
#if defined(VERSION_GGTP01)
const char sOutlineAllocationTag[] = "(outline)";
#else
const char sOutlineAllocationTag[16] = "(outline)\0"
                                       "000000";
#endif
const unsigned char sBlurOpaqueBlack[4]       = { 0, 0, 0, 255 };
const unsigned char sOutlineDefaultLine[4]    = { 34, 9, 25, 4 };
const unsigned char sOutlineDefaultTint[4]    = { 200, 0, 255, 128 };
const unsigned char sOutlineDefaultFill[4]    = { 100, 100, 200, 128 };
const unsigned char sOutlineLineWhite[4]      = { 255, 255, 255, 4 };
const unsigned char sOutlineFillWhite[4]      = { 255, 255, 255, 0 };
const unsigned char sOutlineThickFillWhite[4] = { 255, 255, 255, 0 };
const unsigned char sOutlineOffsetColor0[4]   = { 255, 255, 255, 8 };
const unsigned char sOutlineOffsetColor1[4]   = { 255, 255, 255, 64 };
const float sFbEffectZero                     = 0.0f;
const float sFbEffectOne                      = 1.0f;
#if !defined(VERSION_GGTP01)
const unsigned int sFbEffectUnsignedConversionBiasAlignment = 0x30303030;
#endif
const double sFbEffectUnsignedConversionBias = 4503599627370496.0;
const float sFbEffectPatternRotation         = -0.2617994f;
const float sFbEffectHalf                    = 0.5f;
const float sFbEffectPi                      = 3.1415927f;
const float sFbEffectAlphaScale              = 255.0f;
const float sFbEffectQuarterTurn             = 1.5707964f;
const float sFbEffectOffsetRadius            = 3.0f;
const float sFbEffectRotationStep            = 0.034906585f;
#if !defined(VERSION_GGTP01)
const unsigned int sFbEffectSignedConversionBiasAlignment = 0x30303030;
#endif
const double sFbEffectSignedConversionBias     = 4503601774854144.0;
const float sFbEffectPatternBaseScale          = 1.3f;
const float sFbEffectPatternScaleDivisor       = 1000.0f;
const float sFbEffectPatternTranslationDivisor = 100.0f;
const float sFbEffectDefaultThickness          = 2.0f;
#if defined(VERSION_GGTP01)
const char sBlurAllocationTag[] = "(blur)";
#else
const char sBlurAllocationTag[8] = "(blur)\0"
                                   "0";
#endif

#pragma section R ".sdata2" \
                  ".sbss2"
__declspec(section ".sdata2") const GXColor sOutlineDisabledColor;
__declspec(section ".sdata2") const GXColor sBlurTransparentBlack;
