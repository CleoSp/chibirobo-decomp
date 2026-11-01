#ifndef _GAME_XOBJCHR_H
#define _GAME_XOBJCHR_H

#include "types.h"

// chibi robo's own object, the units from 0x80093000 to 0x800BC000. the class is in
// game/xchr.h, this one stays light

struct Vec;
class CJObj;
class CPad;
class CVec;
class CXObj;
class XObj;
class XObjChr;
class OXMenu;
struct AttachmentValues;

extern "C" {
// the yes/no and number choice the msg and select commands open
void fn_80096E38(OXMenu* menu, CPad* pad);
s32 fn_8009C1BC(AttachmentValues* values, s32 id);
void fn_8009C398(AttachmentValues* values);
void fn_8009FCC4(XObjChr* chr);
BOOL fn_8009FDE4(void* player);
CVec fn_800A0894(CJObj* object, s32 depth, s32* path);
CVec fn_800A0978(CJObj* object);
void fn_800A30EC(XObj* player, s32 arg);
void fn_800B51FC(XObjChr* chr, s32 first, s32 second);
void fn_800B54A4(XObjChr* chr, s32 selection);
void fn_800B558C(XObjChr* chr);
void fn_800B5E1C(XObjChr* chr);
BOOL fn_800B9758(XObjChr* chr);

// two positions the player code keeps
extern Vec lbl_8037827C;
extern Vec lbl_80378288;
}

#endif
