#include "Version.h"
#include <sysdolphin/cobj.h>

const char sSubwindowAllocationName[12] = "(subwin)";
#if defined(VERSION_PAL)
#pragma section R ".sdata2" \
                  ".sbss2"
__declspec(section ".sdata2") const HSD_RectS16 sSubwindowInitialViewport;
#else
const HSD_RectS16 sSubwindowInitialViewport = { 0, 320, 0, 224 };
#endif
