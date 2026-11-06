#include "game/titleresources.h"

const char sTitleEulerCondition[40]       = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sTitleTranslationCondition[12] = "translate";
#if defined(VERSION_GGTJ01_01)
const char gTitleTexturePath[16] = "tpl/title.tpl";
#else
const char gTitleTexturePath[16] = "tpl/title%s.tpl";
#endif

const char sTitleGXTextureTypeName[12] = "_GXTexObj";
const struct {
	char gxColor[12], status[12], coordInfo[12], attrInfo[12], eventData[12], animationQueue[12];
} sTitleClassNames = { "_GXColor", "CStatus64", "CoordInfo", "AttrInfo", "CEvtData",
#if defined(VERSION_GGTP01)
	                   "CAnmQueue"
#else
 "CAnmQueue\0" "00"
#endif
};

const void* gTitleGXTextureTypeInfo[] = { sTitleGXTextureTypeName, 0 };
const void* gTitleGXColorTypeInfo[]   = { sTitleClassNames.gxColor, 0 };
