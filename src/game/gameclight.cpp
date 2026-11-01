#include "game/hsd_jobj.h"
#include "game/clight.h"

void operator delete(void* object);

extern "C" void* __vt__6CLight[];

/**
 * @note Address: 0x800084CC
 * @note Size: 0x13C
 */
// todo: weak copy
extern "C" CLight* __dt__6CLightFv(CLight* light, s32 flags)
{
	if (light != NULL) {
		*(void***)((u8*)light + 0x354) = __vt__6CLight;
		light->mShadow.CShadow::~CShadow();
		light->mSpecColor.CColor::~CColor();
		light->mColor.CColor::~CColor();
		light->_F8.CVec::~CVec();
		light->mInterest.CVec::~CVec();
		light->mPosition.CVec::~CVec();
		if ((s16)flags > 0)
			::operator delete(light);
	}
	return light;
}
