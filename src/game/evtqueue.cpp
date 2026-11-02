#include "game/evtqueue.h"

static inline void ClearScript(CEvt* evt)
{
	evt->mData.mScript  = NULL;
	evt->_18            = 0;
	evt->_1C            = 0;
	evt->_20            = 0;
	evt->mCalls.mCount  = 0;
	evt->mFrames.mCount = 0;
	evt->mValues.mCount = 0;
}

static inline void ClearQueue()
{
	sEvtQueue.mRead = 0;
	for (u32 i = 0; i < 128; ++i) {
		sEvtQueue.mScript[i] = NULL;
	}
}

static inline void ClearResults(CEvt* evt)
{
	evt->_10 = 0;
	evt->_14 = 0;
	evt->_58 = 0;
	evt->_5C = -1;
	evt->_5E = -1;
}

/**
 * @note Address: 0x8007EEA4
 * @note Size: 0x194
 */
extern "C" void fn_8007EEA4(CEvt* evt, u8* script, u32 context, s16 argument0, s16 argument1)
{
	if (script == NULL)
		return;
	if (evt->mData.mScript != NULL && evt->_00 == 0) {
		s32 cursor = sEvtQueue.mRead;
		for (s32 i = 0; i < 128; ++i) {
			s32 index = cursor % 128;
			if (sEvtQueue.mScript[index] == NULL) {
				sEvtQueue.mScript[index]  = script;
				sEvtQueue.mContext[index] = context;
				sEvtQueue.mArg0[index]    = argument0;
				sEvtQueue.mArg1[index]    = argument1;
				return;
			}
			++cursor;
		}
		return;
	}
	ClearScript(evt);
	if (evt->_00 == 0)
		ClearQueue();
	ClearResults(evt);
	evt->mData.mScript = script;
	evt->mData._04     = evt->_00 != 0 ? evt->_00 : context;
	evt->_5C           = argument0;
	evt->_5E           = argument1;
}

/**
 * @note Address: 0x8007F038
 * @note Size: 0x8
 */
extern "C" BOOL fn_8007F038(CEvt* evt)
{
	return TRUE;
}

/**
 * @note Address: 0x8007F040
 * @note Size: 0x154
 */
extern "C" void fn_8007F040(CEvt* evt, BOOL clearTimers, BOOL clearGlobal)
{
	ClearScript(evt);
	if (evt->_00 == 0) {
		if (clearTimers) {
			for (u32 i = 0; i < 10; ++i) {
				sEvtTimer.mFrames[i] = 0;
				sEvtTimer.mValue[i]  = 0;
			}
		}
		ClearQueue();
		if (clearGlobal)
			lbl_802247B0.mCount = 0;
	}
	ClearResults(evt);
}
