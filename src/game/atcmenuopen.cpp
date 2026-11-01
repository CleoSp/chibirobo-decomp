#include "game/xchr.h"
#include <stdlib.h>
#include "game/gmath.h"

extern "C" s32 fn_800991FC(XObjChr* chr, s32 type);

// AtcMenu::Open, OctaMenu's plus the item counts and the start item
/**
 * @note Address: 0x80098E48
 * @note Size: 0x3B4
 */
extern "C" void fn_80098E48(AtcMenu* menu, s32 focus)
{
	for (s32 i = 0; i < 8; ++i) {
		if (menu->_280 == 2)
			menu->mItems[i].mAvailable = 1;
		else
			menu->mItems[i].mAvailable = fn_800991FC(menu->mChr, menu->mItems[i].mType);
	}
	if (menu->mOpen == 0) {
		menu->_16C      = menu->mChr->mMoveRotate.y;
		CJointRot* neck = &menu->mChr->mJointRot[1];
		neck->mMode     = 1;
		neck->mTarget   = 0.0f;
		neck->mAngle    = 0.0f;
		CJointRot* head = &menu->mChr->mJointRot[2];
		head->mMode     = 1;
		head->mTarget   = -1.52f;
		head->mAngle    = -1.52f;
	}
	menu->mOpen  = 1;
	menu->mFocus = focus;
	if (menu->mFocus != 0)
		menu->_0C = 1.0f;
	Vec offset = { 0.0f, 8.8f, 0.0f };
	menu->mChr->vf10C(&offset, &menu->_158, &menu->_15C, NULL, 0);
	for (s32 i = 0; i < 8; ++i) {
		menu->mItems[i]._08 = DEG2RAD * (f32)(rand() % 360);
		menu->mItems[i]._0C = DEG2RAD * (f32)(rand() % 360);
		menu->mItems[i]._10 = menu->mFocus != 0 ? 0.3f : 0.5f;
	}
	menu->_04  = -1;
	menu->_170 = 0;
	for (s32 i = 0; i < (s32)ARRAY_SIZE(menu->_174); ++i) {
		menu->_174[i]._04 = DEG2RAD * (f32)(rand() % 360);
		menu->_174[i]._08 = DEG2RAD * (f32)(rand() % 360);
	}
	menu->mChr->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	if (menu->_280 == 3) {
		if (menu->_28C >= 0) {
			menu->_288 = menu->_28C;
			menu->_28C = -1;
		} else {
			for (s32 i = 0; i < 8; ++i) {
				if (menu->mItems[i].mType == menu->mChr->GetAttachment()) {
					menu->_288 = i - 3;
					if (menu->_288 < 0)
						menu->_288 += 7;
				}
			}
		}
	}
}
