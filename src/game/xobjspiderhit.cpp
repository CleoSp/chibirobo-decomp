#include "game/xobjspider.h"
#include "game/cgame.h"
#include "game/hsd_cobj.h"
#include "game/csound.h"

/**
 * @note Address: 0x800EFDAC
 * @note Size: 0x4
 */
extern "C" void fn_800EFDAC(XObjSpider* spider)
{
}

/**
 * @note Address: 0x800EFDB0
 * @note Size: 0x2A8
 */
extern "C" void fn_800EFDB0(XObjSpider* spider, u32 frames)
{
	switch (spider->mPhase) {
	case 0:
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x129, 255);
		spider->mFlags.mFlags &= ~0x800U;
		spider->mFlags.mFlags |= 0x10;
		spider->mSpeed = 0.0f;
		if (spider->mHits <= 0) {
			spider->mFlags.mFlags |= 0x10000;
			spider->SetState(3);
		} else {
			spider->SetAnim(21, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			++spider->mPhase;
		}
		break;
	case 1:
		HSD_AObj* aobj = GetAObj(spider->mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(spider->mJObj);
		f32 current    = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
		if (end - (f32)frames <= current) {
			spider->mFlags.mFlags &= ~0x10U;
			spider->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			spider->SetState(4);
		}
		break;
	}
}

/**
 * @note Address: 0x800F0058
 * @note Size: 0x38
 */
extern "C" void fn_800F0058(XObjSpider* spider)
{
	if (spider->_25C == 11 && (spider->mFlags.mFlags & 0x400000))
		spider->SetState(4);
}
