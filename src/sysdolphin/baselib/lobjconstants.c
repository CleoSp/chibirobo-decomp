#pragma section RW ".sdata" ".sbss"
#pragma section R ".sdata2" ".sbss2"

char sLightPositionCondition[] = "lobj->position";
char sLightInterestCondition[] = "lobj->interest";
char sLightDescendantCondition[] = "hsdIsDescendantOf(info, &hsdLObj)";
char sLightUnexpectedFlags[] = "unexpected lightdesc flags (%x)\n";
char sLightLibraryName[] = "sysdolphin_base_library";
char sLightClassName[] = "hsd_lobj";

char sLightSourceFile[] = "lobj.c";
char sLightFalseCondition[] = "0";
char sLightHeaderFile[] = "lobj.h";
char sLightObjectCondition[] = "lobj";
char sLightAllocCondition[] = "new";
__declspec(section ".sdata") char sLightEmptyMessage[] = "";
char sLightLoadCondition[] = "*p";

const double sLightVisibilityThreshold = 0.5;
__declspec(section ".sdata2") const float sLightZero = 0.0f;
const float sLightOne = 1.0f;
const float sLightColorChannelMax = 255.0f;
const double sLightNormalizeThreshold = 1.17549435e-38F;
const float sLightHalf = 0.5f;
const float sLightNegativeOne = -1.0f;
const float sLightDefaultShininess = 50.0f;
const float sLightInfinitePositionScale = 1048576.0f;
const float sLightMinAttenuation = 1.1920929e-07F;
