#include "Dolphin/GX/GXTypes.h"

const GXColor sGsDefaultClearColor                      = { 0, 0, 32, 0 };
const unsigned char sGsDefaultColorUpdate               = 1;
const unsigned char sGsDefaultDepthCompareBeforeTexture = 1;
const unsigned char sGsDefaultDither                    = 1;
#if !defined(VERSION_GGTP01)
const unsigned char sGsRenderDefaultsPadding = '0';
#endif
const int sGsDefaultCullMode               = GX_CULL_BACK;
const unsigned char sGsDefaultLineSize[4]  = { 12, 0, 0, 0 };
const int sGsDefaultLineTexOffset          = 5;
const unsigned char sGsDefaultPointSize[4] = { 24, 0, 0, 0 };
const int sGsDefaultPointTexOffset         = 5;
const unsigned int sGsSecondaryTevStage    = 1;
const GXColor sGsColor0DefaultAmbient      = { 0, 0, 0, 255 };
const GXColor sGsColor0DefaultMaterial     = { 255, 255, 255, 255 };
const int sGsDefaultAlpha0Channel          = GX_ALPHA0;
const GXColor sGsAlpha0DefaultAmbient      = { 0, 0, 0, 255 };
const GXColor sGsAlpha0DefaultMaterial     = { 255, 255, 255, 255 };
const int sGsDefaultColor1Channel          = GX_COLOR1;
const GXColor sGsColor1DefaultAmbient      = { 0, 0, 0, 255 };
const GXColor sGsColor1DefaultMaterial     = { 255, 255, 255, 255 };
const int sGsDefaultAlpha1Channel          = GX_ALPHA1;
const GXColor sGsAlpha1DefaultAmbient      = { 0, 0, 0, 255 };
const GXColor sGsAlpha1DefaultMaterial     = { 255, 255, 255, 255 };
const float sGsScalarZero                  = 0.0f;
const float sGsScalarOne                   = 1.0f;
const float sGsScalarHalf                  = 0.5f;
const float sGsScalarNegativeOne           = -1.0f;
const float sGsStrokePeriodX               = 0.25f;
const float sGsStrokeInnerPoint            = 0.35000002f;
const float sGsStrokeOuterPoint            = 0.75f;
const float sGsStrokePercentHigh           = 0.95f;
const float sGsStrokePercentLow            = 0.15f;
const float sGsStrokePeriodY               = 0.84999996f;
const float sGsStrokeInset                 = 0.2f;
const float sGsStrokeGridStep              = 0.11f;
#if !defined(VERSION_GGTP01)
const unsigned int sGsConversionAlignment = 0x30303030;
#endif
const double sGsSignedConversionBias   = 4503601774854144.0;
const float sGsFontCellSize            = 24.0f;
const float sGsScalarTwo               = 2.0f;
const double sGsUnsignedConversionBias = 4503599627370496.0;

#pragma section R ".sdata2" \
                  ".sbss2"
// The last width occupies the final eight-byte slot in the retail section.
__declspec(section ".sdata2") const struct {
	float width;
	unsigned char padding[4];
} sGsInitialMaxStrokeLineWidth;
__declspec(section ".sdata2") const float sGsInitialStrokeLineWidth ATTRIBUTE_ALIGN(4);
__declspec(section ".sdata2") const float sGsInitialMaxLineWidth ATTRIBUTE_ALIGN(4);
__declspec(section ".sdata2") const float sGsInitialLineWidth ATTRIBUTE_ALIGN(4);
__declspec(section ".sdata2") const GXColor sGsTransparentBlack ATTRIBUTE_ALIGN(4);
__declspec(section ".sdata2") const int sGsDefaultColor0Channel ATTRIBUTE_ALIGN(4);
__declspec(section ".sdata2") const unsigned int sGsPrimaryTevStage ATTRIBUTE_ALIGN(4);
__declspec(section ".sdata2") const unsigned short sGsDefaultDestinationAlpha ATTRIBUTE_ALIGN(4);
__declspec(section ".sdata2") const unsigned char sGsDefaultAlphaUpdate ATTRIBUTE_ALIGN(4);
