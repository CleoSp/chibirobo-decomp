#include "Dolphin/mtx.h"

const Vec sMessageDefaultFontPosition        = { 0.0f, 0.0f, 0.0f };
const char sMessageUnknownFontCommand[32]    = "Unknown msg-setfont cmd - %d\n";
const char sMessageTranslationCondition[12]  = "translate";
const char sMessageNoQuaternionCondition[40] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
#if defined(VERSION_GGTJ01_01)
const char sMessageTextureTypeName[12]   = "_GXTexObj";
const char sMessageCoordInfoTypeName[16] = "CoordInfo\0"
                                           "000000";
#else
const char sMessageCoordInfoTypeName[12] = "CoordInfo";
#if defined(VERSION_GGTP01)
const char sMessageTextureTypeName[16] = "_GXTexObj";
#else
const char sMessageTextureTypeName[16] = "_GXTexObj\0"
                                         "000000";
#endif
#endif

const float sMessageDegreesToRadians = 0.017453292f;
const float sMessagePercentScale     = 100.0f;
const char sPathMsgCpp[8]            = "msg.cpp";
#if defined(VERSION_GGTP01)
const char sMessagePanicFormat[] = "%s%s";
#else
const char sMessagePanicFormat[8] = "%s%s\0"
                                    "000";
#endif
const double sMessageUnsignedConversionBias = 4503599627370496.0;
const double sMessageSignedConversionBias   = 4503601774854144.0;
const float sMessageZero                    = 0.0f;
const float sMessageOne                     = 1.0f;
