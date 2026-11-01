#include "types.h"

// state fn_8001BCFC draws from, role unknown so far
struct SysWork {
	s32 _00; // _00, fn_8001BCFC checks it against 10
	s32 _04; // _04
	f32 _08; // _08
	f32 _0C; // _0C
	s32 _10; // _10
	s32 _14; // _14
};
STATIC_ASSERT(sizeof(SysWork) == 0x18);

extern "C" SysWork lbl_80210730;

/**
 * @note Address: 0x8001C89C
 * @note Size: 0x28
 */
extern "C" void fn_8001C89C()
{
	lbl_80210730._00 = 0;
	lbl_80210730._04 = -1;
	lbl_80210730._08 = 0.0f;
	lbl_80210730._10 = 0;
	lbl_80210730._14 = 0;
}
