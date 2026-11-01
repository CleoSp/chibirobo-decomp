#include "game/cmotioncontrolcamera.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__23CInterfaceInterpolation[];
extern void* __vt__14CCameraMemento[];
extern void* __vt__8CMsgData[];
}

// todo: weak copy
/**
 * @note Address: 0x80016F54
 * @note Size: 0x48
 */
extern "C" CInterfaceInterpolation* __dt__23CInterfaceInterpolationFv(CInterfaceInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__23CInterfaceInterpolation;
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}

// todo: weak copy
/**
 * @note Address: 0x80016F9C
 * @note Size: 0x88
 */
extern "C" CCameraMemento* __dt__14CCameraMementoFv(CCameraMemento* memento, s32 flags)
{
	if (memento != NULL) {
		*(void***)memento = __vt__14CCameraMemento;
		memento->mAngle.CVec::~CVec();
		memento->mInterest.CVec::~CVec();
		memento->mEye.CVec::~CVec();
		if ((s16)flags > 0) {
			::operator delete(memento);
		}
	}
	return memento;
}

// todo: weak copy
// CMsgData lives in msg.cpp for now, its vtable is the last word
/**
 * @note Address: 0x80017024
 * @note Size: 0x48
 */
extern "C" void* __dt__8CMsgDataFv(void* data, s32 flags)
{
	if (data != NULL) {
#if defined(VERSION_JP)
		*(void***)((u8*)data + 0x894) = __vt__8CMsgData;
#else
		*(void***)((u8*)data + 0x898) = __vt__8CMsgData;
#endif
		if ((s16)flags > 0) {
			::operator delete(data);
		}
	}
	return data;
}
