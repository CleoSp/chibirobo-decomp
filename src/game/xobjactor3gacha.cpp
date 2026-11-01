#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/xchr.h"
#include "game/cgame.h"
#include <math.h>
#include "game/gmath.h"
#include "game/globals.h"

extern "C" {
void fn_80068BAC(void* context);
void fn_80068BC0(void* context);
void fn_80068BD4(void* context);
void fn_80068BE8(void* context);
void fn_80068BFC(void* context);
void fn_80068C10(void* context);
}

/**
 * @note Address: 0x80114540
 * @note Size: 0xFC
 */
extern "C" void fn_80114540(XObjActor3* object)
{
	switch (object->_B94) {
	case 1:
		if (object->_B98 != 0)
			fn_80068BC0(lbl_8025C400.mInfoScrn);
		else
			fn_80068BAC(lbl_8025C400.mInfoScrn);
		object->_B94 = 0;
		break;
	case 2:
		if (object->_B98 != 0) {
			fn_80068C10(lbl_8025C400.mInfoScrn);
			fn_80068BE8(lbl_8025C400.mInfoScrn);
			fn_80068BC0(lbl_8025C400.mInfoScrn);
		} else {
			fn_80068BFC(lbl_8025C400.mInfoScrn);
			fn_80068BD4(lbl_8025C400.mInfoScrn);
			fn_80068BAC(lbl_8025C400.mInfoScrn);
		}
		object->_B94 = 0;
		break;
	}
}

/**
 * @note Address: 0x8011463C
 * @note Size: 0xB8
 */
extern "C" void fn_8011463C(XObjActor3* object)
{
	s32 count = 0;
	switch (object->_B94) {
	case 1:
		for (s32 i = 0; i < 256; ++i) {
			InventoryItem* entries = lbl_8025C400.mPlayer->mParams.mInventory.mItems;
			s16 id                 = entries[i].mId;
			s16 amount             = entries[i].mCount;
			if (fn_8006E44C(id, 4) && id >= 0 && amount > 0)
				++count;
		}
		lbl_8025C400._5D08C = count;
		object->_B94        = 0;
		break;
	}
}

/**
 * @note Address: 0x801146F4
 * @note Size: 0x4
 */
extern "C" void fn_801146F4()
{
}

/**
 * @note Address: 0x801146F8
 * @note Size: 0xC8
 */
extern "C" void fn_801146F8(XObjActor3* object)
{
	object->_B88 = (s32)((u32)object->_B88 + 1);
	s32 degrees  = (s32)((u32)object->_B88 * 6) % 360;
	f32 value    = (f32)sin(DEG2RAD * (f32)degrees);
	s32 kind     = object->GetKind();
	if (kind == 0x437)
		object->mMoveRotate.x = value * 0.25f;
	else
		object->mMoveRotate.z = value * 0.25f;
}
