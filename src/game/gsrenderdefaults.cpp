#include "game/cgs.h"

extern "C" {
// The original initializer addresses its render presets relative to this base.
extern const unsigned char sGsRenderDefaultsBase[24] = { 0 };
#if !defined(VERSION_GGTJ01_01)
extern const Vec sGsDefaultTextRotation = { 0.0f, 0.0f, 0.0f };
#endif
extern const GsBlendMode sGsDefaultBlend           = { 1, 4, 5, 15 };
extern const GsAlphaCompare sGsDefaultAlphaCompare = { 4, 0, 0, 3, 255 };
extern const GsZMode sGsDefaultDepthMode           = { 0, 3, 0 };
extern const GsTevOrder sGsPrimaryOrder            = { 0xFF, GX_TEXMAP_NULL, GX_COLOR0A0 };
extern const GsTevOp sGsPrimaryColorOp             = { GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, 0 };
extern const GsTevIn sGsPrimaryColorInputs         = { 0xA, 0xF, 0xF, 0xF };
extern const GsTevOp sGsPrimaryAlphaOp             = { GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, 0 };
extern const GsTevIn sGsPrimaryAlphaInputs         = { 5, 7, 7, 7 };
extern const GsTevOrder sGsSecondaryOrder          = { 0xFF, GX_TEXMAP_NULL, GX_COLOR1A1 };
extern const GsTevOp sGsSecondaryColorOp           = { GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, 0 };
extern const GsTevIn sGsSecondaryColorInputs       = { 0xF, 0xA, 0xC, 0 };
extern const GsTevOp sGsSecondaryAlphaOp           = { GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, 0 };
extern const GsTevIn sGsSecondaryAlphaInputs       = { 7, 5, 6, 0 };
extern const GsChanCtrl sGsColor0DefaultControl    = { 0, 0, 0, 0, 2, 2 };
extern const GsChanCtrl sGsAlpha0DefaultControl    = { 0, 0, 0, 0, 0, 2 };
extern const GsChanCtrl sGsColor1DefaultControl    = { 1, 0, 0, 0, 0, 0 };
extern const GsChanCtrl sGsAlpha1DefaultControl    = { 0, 0, 0, 0, 0, 2 };

extern const PrimVertex sGsLineVertexTemplate[2] = {};
// This zero-filled span follows the two line vertices in each retail image.
extern const unsigned char sGsVertexDataGap[48]  = {};
extern const PrimVertex sGsQuadVertexTemplate[4] = {};
}
