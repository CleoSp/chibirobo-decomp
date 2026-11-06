#include "game/progressive.h"
#include "game/cgame.h"
#include "game/cmdline.h"

/**
 * @note Address: 0x8001C89C
 * @note Size: 0x28
 */
extern "C" void ResetProgressiveScanPrompt(void)
{
	lbl_80210730._00 = 0;
	lbl_80210730._04 = -1;
	lbl_80210730._08 = 0.0f;
	lbl_80210730._10 = 0;
	lbl_80210730._14 = 0;
}

struct BootConfigEntry {
	const char* name;
	s32 kind;
	u64 value;
};

extern "C" {
extern const BootConfigEntry gBootConfigEntries[];
u32 OSGetSoundMode(void);
char* strtok(char*, const char*);
int strcmp(const char*, const char*);
int atoi(const char*);
s32 GetStageNum();
const char* GetStageFileName(s32);
s32 GetDefaultStageIndex();
}

extern "C" void LoadBootConfig()
{
	CGame& game = gGame;
	u64& flags  = game.mStatus.mFlags;
	flags |= 1ULL << 34;
	flags |= 1ULL << 29;
	flags |= 1ULL << 30;
	flags |= 1ULL << 32;
	flags |= 1ULL << 33;
	if (OSGetSoundMode() == 1)
		flags |= 1ULL << 36;
	else
		flags &= ~(1ULL << 36);
	flags |= 1ULL << 35;
	flags |= 1ULL << 14;
	flags |= 1ULL << 16;
	flags |= 1ULL << 38;
	flags |= 1ULL << 40;
	flags |= 1ULL << 41;
	flags |= 1ULL << 42;
	flags |= 1ULL << 47;
	flags |= 1ULL << 48;
	flags |= 1ULL << 49;
	flags |= 1ULL << 54;

	CFileText file;
	file.mPos  = 0;
	s32 result = file.Read("/qp.ini", 0, 0);
	if (result == 0) {
		char line[256];
		BOOL restricted = FALSE;
		while (file.GetLine(line) == TRUE) {
			for (char* p = line; *p; ++p)
				if (*p == ';') {
					*p = 0;
					break;
				}
			char* key = strtok(line, " \t,=");
			if (!key)
				continue;
			if (strcmp(key, "&") == 0)
				restricted = TRUE;
			char* text = strtok(NULL, " \t,=");
			if (!text)
				continue;
			for (const BootConfigEntry* e = gBootConfigEntries; e->name; ++e) {
				if (strcmp(key, e->name) != 0)
					continue;
				switch (e->kind) {
				case 0:
					if (restricted)
						break;
				case 1:
					if (atoi(text))
						flags |= e->value;
					else
						flags &= ~e->value;
					break;
				case 2:
					if (!restricted) {
						s32 stage = lbl_8020FBC0.mStage >= 0 ? lbl_8020FBC0.mStage : atoi(text);
						if (stage >= 100)
							stage = GetStageNum() + stage - 99;
						lbl_8020FBC0.mStage = stage;
						gGame.SetStage(stage);
						GetStageFileName(stage);
					}
					break;
				case 3:
					break;
				case 5: {
					flags |= e->value;
					s32 x = atoi(text);
					text  = strtok(NULL, " \t,=");
					if (text) {
						lbl_8020FBC0.mCrosshairX = x;
						lbl_8020FBC0.mCrosshairY = atoi(text);
						atoi(text);
					}
					break;
				}
				case 4:
					if (restricted)
						break;
					switch (e->value) {
					case 0:
						lbl_8020FBC0.mMenuMode = atoi(text);
						break;
					case 1:
						lbl_8020FBC0.mPick1 = atoi(text) + 1;
						break;
					case 2:
						lbl_8020FBC0.mPick2 = atoi(text) + 1;
						break;
					case 3:
						lbl_8020FBC0.mPick3 = atoi(text) + 1;
						break;
					case 4:
						lbl_8020FBC0.mPlayerFlags = atoi(text);
						break;
					case 5:
						lbl_8020FBC0.mPlayerSetting0 = atoi(text);
						break;
					case 6:
						lbl_8020FBC0.mPlayerSetting1 = atoi(text);
						break;
					case 7:
						lbl_8020FBC0.mTitleCursor = atoi(text);
						break;
					}
					atoi(text);
					break;
				}
			}
		}
		file.Close();
	} else if (result == -3) {
		s32 stage           = lbl_8020FBC0.mStage >= 0 ? lbl_8020FBC0.mStage : GetDefaultStageIndex();
		lbl_8020FBC0.mStage = stage;
		gGame.SetStage(stage);
	}
}
