#include "game/ccameraaction.h"

void operator delete(void* object);

template <class T>
struct InterpolationVtable;

extern "C" {
extern void* __vt__20CInterpolationLinear[];
extern InterpolationVtable<CInterpolationEaseInOut> __vt__23CInterpolationEaseInOut;
extern InterpolationVtable<CInterpolationEaseOut> __vt__21CInterpolationEaseOut;
extern void* __vt__27CInterpolationEaseOutDivide[];
}

/**
 * @note Address: 0x80009858
 * @note Size: 0x5C
 */
extern "C" CInterpolationLinear* __dt__20CInterpolationLinearFv(CInterpolationLinear* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__20CInterpolationLinear;
		interp->CInterfaceInterpolation::~CInterfaceInterpolation();
		if ((s16)flags > 0)
			::operator delete(interp);
	}
	return interp;
}

/**
 * @note Address: 0x800098B4
 * @note Size: 0x5C
 */
extern "C" CInterpolationEaseInOut* __dt__23CInterpolationEaseInOutFv(CInterpolationEaseInOut* interp, s32 flags)
{
	if (interp != NULL) {
		*(InterpolationVtable<CInterpolationEaseInOut>**)interp = &__vt__23CInterpolationEaseInOut;
		interp->CInterfaceInterpolation::~CInterfaceInterpolation();
		if ((s16)flags > 0)
			::operator delete(interp);
	}
	return interp;
}

/**
 * @note Address: 0x80009910
 * @note Size: 0x5C
 */
extern "C" CInterpolationEaseOut* __dt__21CInterpolationEaseOutFv(CInterpolationEaseOut* interp, s32 flags)
{
	if (interp != NULL) {
		*(InterpolationVtable<CInterpolationEaseOut>**)interp = &__vt__21CInterpolationEaseOut;
		interp->CInterfaceInterpolation::~CInterfaceInterpolation();
		if ((s16)flags > 0)
			::operator delete(interp);
	}
	return interp;
}

/**
 * @note Address: 0x8000996C
 * @note Size: 0x5C
 */
extern "C" CInterpolationEaseOutDivide* __dt__27CInterpolationEaseOutDivideFv(CInterpolationEaseOutDivide* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__27CInterpolationEaseOutDivide;
		interp->CInterfaceInterpolation::~CInterfaceInterpolation();
		if ((s16)flags > 0)
			::operator delete(interp);
	}
	return interp;
}
