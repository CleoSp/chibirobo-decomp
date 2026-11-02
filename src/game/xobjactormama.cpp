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
