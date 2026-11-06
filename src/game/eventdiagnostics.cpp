#include "game/eventhudcommands.h"
#include <dolphin/mtx/mtxtypes.h>
#include <dolphin/gx/gxtypes.h>

extern "C" {
#include "game/eventconstantpool.inc"

GXColor sEvtWindowColorTemplate;
GXColor sEvtWindowDefaultColor = { 0, 0, 0, 0x80 };

#if defined(VERSION_GGTP01)
char sEvtUpdateName[8] = "Update";
#else
char sEvtUpdateName[8] = { 'U', 'p', 'd', 'a', 't', 'e', '\0', '0' };
#endif
char sEvtEvalSetName[] = "EvalSet";
char sEvtEvalCmpName[] = "EvalCmp";
}
