#include "game/evtqueue.h"
#include "PowerPC_EABI_Support/Runtime/ptmf.h"

extern "C" {
s32 CEvt__cmd_ABORT(CEvt* evt);
s32 CEvt__cmd_RETURN(CEvt* evt);
s32 CEvt__cmd_GOTO(CEvt* evt);
s32 CEvt__cmd_SET(CEvt* evt);
s32 CEvt_EvalCmp(CEvt* evt);
s32 CEvt__cmd_RUN(CEvt* evt);
s32 CEvt__cmd_LIB(CEvt* evt);
s32 CEvt__cmd_PUSHBP(CEvt* evt);
s32 CEvt__cmd_POPBP(CEvt* evt);
s32 CEvt__cmd_SETSP(CEvt* evt);
s32 CEvt__cmd_ANIM(CEvt* evt, u8 command);
s32 CEvt__cmd_ANIM2(CEvt* evt, u8 command);
s32 CEvt__cmd_ATTACH(CEvt* evt, u8 command);
s32 CEvt__cmd_BORN(CEvt* evt);
s32 CEvt__cmd_CALL(CEvt* evt);
s32 CEvt__cmd_CAMERA(CEvt* evt);
s32 CEvt__cmd_WAIT(CEvt* evt, u8 command, s32 step);
s32 CEvt__cmd_COLOR(CEvt* evt);
s32 CEvt__cmd_DIR(CEvt* evt, u8 command);
s32 CEvt__cmd_DISP(CEvt* evt);
s32 CEvt__cmd_KILL(CEvt* evt);
s32 CEvt__cmd_LIGHT(CEvt* evt);
s32 CEvt__cmd_MENU(CEvt* evt);
s32 CEvt__cmd_MOVE(CEvt* evt);
s32 CEvt__cmd_MOVETO(CEvt* evt);
s32 CEvt__cmd_MOVIE(CEvt* evt);
s32 CEvt__cmd_MSG(CEvt* evt, u8 cmd);
s32 CEvt__cmd_POS(CEvt* evt);
s32 CEvt__cmd_PRINTF(CEvt* evt);
s32 CEvt__cmd_PTCL(CEvt* evt);
s32 CEvt__cmd_READ(CEvt* evt);
s32 CEvt__cmd_SCALE(CEvt* evt, u8 cmd);
s32 CEvt__cmd_SCRN(CEvt* evt);
s32 CEvt__cmd_SFX(CEvt* evt);
s32 CEvt__cmd_TIMER(CEvt* evt);
s32 CEvt__cmd_WARP(CEvt* evt);
s32 CEvt__cmd_WIN(CEvt* evt);

#define EVENT_COMMAND(name, flags, fn) extern const char sEvtCommandName##name[] = #name;
#include "game/eventcommands.inc"
#undef EVENT_COMMAND

#pragma push
#pragma dont_zero_bss on
EvtObjIdTable gEventObjectIds = { 0 };
#pragma pop

// The event initializer copies these direct member-function descriptors into the dispatch table.
PTMF gEventCommandHandlers[] = {
#define EVENT_COMMAND(name, flags, fn) { 0, -1, { (void*)fn } },
#include "game/eventcommands.inc"
#undef EVENT_COMMAND
};

EvtCmdEntry evtCmdTable[] = {
#define EVENT_COMMAND(name, flags, fn) { EVT_CMD_##name, { flags, 0, 0 }, sEvtCommandName##name },
#include "game/eventcommands.inc"
#undef EVENT_COMMAND
	{ 0 },
};
}
