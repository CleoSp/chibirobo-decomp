#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/cgame.h"

extern "C" {
extern XObjActorJenny::StateFunc lbl_8022D388;
}

/**
 * @note Address: 0x801262F0
 * @note Size: 0x12C
 */
extern "C" void fn_801262F0(XObjActorJenny* object)
{
	XObjChr* player = lbl_8025C400.mPlayer;
	if (player->_980.mFlags & 0x40000000) {
		player->_980.mFlags = 0;
		player->_980.mFlags |= 0x20000000;
		player->_980.mFlags |= 7;
		player->mPadReset = TRUE;
	}
	object->_998.mFlags = 0;
	object->_C90.mFlags = 0;
	object->_C98.mFlags = 0;
	object->_980        = 0.0f;
	object->_984        = 0.0f;
	object->_988        = 0.0f;
	object->_C84        = object->_988;
	object->_C88        = 0.0f;
	object->_C8C        = 0;
	object->_CA0        = 0;
	object->mState      = lbl_8022D388;
	fn_8010706C(object);
	for (u32 i = 0; i < 20; ++i)
		object->mCmd[i] = 0;
	object->_C90.mFlags &= ~8;
}

/**
 * @note Address: 0x8012641C
 * @note Size: 0x24
 */
extern "C" void fn_8012641C(XObjActorJenny* object)
{
	object->mState = NULL;
}

/**
 * @note Address: 0x80126440
 * @note Size: 0x58
 */
extern "C" void fn_80126440(XObjActorJenny* object, u32 frames)
{
	if (object->mState != NULL)
		(object->*object->mState)(frames);
}
