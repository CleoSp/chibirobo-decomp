#include "game/evtqueue.h"
#include "game/xobj.h"

struct Unk80192214;
extern "C" void fn_80192214(Unk80192214* stack, s32 capacity);

static inline void ClearQueue()
{
	lbl_80369A3C.mRead = 0;
	for (u32 i = 0; i < 128; ++i)
		lbl_80369A3C.mScript[i] = NULL;
}

static inline void ClearTimers()
{
	for (u32 i = 0; i < 10; ++i) {
		lbl_8036A050.mFrames[i] = 0;
		lbl_8036A050.mValue[i]  = 0;
	}
}

/**
 * @note Address: 0x8007F194
 * @note Size: 0x3DC
 */
extern "C" void fn_8007F194(CEvt* evt, u8* context, void* owner)
{
	evt->_00 = (u32)owner;
	evt->_54 = context;
	if (evt->_00 == 0) {
		for (u32 i = 0; i < 128; ++i)
			lbl_8036781C[i] = 0;
		for (u32 i = 0; i < 2048; ++i)
			lbl_80367A2C[i] = 0;
		ClearQueue();
		ClearTimers();
		fn_800929C4(&lbl_8036A0B0, 0);
	}
	fn_80192214((Unk80192214*)&evt->_24, 32);
	fn_80192214((Unk80192214*)&evt->_34, 64);
	fn_80192214((Unk80192214*)&evt->_44, 128);
	evt->mCameraMemento = NULL;
	evt->_04            = NULL;
	evt->_18            = 0;
	evt->_1C            = 0;
	evt->_20            = 0;
	evt->_28            = 0;
	evt->_38            = 0;
	evt->_48            = 0;
	if (evt->_00 == 0) {
		ClearTimers();
		ClearQueue();
		lbl_802247B0.mCount = 0;
	}
	evt->_10 = 0;
	evt->_14 = 0;
	evt->_58 = 0;
	evt->_5C = -1;
	evt->_5E = -1;
}
