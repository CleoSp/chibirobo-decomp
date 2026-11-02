#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/gmath.h"

void XObjActorMama::vf150()
{
	mState = NULL;
}

/**
 * @note Address: 0x80126440
 * @note Size: 0x58
 */
void XObjActorMama::vf14C(u32 frames)
{
	if (mState != NULL) {
		(this->*mState)(frames);
	}
}

/**
 * @note Address: 0x80126498
 * @note Size: 0x78
 */
XObjActorMama::~XObjActorMama()
{
}

/**
 * @note Address: 0x80126510
 * @note Size: 0xAC
 */
HSD_JObj* XObjActorMama::vf168()
{
	s32 path[4] = { 0, 4, 0, 0 };
	return FindJointByPath(mJObj, path, 4);
}

/**
 * @note Address: 0x801265BC
 * @note Size: 0x5C
 */
CVec XObjActorMama::vf16C()
{
	s32 path[4] = { 0, 4, 0, 0 };
	return fn_8007E7C8(this, 4, path);
}

/**
 * @note Address: 0x80126618
 * @note Size: 0x8
 */
f32 XObjActorMama::vf17C()
{
	return -0.17453292f;
}

/**
 * @note Address: 0x80126620
 * @note Size: 0x8
 */
f32 XObjActorMama::vf178()
{
	return 0.17453292f;
}

/**
 * @note Address: 0x80126628
 * @note Size: 0x8
 */
f32 XObjActorMama::vf174()
{
	return -0.6981317f;
}

/**
 * @note Address: 0x80126630
 * @note Size: 0x8
 */
f32 XObjActorMama::vf170()
{
	return 0.6981317f;
}

/**
 * @note Address: 0x80126638
 * @note Size: 0x210
 */
s32 XObjActorMama::vf124(s32 count, s32* command)
{
	fn_801070A4(this, count, command);
	switch (command[0]) {
	case 1:
		switch (command[1]) {
		case 0:
			fn_8010771C(this, 0.08726646f);
			break;
		case 1:
			fn_801076FC(this, (s32)&lbl_8025C400.GetPlayer()->mMoveTranslate, 0.08726646f);
			break;
		case 2:
			if (FindObj(command[2]) != NULL) {
				fn_801076FC(this, (s32)&FindObj(command[2])->mMoveTranslate, DEG2RAD * command[3]);
			}
			break;
		case 3:
			fn_801076DC(this, DEG2RAD * command[2], DEG2RAD * command[3]);
			break;
		}
		break;
	default:
		if (count < 1) {
			return 0;
		}
		mCmd[0] = command[0];
		if (count < 2) {
			mCmd[1] = 0;
		} else {
			mCmd[1] = command[1];
		}
		mCmd[2] = command[2];
		mCmd[3] = command[3];
		mCmd[4] = command[4];
		mCmd[5] = command[5];
		mCmd[6] = command[6];
		mCmd[7] = command[7];
		mCmd[8] = command[8];
		mCmd[9] = command[9];
		break;
	}
	return 0;
}

/**
 * @note Address: 0x80126848
 * @note Size: 0x4
 */
void XObjActorMama::StateNone(u32 frames)
{
}

/**
 * @note Address: 0x80128230
 * @note Size: 0x38C
 */
// lets go of what she carries
// NONMATCHING: the first flag test
void XObjActorMama::Drop()
{
	s32 kind = -1;
	if (_C8C.Check(4)) {
		_C8C.mFlags &= ~4;
		kind = 0x7D;
	} else if (_C8C.Check(1)) {
		_C8C.mFlags &= ~1;
		kind = 0x40;
	} else if (_C8C.Check(2)) {
		_C8C.mFlags &= ~2;
		kind = 0x5B;
	} else if (_C8C.Check(8)) {
		_C8C.mFlags &= ~8;
		kind = 0x196;
	} else if (_C8C.Check(0x10)) {
		_C8C.mFlags &= ~0x10;
		kind = 0x1A1;
	} else if (_C8C.Check(0x20)) {
		_C8C.mFlags &= ~0x20;
		kind = 0x45B;
	}
	if (kind < 0) {
		return;
	}
	switch (kind) {
	case 0x40:
		for (s32 i = 0; i < 64; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[8].Get(i);
			if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x5B:
	case 0x7D:
	case 0x196:
	case 0x45B:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
			if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x1A1:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
			if (obj->GetKind() == 0x1A0) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			} else if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	}
}

/**
 * @note Address: 0x80128210
 * @note Size: 0x20
 */
void XObjActorMama::vfA4(u32 frames)
{
	fn_80107DE0(this, frames);
}

/**
 * @note Address: 0x8012926C
 * @note Size: 0xB0
 */
void XObjActorMama::vf15C()
{
	XObjChr* player = lbl_8025C400.GetPlayer();
	if (player->_980.mFlags & 0x40000000) {
		player->_980.mFlags = 0;
		player->_980.mFlags |= 0x20000000;
		player->_980.mFlags |= 7;
		player->mPadReset = TRUE;
	}
	_998.mFlags = 0;
	_C8C.mFlags = 0;
	_980        = 0.0f;
	_984        = 0.0f;
	_988        = 0.0f;
	_C84        = _988;
	_C88        = 0;
	mState      = &XObjActorMama::StateNone;
	fn_8010706C(this);
}
