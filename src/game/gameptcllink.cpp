#include "game/cparticle.h"

/**
 * @note Address: 0x8001364C
 * @note Size: 0x34
 */
// the particle jobj link callback, kind 0x48 on link 0 goes to link 1
extern "C" HSD_Generator* fn_8001364C(int linkNo, int bank, int kind, HSD_JObj* jobj)
{
	if (linkNo == 0 && kind == 0x48)
		linkNo = 1;
	return fn_801E4380(linkNo, bank, kind, jobj);
}

/**
 * @note Address: 0x80013680
 * @note Size: 0x34
 */
// the same for the sound link
extern "C" HSD_Generator* fn_80013680(int linkNo, int bank, int kind, HSD_JObj* jobj)
{
	if (linkNo == 0 && kind == 0x48)
		linkNo = 1;
	return fn_801E4C58(linkNo, bank, kind, jobj);
}
