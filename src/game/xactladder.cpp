#include "game/xact.h"
#include "game/cgame.h"

/**
 * @note Address: 0x800C9B74
 * @note Size: 0xC0
 */
extern "C" void fn_800C9B74(HSD_JObj* joint, f32 angle)
{
	HSD_JObjSetRotationY(joint, angle);
}

/**
 * @note Address: 0x800C9C34
 * @note Size: 0x3C
 */
extern "C" void fn_800C9C34(XObj* object)
{
	if (object->mMoveTranslate.y != object->_264.y)
		object->mStatus.mFlags |= 0x0010000000000000ULL;
	object->_264.x = object->mMoveTranslate.x;
	object->_264.y = object->mMoveTranslate.y;
	object->_264.z = object->mMoveTranslate.z;
}

/**
 * @note Address: 0x800C9C70
 * @note Size: 0xE4
 */
extern "C" void fn_800C9C70(XActLadder* act, const CPadOne* pad, f32* amount)
{
	act->_2C = FALSE;
	if (act->mState == 5) {
		bool active = false;
		if (lbl_8025C400.mStatus.mFlags & 1ULL) {
			if (!(lbl_8025C400.mStatus.mFlags & 0x22ULL)) {
				bool held = false;
				if (lbl_8025C400.mDvdStatus != 0 || lbl_8025C400.mPrevDvdStatus != 0) {
					if (lbl_8025C400.mDiskOk == 0)
						held = true;
				}
				if (!held)
					active = true;
			}
		}
		if (active) {
			if (lbl_8025C400._5D080 == NULL && (pad->mTrigger & 0x200))
				act->_2C = TRUE;
		}
	} else if (act->mState != 0) {
		*amount = 0.0f;
	}
}
