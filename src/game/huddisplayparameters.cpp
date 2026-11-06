#include "Version.h"
#include <dolphin/mtx/mtxtypes.h>

extern "C" {
f32 gHudOpacityScale                            = 1.0f;
extern const Vec sHudPlayerIconProjectionOffset = { 0.0f, 10.0f, 0.0f };

#if defined(VERSION_PAL)
extern const f32 sHudCounterDigitYOffset = 2.0f;
#endif
extern const double sHudSignedConversionBias   = 4503601774854144.0;
extern const double sHudUnsignedConversionBias = 4503599627370496.0;
extern const f32 sHudCounterHiddenX            = -220.0f;
extern const f32 sHudCounterApproachFraction   = 0.25f;
#if defined(VERSION_JP)
extern const char sHudCounterStageLookup[] = "stage19";
#endif
extern const f32 sHudBottomEdgeLimit           = 418.0f;
extern const f32 sHudHalfScale                 = 0.5f;
extern const f32 sHudCounterBottomInset        = 64.0f;
extern const f32 sHudCounterRowSpacing         = 40.0f;
extern const f32 sHudCounterVisibleX           = 60.0f;
extern const f32 sHudCounterFadeRate           = 0.08f;
extern const f32 sHudScrapPulseThreshold       = 300.0f;
extern const f32 sHudDegreesToRadians          = 0.017453292f;
extern const f32 sHudWindowAnimationAngle      = 4.712389f;
extern const f32 sHudWindowCornerInset         = 8.0f;
extern const f32 sHudWindowBackdropDepth       = -1.0f;
extern const f32 sHudWindowCornerDiameter      = 16.0f;
extern const f32 sHudWindowHorizontalCapsWidth = 128.0f;
extern const f32 sHudWindowHalfTileWidth       = 32.0f;
extern const f32 sHudWindowTileInset           = 3.0f;
extern const f32 sHudWindowCenterTileOverlap   = 6.0f;
extern const f32 sHudWindowSideTileExtension   = 20.0f;
extern const f32 sHudBatteryHiddenOffset       = -52.0f;
extern const f32 sHudScanlineWidth             = 640.0f;
#if !defined(VERSION_PAL)
extern const f32 sHudTimerMaxFrames = 359999.0f;
#endif
extern const f32 sHudTimerHundredthsPerMinute  = 6000.0f;
extern const f32 sHudTimerHundredthsPerSecond  = 100.0f;
extern const f32 sHudTimerDecimalMinuteWeight  = 10000.0f;
extern const f32 sHudTimerDigitScale           = 1.8f;
extern const f32 sHudTimerFractionalDigitScale = 0.7f;
extern const f32 sHudTimerLastDigitOffset      = -7.2f;
extern const f32 sHudTimerTenthsDigitOffset    = -1.8f;
extern const f32 sHudTimerMinutesOffset        = -9.0f;
extern const f32 sHudTimerDigitsOriginX        = 372.0f;
extern const f32 sHudTimerDigitOverlap         = 3.6f;
extern const f32 sHudTimerDigitsInsetX         = 14.4f;
extern const f32 sHudTimerFractionalDigitsY    = 75.6f;
extern const f32 sHudTimerDigitsY              = 72.0f;
extern const f32 sHudTimerColonX               = 472.8f;
extern const f32 sHudTimerColonY               = 72.9f;
extern const f32 sHudTimerColonScale           = 0.6f;
extern const f32 sHudPlayerIconWidthScale      = 1.1f;
}
