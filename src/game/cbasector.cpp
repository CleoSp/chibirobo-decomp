#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/ccamera.h"
#include "game/clight.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern void* __vt__5CBase[];
extern void* __vt__5CFont[];
extern void* __vt__7CTexObj[];
extern void* __vt__9CStatus64[];
CGs* __ct__3CGsFv(CGs* gs);
// CPadEx::CPadEx(), cgame.h can't take cpad.h
void* __ct__6CPadExFv(void* pad);
// the weak ctors and dtors of CBase's arrays
CCamera* __ct__7CCameraFv(CCamera* camera);
CCamera* __dt__7CCameraFv(CCamera* camera, s32 flags);
CLight* __ct__6CLightFv(CLight* light);
CLight* __dt__6CLightFv(CLight* light, s32 flags);
CStopwatch* __ct__10CStopwatchFv(CStopwatch* watch);
CStopwatch* __dt__10CStopwatchFv(CStopwatch* watch, s32 flags);
}

/**
 * @note Address: 0x8001B8A0
 * @note Size: 0xDC
 */
// todo: weak copy
extern "C" CBase* __ct__5CBaseFv(CBase* base)
{
	// the vtable comes right after mReset, sizeof is padded to 8
	*(void***)((u8*)&base->mReset + 4) = __vt__5CBase;
	__ct__3CGsFv((CGs*)base->mGs);
	CFont* font                         = (CFont*)base->mFont;
	*(void***)((u8*)font + 0x13C)       = __vt__5CFont;
	*(void***)((u8*)&font->mTex + 0xFC) = __vt__7CTexObj;
	font->mDesc.textureHeader           = NULL;
	__ct__6CPadExFv(base->mPad);
	*(void***)((u8*)&base->mStatus + 0x8) = __vt__9CStatus64;
	base->mStatus.mFlags                  = 0;
	__construct_array(base->mCameras, (ConstructorDestructor)__ct__7CCameraFv, (ConstructorDestructor)__dt__7CCameraFv, sizeof(CCamera), 2);
	__construct_array(base->mLights, (ConstructorDestructor)__ct__6CLightFv, (ConstructorDestructor)__dt__6CLightFv, sizeof(CLight), 6);
	__construct_array(base->mStopwatch, (ConstructorDestructor)__ct__10CStopwatchFv, (ConstructorDestructor)__dt__10CStopwatchFv,
	                  sizeof(CStopwatch), 3);
	return base;
}
