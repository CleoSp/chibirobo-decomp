#include "game/ccameraaction.h"

void operator delete(void* object);

extern "C" void* __vt__17CCameraActionArea[];

// todo: cleanup, CCameraActionArea only lives in ccameraactionarea.cpp
struct CCameraActionAreaDtor : CCameraActionAreaExM {
	u8 _40[0x70 - 0x40];                           // _40
	CVectorEaseInOutInterpolation _70;             // _70
	CVectorLinearInterpolation _CC;                // _CC
	CVectorEaseInOutInterpolation _100;            // _100
	CVectorEaseOutInterpolation _15C;              // _15C
	CVectorEaseOutDivideInterpolation _1C0;        // _1C0
	CInterfaceVectorInterpolation* mEyeInterps[5]; // _1F8
	CInterpolationLinear _20C;                     // _20C
	CInterpolationEaseInOut _21C;                  // _21C
	CInterpolationEaseOut _238;                    // _238
	CInterpolationEaseOutDivide _258;              // _258
};
STATIC_ASSERT(sizeof(CCameraActionAreaDtor) == 0x268);

/**
 * @note Address: 0x800095C0
 * @note Size: 0x298
 */
// todo: weak copy
extern "C" CCameraActionAreaDtor* __dt__17CCameraActionAreaFv(CCameraActionAreaDtor* area, s32 flags)
{
	if (area != NULL) {
		*(void***)((u8*)area + 0x3C) = __vt__17CCameraActionArea;
		area->_258.CInterpolationEaseOutDivide::~CInterpolationEaseOutDivide();
		area->_238.CInterpolationEaseOut::~CInterpolationEaseOut();
		area->_21C.CInterpolationEaseInOut::~CInterpolationEaseInOut();
		area->_20C.CInterpolationLinear::~CInterpolationLinear();
		area->_1C0.CVectorEaseOutDivideInterpolation::~CVectorEaseOutDivideInterpolation();
		area->_15C.CVectorEaseOutInterpolation::~CVectorEaseOutInterpolation();
		area->_100.CVectorEaseInOutInterpolation::~CVectorEaseInOutInterpolation();
		area->_CC.CVectorLinearInterpolation::~CVectorLinearInterpolation();
		area->_70.CVectorEaseInOutInterpolation::~CVectorEaseInOutInterpolation();
		area->CCameraActionAreaExM::~CCameraActionAreaExM();
		if ((s16)flags > 0)
			::operator delete(area);
	}
	return area;
}
