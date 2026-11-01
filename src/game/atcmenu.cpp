#include "game/xchr.h"
#include <math.h>
#include "game/globals.h"

// the globals label and icon ids are 2 lower in japan
#if defined(VERSION_JP)
#define ATC_LABEL(id) ((id) - 2)
#else
#define ATC_LABEL(id) (id)
#endif

// AtcMenu::vf28, the item's color, pulsing while selected
/**
 * @note Address: 0x80098A3C
 * @note Size: 0x1E4
 */
extern "C" u32 fn_80098A3C(AtcMenu* menu, s32 index)
{
	s32 red, green, blue;
	if (menu->mItems[index].mAvailable == 0) {
		blue  = 32;
		green = 32;
		red   = 32;
	} else if (menu->_280 == 3) {
		if (menu->mChr->GetAttachment() == menu->mItems[index].mType) {
			red   = 50;
			green = 200;
			blue  = 200;
		} else {
			red   = 50;
			green = 50;
			blue  = 200;
		}
	} else if (index == menu->_04 && menu->mChr->GetAttachment() == menu->mItems[index].mType) {
		red   = 200;
		green = 100;
		blue  = 150;
	} else if (index == menu->_04) {
		red   = 200;
		green = 100;
		blue  = 100;
	} else if (menu->mChr->GetAttachment() == menu->mItems[index].mType) {
		red   = 50;
		green = 200;
		blue  = 200;
	} else {
		red   = 50;
		green = 50;
		blue  = 200;
	}
	if (index == menu->_04) {
		red += (s32)(25.0f * (f32)sin(1.2f * menu->mItems[index]._08) + 25.0f);
		green += (s32)(25.0f * (f32)sin(1.2f * menu->mItems[index]._08) + 25.0f);
		blue += (s32)(25.0f * (f32)sin(1.2f * menu->mItems[index]._08) + 25.0f);
	}
	return ((u32)(u8)red << 24) | ((u32)(u8)green << 16) | ((u32)(u8)blue << 8);
}

// AtcMenu::vf24, the item's label. the last one stays up while a switch is pending or an
// action that holds the attachment is running
/**
 * @note Address: 0x80098C20
 * @note Size: 0xCC
 */
extern "C" const char* fn_80098C20(AtcMenu* menu, s32 index)
{
	static const char* sLabel = "";
	if (menu->mItems[index].mAvailable > 0) {
		XObjChr* chr = menu->mChr;
		if (chr->mAttachmentRequest < 0 && chr->mLift.mState != 20 && chr->mLift.mState != 21 && chr->mLift.mState != 22) {
			if (menu->mItems[index].mType == chr->GetAttachment())
				sLabel = fn_8006DBF0(ATC_LABEL(12));
			else
				sLabel = (const char*)fn_8006E160(menu->mItems[index].mType);
		}
	} else {
		sLabel = fn_8006DBF0(ATC_LABEL(14));
	}
	return sLabel;
}

// AtcMenu::vf20, the item's icon
/**
 * @note Address: 0x80098CEC
 * @note Size: 0xB0
 */
extern "C" s32 fn_80098CEC(AtcMenu* menu, s32 index)
{
	s32 icons[9] = { ATC_LABEL(126), ATC_LABEL(91), ATC_LABEL(92), ATC_LABEL(93), ATC_LABEL(94),
		             ATC_LABEL(96),  ATC_LABEL(95), ATC_LABEL(97), ATC_LABEL(126) };
	if (index == -1)
		return ATC_LABEL(106);
	if (index == -2)
		return ATC_LABEL(108);
	if (menu->mItems[index].mAvailable > 0)
		return icons[menu->mItems[index].mType];
	return ATC_LABEL(105);
}

// AtcMenu::vf2C
/**
 * @note Address: 0x80098D9C
 * @note Size: 0x38
 */
extern "C" u8 fn_80098D9C(AtcMenu* menu, s32 index)
{
	return menu->mChr->GetAttachment() == menu->mItems[index].mType;
}

// AtcMenu::vf1C, switches to the selected attachment, or puts the current one away
/**
 * @note Address: 0x80098DD4
 * @note Size: 0x74
 */
extern "C" void fn_80098DD4(AtcMenu* menu)
{
	XObjChr* chr = menu->mChr;
	s32 current  = chr->GetAttachment();
	s32 type     = menu->mItems[menu->_04].mType;
	if (type == current) {
		if (chr->mAttachment != 0) {
			chr->mAttachmentRequest = 0;
			chr->_95D8              = 0;
		}
	} else {
		if (type != chr->mAttachment) {
			chr->mAttachmentRequest = type;
			chr->_95D8              = 0;
		}
	}
}
