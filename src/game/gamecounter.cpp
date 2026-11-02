#include <types.h>

/**
 * @note Address: 0x80012A7C
 * @note Size: 0x10
 */
// hands out the next number from a counter, the first argument is unused
extern "C" u32 fn_80012A7C(void* unused, u32* counter)
{
	return (*counter)++;
}
