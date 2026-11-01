#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern void* __vt__16CCameraActionEvt[];
extern void* __vt__18CInterpolationNone[];
extern void* __vt__20CInterpolationLinear[];
extern void* __vt__21CInterpolationEaseOut[];
extern void* __vt__22CInterfaceCameraAction[];
extern void* __vt__23CInterfaceInterpolation[];
extern void* __vt__23CInterpolationEaseInOut[];
extern void* __vt__24CVectorNoneInterpolation[];
extern void* __vt__26CVectorLinearInterpolation[];
extern void* __vt__27CVectorEaseOutInterpolation[];
extern void* __vt__29CInterfaceVectorInterpolation[];
extern void* __vt__29CVectorEaseInOutInterpolation[];
extern void* __vt__4CVec[];
extern void* __vt__7CStatus[];
void* fn_80009858(void* interp, s32 flags);
void* fn_800098B4(void* interp, s32 flags);
void* fn_80009910(void* interp, s32 flags);
void* fn_80009EC4(void* interp, s32 flags);
}

// todo: weak copy
/**
 * @note Address: 0x8001A81C
 * @note Size: 0x384
 */
extern "C" CCameraActionEvt* fn_8001A81C(CCameraActionEvt* evt)
{
	*(void***)((u8*)evt + 0x3C)            = __vt__22CInterfaceCameraAction; // _3C vtable
	*(void***)((u8*)&evt->mEye + 0xC)      = __vt__4CVec;
	*(void***)((u8*)&evt->mInterest + 0xC) = __vt__4CVec;
	*(void***)((u8*)&evt->mAngle + 0xC)    = __vt__4CVec;
	*(void***)((u8*)evt + 0x3C)            = __vt__16CCameraActionEvt; // _3C vtable
	*(void***)((u8*)&evt->mAxes + 0x4)     = __vt__7CStatus;
	evt->mAxes.mFlags                      = 0;
	*(void***)&evt->mEyeLinear             = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mEyeLinear             = __vt__26CVectorLinearInterpolation;
	__construct_array(evt->mEyeLinear.mAxes, (ConstructorDestructor)fn_8001AC9C, (ConstructorDestructor)fn_80009858,
	                  sizeof(CInterpolationLinear), 3);
	*(void***)&evt->mEyeEaseInOut = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mEyeEaseInOut = __vt__29CVectorEaseInOutInterpolation;
	__construct_array(evt->mEyeEaseInOut.mAxes, (ConstructorDestructor)fn_8001ACB8, (ConstructorDestructor)fn_800098B4,
	                  sizeof(CInterpolationEaseInOut), 3);
	*(void***)&evt->mEyeEaseOut = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mEyeEaseOut = __vt__27CVectorEaseOutInterpolation;
	__construct_array(evt->mEyeEaseOut.mAxes, (ConstructorDestructor)fn_8001AC80, (ConstructorDestructor)fn_80009910,
	                  sizeof(CInterpolationEaseOut), 3);
	*(void***)&evt->mEyeNone = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mEyeNone = __vt__24CVectorNoneInterpolation;
	__construct_array(evt->mEyeNone.mAxes, (ConstructorDestructor)fn_8001A3A8, (ConstructorDestructor)fn_80009EC4,
	                  sizeof(CInterpolationNone), 3);
	*(void***)&evt->mInterestLinear = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mInterestLinear = __vt__26CVectorLinearInterpolation;
	__construct_array(evt->mInterestLinear.mAxes, (ConstructorDestructor)fn_8001AC9C, (ConstructorDestructor)fn_80009858,
	                  sizeof(CInterpolationLinear), 3);
	*(void***)&evt->mInterestEaseInOut = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mInterestEaseInOut = __vt__29CVectorEaseInOutInterpolation;
	__construct_array(evt->mInterestEaseInOut.mAxes, (ConstructorDestructor)fn_8001ACB8, (ConstructorDestructor)fn_800098B4,
	                  sizeof(CInterpolationEaseInOut), 3);
	*(void***)&evt->mInterestEaseOut = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mInterestEaseOut = __vt__27CVectorEaseOutInterpolation;
	__construct_array(evt->mInterestEaseOut.mAxes, (ConstructorDestructor)fn_8001AC80, (ConstructorDestructor)fn_80009910,
	                  sizeof(CInterpolationEaseOut), 3);
	*(void***)&evt->mInterestNone = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mInterestNone = __vt__24CVectorNoneInterpolation;
	__construct_array(evt->mInterestNone.mAxes, (ConstructorDestructor)fn_8001A3A8, (ConstructorDestructor)fn_80009EC4,
	                  sizeof(CInterpolationNone), 3);
	*(void***)&evt->mFovLinear         = __vt__23CInterfaceInterpolation;
	*(void***)&evt->mFovLinear         = __vt__20CInterpolationLinear;
	*(void***)&evt->mFovEaseInOut      = __vt__23CInterfaceInterpolation;
	*(void***)&evt->mFovEaseInOut      = __vt__23CInterpolationEaseInOut;
	*(void***)&evt->mFovEaseOut        = __vt__23CInterfaceInterpolation;
	*(void***)&evt->mFovEaseOut        = __vt__21CInterpolationEaseOut;
	*(void***)&evt->mFovNone           = __vt__23CInterfaceInterpolation;
	*(void***)&evt->mFovNone           = __vt__18CInterpolationNone;
	*(void***)&evt->mDistanceLinear    = __vt__23CInterfaceInterpolation;
	*(void***)&evt->mDistanceLinear    = __vt__20CInterpolationLinear;
	*(void***)&evt->mDistanceEaseInOut = __vt__23CInterfaceInterpolation;
	*(void***)&evt->mDistanceEaseInOut = __vt__23CInterpolationEaseInOut;
	*(void***)&evt->mDistanceEaseOut   = __vt__23CInterfaceInterpolation;
	*(void***)&evt->mDistanceEaseOut   = __vt__21CInterpolationEaseOut;
	*(void***)&evt->mDistanceNone      = __vt__23CInterfaceInterpolation;
	*(void***)&evt->mDistanceNone      = __vt__18CInterpolationNone;
	*(void***)&evt->mAngleLinear       = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mAngleLinear       = __vt__26CVectorLinearInterpolation;
	__construct_array(evt->mAngleLinear.mAxes, (ConstructorDestructor)fn_8001AC9C, (ConstructorDestructor)fn_80009858,
	                  sizeof(CInterpolationLinear), 3);
	*(void***)&evt->mAngleEaseInOut = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mAngleEaseInOut = __vt__29CVectorEaseInOutInterpolation;
	__construct_array(evt->mAngleEaseInOut.mAxes, (ConstructorDestructor)fn_8001ACB8, (ConstructorDestructor)fn_800098B4,
	                  sizeof(CInterpolationEaseInOut), 3);
	*(void***)&evt->mAngleEaseOut = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mAngleEaseOut = __vt__27CVectorEaseOutInterpolation;
	__construct_array(evt->mAngleEaseOut.mAxes, (ConstructorDestructor)fn_8001AC80, (ConstructorDestructor)fn_80009910,
	                  sizeof(CInterpolationEaseOut), 3);
	*(void***)&evt->mAngleNone = __vt__29CInterfaceVectorInterpolation;
	*(void***)&evt->mAngleNone = __vt__24CVectorNoneInterpolation;
	__construct_array(evt->mAngleNone.mAxes, (ConstructorDestructor)fn_8001A3A8, (ConstructorDestructor)fn_80009EC4,
	                  sizeof(CInterpolationNone), 3);
	return evt;
}
