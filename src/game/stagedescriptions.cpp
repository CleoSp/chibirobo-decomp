#include "game/stage.h"

// US and JP use ASCII zeroes in the unused bytes of each word-aligned string.
#if defined(VERSION_PAL)
const char StageNamePad = 0;
#else
const char StageNamePad = '0';
#endif

template <int length, int padding>
struct PaddedStageName {
	char text[length];
	char tail[padding];
};

extern "C" {
#if defined(VERSION_JP)
extern const char sGlobalResourcePath[] = "bin/globals.bin";
#else
extern const char sPathGlobalsBin[] = "globals.bin";
#endif
#if defined(VERSION_PAL)
extern const char sTakanabeStageName[12] = "takanabe";
#else
extern const char sTakanabeStageName[12] = { 't', 'a', 'k', 'a', 'n', 'a', 'b', 'e', 0, '0', '0', '0' };
#endif
}

#if !defined(VERSION_JP)
extern "C" {
extern const PaddedStageName<3, 1> sEnglishResourceDirectorySuffix = { "/e", { StageNamePad } };
extern const PaddedStageName<3, 1> sGermanResourceDirectorySuffix  = { "/g", { StageNamePad } };
extern const PaddedStageName<3, 1> sFrenchResourceDirectorySuffix  = { "/f", { StageNamePad } };
extern const PaddedStageName<3, 1> sSpanishResourceDirectorySuffix = { "/s", { StageNamePad } };
extern const PaddedStageName<3, 1> sItalianResourceDirectorySuffix = { "/i", { StageNamePad } };
extern const char gEmptyResourceDirectorySuffix[];
}
const char* gResourceDirectorySuffixByLanguage[]
    = { sEnglishResourceDirectorySuffix.text, sGermanResourceDirectorySuffix.text,  sFrenchResourceDirectorySuffix.text,
	    sSpanishResourceDirectorySuffix.text, sItalianResourceDirectorySuffix.text, sEnglishResourceDirectorySuffix.text,
	    gEmptyResourceDirectorySuffix };
#endif

#if defined(VERSION_PAL)
#define STAGE(file, message, flags) { file, file, message, flags }
#else
#define STAGE(file, message, flags) { file, message, flags }
#endif

extern "C" {
extern const PaddedStageName<5, 3> sShunStageName;
extern const PaddedStageName<5, 3> sHoriStageName;
extern const char sAhkStageName[4];
extern const PaddedStageName<6, 2> sJunkoStageName;
extern const PaddedStageName<7, 1> sSayokoStageName;
extern const PaddedStageName<5, 3> sMoryStageName;
extern const char sRyosukeStageName[8];
extern const PaddedStageName<7, 1> sMarikoStageName;
}

StageDesc stageTable[] = {
	STAGE("stage00", 0, 5),
	STAGE("stage01", 1, 5),
	STAGE("stage02", 2, 5),
	STAGE("stage03", 3, 5),
	STAGE("stage04", 4, 5),
	STAGE("stage05", 5, 4),
	STAGE("stage06", 6, 5),
	STAGE("stage07", 7, 5),
	STAGE("stage08", 8, 5),
	STAGE("stage09", 9, 5),
	STAGE("stage10", 10, 1),
	STAGE("stage11", 11, 5),
	STAGE("stage12", 12, 5),
	STAGE("stage13", 13, 5),
	STAGE("stage14", 14, 5),
	STAGE("stage15", 15, 5),
	STAGE("stage16", 16, 5),
	STAGE("stage17", 17, 5),
	STAGE("stage18", 18, 5),
	STAGE("stage19", 19, 5),
	STAGE("stage20", 20, 5),
	STAGE("stage21", 21, 5),
	STAGE("stage22", 22, 5),
	STAGE("stage23", 23, 5),
	STAGE("stage24", 24, 5),
	STAGE("stage25", 25, 5),
	STAGE("stage26", 26, 5),
	STAGE("stage27", 27, 5),
#if defined(VERSION_PAL)
	{ "stage28", "stage07", 28, 5 },
	{ "stage29", "stage07", 29, 5 },
#else
	STAGE("stage28", 28, 5),
	STAGE("stage29", 29, 5),
#endif
	STAGE(NULL, -1, 0),
	STAGE(sShunStageName.text, -1, 5),
	STAGE(sHoriStageName.text, -1, 5),
	STAGE(sAhkStageName, -1, 5),
	STAGE(sJunkoStageName.text, -1, 5),
	STAGE(sSayokoStageName.text, -1, 5),
	STAGE(sMoryStageName.text, -1, 5),
	STAGE(sRyosukeStageName, -1, 5),
	STAGE(sTakanabeStageName, -1, 5),
	STAGE(sMarikoStageName.text, -1, 5),
	STAGE(NULL, 0, 0),
};

extern "C" {
extern const PaddedStageName<5, 3> sShunStageName   = { "shun", { StageNamePad, StageNamePad, StageNamePad } };
extern const PaddedStageName<5, 3> sHoriStageName   = { "hori", { StageNamePad, StageNamePad, StageNamePad } };
extern const char sAhkStageName[4]                  = "ahk";
extern const PaddedStageName<6, 2> sJunkoStageName  = { "junko", { StageNamePad, StageNamePad } };
extern const PaddedStageName<7, 1> sSayokoStageName = { "sayoko", { StageNamePad } };
extern const PaddedStageName<5, 3> sMoryStageName   = { "mory", { StageNamePad, StageNamePad, StageNamePad } };
extern const char sRyosukeStageName[8]              = "ryosuke";
extern const PaddedStageName<7, 1> sMarikoStageName = { "mariko", { StageNamePad } };
}

#if !defined(VERSION_JP)
extern "C" {
// Slots select the language; both English slots share their tables.
extern const char* gEnglishInterfaceText[];
extern const char* gGermanInterfaceText[];
extern const char* gFrenchInterfaceText[];
extern const char* gSpanishInterfaceText[];
extern const char* gItalianInterfaceText[];
extern const char* gJapaneseInterfaceText[];
extern const char* gEnglishPCDialogue[];
extern const char* gGermanPCDialogue[];
extern const char* gFrenchPCDialogue[];
extern const char* gSpanishPCDialogue[];
extern const char* gItalianPCDialogue[];
extern const char* gJapanesePCDialogue[];

const char** gInterfaceTextByLanguage[] = { gEnglishInterfaceText, gGermanInterfaceText,  gFrenchInterfaceText,  gSpanishInterfaceText,
	                                        gItalianInterfaceText, gEnglishInterfaceText, gJapaneseInterfaceText };
const char** gPCDialogueByLanguage[]    = { gEnglishPCDialogue, gGermanPCDialogue,  gFrenchPCDialogue,  gSpanishPCDialogue,
	                                        gItalianPCDialogue, gEnglishPCDialogue, gJapanesePCDialogue };
}
#endif
