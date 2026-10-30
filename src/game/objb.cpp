#include "game/objb.h"

// XObjChr's own functions, the robot. the weak destructors of the menus, actions and
// attachments it holds come after them, the ones that set vtables are left to the classes

// one of the OXMenu's two answers
struct OXMenuItem {
	u8 _00[0x10]; // _00
	XObj mObj;    // _10
};

extern "C" {
CVec fn_800A0894(XObjChr* chr, s32 depth, s32* path); // where a joint of the robot is
BOOL fn_8008A38C(CJAnim* anim, const char* path);     // starts reading an anim archive
void fn_800C5A00(XAction* act);
void fn_800B8850(XObjChr* chr);
void fn_8007F040(XObjEvt* evt, s32 a, s32 b);
}

// the anim archive for each costume, NULL where there's none
extern "C" const char* lbl_802262AC[];

// ---- functions ----

extern "C" {

/**
 * @note Address: 0x800B7898
 * @note Size: 0x64
 */
// NONMATCHING: the dol loads mNearPoint first and keeps the temp's address for its destructor
void fn_800B7898(XObj* obj)
{
	obj->mMoveTranslate + obj->mSpots[obj->mNearPoint].mPos;
}

/**
 * @note Address: 0x800B7B5C
 * @note Size: 0x1F8
 */
void fn_800B7B5C(XObjChr* chr)
{
	chr->mStatus = 0;
	chr->mStatus |= 0x20000000;
	chr->mStatus |= 7;
	chr->_12DF4 = 1;
	if (chr->mJObj != NULL) {
		HSD_JObjClearUserMtx(chr->mJObj);
	}
	if (chr->mActLift.mState != 0) {
		XObj* target = chr->mActLift.mTarget;
		if (target != NULL && target->GetId() == 8) {
			chr->_15894 = target->GetKind();
			chr->_15898 = chr->mActLift.mTarget->mAnimId;
			chr->_1589C = chr->mActLift._24;
		}
	} else {
		switch (chr->GetMode()) {
		case 4:
		case 5:
		case 6:
		case 7:
			chr->_95D0 = 0;
			break;
		}
	}
	chr->mAnim.Release();
	chr->mAnims[0].Release();
	chr->mAnims[1].Release();
	fn_800B8850(chr);
	if (chr->mEvt.mEvt != NULL) {
		fn_8007F040(&chr->mEvt, 1, 0);
	}
}

/**
 * @note Address: 0x800B9758
 * @note Size: 0xA4
 */
BOOL fn_800B9758(XObjChr* chr)
{
	const char* path = lbl_802262AC[chr->_95D0];
	if (path != NULL) {
		if (fn_8008A38C(&chr->mAnims[0], path)) {
			chr->SetAnim(0x11, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

/**
 * @note Address: 0x800BAD80
 * @note Size: 0x2C
 */
CVec fn_800BAD80(XObjChr* chr, s32 depth, s32* path)
{
	return fn_800A0894(chr, depth, path);
}

/**
 * @note Address: 0x800BADAC
 * @note Size: 0xC4
 */
void fn_800BADAC(XObjChr* chr)
{
	chr->_988.Set(0.0f, 0.0f, 0.0f);
	chr->_998.Set(0.0f, 0.0f, 0.0f);
	chr->_9A8.Set(0.0f, 0.0f, 0.0f);
	chr->mMoveRotate.Set(chr->mMoveRotate.mTarget);
	if (chr->mActBE8.mState != 0) {
		fn_800C5A00(&chr->mActBE8);
		chr->mActBE8.mState = 0;
		chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		chr->_A2C = 3;
	}
}

/**
 * @note Address: 0x800BB708
 * @note Size: 0x18
 */
void fn_800BB708(XAction* act)
{
	act->mState  = 0;
	act->_0C     = 0;
	act->mTarget = NULL;
	act->_24     = 0;
}

/**
 * @note Address: 0x800BB720
 * @note Size: 0x8
 */
s32 fn_800BB720(XObj* obj)
{
	return obj->mCount;
}

/**
 * @note Address: 0x800BB728
 * @note Size: 0x54
 */
void fn_800BB728(XObjAttachment* obj)
{
	obj->mShown = FALSE;
	if (obj->mFlags & 1) {
		HSD_JObjSetFlagsAll(obj->mJObj, 0x10);
	}
}

/**
 * @note Address: 0x800BBD7C
 * @note Size: 0x30
 */
void fn_800BBD7C(XAction* act, XObjChr* chr)
{
	act->mChr = chr;
	act->Reset();
}

/**
 * @note Address: 0x800BCBB8
 * @note Size: 0x94
 */
XActPopperShot* fn_800BCBB8(XActPopperShot* shot, s16 del)
{
	if (shot) {
		shot->mPtcl.~CPtclSet();
		shot->_004.CVec::~CVec();
		if (del > 0) {
			operator delete(shot);
		}
	}
	return shot;
}

XObjPlugNode* fn_800BCCF4(XObjPlugNode* node, s16 del);

/**
 * @note Address: 0x800BCCF4
 * @note Size: 0x90
 */
XObjPlugNode* fn_800BCCF4(XObjPlugNode* node, s16 del)
{
	if (node) {
		node->_34.CVec::~CVec();
		node->_14.CInterpolationEaseInOut::~CInterpolationEaseInOut();
		node->_04.CVec::~CVec();
		if (del > 0) {
			operator delete(node);
		}
	}
	return node;
}

/**
 * @note Address: 0x800BCFC4
 * @note Size: 0x58
 */
OXMenuItem* fn_800BCFC4(OXMenuItem* item, s16 del)
{
	if (item) {
		fn_8000877C(&item->mObj, -1);
		if (del > 0) {
			operator delete(item);
		}
	}
	return item;
}

/**
 * @note Address: 0x800BD320
 * @note Size: 0x4
 */
void fn_800BD320(void)
{
}

} // extern "C"
