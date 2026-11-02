#include "game/xobjmap.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__9XObjLight[];
void* __dt__6CLightFv(void* object, s32 flags);
}

// todo: weak copy
/**
 * @note Address: 0x80092DBC
 * @note Size: 0x7C
 */
extern "C" XObjLight* __dt__9XObjLightFv(XObjLight* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__9XObjLight;
		__destroy_arr(object->mLights, (ConstructorDestructor*)__dt__6CLightFv, sizeof(CLight), 8);
		__dt__4XObjFv(object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
