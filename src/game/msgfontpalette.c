#include "Dolphin/GX/GXTypes.h"

u32 msgColorTable[10] = {
	0xFFFFFFFF, 0x808080FF, 0x101010FF, 0x00FFFFFF, 0x00E000FF, 0x0000E0FF, 0xFF00FFFF, 0xE00000FF, 0xFFFF00FF, 0xFF8000FF,
};

#if defined(VERSION_GGTP01)
char sMessageSetFontCommandName[] = "_cmd_SETFONT";
#else
char sMessageSetFontCommandName[16] = "_cmd_SETFONT\0"
                                      "000";
#endif
GXColor gMessageWindowDefaultColor = { 0, 0, 0, 128 };

#pragma section RW ".sdata" \
                   ".sbss"
#if defined(VERSION_GGTP01)
__declspec(section ".sdata") char sMessageWindowColorAlignment[4] = { 0 };
#else
__declspec(section ".sdata") char sMessageWindowColorAlignment[4] = { '0', '0', '0', '0' };
#endif
