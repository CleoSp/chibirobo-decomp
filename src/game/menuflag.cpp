#include "types.h"
#include "game/globals.h"
#include "game/evt.h"

// whether the event flag the globals file's entry 3 names is set
/**
 * @note Address: 0x80051C60
 * @note Size: 0x5C
 */
extern "C" BOOL fn_80051C60()
{
	BOOL set = FALSE;
	u32 bit  = fn_8006DCFC(3);
	if (lbl_8036781C[bit / 32] & (1U << (bit & 31)))
		set = TRUE;
	return set;
}
