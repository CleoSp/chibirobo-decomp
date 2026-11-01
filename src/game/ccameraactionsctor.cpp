#include "game/hsd_jobj.h"
#include "game/cmotioncontrolcamera.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

inline void* operator new(size_t, void* storage)
{
	return storage;
}

extern "C" {
extern void* __vt__14CCameraActions[];
extern void* __vt__16CCameraActionEye[];
extern void* __vt__17CCameraActionAnim[];
extern void* __vt__17CCameraActionArea[];
extern void* __vt__17CCameraActionLadr[];
extern void* __vt__18CCameraActionChase[];
extern void* __vt__18CCameraActionReset[];
extern void* __vt__18CInterpolationNone[];
extern void* __vt__19CCameraActionScroll[];
extern void* __vt__20CCameraActionAreaExM[];
extern void* __vt__20CCameraActionDefault[];
extern void* __vt__20CCameraActionScroll2[];
extern void* __vt__20CInterpolationLinear[];
extern void* __vt__21CInterpolationEaseOut[];
extern void* __vt__22CInterfaceCameraAction[];
extern void* __vt__23CInterfaceInterpolation[];
extern void* __vt__23CInterpolationEaseInOut[];
extern void* __vt__24CVectorNoneInterpolation[];
extern void* __vt__26CVectorLinearInterpolation[];
extern void* __vt__27CInterpolationEaseOutDivide[];
extern void* __vt__27CVectorEaseOutInterpolation[];
extern void* __vt__29CInterfaceVectorInterpolation[];
extern void* __vt__29CVectorEaseInOutInterpolation[];
extern void* __vt__33CVectorEaseOutDivideInterpolation[];
extern void* __vt__4CVec[];
extern void* __vt__7CObjVec[];
void* fn_80009858(void* interp, s32 flags);
void* fn_800098B4(void* interp, s32 flags);
void* fn_80009910(void* interp, s32 flags);
void* fn_8000996C(void* interp, s32 flags);
void* fn_80009EC4(void* interp, s32 flags);
CCameraActionEvt* fn_8001A81C(CCameraActionEvt* evt);
}

// todo: weak copy, mArea and mEye are still untyped
/**
 * @note Address: 0x80019E5C
 * @note Size: 0x488
 */
extern "C" CCameraActions* fn_80019E5C(CCameraActions* actions)
{
	u8* area                                            = actions->mArea;
	CCameraActionReset* reset                           = &actions->mReset;
	*(void***)((u8*)actions + 0x1E70)                   = __vt__14CCameraActions; // _1E70 vtable
	*(void***)((u8*)&actions->mDefault + 0x3C)          = __vt__22CInterfaceCameraAction;
	*(void***)((u8*)&actions->mDefault.mEye + 0xC)      = __vt__4CVec;
	*(void***)((u8*)&actions->mDefault.mInterest + 0xC) = __vt__4CVec;
	*(void***)((u8*)&actions->mDefault.mAngle + 0xC)    = __vt__4CVec;
	*(void***)((u8*)&actions->mDefault + 0x3C)          = __vt__20CCameraActionDefault;
	*(void***)(area + 0x3C)                             = __vt__22CInterfaceCameraAction;
	*(void***)(area + 0xC)                              = __vt__4CVec;
	*(void***)(area + 0x1C)                             = __vt__4CVec;
	*(void***)(area + 0x2C)                             = __vt__4CVec;
	*(void***)(area + 0x3C)                             = __vt__20CCameraActionAreaExM;
	*(void***)(area + 0x3C)                             = __vt__17CCameraActionArea;
	*(void***)(area + 0x70)                             = __vt__29CInterfaceVectorInterpolation;
	*(void***)(area + 0x70)                             = __vt__29CVectorEaseInOutInterpolation;
	__construct_array(area + 0x74, (ConstructorDestructor)fn_8001ACB8, (ConstructorDestructor)fn_800098B4, 0x1C, 3);
	*(void***)(area + 0xCC) = __vt__29CInterfaceVectorInterpolation;
	*(void***)(area + 0xCC) = __vt__26CVectorLinearInterpolation;
	__construct_array(area + 0xD0, (ConstructorDestructor)fn_8001AC9C, (ConstructorDestructor)fn_80009858, 0x10, 3);
	*(void***)(area + 0x100) = __vt__29CInterfaceVectorInterpolation;
	*(void***)(area + 0x100) = __vt__29CVectorEaseInOutInterpolation;
	__construct_array(area + 0x104, (ConstructorDestructor)fn_8001ACB8, (ConstructorDestructor)fn_800098B4, 0x1C, 3);
	*(void***)(area + 0x15C) = __vt__29CInterfaceVectorInterpolation;
	*(void***)(area + 0x15C) = __vt__27CVectorEaseOutInterpolation;
	__construct_array(area + 0x160, (ConstructorDestructor)fn_8001AC80, (ConstructorDestructor)fn_80009910, 0x20, 3);
	*(void***)(area + 0x1C0) = __vt__29CInterfaceVectorInterpolation;
	*(void***)(area + 0x1C0) = __vt__33CVectorEaseOutDivideInterpolation;
	__construct_array(area + 0x1C4, (ConstructorDestructor)fn_8001AC64, (ConstructorDestructor)fn_8000996C, 0x10, 3);
	*(void***)(area + 0x20C)                          = __vt__23CInterfaceInterpolation;
	*(void***)(area + 0x20C)                          = __vt__20CInterpolationLinear;
	*(void***)(area + 0x21C)                          = __vt__23CInterfaceInterpolation;
	*(void***)(area + 0x21C)                          = __vt__23CInterpolationEaseInOut;
	*(void***)(area + 0x238)                          = __vt__23CInterfaceInterpolation;
	*(void***)(area + 0x238)                          = __vt__21CInterpolationEaseOut;
	*(void***)(area + 0x258)                          = __vt__23CInterfaceInterpolation;
	*(void***)(area + 0x258)                          = __vt__27CInterpolationEaseOutDivide;
	*(void***)((u8*)&actions->mChase + 0x3C)          = __vt__22CInterfaceCameraAction;
	*(void***)((u8*)&actions->mChase.mEye + 0xC)      = __vt__4CVec;
	*(void***)((u8*)&actions->mChase.mInterest + 0xC) = __vt__4CVec;
	*(void***)((u8*)&actions->mChase.mAngle + 0xC)    = __vt__4CVec;
	*(void***)((u8*)&actions->mChase + 0x3C)          = __vt__18CCameraActionChase;
	*(void***)((u8*)&actions->mChase.mLastPos + 0xC)  = __vt__4CVec;
	*(void***)((u8*)&actions->mChase.mPos + 0xC)      = __vt__4CVec;
	fn_8001A81C(&actions->mEvt);
	*(void***)((u8*)&actions->mScroll2 + 0x3C)          = __vt__22CInterfaceCameraAction;
	*(void***)((u8*)&actions->mScroll2.mEye + 0xC)      = __vt__4CVec;
	*(void***)((u8*)&actions->mScroll2.mInterest + 0xC) = __vt__4CVec;
	*(void***)((u8*)&actions->mScroll2.mAngle + 0xC)    = __vt__4CVec;
	*(void***)((u8*)&actions->mScroll2 + 0x3C)          = __vt__20CCameraActionScroll2;
	*(void***)(actions->mEye + 0x3C)                    = __vt__22CInterfaceCameraAction;
	*(void***)(actions->mEye + 0xC)                     = __vt__4CVec;
	*(void***)(actions->mEye + 0x1C)                    = __vt__4CVec;
	*(void***)(actions->mEye + 0x2C)                    = __vt__4CVec;
	*(void***)(actions->mEye + 0x3C)                    = __vt__16CCameraActionEye;
	*(void***)(actions->mEye + 0x4C)                    = __vt__4CVec;
	*(void***)(actions->mEye + 0x4C)                    = __vt__7CObjVec;
	*(void***)(actions->mEye + 0x5C)                    = __vt__4CVec;
	*(void***)(actions->mEye + 0x6C)                    = __vt__4CVec;
	*(void***)(actions->mEye + 0x7C)                    = __vt__4CVec;
	*(void***)((u8*)&actions->mAnim + 0x3C)             = __vt__22CInterfaceCameraAction;
	*(void***)((u8*)&actions->mAnim.mEye + 0xC)         = __vt__4CVec;
	*(void***)((u8*)&actions->mAnim.mInterest + 0xC)    = __vt__4CVec;
	*(void***)((u8*)&actions->mAnim.mAngle + 0xC)       = __vt__4CVec;
	*(void***)((u8*)&actions->mAnim + 0x3C)             = __vt__17CCameraActionAnim;
	new (&actions->mMap) CCameraActionMap;
	*(void***)((u8*)&actions->mLadr + 0x3C)          = __vt__22CInterfaceCameraAction;
	*(void***)((u8*)&actions->mLadr.mEye + 0xC)      = __vt__4CVec;
	*(void***)((u8*)&actions->mLadr.mInterest + 0xC) = __vt__4CVec;
	*(void***)((u8*)&actions->mLadr.mAngle + 0xC)    = __vt__4CVec;
	*(void***)((u8*)&actions->mLadr + 0x3C)          = __vt__17CCameraActionLadr;
	*(void***)((u8*)reset + 0x3C)                    = __vt__22CInterfaceCameraAction;
	*(void***)((u8*)&reset->mEye + 0xC)              = __vt__4CVec;
	*(void***)((u8*)&reset->mInterest + 0xC)         = __vt__4CVec;
	*(void***)((u8*)&reset->mAngle + 0xC)            = __vt__4CVec;
	*(void***)((u8*)reset + 0x3C)                    = __vt__20CCameraActionAreaExM;
	*(void***)((u8*)reset + 0x3C)                    = __vt__18CCameraActionReset;
	*(void***)&reset->mEyeLinear                     = __vt__29CInterfaceVectorInterpolation;
	*(void***)&reset->mEyeLinear                     = __vt__26CVectorLinearInterpolation;
	__construct_array((u8*)&reset->mEyeLinear + 0x4, (ConstructorDestructor)fn_8001AC9C, (ConstructorDestructor)fn_80009858, 0x10, 3);
	*(void***)&reset->mEyeEaseInOut = __vt__29CInterfaceVectorInterpolation;
	*(void***)&reset->mEyeEaseInOut = __vt__29CVectorEaseInOutInterpolation;
	__construct_array((u8*)&reset->mEyeEaseInOut + 0x4, (ConstructorDestructor)fn_8001ACB8, (ConstructorDestructor)fn_800098B4, 0x1C, 3);
	*(void***)&reset->mEyeEaseOut = __vt__29CInterfaceVectorInterpolation;
	*(void***)&reset->mEyeEaseOut = __vt__27CVectorEaseOutInterpolation;
	__construct_array((u8*)&reset->mEyeEaseOut + 0x4, (ConstructorDestructor)fn_8001AC80, (ConstructorDestructor)fn_80009910, 0x20, 3);
	*(void***)&reset->mEyeNone = __vt__29CInterfaceVectorInterpolation;
	*(void***)&reset->mEyeNone = __vt__24CVectorNoneInterpolation;
	__construct_array((u8*)&reset->mEyeNone + 0x4, (ConstructorDestructor)fn_8001A3A8, (ConstructorDestructor)fn_80009EC4, 0x10, 3);
	*(void***)&reset->mFovLinear                       = __vt__23CInterfaceInterpolation;
	*(void***)&reset->mFovLinear                       = __vt__20CInterpolationLinear;
	*(void***)&reset->mFovEaseInOut                    = __vt__23CInterfaceInterpolation;
	*(void***)&reset->mFovEaseInOut                    = __vt__23CInterpolationEaseInOut;
	*(void***)&reset->mFovEaseOut                      = __vt__23CInterfaceInterpolation;
	*(void***)&reset->mFovEaseOut                      = __vt__21CInterpolationEaseOut;
	*(void***)&reset->mFovNone                         = __vt__23CInterfaceInterpolation;
	*(void***)&reset->mFovNone                         = __vt__18CInterpolationNone;
	*(void***)((u8*)&actions->mScroll + 0x3C)          = __vt__22CInterfaceCameraAction;
	*(void***)((u8*)&actions->mScroll.mEye + 0xC)      = __vt__4CVec;
	*(void***)((u8*)&actions->mScroll.mInterest + 0xC) = __vt__4CVec;
	*(void***)((u8*)&actions->mScroll.mAngle + 0xC)    = __vt__4CVec;
	*(void***)((u8*)&actions->mScroll + 0x3C)          = __vt__19CCameraActionScroll;
	*(void***)&actions->mScroll.mDistanceEase          = __vt__23CInterfaceInterpolation;
	*(void***)&actions->mScroll.mDistanceEase          = __vt__27CInterpolationEaseOutDivide;
	return actions;
}
