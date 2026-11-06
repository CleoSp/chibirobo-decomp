// LayoutMainMenuRows writes the selected slot letter into the retail label.
#pragma section RW ".rodata"

#if defined(VERSION_GGTJ01_01)
const char sMainMenuStatusLabel[12]                             = "\x83\x58\x83\x65\x81\x5B\x83\x5E\x83\x58";
__declspec(section ".rodata") char sMainMenuMemoryCardLabel[12] = "\x83\x81\x83\x82\x83\x4A\x20\x28\x3F\x29";
const char sMainMenuSelectStageLabel[12]                        = "\x83\x7D\x83\x62\x83\x76\x91\x49\x91\xF0";
#else
const char sMainMenuStatusLabel[12]                             = "StatusMenu";
__declspec(section ".rodata") char sMainMenuMemoryCardLabel[12] = "MemCard(?)";
const char sMainMenuSelectStageLabel[12]                        = "SelectStage";
#endif
