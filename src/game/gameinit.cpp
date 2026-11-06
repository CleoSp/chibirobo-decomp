#include "game/cgame.h"
#include "game/xobjmap.h"
#include "game/cgameutil.h"

extern "C" {
extern const char sCmdLineOptionDelimiter[2];
extern const char sCmdLineStageOption[6];
extern const char sCmdLineSlotOption[5];
extern const char sStageArgumentDelimiter[2];

char* strtok(char* str, const char* delim);
int strcmp(const char* a, const char* b);
}

#pragma section RX ".text.gameinit"

/**
 * @note Address: 0x8001357C
 * @note Size: 0x4
 */
__declspec(section ".text.gameinit") void CScrnFade::Reset()
{
}

/**
 * @note Address: 0x80013580
 * @note Size: 0xCC
 */
// reads /stage:name and /slot:letter off the command line
extern "C" __declspec(section ".text.gameinit") void Initialize__5CGameFiPPc(CGame* game, int argc, char** argv)
{
	for (s32 i = 1; i < argc; i++) {
		switch (argv[i][0]) {
		case '/':
			char* name = strtok(argv[i] + 1, sCmdLineOptionDelimiter);
			if (strcmp(name, sCmdLineStageOption) == 0) {
				gCmdLineArgs.mStage = FindStageIndexByName(strtok(NULL, sStageArgumentDelimiter));
			} else if (strcmp(name, sCmdLineSlotOption) == 0) {
				gCmdLineArgs.mSlot = *strtok(NULL, sStageArgumentDelimiter) - 'A';
			}
			break;
		}
	}
}
