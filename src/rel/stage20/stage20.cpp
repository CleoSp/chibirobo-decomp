#include "types.h"
#include "Version.h"
#include "game/cgame.h"

extern "C" s32 CmdNop(s32 cmd, void* arg, void* arg2);
extern "C" void fn_16_8(void* arg);
extern "C" void UpdateStage(void* arg);
extern "C" void InitStage(void* arg);

typedef void (*RelSlotFunc)(void* arg);
typedef s32 (*RelCmdFunc)(s32 cmd, void* arg, void* arg2);

extern "C" {
RelSlotFunc _prolog_slots[7] = { InitStage, UpdateStage, 0, 0, 0, fn_16_8 };
RelCmdFunc _prolog_cmds[]    = { CmdNop };
}

static XObj* timeAnimObj;

/**
 * @note Address: stage20 .text 0x0
 * @note Size: 0x8
 */
extern "C" s32 CmdNop(s32 cmd, void* arg, void* arg2)
{
	return 0;
}

/**
 * @note Address: stage20 .text 0x8
 * @note Size: 0x4
 */
extern "C" void fn_16_8(void* arg)
{
}

/**
 * @note Address: stage20 .text 0xC
 * @note Size: 0xA0
 */
extern "C" void UpdateStage(void* arg)
{
	if (timeAnimObj != NULL) {
		f32 x = 150.0f * lbl_8025C400.mTimeState.mTime;
		if (lbl_8025C400.mTimeState.mMode == 1) {
			x += 150.0f;
		}
		timeAnimObj->SetAnim(0, 1, 0, 0, 0, x, 1.0f, 0.0f, 10.0f);
	}
}

/**
 * @note Address: stage20 .text 0xAC
 * @note Size: 0x78
 */
extern "C" void InitStage(void* arg)
{
	XObj* obj;
	s32 i;

	timeAnimObj = 0;
	for (i = 0; i < lbl_8025C400.mXObjs.mObjectCount; i++) {
		obj = lbl_8025C400.GetObject(i);
		if ((obj->mStatus.mFlags & 1) != 0 && obj->mCoord._00->_00 == 0x2CD) {
			timeAnimObj = obj;
		}
	}
}
