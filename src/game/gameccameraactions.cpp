#include "game/hsd_jobj.h"
#include "game/cmotioncontrolcamera.h"
#include "game/ccameraactioneye.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__14CCameraActions[];
CCameraActionReset* __dt__18CCameraActionResetFv(CCameraActionReset* reset, s32 flags);
CCameraActionMap* __dt__16CCameraActionMapFv(CCameraActionMap* map, s32 flags);
CCameraActionEvt* __dt__16CCameraActionEvtFv(CCameraActionEvt* evt, s32 flags);
CCameraActionArea* __dt__17CCameraActionAreaFv(CCameraActionArea* area, s32 flags);
}

/**
 * @note Address: 0x800091CC
 * @note Size: 0x3F4
 */
// todo: weak copy
extern "C" CCameraActions* __dt__14CCameraActionsFv(CCameraActions* actions, s32 flags)
{
	if (actions != NULL) {
		*(void***)((u8*)actions + 0x1E70) = __vt__14CCameraActions;
		actions->mScroll.CCameraActionScroll::~CCameraActionScroll();
		__dt__18CCameraActionResetFv(&actions->mReset, -1);
		actions->mLadr.CCameraActionLadr::~CCameraActionLadr();
		__dt__16CCameraActionMapFv(&actions->mMap, -1);
		actions->mAnim.CCameraActionAnim::~CCameraActionAnim();
		((CCameraActionEye*)actions->mEye)->CCameraActionEye::~CCameraActionEye();
		actions->mScroll2.CCameraActionScroll2::~CCameraActionScroll2();
		__dt__16CCameraActionEvtFv(&actions->mEvt, -1);
		actions->mChase.CCameraActionChase::~CCameraActionChase();
		__dt__17CCameraActionAreaFv((CCameraActionArea*)actions->mArea, -1);
		actions->mDefault.CCameraActionDefault::~CCameraActionDefault();
		if ((s16)flags > 0)
			::operator delete(actions);
	}
	return actions;
}
