#include "types.h"
#include "Version.h"

extern "C" u8 gGame[];
extern "C" void InfoScrn_SetTimerHundredths(void* obj, s32 arg);
extern "C" s32 InfoScrn_GetTimerHundredths(void* obj);
extern "C" void InfoScrn_ShowTimer(void* obj);
extern "C" void InfoScrn_HideTimer(void* obj);
extern "C" void InfoScrn_ApplyTimerHudCommand(void* obj, s32, s32);
extern "C" void SetFafaPartVisibility(s32 arg);

#if defined(VERSION_JP)
#define SHUN_INFO_SCREEN() ((void*)(gGame + 0x8EF0))
#else
#define SHUN_INFO_SCREEN() ((void*)(gGame + 0x8F38))
#endif

extern "C" {
s32 CmdSetJunkUnkoFlag(s32 cmd, s32 count, s32* args);
s32 CmdApplySavedObjFlags(s32 cmd, s32 count, s32* args);
s32 CmdSaveObjFlag(s32 cmd, s32 count, s32* args);
s32 CmdClearSavedObjFlags(s32 cmd, s32 count, s32* args);
s32 CmdSetOrGetTimer(s32 cmd, s32 count, s32* args);
s32 CmdSetTimerRunning(s32 cmd, s32 count, s32* args);
s32 CmdShowTimer(s32 cmd, s32 count, s32* args);
s32 CmdSetFafaPartVisibility(s32 cmd, s32 count, s32* args);
s32 CmdCheckOnOffFlag(s32 cmd, s32 count, s32* args);
s32 CmdStartOnOff2(s32 cmd, s32 count, s32* args);
s32 CmdStartOnOff1(s32 cmd, s32 count, s32* args);
s32 CmdStartOnOff0(s32 cmd, s32 count, s32* args);
s32 CmdResetOnOff(s32 cmd, s32 count, s32* args);
s32 CmdPrintArgs(s32 cmd, s32 count, s32* args);
void ShunNoOpSlot5(void* arg);
void DrawStageHud(void* arg);
void UpdateStage(void* arg);
void InitStage(void* arg);

typedef void (*RelSlotFunc)(void* arg);
typedef s32 (*RelCmdFunc)(s32 cmd, s32 count, s32* args);

RelSlotFunc _prolog_slots[7] = { InitStage, UpdateStage, NULL, NULL, DrawStageHud, ShunNoOpSlot5, NULL };
RelCmdFunc _prolog_cmds[]    = {
    CmdPrintArgs,          CmdResetOnOff,      CmdStartOnOff0,           CmdStartOnOff1,
    CmdStartOnOff2,        CmdCheckOnOffFlag,  CmdSetFafaPartVisibility, CmdShowTimer,
    CmdSetTimerRunning,    CmdSetOrGetTimer,   CmdClearSavedObjFlags,    CmdSaveObjFlag,
    CmdApplySavedObjFlags, CmdSetJunkUnkoFlag,
};
}

/**
 * @note Address: shun .text 0x408
 * @note Size: 0x6C
 */
extern "C" s32 CmdSetOrGetTimer(s32 cmd, s32 count, s32* args)
{
	s32 ret = 0;

	if (args[0] != 0) {
		InfoScrn_SetTimerHundredths(SHUN_INFO_SCREEN(), args[1]);
	} else {
		ret = InfoScrn_GetTimerHundredths(SHUN_INFO_SCREEN());
	}
	return ret;
}

/**
 * @note Address: shun .text 0x474
 * @note Size: 0x58
 */
extern "C" s32 CmdSetTimerRunning(s32 cmd, s32 count, s32* args)
{
	if (args[0] != 0) {
		InfoScrn_ShowTimer(SHUN_INFO_SCREEN());
	} else {
		InfoScrn_HideTimer(SHUN_INFO_SCREEN());
	}
	return 0;
}

/**
 * @note Address: shun .text 0x4CC
 * @note Size: 0x68
 */
extern "C" s32 CmdShowTimer(s32 cmd, s32 count, s32* args)
{
	if (args[0] != 0) {
		InfoScrn_ApplyTimerHudCommand(SHUN_INFO_SCREEN(), 1, 1);
	} else {
		InfoScrn_ApplyTimerHudCommand(SHUN_INFO_SCREEN(), 0, 1);
	}
	return 0;
}

/**
 * @note Address: shun .text 0x534
 * @note Size: 0x28
 */
extern "C" s32 CmdSetFafaPartVisibility(s32 cmd, s32 count, s32* args)
{
	SetFafaPartVisibility(args[0]);
	return 0;
}
