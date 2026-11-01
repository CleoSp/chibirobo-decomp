#include "game/xobjleticker.h"

extern "C" AnimSfxEvent lbl_8022A034[];

/**
 * @note Address: 0x800E6410
 * @note Size: 0x98
 */
extern "C" void Reset__12XObjLetickerFv(XObjLeticker* leticker)
{
	fn_800928E8(leticker);
	leticker->mLinkedCount = 0;
	leticker->mFunc        = NULL;
	leticker->_99C         = 0;
	leticker->_9B4         = 0;
	leticker->_9F8         = 0.0f;
	leticker->mHeight      = 0.0f;
	leticker->mPtcl        = 1;
	leticker->_A2D         = 0;
	leticker->_A30         = 0;
	leticker->mState       = 0;
	leticker->mHeading     = 0.0f;
	leticker->_9B8         = -1;
	leticker->_9D8         = -1;
	leticker->mSfx.mEvents = lbl_8022A034;
}
