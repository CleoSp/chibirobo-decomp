#include "Dolphin/GX/GXTypes.h"

const GXVtxAttrFmtList sGsStrokeVertexFormat[] = {
	{ GX_VA_POS, GX_POS_XYZ, GX_F32, 0 },
	{ GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0 },
	{ GX_VA_NULL, 0, 0, 0 },
};
const GXVtxDescList sGsStrokeVertexDesc[] = {
	{ GX_VA_POS, GX_DIRECT },
	{ GX_VA_CLR0, GX_DIRECT },
	{ GX_VA_NULL, 0 },
};
const char sGsScreenDebugFormat[76]     = "----------------------\nfb (%3d, %3d), xfb=%3d\norg(%3d, %3d)\nvi (%3d, %3d)\n";
const char sGsTypeNameCGsRenderMode[16] = "CGsRenderMode";
const char sGsTypeNameHSDPEDesc[12]     = "_HSD_PEDesc";
const char sGsTypeNameCGsTevStage[12]   = "CGsTevStage";
const char sGsTypeNameHSDTevDesc[16]    = "_HSD_TevDesc";
const char sGsTypeNameCGsChannel[12]    = "CGsChannel";
const char sGsTypeNameHSDChan[12]       = "_HSD_Chan";
#if defined(VERSION_GGTJ01_01)
const char sGsTypeNameGXTexObj[12] = "_GXTexObj\0"
                                     "00";
#elif defined(VERSION_GGTP01)
const char sGsTypeNameGXTexObj[16] = "_GXTexObj";
#else
const char sGsTypeNameGXTexObj[16] = "_GXTexObj\0"
                                     "000000";
#endif
