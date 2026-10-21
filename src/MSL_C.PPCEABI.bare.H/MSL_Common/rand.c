#include "PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h"

static u32 next = 1;

/**
 * @note Address: 0x80158A14
 * @note Size: 0x20
 */
int rand()
{
	next = next * 1103515245 + 12345;
	return ((next >> 16) & 0x7fff);
}

/**
 * @note Address: 0x80158A0C
 * @note Size: 0x8
 */
void srand(u32 seed)
{
	next = seed;
}
