#include "game/menulistentry.h"

/**
 * @note Address: 0x800345A8
 * @note Size: 0xC4
 */
extern "C" s32 fn_800345A8()
{
	s32 count = 0;
	for (s32 i = 0; i < 20; ++i) {
		s16 item  = lbl_80366DB4[i].mItem;
		s16 value = lbl_80366DB4[i]._02;
		if (item >= 0 && value >= 0)
			++count;
	}
	return count;
}
