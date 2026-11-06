#include "game/mic.h"
#include "game/video.h"

extern "C" {
#if defined(VERSION_PAL)
s32 gVideoFramesPerSecond = 50;
#endif
s32 gActiveMicChannel = -1;

extern const float gGameFloatZero = 0.0f;
#if !defined(VERSION_JP)
extern const char sStageArgumentDelimiter[] = " ";
#endif
extern const float sStageNameBobAmplitude  = 4.0f;
extern const float sStageNameTiltFrequency = 0.7f;
extern const float sGamePi                 = 3.1415927f;
extern const float sGameHalfPi             = 1.5707964f;
extern const float sStageNameSwellWeight   = 0.2f;
#if defined(VERSION_JP)
extern const float sStageNameHalfSpacing = 29.0f;
#else
extern const float sStageNameHalfSpacing = 21.6f;
#endif
extern const float sGameScreenCenterX = 320.0f;
#if defined(VERSION_JP)
extern const float sStageNameSpacing = 58.0f;
#else
extern const float sStageNameSpacing = 43.2f;
#endif
extern const float sGameHalf             = 0.5f;
extern const float sStageNameOpaqueAlpha = 255.0f;
#if defined(VERSION_JP)
extern const float sStageNameGlyphSize = 25.0f;
#else
extern const float sStageNameGlyphSize = 22.0f;
#endif
extern const float sStageNameBubbleScale       = 2.2f;
extern const float sStageNameBaselineY         = 150.0f;
extern const float sGameDegreesToRadians       = 0.017453292f;
extern const float sStageNameMaxTiltDegrees    = 10.0f;
extern const float sStageNameGlyphTextureWidth = 24.0f;
extern const float sStageNameGlyphScale        = 1.5f;

float sStageNameShadowX[4] = { -1.0f, 1.0f, 1.0f, -1.0f };
float sStageNameShadowY[4] = { -1.0f, -1.0f, 1.0f, 1.0f };
}
