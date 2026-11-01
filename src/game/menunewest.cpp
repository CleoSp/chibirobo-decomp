#include "types.h"

// which of the three is newest, the first wins a tie
/**
 * @note Address: 0x80035184
 * @note Size: 0x70
 */
extern "C" s32 fn_80035184(s64 first, s64 second, s64 third)
{
	if (first >= second) {
		if (first >= third)
			return 0;
		return 2;
	}
	if (second >= third)
		return 1;
	return 2;
}
