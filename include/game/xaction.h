#ifndef _GAME_XACTION_H
#define _GAME_XACTION_H

#include "types.h"

// chibi robo's actions, the classes are in game/xact.h. this one stays light, units without
// the hsd headers include it

class XAction;
class XActLift;
class XActPush;

extern "C" {
void ReleasePushAction(XActPush* action);
void fn_800D482C(XAction* action);
BOOL fn_800D4C78(XActLift* action);
BOOL fn_800D4D54(XActLift* action);
BOOL fn_800D4E30(XActLift* action);
BOOL fn_800D4F0C(XActLift* action);
BOOL fn_800D4FE8(XActLift* action);
void ResetPlayerLiftAction(XActLift* action);
}

#endif
