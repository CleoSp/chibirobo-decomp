#ifndef _GAME_STAGEDATA_H
#define _GAME_STAGEDATA_H

#include "types.h"

// what stage.cpp hands out of the loaded stage file. apart from stage.h, which drags in
// the sdk os headers through CModule

class EnvInfo;
struct UnkStageObj;
struct CamInfoData;
struct CamAreaRecord;

extern "C" {
UnkStageObj* GetSpawnData(s32 index);
UnkStageObj* fn_8006EDB4(s32 index);
s32 fn_8006F338(s32 function, s32 count, s32* args);
s32 fn_8006F390(s32 index);
CamAreaRecord* fn_8006F3AC(void);
CamInfoData* fn_8006F3BC(void);
CamAreaRecord* fn_8006F3CC(void);
u8* fn_8006F490(void);
EnvInfo* fn_8006F4B8(void);
s32 fn_8006F654(const char* name);
}

#endif
