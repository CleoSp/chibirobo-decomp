#include "game/hsd_jobj.h"
#include "game/clight.h"

extern "C" {
extern void* __vt__6CLight[];
extern void* __vt__4CVec[];
extern void* __vt__6CColor[];
extern void* __vt__7CShadow[];
extern void* __vt__7CCamera[];
}

/**
 * @note Address: 0x8001B538
 * @note Size: 0x6C
 */
// todo: weak copy
extern "C" CLight* __ct__6CLightFv(CLight* light)
{
	*(void***)((u8*)light + 0x354)                           = __vt__6CLight;
	*(void***)((u8*)&light->mPosition + 0xC)                 = __vt__4CVec;
	*(void***)((u8*)&light->mInterest + 0xC)                 = __vt__4CVec;
	*(void***)((u8*)&light->_F8 + 0xC)                       = __vt__4CVec;
	*(void***)((u8*)&light->mColor + 0x4)                    = __vt__6CColor;
	*(void***)((u8*)&light->mSpecColor + 0x4)                = __vt__6CColor;
	*(void***)((u8*)&light->mShadow + 0x1F4)                 = __vt__7CShadow;
	*(void***)((u8*)&light->mShadow.mCamera + 0x190)         = __vt__7CCamera;
	*(void***)((u8*)&light->mShadow.mCamera.mEye + 0xC)      = __vt__4CVec;
	*(void***)((u8*)&light->mShadow.mCamera.mInterest + 0xC) = __vt__4CVec;
	*(void***)((u8*)&light->mShadow.mCamera.mAngle + 0xC)    = __vt__4CVec;
	*(void***)((u8*)&light->mShadow.mCamera.mUp + 0xC)       = __vt__4CVec;
	*(void***)((u8*)&light->mShadow.mCamera.mScale + 0xC)    = __vt__4CVec;
	light->mShadow.mActive                                   = FALSE;
	light->mShadow.mShadow                                   = NULL;
	return light;
}
