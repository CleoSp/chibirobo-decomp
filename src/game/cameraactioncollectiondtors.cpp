#include "game/hsd_jobj.h"
#include "game/msg.h"
#include "game/cmotioncontrolcamera.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__20CMotionControlCamera[];
extern void* __vt__14CCameraActions[];
extern void* __vt__17CCameraActionArea[];
CCameraActionArea* __dt__17CCameraActionAreaFv(CCameraActionArea* area, s32 flags);
CCameraActionEvt* __dt__16CCameraActionEvtFv(CCameraActionEvt* evt, s32 flags);
CCameraActionReset* __dt__18CCameraActionResetFv(CCameraActionReset* reset, s32 flags);
}

/**
 * @note Address: 0x80008FB0
 * @note Size: 0x7C
 */
// todo: weak copy
extern "C" MsgFontGlyph* __dt__12MsgFontGlyphFv(MsgFontGlyph* glyph, s32 flags)
{
	if (glyph != NULL) {
		glyph->mSprite.CSprite::~CSprite();
		glyph->mTexture.CTexObj::~CTexObj();
		if ((s16)flags > 0)
			::operator delete(glyph);
	}
	return glyph;
}

/**
 * @note Address: 0x8000902C
 * @note Size: 0x1A0
 */
// todo: weak copy
extern "C" CMotionControlCamera* __dt__20CMotionControlCameraFv(CMotionControlCamera* camera, s32 flags)
{
	if (camera != NULL) {
		*(void***)((u8*)camera + 0x1F4) = __vt__20CMotionControlCamera;
		camera->mMemento.CCameraMemento::~CCameraMemento();
		camera->mFollowStep.CVec::~CVec();
		camera->mFollowValue.CVec::~CVec();
		camera->mFollowTarget.CVec::~CVec();
		camera->mFlags.CStatus::~CStatus();
		camera->mAngle.CVec::~CVec();
		camera->mInterest.CVec::~CVec();
		camera->mEye.CVec::~CVec();
		camera->mCamera.CCamera::~CCamera();
		if ((s16)flags > 0)
			::operator delete(camera);
	}
	return camera;
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
		actions->mMap.CCameraActionMap::~CCameraActionMap();
		actions->mAnim.CCameraActionAnim::~CCameraActionAnim();
		actions->mEye.CCameraActionEye::~CCameraActionEye();
		actions->mScroll2.CCameraActionScroll2::~CCameraActionScroll2();
		__dt__16CCameraActionEvtFv(&actions->mEvt, -1);
		actions->mChase.CCameraActionChase::~CCameraActionChase();
		__dt__17CCameraActionAreaFv(&actions->mArea, -1);
		actions->mDefault.CCameraActionDefault::~CCameraActionDefault();
		if ((s16)flags > 0)
			::operator delete(actions);
	}
	return actions;
}

/**
 * @note Address: 0x800095C0
 * @note Size: 0x298
 */
// todo: weak copy
extern "C" CCameraActionArea* __dt__17CCameraActionAreaFv(CCameraActionArea* area, s32 flags)
{
	if (area != NULL) {
		*(void***)((u8*)area + 0x3C) = __vt__17CCameraActionArea;
		area->_258.CInterpolationEaseOutDivide::~CInterpolationEaseOutDivide();
		area->_238.CInterpolationEaseOut::~CInterpolationEaseOut();
		area->_21C.CInterpolationEaseInOut::~CInterpolationEaseInOut();
		area->_20C.CInterpolationLinear::~CInterpolationLinear();
		area->_1C0.CVectorEaseOutDivideInterpolation::~CVectorEaseOutDivideInterpolation();
		area->_15C.CVectorEaseOutInterpolation::~CVectorEaseOutInterpolation();
		area->_100.CVectorEaseInOutInterpolation::~CVectorEaseInOutInterpolation();
		area->_CC.CVectorLinearInterpolation::~CVectorLinearInterpolation();
		area->_70.CVectorEaseInOutInterpolation::~CVectorEaseInOutInterpolation();
		area->CCameraActionAreaExM::~CCameraActionAreaExM();
		if ((s16)flags > 0)
			::operator delete(area);
	}
	return area;
}
