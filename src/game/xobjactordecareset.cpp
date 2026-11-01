#include "game/xobjactor.h"
#include "game/gmath.h"

extern "C" {
extern XObjActorDeca::StateFunc lbl_8022DCA0;
}

/**
 * @note Address: 0x80131AB0
 * @note Size: 0x20
 */
extern "C" void fn_80131AB0(XObjActorDeca* object, u32 frames)
{
	object->CXObjIntp::UpdateAnim(frames);
}

/**
 * @note Address: 0x80131AD0
 * @note Size: 0x20
 */
extern "C" void fn_80131AD0(XObjActorDeca* object, u32 frames)
{
	fn_80107DE0(object, frames);
}

/**
 * @note Address: 0x80131AF0
 * @note Size: 0x1C
 */
extern "C" void fn_80131AF0(XObjActorDeca* object)
{
	if (object->_998.mFlags & 2)
		object->_998.mFlags &= ~2U;
}

/**
 * @note Address: 0x80131B0C
 * @note Size: 0xDC
 */
extern "C" void fn_80131B0C(XObjActorDeca* object)
{
	object->_998.mFlags = 0;
	object->_C90.mFlags = 0;
	object->_C98.mFlags = 0;
	object->_980        = 0.0f;
	object->_984        = 0.0f;
	object->_988        = 0.0f;
	object->_C84        = object->_988;
	object->_C88        = 0.0f;
	object->_C8C        = 0;
	object->_CA0        = PI;
	object->_CA4        = PI;
	object->mState      = lbl_8022DCA0;
	fn_8010706C(object);
	object->mCmd[0]  = 0;
	object->mCmd[1]  = 0;
	object->mCmd[2]  = 0;
	object->mCmd[3]  = 0;
	object->mCmd[4]  = 0;
	object->mCmd[5]  = 0;
	object->mCmd[6]  = 0;
	object->mCmd[7]  = 0;
	object->mCmd[8]  = 0;
	object->mCmd[9]  = 0;
	object->mCmd[10] = 0;
	object->mCmd[11] = 0;
	object->mCmd[12] = 0;
	object->mCmd[13] = 0;
	object->mCmd[14] = 0;
	object->mCmd[15] = 0;
	object->mCmd[16] = 0;
	object->mCmd[17] = 0;
	object->mCmd[18] = 0;
	object->mCmd[19] = 0;
}
