#include "game/xchr.h"
#include <stdlib.h>
#include "game/gmath.h"

// OctaMenu::vf1C, nothing to decide on the base menu
/**
 * @note Address: 0x8009BE18
 * @note Size: 0x4
 */
extern "C" void fn_8009BE18(OctaMenu* menu)
{
}

// OctaMenu::Select, closes the ring and turns chibi robo back
/**
 * @note Address: 0x8009BE1C
 * @note Size: 0xF8
 */
extern "C" void fn_8009BE1C(OctaMenu* menu, s32 result)
{
	if (result > 0 && menu->mFocus == 0)
		menu->mOpen = -1;
	else
		menu->mOpen = -2;
	menu->_28C   = -1;
	menu->mFocus = 0;
	if (menu->mChr->mTumble.mState == 0) {
		menu->mChr->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		menu->mChr->SetRotateY(menu->_16C, DEG2RAD * 12.0f);
		CJointRot* neck = &menu->mChr->mJointRot[1];
		neck->mMode     = 2;
		neck->mTarget   = 0.0f;
		neck->mLookAt   = NULL;
		CJointRot* head = &menu->mChr->mJointRot[2];
		head->mMode     = 2;
		head->mTarget   = -1.52f;
		head->mLookAt   = NULL;
	}
}

// OctaMenu::Open
/**
 * @note Address: 0x8009BF14
 * @note Size: 0x2A8
 */
extern "C" void fn_8009BF14(OctaMenu* menu, s32 focus)
{
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
}
