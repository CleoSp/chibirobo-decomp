#include "game/xact.h"
#include "game/xobjattachment.h"

/**
 * @note Address: 0x800BB708
 * @note Size: 0x18
 */
// XAction::Reset
extern "C" void fn_800BB708(XAction* action)
{
	action->mState  = 0;
	action->_0C     = 0;
	action->mTarget = NULL;
	action->_24     = 0;
}

/**
 * @note Address: 0x800BB720
 * @note Size: 0x8
 */
extern "C" s32 fn_800BB720(XObj* object)
{
	return object->_260;
}

/**
 * @note Address: 0x800BB728
 * @note Size: 0x54
 */
// XObjAttachment::Hide
extern "C" void fn_800BB728(XObjAttachment* attachment)
{
	attachment->mVisible = FALSE;
	if (attachment->mStatus.mFlags & 1ULL)
		HSD_JObjSetFlagsAll(attachment->mJObj, 0x10);
}
