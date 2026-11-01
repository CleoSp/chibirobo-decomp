#include "game/hsd_jobj.h"
#include "game/logo.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__4Logo[];
extern void* __vt__9CScrnFade[];
extern void* __vt__10CScrnColor[];
extern void* __vt__8InfoScrn[];
}

// todo: weak copy
/**
 * @note Address: 0x80018D28
 * @note Size: 0x90
 */
extern "C" Logo* fn_80018D28(Logo* logo, s32 flags)
{
	if (logo != NULL) {
		*(void***)((u8*)logo + 0x200) = __vt__4Logo; // _200 vtable
		logo->mTpl.CTpl::~CTpl();
		if ((s16)flags > 0) {
			::operator delete(logo);
		}
	}
	return logo;
}

// todo: weak copy
/**
 * @note Address: 0x80018DB8
 * @note Size: 0x5C
 */
extern "C" CScrnFade* fn_80018DB8(CScrnFade* fade, s32 flags)
{
	if (fade != NULL) {
		*(void***)((u8*)fade + 0x30) = __vt__9CScrnFade; // _30 vtable
		fade->CScrnColor::~CScrnColor();
		if ((s16)flags > 0) {
			::operator delete(fade);
		}
	}
	return fade;
}

// todo: weak copy
/**
 * @note Address: 0x80018E14
 * @note Size: 0x48
 */
extern "C" CScrnColor* fn_80018E14(CScrnColor* color, s32 flags)
{
	if (color != NULL) {
		*(void***)((u8*)color + 0x30) = __vt__10CScrnColor; // _30 vtable
		if ((s16)flags > 0) {
			::operator delete(color);
		}
	}
	return color;
}

// todo: weak copy
/**
 * @note Address: 0x80018E5C
 * @note Size: 0x98
 */
extern "C" InfoScrn* fn_80018E5C(InfoScrn* info, s32 flags)
{
	if (info != NULL) {
		*(void***)((u8*)info + 0x1D8) = __vt__8InfoScrn; // _1D8 vtable
		info->mWipe.CScrnWipe::~CScrnWipe();
		info->mFade.CScrnFade::~CScrnFade();
		if ((s16)flags > 0) {
			::operator delete(info);
		}
	}
	return info;
}
