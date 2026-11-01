#include "game/evtqueue.h"

static inline void ClearScript(CEvt* evt)
{
	evt->_04 = NULL;
	evt->_18 = 0;
	evt->_1C = 0;
	evt->_20 = 0;
	evt->_28 = 0;
	evt->_38 = 0;
	evt->_48 = 0;
}

static inline void ClearQueue()
{
	lbl_80369A3C.mRead = 0;
	for (u32 i = 0; i < 128; ++i) {
		lbl_80369A3C.mScript[i] = NULL;
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
extern "C" void fn_8007EEA4(CEvt* evt, u8* script, u32 context, s32 argument0, s32 argument1)
{
	if (script == NULL)
		return;
	if (evt->_04 != NULL && evt->_00 == 0) {
		s32 cursor = lbl_80369A3C.mRead;
		for (s32 i = 0; i < 128; ++i) {
			s32 index = cursor % 128;
			if (lbl_80369A3C.mScript[index] == NULL) {
				lbl_80369A3C.mScript[index]  = script;
				lbl_80369A3C.mContext[index] = context;
				lbl_80369A3C.mArg0[index]    = argument0;
				lbl_80369A3C.mArg1[index]    = argument1;
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
	evt->_04 = script;
	evt->_08 = evt->_00 != 0 ? evt->_00 : context;
	evt->_5C = argument0;
	evt->_5E = argument1;
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
				lbl_8036A050.mFrames[i] = 0;
				lbl_8036A050.mValue[i]  = 0;
			}
		}
		ClearQueue();
		if (clearGlobal)
			lbl_802247B0.mCount = 0;
	}
	ClearResults(evt);
}
