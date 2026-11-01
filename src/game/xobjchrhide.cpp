#include "game/xobj.h"
#include "game/cjobjinlineattach.h"

struct XObjChrHidePath {
	s32 mSteps[5]; // _00
};
extern "C" const XObjChrHidePath lbl_80209AF8;

// hides or shows one joint subtree of chibi robo's model
/**
 * @note Address: 0x800A2E88
 * @note Size: 0x264
 */
extern "C" void fn_800A2E88(XObj* chr, BOOL hidden)
{
	if (chr->mStatus.mFlags & 1ULL) {
		XObjChrHidePath path = lbl_80209AF8;
		CJObj joint(GetJointByPath(chr->mJObj, path.mSteps, 5));
		if (hidden)
			HSD_JObjSetFlagsAll(joint.mJObj, 0x10);
		else
			HSD_JObjClearFlagsAll(joint.mJObj, 0x10);
	}
}
