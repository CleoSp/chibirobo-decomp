#include "game/xobjactor.h"
#include "game/cgame.h"
#include "game/csound.h"

extern "C" {
extern XObjActorTao::StateFunc lbl_8022DA60;
}

/**
 * @note Address: 0x8012F314
 * @note Size: 0x50
 */
extern "C" void fn_8012F314(XObjActorTao* object)
{
	if (object->_998.mFlags & 2) {
		object->_998.mFlags &= ~2U;
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x15D, 255);
	}
}

/**
 * @note Address: 0x8012F364
 * @note Size: 0xC4
 */
extern "C" void fn_8012F364(XObjActorTao* object)
{
	object->_998.mFlags = 0;
	object->_C8C.mFlags = 0;
	object->_980        = 0.0f;
	object->_984        = 0.0f;
	object->_988        = 0.0f;
	object->_C84        = object->_988;
	object->_C88        = 0;
	object->mState      = lbl_8022DA60;
	fn_8010706C(object);
	object->mCmd[2] = 0;
	object->mCmd[3] = 0;
	object->mCmd[4] = 0;
	object->mCmd[5] = 0;
	object->mCmd[6] = 0;
	object->mCmd[7] = 0;
	object->mCmd[8] = 0;
	object->mCmd[9] = 0;
	object->_CF4    = -1;
	object->_CF8    = 0;
	object->_CFC    = 0;
	object->_CF4    = 0x82;
	object->_CF8    = 1;
	object->_998.mFlags |= 0x2000;
}

/**
 * @note Address: 0x8012F428
 * @note Size: 0x24
 */
extern "C" void fn_8012F428(XObjActorDeca* object)
{
	object->mState = NULL;
}

/**
 * @note Address: 0x8012F44C
 * @note Size: 0x58
 */
extern "C" void fn_8012F44C(XObjActorDeca* object, u32 frames)
{
	if (object->mState != NULL)
		(object->*object->mState)(frames);
}
