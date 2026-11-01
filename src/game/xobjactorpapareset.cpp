#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/cgame.h"

extern "C" {
extern XObjActorPapa::StateFunc lbl_8022D818;
}

/**
 * @note Address: 0x8012C7A0
 * @note Size: 0xB8
 */
extern "C" void vf15C__13XObjActorPapaFv(XObjActorPapa* object)
{
	XObjChr* player = lbl_8025C400.mPlayer;
	if (player->_980.mFlags & 0x40000000) {
		player->_980.mFlags = 0;
		player->_980.mFlags |= 0x20000000;
		player->_980.mFlags |= 7;
		player->mPadReset = TRUE;
	}
	object->_998.mFlags = 0;
	object->_C8C.mFlags = 0;
	object->_980        = 0.0f;
	object->_984        = 0.0f;
	object->_988        = 0.0f;
	object->_C84        = object->_988;
	object->_C88        = 0;
	object->_C94        = 1;
	object->mState      = lbl_8022D818;
	fn_8010706C(object);
}

/**
 * @note Address: 0x8012C858
 * @note Size: 0x24
 */
extern "C" void vf150__12XObjActorTaoFv(XObjActorPapa* object)
{
	object->mState = NULL;
}

/**
 * @note Address: 0x8012C87C
 * @note Size: 0x58
 */
extern "C" void vf14C__12XObjActorTaoFUl(XObjActorPapa* object, u32 frames)
{
	if (object->mState != NULL)
		(object->*object->mState)(frames);
}
