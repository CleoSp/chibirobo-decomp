#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/cscrn.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__9CParticle[];
extern void* __vt__10CScrnQuake[];
void* fn_80008D80(void* gen, s32 flags);
void* __dt__5CFileFv(void* file, s32 flags);
void* dtor_80008DC8(void* vec, s32 flags);
}

// todo: weak copy
/**
 * @note Address: 0x80018EF4
 * @note Size: 0x88
 */
extern "C" CParticle* fn_80018EF4(CParticle* particle, s32 flags)
{
	if (particle != NULL) {
		*(void***)((u8*)particle + 0x544) = __vt__9CParticle; // _544 vtable
		__destroy_arr(particle->mGenerators, (ConstructorDestructor*)fn_80008D80, sizeof(CGenerator), CPARTICLE_GEN_NUM);
		__destroy_arr(particle->mFile, (ConstructorDestructor*)__dt__5CFileFv, sizeof(CFile), CPARTICLE_FILE_NUM);
		if ((s16)flags > 0) {
			::operator delete(particle);
		}
	}
	return particle;
}

// todo: weak copy
/**
 * @note Address: 0x80018F7C
 * @note Size: 0x6C
 */
extern "C" CScrnQuake* fn_80018F7C(CScrnQuake* quake, s32 flags)
{
	if (quake != NULL) {
		*(void***)((u8*)quake + 0x34) = __vt__10CScrnQuake; // _34 vtable
		__destroy_arr(&quake->mAmount, (ConstructorDestructor*)dtor_80008DC8, sizeof(CVec), 3);
		if ((s16)flags > 0) {
			::operator delete(quake);
		}
	}
	return quake;
}
