#include "game/xobjmap.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__7XObjMap[];
void* __dt__6CLightFv(void* object, s32 flags);
void* fn_800931C8(void* object, s32 flags);
}

// todo: weak copy
/**
 * @note Address: 0x80093134
 * @note Size: 0x94
 */
extern "C" XObjMap* fn_80093134(XObjMap* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__7XObjMap;
		__destroy_arr(object->mLights, (ConstructorDestructor*)__dt__6CLightFv, sizeof(CLight), 8);
		__destroy_arr(object->mTrigon, (ConstructorDestructor*)fn_800931C8, sizeof(CTrigon), 16);
		fn_8000877C(object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
