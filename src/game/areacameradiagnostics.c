// US and JP fill the bytes after each terminator with ASCII zeroes.
#if defined(VERSION_GGTP01)
#define AREA_PADDING(bytes) ""
#else
#define AREA_PADDING(bytes) "\0" bytes
#endif

const char sAreaCameraSourceFile[24]  = "ccameraactionarea.cpp" AREA_PADDING("00");
const char sAreaCameraSetEyeError[44] = "error ! : CCameraActionArea::SetEye_I()\n" AREA_PADDING("000");
#if !defined(VERSION_GGTP01)
const char sAreaCameraUnknownTarget[28] = "SetInte_I( unknown data )\n" AREA_PADDING("0");
#endif
const char sAreaCameraSelectionFormat[32] = "\\B000000A0AREA(%3d,%3d)(%3d)\n" AREA_PADDING("00");
#if defined(VERSION_GGTP01)
const char sAreaCameraOverlayTitle[24] = "\\B000000A0ACAREA\n";
#else
const char sAreaCameraOverlayTitle[20] = "\\B000000A0ACAREA\n" AREA_PADDING("00");
#endif
char sAreaCameraSetEyeLabel[12] = "SetEye_I" AREA_PADDING("000");
