#include <sysdolphin/util.h>
#include <functions.h>

/**
 * @note Address: 0x801D04B8
 * @note Size: 0x6C
 */
void HSD_MulColor(GXColor* arg0, GXColor* arg1, GXColor* dest)
{
	dest->r = (arg0->r * arg1->r) / 255U;
	dest->g = (arg0->g * arg1->g) / 255U;
	dest->b = (arg0->b * arg1->b) / 255U;
	dest->a = (arg0->a * arg1->a) / 255U;
}

/**
 * @note Address: 0x801D0524
 * @note Size: 0xC0
 */
s32 HSD_GetNbBits(u32 c)
{
	int ret = 0;
	int i;

	for (i = 0; i < 32; i++) {
		if ((c) & (1 << i)) {
			ret++;
		}
	}
	return ret;
}

/**
 * @note Address: 0x801D05E4
 * @note Size: 0xA0
 */
s32 HSD_Index2PosNrmMtx(u32 arg0)
{
	switch (arg0) {
	case 0:
		return 0;
	case 1:
		return 3;
	case 2:
		return 6;
	case 3:
		return 9;
	case 4:
		return 12;
	case 5:
		return 15;
	case 6:
		return 18;
	case 7:
		return 21;
	case 8:
		return 24;
	case 9:
		return 27;
	default:
		assert_line(132, 0);
		return 0;
	}
}
