#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/ut.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__10CGenerator[];
extern void* __vt__4CVec[];
}

/**
 * @note Address: 0x80008D80
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" CGenerator* __dt__10CGeneratorFv(CGenerator* generator, s32 flags)
{
	if (generator != NULL) {
		*(void***)((u8*)generator + 0x8) = __vt__10CGenerator;
		if ((s16)flags > 0)
			::operator delete(generator);
	}
	return generator;
}

/**
 * @note Address: 0x80008DC8
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" CVec* __dt__4CVecFv(CVec* vec, s32 flags)
{
	if (vec != NULL) {
		*(void***)((u8*)vec + 0xC) = __vt__4CVec;
		if ((s16)flags > 0)
			::operator delete(vec);
	}
	return vec;
}
