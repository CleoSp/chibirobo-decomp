#include "types.h"
#include "Version.h"

extern "C" {
extern const char sTypeNameCInterpolationEaseOut[]       = "CInterpolationEaseOut";
extern const char sEaseTypeNameCInterfaceInterpolation[] = "CInterfaceInterpolation";
extern const char sTypeNameCInterpolationEaseInOut[]     = "CInterpolationEaseInOut";
extern const u32 sAreaCameraDebugColors[] = { 0xFF000064, 0x00FF0064, 0x0000FF64, 0xFFFF0064, 0xFF00FF64, 0xFFFFFF64, 0xFF960064 };
extern const u32 sMapCameraDebugColors[]  = { 0xFF000064, 0x00FF0064, 0x0000FF64, 0xFFFF0064, 0xFF00FF64, 0xFFFFFF64, 0xFF960064 };
#if !defined(VERSION_PAL)
extern const char sCameraAreaDataAddressFormat[] = "CCameraActionAreaEx :: カメラデータのアドレスは0x%xです。\n";
extern const char sCameraAreaZoomResetMessage[]  = "reset zoom lv = CAM_AREA_ZOOM_LV1\n";
extern const char sAreaCameraDebugIndexFormat[]  = "DEBUG:CCameraActionAreaEx::DrawDebug3d():%d\n";
#endif
extern const f32 sCameraAreaEdgeZTolerance = 0.01f;
extern const char sTypeNameCamInfo[]       = "CamInfo";
}

struct InterpolationBaseList;

struct InterpolationTypeInfo {
	const char* name;
	const InterpolationBaseList* baseTypes;
};

struct InterpolationBaseList {
	const InterpolationTypeInfo* base;
	s32 offset;
	const InterpolationTypeInfo* end;
};

template <class T>
struct InterpolationVtable {
	const InterpolationTypeInfo* typeInfo;
	s32 offset;
	T* (*destroy)(T*, s32);
	bool (*start)(T*, f32, f32, f32);
	bool (*step)(T*, f32);
	bool (*update)(T*, u32, f32);
	f32 (*get)(T*);
};

class CInterpolationEaseOut;
class CInterpolationEaseInOut;

extern "C" {
extern InterpolationBaseList sBaseTypesCInterpolationEaseOut;
extern InterpolationBaseList sBaseTypesCInterpolationEaseInOut;
InterpolationTypeInfo gEaseTypeInfoCInterfaceInterpolation = { sEaseTypeNameCInterfaceInterpolation, NULL };
InterpolationTypeInfo gTypeInfoCInterpolationEaseOut       = { sTypeNameCInterpolationEaseOut, &sBaseTypesCInterpolationEaseOut };
InterpolationTypeInfo gTypeInfoCInterpolationEaseInOut     = { sTypeNameCInterpolationEaseInOut, &sBaseTypesCInterpolationEaseInOut };

CInterpolationEaseOut* __dt__21CInterpolationEaseOutFv(CInterpolationEaseOut*, s32);
bool Start__21CInterpolationEaseOutFfff(CInterpolationEaseOut*, f32, f32, f32);
bool Step__21CInterpolationEaseOutFf(CInterpolationEaseOut*, f32);
bool Update__21CInterpolationEaseOutFUlf(CInterpolationEaseOut*, u32, f32);
f32 Get__21CInterpolationEaseOutFv(CInterpolationEaseOut*);
InterpolationBaseList sBaseTypesCInterpolationEaseOut = { &gEaseTypeInfoCInterfaceInterpolation, 0, NULL };
InterpolationVtable<CInterpolationEaseOut> __vt__21CInterpolationEaseOut
    = { &gTypeInfoCInterpolationEaseOut, 0,
	    __dt__21CInterpolationEaseOutFv, Start__21CInterpolationEaseOutFfff,
	    Step__21CInterpolationEaseOutFf, Update__21CInterpolationEaseOutFUlf,
	    Get__21CInterpolationEaseOutFv };

CInterpolationEaseInOut* __dt__23CInterpolationEaseInOutFv(CInterpolationEaseInOut*, s32);
bool Start__23CInterpolationEaseInOutFfff(CInterpolationEaseInOut*, f32, f32, f32);
bool Step__23CInterpolationEaseInOutFf(CInterpolationEaseInOut*, f32);
bool Update__23CInterpolationEaseInOutFUlf(CInterpolationEaseInOut*, u32, f32);
f32 Get__23CInterpolationEaseInOutFv(CInterpolationEaseInOut*);
InterpolationBaseList sBaseTypesCInterpolationEaseInOut = { &gEaseTypeInfoCInterfaceInterpolation, 0, NULL };
InterpolationVtable<CInterpolationEaseInOut> __vt__23CInterpolationEaseInOut
    = { &gTypeInfoCInterpolationEaseInOut, 0,
	    __dt__23CInterpolationEaseInOutFv, Start__23CInterpolationEaseInOutFfff,
	    Step__23CInterpolationEaseInOutFf, Update__23CInterpolationEaseInOutFUlf,
	    Get__23CInterpolationEaseInOutFv };
}
