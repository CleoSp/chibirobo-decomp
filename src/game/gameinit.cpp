#include "types.h"
#include <string.h>
#include "game/stagedata.h"

class CGame;

// the boot options, -1 for anything not given
struct StartupArguments {
	s32 mStage;          // _00, /stage:name
	s32 mSlot;           // _04, /slot:A, 0 based
	u8 _08[0x20 - 0x08]; // _08
};

extern "C" {
char* strtok(char* string, const char* delimiters);
extern StartupArguments lbl_8020FBC0;
}

/**
 * @note Address: 0x80013580
 * @note Size: 0xCC
 */
// CGame's Initialize, reads the /stage: and /slot: options
extern "C" void fn_80013580(CGame* game, s32 argc, char** argv)
{
	for (s32 i = 1; i < argc; ++i) {
		switch (argv[i][0]) {
		case '/':
			break;
		default:
			continue;
		}
		char* option = strtok(argv[i] + 1, ":");
		if (strcmp(option, "stage") == 0) {
			lbl_8020FBC0.mStage = fn_8006F654(strtok(NULL, " "));
		} else if (strcmp(option, "slot") == 0) {
			lbl_8020FBC0.mSlot = (s8)strtok(NULL, " ")[0] - 'A';
		}
	}
}
