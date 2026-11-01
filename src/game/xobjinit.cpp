#include "game/cgame.h"
#include "game/xobjs.h"
#include <math.h>

extern "C" void fn_8007F194(void* event, void* context, void* owner);

// XObj::vf98
/**
 * @note Address: 0x800928E8
 * @note Size: 0xDC
 */
extern "C" void fn_800928E8(XObj* object)
{
	object->_80C   = 0;
	object->_788   = -1;
	object->_78C   = 0;
	object->_790   = 0.0f;
	object->_794.x = 0.0f;
	object->_794.y = 0.0f;
	object->_794.z = 0.0f;
	object->mAnmQueue.Clear();
	object->mAnchorX = object->mAnchorZ = NAN;
	object->_8B0                        = 0;
	object->_8B0 |= 0x80000000;
	object->mColorStep[0].mValue.x = object->mColorStep[0].mValue.y = object->mColorStep[0].mValue.z = 255.0f;
	object->mColorStep[0].mTarget.x = object->mColorStep[0].mTarget.y = object->mColorStep[0].mTarget.z = 255.0f;
	object->mColorStep[0].mSpeed.x = object->mColorStep[0].mSpeed.y = object->mColorStep[0].mSpeed.z = 0.0f;
	object->mColorStep[1].mValue.x = object->mColorStep[1].mValue.y = object->mColorStep[1].mValue.z = 0.0f;
	object->mColorStep[1].mTarget.x = object->mColorStep[1].mTarget.y = object->mColorStep[1].mTarget.z = 0.0f;
	object->mColorStep[1].mSpeed.x = object->mColorStep[1].mSpeed.y = object->mColorStep[1].mSpeed.z = 0.0f;
}

// XObj::vf90
/**
 * @note Address: 0x800929C4
 * @note Size: 0xE0
 */
extern "C" void fn_800929C4(XObj* object, u32 id)
{
	object->CXObj::Init(id);
	object->mStatus.mFlags = 0;
	object->mCoord._00     = NULL;
	object->mAttr._00      = NULL;
	object->mAttr._04      = NULL;
	object->mFloors        = NULL;
	object->_258           = 0;
	object->vf98();
	fn_8007F194(&object->mEvt, lbl_8025C400.mEvtData[1], object);
	object->mAnmQueue.Clear();
	object->mAnim.Clear();
}
