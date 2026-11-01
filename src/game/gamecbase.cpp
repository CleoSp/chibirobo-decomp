extern "C" {
#include <sysdolphin/controller.h>
}
#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/cpad.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);

// cpad.h and ut.h both have a CPad, so no ccamera.h or clight.h here
extern "C" {
extern void* __vt__5CBase[];
CStopwatch* fn_80008608(CStopwatch* stopwatch, s32 flags);
CLight* __dt__6CLightFv(CLight* light, s32 flags);
CCamera* __dt__7CCameraFv(CCamera* camera, s32 flags);
CGs* fn_80008050(CGs* gs, s32 flags);
}

/**
 * @note Address: 0x80007F3C
 * @note Size: 0x114
 */
// todo: weak copy
extern "C" CBase* __dt__5CBaseFv(CBase* base, s32 flags)
{
	if (base != NULL) {
		*(void***)(&base->mReset + 1) = __vt__5CBase;
		__destroy_arr(base->mStopwatch, (ConstructorDestructor*)fn_80008608, sizeof(CStopwatch), 3);
		__destroy_arr(base->mLights, (ConstructorDestructor*)__dt__6CLightFv, sizeof(base->mLights[0]), 6);
		__destroy_arr(base->mCameras, (ConstructorDestructor*)__dt__7CCameraFv, sizeof(base->mCameras[0]), 2);
		base->mStatus.CStatus64::~CStatus64();
		((CPadEx*)base->mPad)->CPadEx::~CPadEx();
		((CFont*)base->mFont)->CFont::~CFont();
		fn_80008050((CGs*)base->mGs, -1);
		if ((s16)flags > 0)
			::operator delete(base);
	}
	return base;
}
