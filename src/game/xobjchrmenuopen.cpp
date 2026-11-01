#include "game/xchr.h"
#include "game/cgame.h"
#include "game/csound.h"

static inline void OpenMenus(XObjChr* chr)
{
	lbl_8025C400.mStatus.mFlags |= 0x400ULL;
	fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
	if (chr->mMenu.mSel == 0) {
		chr->mMenu.mAtc.Open(0);
		chr->mMenu.mDrs.Open(1);
	} else {
		chr->mMenu.mAtc.Open(1);
		chr->mMenu.mDrs.Open(0);
	}
}

/**
 * @note Address: 0x800B558C
 * @note Size: 0x27C
 */
// stops him and brings up the attachment and costume rings
extern "C" void fn_800B558C(XObjChr* chr)
{
	if (chr->mAttachmentRequest != -1 || chr->mSuitChanged != 0)
		return;
	if (!chr->mMenu.IsOpen()) {
		chr->_988.Set(0.0f, 0.0f, 0.0f);
		chr->_988.mTarget.Set(0.0f, 0.0f, 0.0f);
		chr->_988.mSpeed.Set(0.0f, 0.0f, 0.0f);
		chr->mMoveRotate.Set(chr->mMoveRotate.mTarget);
		if (chr->mPush.mState != 0) {
			fn_800C5A00(&chr->mPush);
			chr->mPush.mState = 0;
			chr->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C = 3;
		}
		OpenMenus(chr);
	} else {
		u8 closing = (u8)(chr->mMenu.mAtc.mOpen < 0 || chr->mMenu.mDrs.mOpen < 0);
		if (closing)
			OpenMenus(chr);
	}
}
