#ifndef _GAME_XOBJS_H
#define _GAME_XOBJS_H

#include "game/xobj.h"

struct UnkXObjList;
// index of obj in the list, -1 when it isn't there
extern "C" s32 fn_8013265C(UnkXObjList* list, CXObj* obj);
extern "C" XObj* fn_80132698(UnkXObjList* list, CoordInfo* info, s32 unused, BOOL async);

#endif
