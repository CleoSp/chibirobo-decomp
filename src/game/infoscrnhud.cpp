#include "game/infoscrn.h"

// mode 0 slides it in or out, mode 1 the same without the slide
static inline void SetSlide(s32& state, u8 show, s32 mode)
{
	switch (mode) {
	case 0:
		if (show)
			state = 1;
		else
			state = 2;
		break;
	case 1:
		if (show)
			state = 3;
		else
			state = 4;
		break;
	}
}

/**
 * @note Address: 0x800660E0
 * @note Size: 0x4
 */
extern "C" void fn_800660E0(InfoScrn* scrn, s32 level)
{
}
/**
 * @note Address: 0x800660E4
 * @note Size: 0x4
 */
extern "C" void fn_800660E4(InfoScrn* scrn, s32 exp)
{
}

/**
 * @note Address: 0x800660E8
 * @note Size: 0x54
 */
extern "C" void fn_800660E8(InfoScrn* scrn, u8 show, s32 mode)
{
	SetSlide(scrn->_1C4, show, mode);
}

/**
 * @note Address: 0x8006613C
 * @note Size: 0x54
 */
extern "C" void fn_8006613C(InfoScrn* scrn, u8 show, s32 mode)
{
	SetSlide(scrn->_1B4, show, mode);
}

/**
 * @note Address: 0x80066190
 * @note Size: 0x54
 */
extern "C" void fn_80066190(InfoScrn* scrn, u8 show, s32 mode)
{
	SetSlide(scrn->_1A0, show, mode);
}

/**
 * @note Address: 0x800661E4
 * @note Size: 0x6C
 */
extern "C" void fn_800661E4(InfoScrn* scrn, s32 value, u8 show, s32 mode)
{
	scrn->_19C = 0;
	switch (mode) {
	case 0:
		if (show) {
			scrn->_188 = 1;
			scrn->_19C = value;
		} else {
			scrn->_188 = 2;
			scrn->_19C = 0;
		}
		break;
	case 1:
		if (show) {
			scrn->_188 = 3;
			scrn->_19C = value;
		} else {
			scrn->_188 = 4;
			scrn->_19C = 0;
		}
		break;
	}
}
