#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/gmath.h"

void XObjActorJenny::vf150()
{
	mState = NULL;
}

/**
 * @note Address: 0x80122E30
 * @note Size: 0x58
 */
void XObjActorJenny::vf14C(u32 frames)
{
	if (mState != NULL) {
		(this->*mState)(frames);
	}
}

/**
 * @note Address: 0x80122E88
 * @note Size: 0x8C
 */
XObjActorJenny::~XObjActorJenny()
{
}

/**
 * @note Address: 0x80122F14
 * @note Size: 0xA4
 */
HSD_JObj* XObjActorJenny::vf168()
{
	s32 path[3] = { 0, 3, 0 };
	return FindJointByPath(mJObj, path, 3);
}

/**
 * @note Address: 0x80122FB8
 * @note Size: 0x54
 */
CVec XObjActorJenny::vf16C()
{
	s32 path[3] = { 0, 3, 0 };
	return fn_8007E7C8(this, 3, path);
}

/**
 * @note Address: 0x8012300C
 * @note Size: 0x28
 */
f32 XObjActorJenny::vf17C()
{
	switch (mAnimId) {
	case 0x412:
	case 0x413:
	case 0x414:
		return 0.34906584f;
	default:
		return -0.34906584f;
	}
}

/**
 * @note Address: 0x80123034
 * @note Size: 0x28
 */
f32 XObjActorJenny::vf178()
{
	switch (mAnimId) {
	case 0x412:
	case 0x413:
	case 0x414:
		return 1.0471976f;
	default:
		return 0.17453292f;
	}
}

/**
 * @note Address: 0x8012305C
 * @note Size: 0x24C
 */
// vf124
extern "C" s32 fn_8012305C(XObjActorJenny* object, s32 count, s32* command)
{
	fn_801070A4(object, count, command);
	switch (command[0]) {
	case 1:
		switch (command[1]) {
		case 0:
			fn_8010771C(object, 0.08726646f);
			break;
		case 1:
			fn_801076B0(object, (s32)&lbl_8025C400.GetPlayer()->mMoveTranslate, 0.08726646f);
			break;
		case 2:
			if (FindObj(command[2]) != NULL) {
				fn_801076FC(object, (s32)&FindObj(command[2])->mMoveTranslate, DEG2RAD * command[3]);
			}
			break;
		case 3:
			fn_801076DC(object, DEG2RAD * command[2], DEG2RAD * command[3]);
			break;
		}
		break;
	default:
		if (count < 1) {
			return 0;
		}
		object->mCmd[0] = command[0];
		if (count < 2) {
			object->mCmd[1] = 0;
		} else {
			object->mCmd[1] = command[1];
		}
		for (int i = 0; i < 8; i++) {
			object->mCmd[i + 2] = command[i + 2];
		}
		if (object->mCmd[1] == 0xD0) {
			object->mCmd[19] = (s32)FindObj(object->mCmd[2]);
		}
		break;
	}
	return 0;
}

/**
 * @note Address: 0x801232A8
 * @note Size: 0x4
 */
void XObjActorJenny::StateNone(u32 frames)
{
}

/**
 * @note Address: 0x80124980
 * @note Size: 0x430
 */
// NONMATCHING: the paths sit at other .rodata offsets
void XObjActorJenny::UpdateAnim(u32 frames)
{
	CXObjIntp::UpdateAnim(frames);
	CJObj joint;
	if (_C90.mFlags & 8) {
		s32 path[7] = { 0, 3, 2, 0, 0, 1, 0 };
		joint.Attach(GetJointFromRoot(mJObj, 7, path));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
	} else {
		s32 path[7] = { 0, 3, 2, 0, 0, 1, 0 };
		joint.Attach(GetJointFromRoot(mJObj, 7, path));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
	}
}

/**
 * @note Address: 0x8012527C
 * @note Size: 0x298
 */
// lets go of what she carries
void XObjActorJenny::Drop()
{
	s32 kind = -1;
	if (_C98.Check(1)) {
		_C98.mFlags &= ~1;
		kind = 0x3D;
	} else if (_C98.Check(2)) {
		_C98.mFlags &= ~2;
		kind = 0x3E;
	} else if (_C98.Check(4)) {
		_C98.mFlags &= ~4;
		kind = 0x3F;
	} else if (_C98.Check(8)) {
		_C98.mFlags &= ~8;
		kind = 0x3B;
	} else if (_C98.Check(0x10)) {
		_C98.mFlags &= ~0x10;
		kind = 0x401;
	}
	if (kind < 0) {
		return;
	}
	switch (kind) {
	case 0x3B:
	case 0x3D:
	case 0x3E:
	case 0x3F:
		for (s32 i = 0; i < 64; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[8].Get(i);
			if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x401:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
			if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	}
}

/**
 * @note Address: 0x801262F0
 * @note Size: 0x12C
 */
void XObjActorJenny::vf15C()
{
	XObjChr* player = lbl_8025C400.GetPlayer();
	if (player->_980.mFlags & 0x40000000) {
		player->_980.mFlags = 0;
		player->_980.mFlags |= 0x20000000;
		player->_980.mFlags |= 7;
		player->mPadReset = TRUE;
	}
	_998.mFlags = 0;
	_C90.mFlags = 0;
	_C98.mFlags = 0;
	_980        = 0.0f;
	_984        = 0.0f;
	_988        = 0.0f;
	_C84        = _988;
	_C88        = 0.0f;
	_C8C        = 0;
	_CA0        = 0;
	mState      = &XObjActorJenny::StateNone;
	fn_8010706C(this);
	for (u32 i = 0; i < 20; i++) {
		mCmd[i] = 0;
	}
	_C90.mFlags &= ~8;
}
